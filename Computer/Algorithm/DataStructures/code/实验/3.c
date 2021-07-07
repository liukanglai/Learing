#include<stdio.h>
#include<string.h>

int main()
{
    char temp[9], disks[1000][8000]={""}; // disks存储磁盘数据, temp 用于求异或时暂时存储，在原题中还要求输入字符的长度有限制，较麻烦，，这里忽略，大概一下吧。
    int n, s, l, m, bi, Sumblock = 0;
    scanf("%d%d%d", &n, &s, &l);
    int Ndisk; // 磁盘号
    for(int i = 0; i < l; i++){ // 读取l块磁盘的数据
        scanf("%d%*c", &Ndisk); // %*c, ingnored  the ' '.
        //int j = 0;
        fgets(disks[Ndisk],8000,stdin); 
        /*
        for( ; j < 8000; j++){
            if((disks[Ndisk][j] = getchar()) != '\n'){
                break;
            }
        } // fgets(disks[Ndisk],8000,stdin); 网上有这个输入，我学的还不到家啊。。。
        */
        //disks[Ndisk][j] = '\0';
        Sumblock = strlen(disks[Ndisk]) / 8; // 存储在一块磁盘上的块数，一块四个字节，32位，存8个字符？？？（这有点。） 还是看题目要求的输出吧。要是两块磁盘上的数据长度不一样怎么办？算了。。
    }
    scanf("%d", &m);
    while(m--){
        scanf("%d", &bi); // bi块编号，注意块编号与条带号均从0开始。
        int band = bi / s, Band = band / (n-1), diskbi = band % (n-1), Bi = Band*s + bi%s;  // band: bi的条带号, Band：bi所在磁盘上前面有多少条带, s > 0, n >= 2. diskbi：bi所在磁盘号, Bi：bi在所在磁盘上的块号, 这2个量找到，题目基本完了。
        if(Bi >= Sumblock || (disks[diskbi][0] == '\0' && n-l > 1)) // 块号超过磁盘上的块数或者该磁盘被损坏且坏掉的磁盘超过1个
            printf("-\n");
        else if(disks[diskbi][0] != '\0'){ //该磁盘数据完好，直接输出对应块的数据即可
            for(int i = 0; i < 8; i++)
                putchar(disks[diskbi][Bi*8 + i]);
            printf("\n");
        }
        else{ //该磁盘被损坏，但数据可恢复
            int a = 0, k;
            for(int i = 0; i < n; i++)//遍历其他的块
                if(diskbi != i){
                    for(int j = 0; j < 8; j++) //将对应的8个字符复制粘贴到temp中
                        temp[j]=disks[i][Bi*8 + j];
                    temp[8]='\0'; //temp末尾字符置\0
                    sscanf(temp, "%x", &k); //将16进制字符串转换为10进制数, .......
                    a ^= k; //异或
                }
            printf("%08X\n",a); //输出8位16进制字符串，不够8位在高位补0
        }
    }
    return 0;
}
