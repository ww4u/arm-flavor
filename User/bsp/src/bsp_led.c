/********************************************************************************************************
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
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
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
********************************************************************************************************/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		�������е�LEDָʾ��GPIOΪ�������ģʽ
		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/
	for (uint8_t i=1; i<=GPIO_PIN_TOTAL; i++)
	{
		bsp_LedOff(i);
	}

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */	
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
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
