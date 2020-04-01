/********************************************************************************************************
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
********************************************************************************************************/

#include "bsp.h"

#define RCC_ALL_LED 	(RCC_APB2Periph_GPIOE)

#define GPIO_PORT_LED1  GPIOE
#define GPIO_PIN_LED1	GPIO_Pin_0

#define GPIO_PORT_LED2  GPIOE
#define GPIO_PIN_LED2	GPIO_Pin_1

#define GPIO_PORT_LED3  GPIOE
#define GPIO_PIN_LED3	GPIO_Pin_2

#define GPIO_PORT_LED4  GPIOE
#define GPIO_PIN_LED4	GPIO_Pin_3

#define GPIO_PORT_LED5  GPIOE
#define GPIO_PIN_LED5	GPIO_Pin_4

#define GPIO_PIN_TOTAL	5

/********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
********************************************************************************************************/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		配置所有的LED指示灯GPIO为推挽输出模式
		由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
		这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/
	for (uint8_t i=1; i<=GPIO_PIN_TOTAL; i++)
	{
		bsp_LedOff(i);
	}

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 推挽输出模式 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
	GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
	GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED5;
	GPIO_Init(GPIO_PORT_LED5, &GPIO_InitStructure);
}

/********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
********************************************************************************************************/
void bsp_LedOn(uint8_t _no)
{	
	switch (_no)
	{
		case 1:
			GPIO_PORT_LED1->BRR = GPIO_PIN_LED1;
			break;
		case 2:
			GPIO_PORT_LED2->BRR = GPIO_PIN_LED2;
			break;
		case 3:
			GPIO_PORT_LED3->BRR = GPIO_PIN_LED3;
			break;
		case 4:
			GPIO_PORT_LED4->BRR = GPIO_PIN_LED4;
			break;
		case 5:
			GPIO_PORT_LED5->BRR = GPIO_PIN_LED5;
			break;
		default:
			break;
	}
}

/********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
********************************************************************************************************/
void bsp_LedOff(uint8_t _no)
{
	switch (_no)
	{
		case 1:
			GPIO_PORT_LED1->BSRR = GPIO_PIN_LED1;
			break;
		case 2:
			GPIO_PORT_LED2->BSRR = GPIO_PIN_LED2;
			break;
		case 3:
			GPIO_PORT_LED3->BSRR = GPIO_PIN_LED3;
			break;
		case 4:
			GPIO_PORT_LED4->BSRR = GPIO_PIN_LED4;
			break;
		case 5:
			GPIO_PORT_LED5->BSRR = GPIO_PIN_LED5;
			break;
		default:
			break;
	}
}

/********************************************************************************************************
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
********************************************************************************************************/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	switch (_no)
	{
		case 1:
			if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) == 0) return 1;
			break;
		case 2:
			if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) == 0) return 1;
			break;
		case 3:
			if ((GPIO_PORT_LED3->ODR & GPIO_PIN_LED3) == 0) return 1;
			break;
		case 4:
			if ((GPIO_PORT_LED4->ODR & GPIO_PIN_LED4) == 0) return 1;
			break;
		case 5:
			if ((GPIO_PORT_LED5->ODR & GPIO_PIN_LED5) == 0) return 1;
			break;
		default:
			break;
	}
	return 0;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
