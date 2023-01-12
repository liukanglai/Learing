#include "reg52.h"

typedef unsigned int u16; //对系统默认数据类型进行重定义
typedef unsigned char u8;
typedef struct {
  u8 sec;
  u8 min;
  u8 hour;
  u8 day;
  u8 month;
  u16 year;
  u8 week; // 记录时间
  u8 mode; // 开始是0，表示正常模式，变为1时，表示正在设置时间
  u8 time_choice; //进入模式设置时，用于切换年月日时分秒哪个需要设置
  u8 add;   //进入模式设置时，用于加减
  u8 setok; //设置完成标志
} _calendar;

_calendar g_calendar;

// 各月的天数，在闰年时二月要改为29
u8 months_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void delay_10us(u16 ten_us); // 用于消除按键抖动和蜂鸣
void delay_ms(u16 ms);       // 用于lcd写延迟
void time0_init(void);

void calendar_test(void); //日历
void calendar_save_set_time(void);
void calendar_set_time(void);
void calendar_dataTochar(u8 *datebuf, u8 *timebuf); //将数据转换为字符显示
void calendar_show(void);                           //时钟显示
void calendar_time_go1(void);                       // 1s过去了

// LCD

//管脚定义，单片机与lcd连接的引脚
sbit LCD1602_RS = P2 ^ 6;   //数据命令选择
sbit LCD1602_RW = P2 ^ 5;   //读写选择
sbit LCD1602_E = P2 ^ 7;    //使能信号
#define LCD1602_DATAPORT P0 //宏定义LCD1602数据端口，便于编程

//函数声明
void lcd1602_init(void);         //初始化
void lcd1602_write_cmd(u8 cmd);  //写命令
void lcd1602_write_data(u8 dat); //写数据
void lcd1602_clear(void);
void lcd1602_show_string(u8 x, u8 y, u8 *str);

// Key

//定义独立按键控制脚
sbit KEY1 = P3 ^ 1;
sbit KEY2 = P3 ^ 0;
sbit KEY3 = P3 ^ 2;
sbit KEY4 = P3 ^ 3;

//使用宏定义独立按键按下的键值
#define KEY1_PRESS 1
#define KEY2_PRESS 2
#define KEY3_PRESS 3
#define KEY4_PRESS 4
#define KEY_UNPRESS 0

u8 key_scan(u8 mode);

// Beep

//管脚定义
sbit BEEP = P1 ^ 5;
//函数声明
void beep_alarm(u16 time, u16 fre);

void main() {
  //时钟初始化2023年01月03日星期二21点00分00秒
  g_calendar.sec = 30;
  g_calendar.min = 59;
  g_calendar.hour = 23;
  g_calendar.day = 31;
  g_calendar.month = 12;
  g_calendar.year = 2023;
  g_calendar.week = 7;

  calendar_test();
  while (1) {
  }
}

void delay_10us(u16 ten_us) // 函数功能: 延时函数，ten_us=1时，大约延时10us
{
  while (ten_us--)
    ;
}

void delay_ms(u16 ms) // 函数功能: ms延时函数，ms=1时，大约延时1ms
{
  u16 i, j;
  for (i = ms; i > 0; i--)
    for (j = 110; j > 0; j--)
      ;
}

void time0_init(
    void) { // 函数功能: 定时器0中断配置函数，通过设置TH和TL即可确定定时时间
  TMOD |= 0X01; //选择为定时器0模式，工作方式1
  TH0 = 0xDC;
  TL0 = 0x00; //给定时器赋初值，定时10ms
  ET0 = 1;    //打开定时器0中断允许
  EA = 1;     //打开总中断
  TR0 = 1;    //打开定时器
}

// 当T0溢出，中断发生，即每过10ms，运行一次中断
void time0() interrupt 1 {
  static u8 cnt = 0;
  TH0 = 0xDC;
  TL0 = 0x00;
  cnt++;

  if (cnt == 100) { // 1s之后
    cnt = 0;
    if (g_calendar.mode == 0) //在设置时间时，停止走
      calendar_time_go1(); //设置时间
  }
}

void calendar_test(void) {
  u8 key_temp = 0; // 记录按下的是什么键

  lcd1602_init();
  time0_init(); //定时器10ms
  while (1) {
    key_temp = key_scan(0);     // 0表示正常模式
    if (key_temp == KEY3_PRESS) //模式设置
    {
      g_calendar.mode++;
      if (g_calendar.mode == 1) {
        g_calendar.setok = 0;       //开始设置
        g_calendar.time_choice = 0; //从0位开始修改
      }
      if (g_calendar.mode == 2) {
        g_calendar.mode = 0;
        g_calendar.setok = 1;
        g_calendar.time_choice = 0; //复位
        g_calendar.mode = 0;
        time0_init(); // 让T0重新开始计时 ---------------
      }
      beep_alarm(100, 10);             //叫一下，频率100us
    } else if (key_temp == KEY2_PRESS) //进入设置模式时，对应位选择设置
    {
      g_calendar.time_choice++;
      if (g_calendar.time_choice == 7)
        g_calendar.time_choice = 0; // 从头再来
      beep_alarm(100, 10);
    } else if (key_temp == KEY1_PRESS) //进入设置模式时，进行数据加操作
    {
      g_calendar.add = 1;
      beep_alarm(100, 10);
    }
    else if (key_temp == KEY4_PRESS)// 用于数据减
    {
      g_calendar.add = 2;
      beep_alarm(100, 10);
    }
    if (g_calendar.mode == 1) //模拟光标闪烁
    {
      if (g_calendar.time_choice < 3)
        lcd1602_show_string(7 - g_calendar.time_choice * 3, 1, " ");
      else if (g_calendar.time_choice >= 3 && g_calendar.time_choice < 4)
        lcd1602_show_string(14, 0, " ");
      else if (g_calendar.time_choice >= 4 && g_calendar.time_choice < 7)
        lcd1602_show_string(21 - g_calendar.time_choice * 3, 0, " ");
      calendar_set_time(); //设置时间
    }
    calendar_show();
  }
}

void calendar_set_time(void) //时钟设置模式
{
  if (g_calendar.mode == 1) //时钟设置
  {
    if (g_calendar.add == 1) {
      switch (g_calendar.time_choice) {
      case 0:
        g_calendar.sec++;
        if (g_calendar.sec >= 60)
          g_calendar.sec = 0;
        break;
      case 1:
        g_calendar.min++;
        if (g_calendar.min >= 60)
          g_calendar.min = 0;
        break;
      case 2:
        g_calendar.hour++;
        if (g_calendar.hour >= 24)
          g_calendar.hour = 0;
        break;
      case 3: // 星期自动计算，不能手动
        /*g_calendar.week++;*/
        /*if (g_calendar.week >= 0x08)*/
        /*g_calendar.week = 1;*/
        break;
      case 4:
        g_calendar.day++;
        if (g_calendar.day > months_days[g_calendar.month])
          g_calendar.day = 1;
        break;
      case 5:
        g_calendar.month++;
        if (g_calendar.month > 12)
          g_calendar.month = 1;
        break;
      case 6:
        g_calendar.year++;
        if (g_calendar.year > 9999)
          g_calendar.year = 1;
        break;
      }
    }
    if (g_calendar.add == 2) {
      switch (g_calendar.time_choice) {
      case 0:
        g_calendar.sec--;
        if (g_calendar.sec == 255)
          g_calendar.sec = 59;
        break;
      case 1:
        g_calendar.min--;
        if (g_calendar.min == 255)
          g_calendar.min = 59;
        break;
      case 2:
        g_calendar.hour--;
        if (g_calendar.hour == 255)
          g_calendar.hour = 23;
        break;
      case 3: // 星期自动计算，不能手动
        /*g_calendar.week++;*/
        /*if (g_calendar.week >= 0x08)*/
        /*g_calendar.week = 1;*/
        break;
      case 4:
        g_calendar.day--;
        if (g_calendar.day == 0)
          g_calendar.day = months_days[g_calendar.month];
        break;
      case 5:
        g_calendar.month--;
        if (g_calendar.month == 0)
          g_calendar.month = 12;
        break;
      case 6:
        g_calendar.year--;
        if (g_calendar.year == 0)
          g_calendar.year = 9999;
        break;
      }
    }
      g_calendar.add = 0;
  }
}

void calendar_dataTochar(u8 *datebuf, u8 *timebuf) { // ASCII码
  datebuf[0] = g_calendar.year / 1000 + 0x30;
  datebuf[1] = (g_calendar.year / 100) % 10 + 0x30;
  datebuf[2] = (g_calendar.year / 10) % 10 + 0x30;
  datebuf[3] = g_calendar.year % 10 + 0x30;
  datebuf[4] = '-';
  datebuf[5] = g_calendar.month / 10 + 0x30;
  datebuf[6] = g_calendar.month % 10 + 0x30;
  datebuf[7] = '-';
  datebuf[8] = g_calendar.day / 10 + 0x30;
  datebuf[9] = g_calendar.day % 10 + 0x30;
  datebuf[10] = ' ';
  datebuf[11] = 'D';
  datebuf[12] = 'a';
  datebuf[13] = 'y';
  datebuf[14] = g_calendar.week + 0x30;
  datebuf[15] = '\0';

  timebuf[0] = g_calendar.hour / 10 + 0x30;
  timebuf[1] = g_calendar.hour % 10 + 0x30;
  timebuf[2] = ':';
  timebuf[3] = g_calendar.min / 10 + 0x30;
  timebuf[4] = g_calendar.min % 10 + 0x30;
  timebuf[5] = ':';
  timebuf[6] = g_calendar.sec / 10 + 0x30;
  timebuf[7] = g_calendar.sec % 10 + 0x30;
  timebuf[8] = '\0';
}

void calendar_show(void) //时钟显示
{
  u8 date_buf[16];
  u8 time_buf[9];
  u16 year_temp = g_calendar.year;
  u8 month_temp = g_calendar.month;

  if (((g_calendar.year % 4 == 0) && (g_calendar.year % 100 != 0)) ||
      g_calendar.year % 400 == 0) {
    months_days[2] = 29;
  }
  else{
    months_days[2] = 28; //开始这步忘了。。。
  }
  if(g_calendar.sec > 59){
    g_calendar.sec = 0;
  }
  if(g_calendar.min > 59){
    g_calendar.min = 0;
  }
  if(g_calendar.hour > 23){
    g_calendar.hour = 0;
  }
  if(g_calendar.month > 12){
    g_calendar.month = 1;
  }
  if(g_calendar.day > months_days[g_calendar.month]){ //用于调年份和月份时，没调天数出错
    g_calendar.day = 1;
  }
  if(g_calendar.year > 9999){
    g_calendar.year = 1;
  }
  // 基姆拉尔森计算公式
  if(g_calendar.month == 1 || g_calendar.month == 2){
    year_temp --;
    month_temp +=12;
  }
  g_calendar.week =
      (g_calendar.day + 2 * month_temp + 3 * (month_temp + 1) / 5 +
       year_temp + year_temp / 4 - year_temp / 100 +
       year_temp / 400) %
      7 + 1; //注意加一

  calendar_dataTochar(date_buf, time_buf);
  lcd1602_show_string(0, 0, date_buf);
  lcd1602_show_string(0, 1, time_buf);
}

void calendar_time_go1(void) { // 1s过去了
  g_calendar.sec++;
  if (g_calendar.sec >= 60) {
    g_calendar.sec = 0;
    g_calendar.min++;
    if (g_calendar.min >= 60) {
      g_calendar.min = 0;
      g_calendar.hour++;
      if (g_calendar.hour >= 24) {
        g_calendar.hour = 0;
        g_calendar.day++;
        if (g_calendar.day > months_days[g_calendar.month]) {
          g_calendar.day = 1;
          g_calendar.month++;
          if (g_calendar.month > 12) {
            g_calendar.month = 1;
            g_calendar.year++;
            if (g_calendar.year > 9999) {
              g_calendar.year = 0;
            }
          }
        }
      }
    }
  }
  //calendar_show(); 导致出错，程序1s执行，与原来的show函数冲突？
}

void lcd1602_init(void) // LCD1602初始化
{
  lcd1602_write_cmd(0x38); //功能设定指令，数据总线8位，显示2行，5*7点阵/字符
  lcd1602_write_cmd(0x0c); //显示开关控制指令，显示功能开，无光标，光标闪烁
  lcd1602_write_cmd(0x06); //模式设置指令，写入新数据后光标右移，显示屏不移动
  lcd1602_write_cmd(0x01); //清屏指令
}

void lcd1602_write_cmd(u8 cmd) // LCD1602写命令
{
  LCD1602_RS = 0;         //选择命令，低电平
  LCD1602_RW = 0;         //选择写，低电平有效
  LCD1602_E = 0;          //变化期间传输数据
  LCD1602_DATAPORT = cmd; //准备命令，是P0口
  delay_ms(
      1); //变化时间是ns级，当机器很快，小于1ns时，命令还未传输完成，此时需要延迟，对于51来讲，机器周期是1us，可以不延时。
  LCD1602_E = 1; //使能脚E先上升沿写入
  delay_ms(1);   // 延时，让命令写入
  LCD1602_E = 0; //使能脚E后负跳变完成写入
}

void lcd1602_write_data(u8 dat) // LCD1602写数据，data是数据存储器，写dat
{
  LCD1602_RS = 1; //选择数据，高电平
  LCD1602_RW = 0; //选择写
  LCD1602_E = 0;
  LCD1602_DATAPORT = dat; //准备数据
  delay_ms(1);
  LCD1602_E = 1; //使能脚E先上升沿写入
  delay_ms(1);
  LCD1602_E = 0; //使能脚E后负跳变完成写入
}

void lcd1602_clear(void) // 函数功能: LCD1602清屏
{
  lcd1602_write_cmd(0x01);
}

void lcd1602_show_string(
    u8 x, u8 y, u8 *str) //输入: x,y：显示坐标，x=0~15，y=0~1; str：显示字符串
{
  u8 i = 0; // 记录已显示的字符数，超过16则换行，在此日期显示中无用。

  if (y > 1 || x > 15)
    return; //行列参数不对则强制退出

  if (y < 1) //第1行显示
  {
    while (*str != '\0') //字符串是以'\0'结尾，只要前面有内容就显示
    {
      if (i < 16 - x) //如果字符长度超过第一行显示范围，则在第二行继续显示
      {
        lcd1602_write_cmd(
            0x80 + i + x); //第一行显示地址设置从80h开始，x是固定值不变，i在变
      } else {
        lcd1602_write_cmd(0x40 + 0x80 + x + i - 16); //第二行显示地址设置+40h
      }
      lcd1602_write_data(*str); //显示内容
      str++;                    //指针递增
      i++;
    }
  } else //第2行显示
  {
    while (*str != '\0') {
      if (i < 16 - x) //如果字符长度超过第二行显示范围，则在第一行继续显示
      {
        lcd1602_write_cmd(0x80 + 0x40 + i + x);
      } else {
        lcd1602_write_cmd(0x80 + i + x - 16);
      }
      lcd1602_write_data(*str);
      str++;
      i++;
    }
  }
}

// mode=0：单次扫描按键，正常模式时，一直按一个键，只检测一次
// mode=1：连续扫描按键，一直按一个键，可以识别
u8 key_scan(u8 mode) {
  static u8 key = 1;

  if (mode)
    key = 1;
  if (key == 1 &&
      (KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0)) //任意按键按下
  {
    delay_10us(1000); //消抖，抖动大致5-10ms
    key = 0;          //只是单次检测
    if (KEY1 == 0)
      return KEY1_PRESS;
    else if (KEY2 == 0)
      return KEY2_PRESS;
    else if (KEY3 == 0)
      return KEY3_PRESS;
    else if (KEY4 == 0)
      return KEY4_PRESS;
  } else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1) //无按键按下
  {
    key = 1;
  }
  return KEY_UNPRESS;
}

void beep_alarm(u16 time,
                u16 fre) // 蜂鸣器报警函数, time：报警持续时间 fre：报警频率
{
  while (time--) {
    BEEP = !BEEP; //低高电平变化，加延迟，高电平时间决定大小，频率决定音色
    delay_10us(fre);
  }
}
