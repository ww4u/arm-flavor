/********************************************************************************************************
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.h
*	版    本 : V1.3
*	说    明 : 头文件
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
********************************************************************************************************/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "stm32f10x.h"

#define TMR_COUNT	6		// 软件定时器的个数 （定时器ID范围 0 - 4)

// 定时器枚举体
typedef enum
{
	TMR_CLOSE_MODE = 0,		// 不工作模式
	TMR_ONCE_MODE  = 1,		// 一次工作模式
	TMR_AUTO_MODE  = 2		// 重复工作模式
}TMR_MODE_E;

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;			// 计数器模式
	volatile uint8_t Flag;			// 定时到达标志
	volatile uint32_t Count;		// 计数器计数值
	volatile uint32_t PreLoad;		// 计数器预装值
	void (*s_TIM_CallBack)(void);	// 计数器回调函数
}SOFT_TMR;

/* 提供给其他C文件调用的函数 */
void bsp_InitTimer(void);

void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);

void bsp_hourCheck(void);
u32 bsp_hourReturn(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
