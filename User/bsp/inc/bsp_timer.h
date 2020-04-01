/********************************************************************************************************
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.h
*	��    �� : V1.3
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
********************************************************************************************************/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "stm32f10x.h"

#define TMR_COUNT	6		// �����ʱ���ĸ��� ����ʱ��ID��Χ 0 - 4)

// ��ʱ��ö����
typedef enum
{
	TMR_CLOSE_MODE = 0,		// ������ģʽ
	TMR_ONCE_MODE  = 1,		// һ�ι���ģʽ
	TMR_AUTO_MODE  = 2		// �ظ�����ģʽ
}TMR_MODE_E;

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;			// ������ģʽ
	volatile uint8_t Flag;			// ��ʱ�����־
	volatile uint32_t Count;		// ����������ֵ
	volatile uint32_t PreLoad;		// ������Ԥװֵ
	void (*s_TIM_CallBack)(void);	// �������ص�����
}SOFT_TMR;

/* �ṩ������C�ļ����õĺ��� */
void bsp_InitTimer(void);

void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);

void bsp_hourCheck(void);
u32 bsp_hourReturn(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
