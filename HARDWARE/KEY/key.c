//********************************************************************
//
//      代码功能：按键初始化代码
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"



//PA4 蓝牙连接判断 
void KeyInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 按键PA4 上拉输入
uint8_t Check_STATE(void) {
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET)
		return 0;
	else 
		return 1;
	
}





