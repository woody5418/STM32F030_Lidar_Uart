/********************************************************************
 *@file        led.h
 *@author      Woody      QQ��2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __LED_H
#define __LED_H
#include "sys.h"



//LED�˿ڶ���
#define LED_Normal_Green   PAout(7) //PCout(13)  //����LED��

#define LED_Normal_H LED_Normal_Green=1
#define LED_Normal_L LED_Normal_Green=0


void LED_Init(void);//��ʼ��

#endif

















