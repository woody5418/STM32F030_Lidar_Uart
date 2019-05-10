/********************************************************************
 *@file        delay.h
 *@author      Woody      QQ£º2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























