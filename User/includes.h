/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM32F103 Evaluation Board
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*************************************************************************************************/
#ifndef __INCLUDES_H
#define __INCLUDES_H
#include "stm32f10x.h"


/*
 *常用头文件
 */
#include "delay.h"
#include "sys.h"
/*
 *驱动库头文件
 */

#include "usart1.h"
#include "usart2.h"
#include "timer3.h"
#include "timer4.h"
#include "key.h"
#include "led.h"
/*
 *C语言库头文件
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdbool.h"

#define 	DEBUG_Printf 	0
#define 	Lidar_Height 	200


void Platform_Init(void);
void Send_T_BlueTooth(bool s,int d,int t);

#endif
/**********************************     END     **********************************/









