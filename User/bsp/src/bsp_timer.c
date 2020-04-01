#include "bsp.h"

//定义用于硬件定时器的TIM
#define TIM_HARD		TIM2
#define TIM_HARD_IRQn	TIM2_IRQn
#define TIM_HARD_RCC	RCC_APB1Periph_TIM2

/*******************************************************************************************************/
//SysTick_Config
//定义软件定时器结构体变量和函数声明
static SOFT_TMR s_tTmr[TMR_COUNT];

//全局运行时间，单位1ms
static volatile uint64_t g_iRunTime = 0;

//全局运行时间，时分秒
volatile uint32_t s_uiHour = 0;
volatile uint32_t hourAddr = 0;
static volatile uint16_t s_uiSecond = 0;
static volatile uint16_t s_uiMilliS = 0;


// 这2个全局变量转用于 bsp_DelayMS() 函数
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

void (*s_TIM_CallBack)(void);	// 计数器回调函数


/********************************************************************************************************
*	函 数 名: bsp_InitTimer
*	功能说明: 配置systick中断，并初始化软件定时器变量
*	形    参:  无
*	返 回 值: 无
********************************************************************************************************/
void bsp_InitTimer(void)
{
	uint8_t i;
	//清零所有的软件定时器，先初始化变量后开中断
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
		s_tTmr[i].s_TIM_CallBack = NULL;
	}
	s_tTmr[1].s_TIM_CallBack = bsp_sTimer_CallBack1;
	s_tTmr[2].s_TIM_CallBack = bsp_sTimer_CallBack2;
	s_tTmr[3].s_TIM_CallBack = bsp_sTimer_CallBack3;
	s_tTmr[4].s_TIM_CallBack = bsp_sTimer_CallBack4;
	s_tTmr[5].s_TIM_CallBack = bsp_sTimer_CallBack5;
	
	/*	配置systic中断周期为1ms，并启动systick中断。
	SystemCoreClock 是固件中定义的系统内核时钟，对于STM32F4XX,一般为 168MHz
	SysTick_Config() 函数的形参表示内核时钟多少个周期后触发一次Systick定时中断.
		-- SystemCoreClock / 1000  表示定时频率为 1000Hz， 也就是定时周期为  1ms
		-- SystemCoreClock / 500   表示定时频率为 500Hz，  也就是定时周期为  2ms
		-- SystemCoreClock / 2000  表示定时频率为 2000Hz， 也就是定时周期为  500us
	对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用，可以设置定时周期为 10ms	*/
	SysTick_Config(SystemCoreClock / 1000);
	//bsp_InitHardTimer();
}


/********************************************************************************************************
*	函 数 名: bsp_hourTimer
*	功能说明: 
*	形    参: 
*	返 回 值: 无
********************************************************************************************************/
void bsp_hourTimer(void)
{	
	bspJumpWrite( (u32*)&hourAddr, s_uiHour, 0x806F8000, 500);
}

void bsp_hourCheck(void)
{	
	printf("%d:%2d:%2d\n",s_uiHour, s_uiSecond/60, s_uiSecond%60);
}

u32 bsp_hourReturn(void)
{
	return s_uiHour;
}

/********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参:  _tmr : 定时器变量指针
*	返 回 值: 无
********************************************************************************************************/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0){
		/* 如果定时器变量减到1则设置定时器到达标志 */
		if (--_tmr->Count <= 0){			
			_tmr->Flag = 1;
			_tmr->s_TIM_CallBack();
			/* 如果是自动模式，则自动重装计数器 */
			if(_tmr->Mode == TMR_AUTO_MODE) _tmr->Count = _tmr->PreLoad;
		}
	}
}
/********************************************************************************************************
*	函 数 名: bsp_SystemTimeDec
*	功能说明: 每隔1ms对所有定时器变量加1。必须被SysTick_ISR周期性调用。
*	形    参: 
*	返 回 值: 无
********************************************************************************************************/
static void bsp_SystemTimeDec(void)
{	
	//g_iRunTime++;
	s_uiMilliS++;
	if (s_uiMilliS >= 1000)
	{
		s_uiMilliS=0;
		s_uiSecond++;
		if(s_uiSecond >= 3600)
		{
			s_uiSecond=0;
			s_uiHour++;
			bsp_hourTimer();
		}			
	}
	if (s_uiMilliS % 10 == 0){/* 每隔10ms调用一次此函数，此函数在 bsp.c */}
}

/********************************************************************************************************
*	函 数 名: SysTick_Handler
*	功能说明: 系统嘀嗒定时器中断服务程序。启动文件中引用了该函数。
*	形    参:  无
*	返 回 值: 无
********************************************************************************************************/
void SysTick_Handler(void)
{
	//全局运行时间每1ms增1
	bsp_SystemTimeDec();
	//每隔1ms，对软件定时器的计数器进行减一操作
	for (uint8_t i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&s_tTmr[i]);
	}
}

/********************************************************************************************************
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位1ms
*	返 回 值: 无
********************************************************************************************************/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		return;
		//while(1); /* 参数异常，死机等待看门狗复位 */		
	}

	DISABLE_INT();  			/* 关中断 */
	s_tTmr[_id].Count = _period;		/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */
	ENABLE_INT();  				/* 开中断 */
}

/********************************************************************************************************
*	函 数 名: bsp_StartAutoTimer
*	功能说明: 启动一个自动定时器，并设置定时周期。
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位10ms
*	返 回 值: 无
********************************************************************************************************/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  		/* 关中断 */
	s_tTmr[_id].Count = _period;			/* 实时计数器初值 */
	s_tTmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */
	ENABLE_INT();  			/* 开中断 */
}

/********************************************************************************************************
*	函 数 名: bsp_StopTimer
*	功能说明: 停止一个定时器
*	形    参:  	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 无
********************************************************************************************************/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* 打印出错的源代码文件名、函数名称 */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* 参数异常，死机等待看门狗复位 */
	}

	DISABLE_INT();  	/* 关中断 */
	s_tTmr[_id].Count = 0;				/* 实时计数器初值 */
	s_tTmr[_id].Flag = 0;				/* 定时时间到标志 */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */
	ENABLE_INT();  		/* 开中断 */
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
