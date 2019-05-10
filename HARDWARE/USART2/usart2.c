//********************************************************************
//
//      ���빦�ܣ�����2��ʼ������
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


u8 USART2_RX_STA=0;  //����״̬���
//���ڽ��ջ�����
u8 USART2_RX_BUF[USART2_REC_LEN];//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.



void uart2_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    //USART1_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX	PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���
}
uint8_t flag=0;
//����2�жϽ���
//MVR1EB,0030,06
//4D 56 52 31 45 42 2C 30 30 33 30 2C 30 36 0D 0A
void USART2_IRQHandler(void)                	//����2�жϷ������
{
    u8 Res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) { //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
        Res =USART_ReceiveData(USART2);
        if(Res == 0x4D) {
            flag =1;
        }
        if(flag == 1) {
            if((USART2_RX_STA&0x80)==0) {
                if(USART2_RX_STA&0x40) {
                    if(Res!=0x0a) {
                        flag=0;
                        USART2_RX_STA=0;
                    } else {
                        flag=0;
                        USART2_RX_STA|=0x80;
                    }
                } else {
                    if(Res==0x0d)USART2_RX_STA|=0x40;
                    else {
                        USART2_RX_BUF[USART2_RX_STA&0X3F]=Res ;
                        USART2_RX_STA++;
                        if(USART2_RX_STA>(USART2_REC_LEN-1)) {
                            flag=0;
                            USART2_RX_STA=0;
                        }
                    }
                }
            }
        }
    }
}


//	�˲��㷨С����
void maopao_XD(int *a,int N)
{
	int i,j,t;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N-1;j++)
		{
			if(a[j]>a[j+1])
			{
				t=a[j];
				a[j]=a[j+1];
				a[j+1]=t;
			}
		}
	}
}
//	�˲��㷨��С
void maopao_DX(int *a,int N)
{
	int i,j,t;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N-1;j++)
		{
			if(a[j]<a[j+1])
			{
				t=a[j];
				a[j]=a[j+1];
				a[j+1]=t;
			}
		}
	}
}

/**
 * ��ȡ�״������
 * ����ֵΪ-1��ʾ���ܴ������
 */
int Get_Lidar_value(void)
{
	char *temp_t;
	int  temp_num=0;
    if(USART2_RX_STA&0x80) {	
#if DEBUG_Printf
        SendMessagePacket((char *)USART2_RX_BUF);
#endif	
		temp_t = strtok((char *)USART2_RX_BUF,",");
		temp_t = strtok(NULL,",");
		temp_num = atoi(temp_t);
#if DEBUG_Printf
		printf("%d\r\n",atoi(temp_t));
#endif
        USART2_RX_STA=0;
        return temp_num;
    } else {
		return -1;
	}
}


void send_Control(void)
{
	USART_SendData(USART2,0x14);
	delay_ms(1000);
    USART_SendData(USART2,0x71);
    delay_ms(100);
    USART_SendData(USART2,0x50);
    //delay_ms(1);
}

//�������˲�
double KalmanFilter(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
    double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;

    static double x_last;

    double x_mid = x_last;
    double x_now;

    static double p_last;

    double p_mid ;
    double p_now;
    double kg;

    x_mid=x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid=p_last+Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
    kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
    x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ

    p_now=(1-kg)*p_mid;//����ֵ��Ӧ��covariance

    p_last = p_now; //����covarianceֵ
    x_last = x_now; //����ϵͳ״ֵ̬

    return x_now;
}

