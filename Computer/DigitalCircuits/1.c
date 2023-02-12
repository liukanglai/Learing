// TMOD 工作方式
// 赋时间初值，写入TH0，TL0，TH1，TL1
// 如果使用中断，对EA赋值，开放定时器中断
// 使TR0或TR1置位，启动定时器定时或计数。
// 1us，1机器周期

#include "reg52.h"

typedef unsigned int u16; //对系统默认数据类型进行重定义
typedef unsigned char u8;

sbit LED1 = P2 ^ 0; //定义LED1管脚

void delay_10us(u16 ten_us) // 函数功能: 延时函数，ten_us=1时，大约延时10us
{
  while (ten_us--)
    ;
}

void time0_init(
    void) // 函数功能: 定时器0中断配置函数，通过设置TH和TL即可确定定时时间
{
  TMOD |= 0X01; //选择为定时器0模式，工作方式1
  TH0 = 0XFC;   //给定时器赋初值，定时1ms
  TL0 = 0X18;
  ET0 = 1; //打开定时器0中断允许
  EA = 1;  //打开总中断
  TR0 = 1; //打开定时器
}

void main() {
  time0_init(); //定时器0中断配置

  while (1) {
  }
}

void time0() interrupt 1 //定时器0中断函数
{
  static u16 i; //定义静态变量i
  TH0 = 0XFC;   //给定时器赋初值，定时1ms
  TL0 = 0X18;
  i++;
  if (i == 1000) {
    i = 0;
    LED1 = !LED1;
  }
}
