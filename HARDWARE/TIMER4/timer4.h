/********************************************************************
 *@file        timer4.h
 *@author      Woody      QQ£º2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief
 *******************************************************************/
#ifndef __TIMER4_H
#define __TIMER4_H

#include "stm32f10x.h"


extern uint8_t Flag_Send_Data;


void TIM4_Init(u16 arr,u16 psc);

#endif





