/****************************************************************************
*       Copyright (C), ������ά���ǽ����Ƽ����޹�˾
*
*       WIFIģ��
*       V0.1
*       2017-06-27
*       Woody
*< STM32F103C8T6 >
			FLASH SIZE:64Kbytes
			RAM SIZE:20Kbytes
			Package:LQFP48
* ����
			FLASH:Kbytes
			RAM:  Kbytes
LED-> PA7

ESP8266  UART1
PA9->TX
PA10->RX

�״�     UART2
PA2->TX
PA3->RX
*****************************************************************************/

#include "includes.h"

int Lidar_value=0;  //��¼��ȡ���״�����.
uint16_t Lidar_frequency=0; //��¼����
uint8_t Lidar_sends_flag=0;  //���ͱ�־λ
uint8_t errtime=0;

int Lidar_buffer[256];
int lidardatas=0;

int main(void) {
    SystemInit();        //ϵͳʱ�ӵȳ�ʼ��
    delay_init(72);      //����ϵͳʱ��72Mhz��ʼ��
    NVIC_Configuration();//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    Platform_Init();

    //printf("\r\n�_ʼ������\r\n");
    send_Control();
    while (1) {
        Lidar_value = Get_Lidar_value();
        if(Lidar_value) {
#if DEBUG_Printf
            printf("%d\r\n",Lidar_value);
#endif
            if(Lidar_value != -1) { //������
                if((Lidar_value<Lidar_Height)&&(errtime == 0)) { //С��200cm�зɻ�
                    if(Lidar_frequency < 100) {
                        Lidar_sends_flag=1;  //��־λ 1����ʾ���Է���
                        Lidar_buffer[Lidar_frequency] = Lidar_value;
                        Lidar_frequency++;
                    } else {
                        errtime=1;
                        delay_ms(800);
                    }
                } else if(Lidar_sends_flag) {
                    errtime=0;
                    if(Lidar_frequency>20) Lidar_frequency = 20;
                    maopao_XD((int *)Lidar_buffer,Lidar_frequency); //��С����
                    if(Lidar_buffer[0] <= Lidar_Height) { //��СֵС������ֵ
                        LED_Normal_H;
                        lidardatas = Lidar_buffer[0];
                        if((lidardatas !=0 )&&(lidardatas>5)) {
                            Send_T_BlueTooth(1,lidardatas,Lidar_Time); //��������ʱ��
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
                    Send_T_BlueTooth(0,0,Lidar_Time);    //����json��������
            }
        }
    }
}
/***********************************************************************/

void Platform_Init(void) {
    TIM4_Init(500,7199);         //ϵͳ�����������ж�ʱ��
    TIM3_Init(10,7199);          //1ms ��ʱ�� ���ڲ���ʱ�� 1ms��ʱ
    uart1_init(115200);          //����1��ʼ�������ڴ��ڷ�������
    uart2_init(115200);          //����2��ʼ�������ڶ�ȡ�����״ﴫ����
    LED_Init();                  //LEDЧ���Ƴ�ʼ��
}



//{"s":"0","d":"1042","t":"6"}
void Send_T_BlueTooth(bool s,int d,int t) {
    char PRemove[32];
    uint8_t packingData[128];        //���800�ֽ�

    strcpy((char *) packingData, "{\"s\":\"");
    sprintf((char *)PRemove,"%d",s);
    strcat((char *) packingData, (char *)PRemove);  //�Զ����ƿ���on/off.;
    strcat((char *) packingData, "\",\"d\":\"");
    sprintf((char *)PRemove,"%d",d);
    strcat((char *) packingData, (char *)PRemove);  //�Զ����ƿ���on/off.;
    strcat((char *) packingData, "\",\"t\":\"");
    sprintf((char *)PRemove,"%d",t);
    strcat((char *) packingData, (char *)PRemove);  //�Զ����ƿ���on/off.;
    strcat((char *) packingData, "\"}\r\n");

    SendMessagePacket((char *)packingData);
}


/*****END OF FILE****/












