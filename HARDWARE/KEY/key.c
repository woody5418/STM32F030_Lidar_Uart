//********************************************************************
//
//      ���빦�ܣ�������ʼ������
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"



//PA4 ���������ж� 
void KeyInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// ����PA4 ��������
uint8_t Check_STATE(void) {
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET)
		return 0;
	else 
		return 1;
	
}





