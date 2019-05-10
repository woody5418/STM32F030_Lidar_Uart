/********************************************************************
 *@file        usart1.h
 *@author      Woody      QQ：2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include <stdio.h>


#define USART_REC_LEN		128			//最大接收缓存字节数

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;         		//接收状态标记


void uart1_init(u32 baud);
void SendMessagePacket(char *message);
#endif


