/********************************************************************
 *@file        usart2.h
 *@author      Woody      QQ：2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"


#define USART2_REC_LEN		64					//最大接收缓存字节数

extern u8 USART2_RX_BUF[USART2_REC_LEN];
extern u8 USART2_RX_STA;


void uart2_init(u32 baud);
void send_Control(void);
int Get_Lidar_value(void);
void bubble_sort(int *a, int n);
void maopao_XD(int *a,int N);
void maopao_DX(int *a,int N);

#endif






