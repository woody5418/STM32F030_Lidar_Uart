/****************************************************************************
*       Copyright (C), 北京四维拓智教育科技有限公司
*
*       WIFI模块
*       V0.1
*       2017-06-27
*       Woody
*< STM32F103C8T6 >
			FLASH SIZE:64Kbytes
			RAM SIZE:20Kbytes
			Package:LQFP48
* 已用
			FLASH:Kbytes
			RAM:  Kbytes
LED-> PA7

ESP8266  UART1
PA9->TX
PA10->RX

雷达     UART2
PA2->TX
PA3->RX
*****************************************************************************/

#include "includes.h"

int Lidar_value=0;  //记录获取的雷达数据.
uint16_t Lidar_frequency=0; //记录次数
uint8_t Lidar_sends_flag=0;  //发送标志位
uint8_t errtime=0;

int Lidar_buffer[256];
int lidardatas=0;

int main(void) {
    SystemInit();        //系统时钟等初始化
    delay_init(72);      //根据系统时钟72Mhz初始化
    NVIC_Configuration();//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    Platform_Init();

    //printf("\r\n開始工作！\r\n");
    send_Control();
    while (1) {
        Lidar_value = Get_Lidar_value();
        if(Lidar_value) {
#if DEBUG_Printf
            printf("%d\r\n",Lidar_value);
#endif
            if(Lidar_value != -1) { //有数据
                if((Lidar_value<Lidar_Height)&&(errtime == 0)) { //小于200cm有飞机
                    if(Lidar_frequency < 100) {
                        Lidar_sends_flag=1;  //标志位 1：表示可以发送
                        Lidar_buffer[Lidar_frequency] = Lidar_value;
                        Lidar_frequency++;
                    } else {
                        errtime=1;
                        delay_ms(800);
                    }
                } else if(Lidar_sends_flag) {
                    errtime=0;
                    if(Lidar_frequency>20) Lidar_frequency = 20;
                    maopao_XD((int *)Lidar_buffer,Lidar_frequency); //从小到大
                    if(Lidar_buffer[0] <= Lidar_Height) { //最小值小于设置值
                        LED_Normal_H;
                        lidardatas = Lidar_buffer[0];
                        if((lidardatas !=0 )&&(lidardatas>5)) {
                            Send_T_BlueTooth(1,lidardatas,Lidar_Time); //发送数据时间
                        }
						delay_ms(500);
						USART2_RX_STA=0;
						memset(USART2_RX_BUF,0,64);
                        LED_Normal_L;
                    }
                    Lidar_frequency=0;
                    Lidar_sends_flag=0;
                    Lidar_value=0;
                }
            } else if(Flag_Send_Data == 1) {
                Flag_Send_Data = 0;
                if(Lidar_sends_flag !=1)
                    Send_T_BlueTooth(0,0,Lidar_Time);    //发送json到服务器
            }
        }
    }
}
/***********************************************************************/

void Platform_Init(void) {
    TIM4_Init(500,7199);         //系统正常工作运行定时器
    TIM3_Init(10,7199);          //1ms 定时器 用于测量时间 1ms定时
    uart1_init(115200);          //串口1初始化用作于串口发送数据
    uart2_init(115200);          //串口2初始化用作于读取激光雷达传感器
    LED_Init();                  //LED效果灯初始化
}



//{"s":"0","d":"1042","t":"6"}
void Send_T_BlueTooth(bool s,int d,int t) {
    char PRemove[32];
    uint8_t packingData[128];        //最大800字节

    strcpy((char *) packingData, "{\"s\":\"");
    sprintf((char *)PRemove,"%d",s);
    strcat((char *) packingData, (char *)PRemove);  //自动控制开关on/off.;
    strcat((char *) packingData, "\",\"d\":\"");
    sprintf((char *)PRemove,"%d",d);
    strcat((char *) packingData, (char *)PRemove);  //自动控制开关on/off.;
    strcat((char *) packingData, "\",\"t\":\"");
    sprintf((char *)PRemove,"%d",t);
    strcat((char *) packingData, (char *)PRemove);  //自动控制开关on/off.;
    strcat((char *) packingData, "\"}\r\n");

    SendMessagePacket((char *)packingData);
}


/*****END OF FILE****/












