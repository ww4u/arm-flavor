
#include "main.h"
/********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
*			 全局变量。
*	形    参：无
*	返 回 值: 无
********************************************************************************************************/
void bsp_Init(void)
{
	/*	由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。
		系统时钟缺省配置为72MHz，如果需要更改，可以修改 system_stm32f103.c 文件		*/

	//	优先级分组设置为
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	
	bsp_InitLed();		/* 配置LED的GPIO端口 */
	//bsp_InitKey();		/* 初始化按键 */	
	bsp_InitTimer();	/* 初始化系统滴答定时器 (此函数会开中断) */	
	bsp_InitUart();		/* 初始化串口驱动 */
}

void bsp_sTimer_CallBack1(void)
{
	bsp_LedOff(1);
	pumpStatusSet(1,0);
}

void bsp_sTimer_CallBack2(void)
{
	bsp_LedOff(2);
	pumpStatusSet(2,0);
}

void bsp_sTimer_CallBack3(void)
{
	bsp_LedOff(3);
	pumpStatusSet(3,0);
}

void bsp_sTimer_CallBack4(void)
{
	bsp_LedOff(4);
	pumpStatusSet(4,0);
}

void bsp_sTimer_CallBack5(void)
{
	bsp_LedOff(5);
	pumpStatusSet(5,0);
}


/*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
extern void SaveScreenToBmp(uint16_t _index);
void bsp_Idle(void)
{
	/* --- 喂狗 */
	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */
	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();
	/* 例如 uIP 协议，可以插入uip轮询函数 */
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
