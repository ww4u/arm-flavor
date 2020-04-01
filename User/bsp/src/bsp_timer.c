#include "bsp.h"

//��������Ӳ����ʱ����TIM
#define TIM_HARD		TIM2
#define TIM_HARD_IRQn	TIM2_IRQn
#define TIM_HARD_RCC	RCC_APB1Periph_TIM2

/*******************************************************************************************************/
//SysTick_Config
//���������ʱ���ṹ������ͺ�������
static SOFT_TMR s_tTmr[TMR_COUNT];

//ȫ������ʱ�䣬��λ1ms
static volatile uint64_t g_iRunTime = 0;

//ȫ������ʱ�䣬ʱ����
volatile uint32_t s_uiHour = 0;
volatile uint32_t hourAddr = 0;
static volatile uint16_t s_uiSecond = 0;
static volatile uint16_t s_uiMilliS = 0;


// ��2��ȫ�ֱ���ת���� bsp_DelayMS() ����
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

void (*s_TIM_CallBack)(void);	// �������ص�����


/********************************************************************************************************
*	�� �� ��: bsp_InitTimer
*	����˵��: ����systick�жϣ�����ʼ�������ʱ������
*	��    ��:  ��
*	�� �� ֵ: ��
********************************************************************************************************/
void bsp_InitTimer(void)
{
	uint8_t i;
	//�������е������ʱ�����ȳ�ʼ���������ж�
	for (i = 0; i < TMR_COUNT; i++)
	{
		s_tTmr[i].Count = 0;
		s_tTmr[i].PreLoad = 0;
		s_tTmr[i].Flag = 0;
		s_tTmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
		s_tTmr[i].s_TIM_CallBack = NULL;
	}
	s_tTmr[1].s_TIM_CallBack = bsp_sTimer_CallBack1;
	s_tTmr[2].s_TIM_CallBack = bsp_sTimer_CallBack2;
	s_tTmr[3].s_TIM_CallBack = bsp_sTimer_CallBack3;
	s_tTmr[4].s_TIM_CallBack = bsp_sTimer_CallBack4;
	s_tTmr[5].s_TIM_CallBack = bsp_sTimer_CallBack5;
	
	/*	����systic�ж�����Ϊ1ms��������systick�жϡ�
	SystemCoreClock �ǹ̼��ж����ϵͳ�ں�ʱ�ӣ�����STM32F4XX,һ��Ϊ 168MHz
	SysTick_Config() �������βα�ʾ�ں�ʱ�Ӷ��ٸ����ں󴥷�һ��Systick��ʱ�ж�.
		-- SystemCoreClock / 1000  ��ʾ��ʱƵ��Ϊ 1000Hz�� Ҳ���Ƕ�ʱ����Ϊ  1ms
		-- SystemCoreClock / 500   ��ʾ��ʱƵ��Ϊ 500Hz��  Ҳ���Ƕ�ʱ����Ϊ  2ms
		-- SystemCoreClock / 2000  ��ʾ��ʱƵ��Ϊ 2000Hz�� Ҳ���Ƕ�ʱ����Ϊ  500us
	���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms	*/
	SysTick_Config(SystemCoreClock / 1000);
	//bsp_InitHardTimer();
}


/********************************************************************************************************
*	�� �� ��: bsp_hourTimer
*	����˵��: 
*	��    ��: 
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_SoftTimerDec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    ��:  _tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
********************************************************************************************************/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->Count > 0){
		/* �����ʱ����������1�����ö�ʱ�������־ */
		if (--_tmr->Count <= 0){			
			_tmr->Flag = 1;
			_tmr->s_TIM_CallBack();
			/* ������Զ�ģʽ�����Զ���װ������ */
			if(_tmr->Mode == TMR_AUTO_MODE) _tmr->Count = _tmr->PreLoad;
		}
	}
}
/********************************************************************************************************
*	�� �� ��: bsp_SystemTimeDec
*	����˵��: ÿ��1ms�����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    ��: 
*	�� �� ֵ: ��
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
	if (s_uiMilliS % 10 == 0){/* ÿ��10ms����һ�δ˺������˺����� bsp.c */}
}

/********************************************************************************************************
*	�� �� ��: SysTick_Handler
*	����˵��: ϵͳ��શ�ʱ���жϷ�����������ļ��������˸ú�����
*	��    ��:  ��
*	�� �� ֵ: ��
********************************************************************************************************/
void SysTick_Handler(void)
{
	//ȫ������ʱ��ÿ1ms��1
	bsp_SystemTimeDec();
	//ÿ��1ms���������ʱ���ļ��������м�һ����
	for (uint8_t i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&s_tTmr[i]);
	}
}

/********************************************************************************************************
*	�� �� ��: bsp_StartTimer
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ1ms
*	�� �� ֵ: ��
********************************************************************************************************/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		return;
		//while(1); /* �����쳣�������ȴ����Ź���λ */		
	}

	DISABLE_INT();  			/* ���ж� */
	s_tTmr[_id].Count = _period;		/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */
	ENABLE_INT();  				/* ���ж� */
}

/********************************************************************************************************
*	�� �� ��: bsp_StartAutoTimer
*	����˵��: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
********************************************************************************************************/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  		/* ���ж� */
	s_tTmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	s_tTmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */
	ENABLE_INT();  			/* ���ж� */
}

/********************************************************************************************************
*	�� �� ��: bsp_StopTimer
*	����˵��: ֹͣһ����ʱ��
*	��    ��:  	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ��
********************************************************************************************************/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* ��ӡ�����Դ�����ļ������������� */
		BSP_Printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		while(1); /* �����쳣�������ȴ����Ź���λ */
	}

	DISABLE_INT();  	/* ���ж� */
	s_tTmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	s_tTmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	s_tTmr[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */
	ENABLE_INT();  		/* ���ж� */
}


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
