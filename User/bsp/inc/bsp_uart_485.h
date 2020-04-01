/********************************************************************************************************
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
********************************************************************************************************/
#ifndef _BSP_USART_485_H_
#define _BSP_USART_485_H_

#include "stm32f10x.h"

/*	������3�� RS485 ͨ�� 		--- TTL ���� �� ����
		PB10/USART3_TX	PB11/USART3_RX	PB2/BOOT1/RS485_TX_EN	*/

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART3_FIFO_EN==1
	#define UART3_BAUD			9600
	#define UART3_TX_BUF_SIZE	1*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif

/* RS485оƬ����ʹ��GPIO, PB2 */
#define RCC_RS485_TXEN 	 RCC_APB2Periph_GPIOB
#define PORT_RS485_TXEN  GPIOB
#define PIN_RS485_TXEN	 GPIO_Pin_2

#define RS485_RX_EN()	PORT_RS485_TXEN->BRR = PIN_RS485_TXEN
#define RS485_TX_EN()	PORT_RS485_TXEN->BSRR = PIN_RS485_TXEN


void bsp_Set485Baud(uint32_t _baud);

void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
void RS485_SendStr(char *_pBuf);

#endif
/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
