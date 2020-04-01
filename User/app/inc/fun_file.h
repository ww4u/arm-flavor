//--<Head>--
#ifndef FUN_FILE_H
#define FUN_FILE_H

//--<Include>--
#include "bsp.h"

#define PUMPOPEN     0
#define PUMPCLOSE    1

#define PUMPMINUTE   3
#define PUMP_LIMIT   6		//1+5
#define PUMP_ALL     0

#define FLASH_PAGE   0x0800
#define FLASH_U32    0x0004

#define TOTAL_ADDR   0x08070000
#define RATIO_ADDR   (TOTAL_ADDR+0x4000)
#define TIME_ADDR	 (TOTAL_ADDR-0x0800)


typedef struct pumppara
{
	unsigned int pumpRatio;	
	unsigned int pumpLength;
	uint32_t weightTatolAddr;
    unsigned int weightTatol;
	uint32_t storageHour;
	uint32_t storageSecond;
	
}pumpPara;

typedef struct pumptimer
{	
	volatile uint8_t Status;		// ������״̬
	volatile uint8_t Mode;			// ������ģʽ
	volatile uint8_t Flag;			// ��ʱ�����־
	volatile uint32_t Count;		// ����������ֵ
	volatile uint32_t PreLoad;		// ������Ԥװֵ
	void (*s_TIM_CallBack)(void);	// �������ص�����
	uint8_t workStatus;
}pumpTimer;

void initPara(void);

int pumpTimeCheck(void);
int minutesTimeCheck(void);
void pumpWork(int index, unsigned long ml_time);
void pumpSecond(int index, int sec_time);
void pumpMinute(int index, int min_time);
void pumpRun(int index, uint8_t run_state);
void pumpClean(void);
u8 pumpParaSave(void);
	
u8 pumpStatusGet(int index);
void pumpStatusSet(int index, u8 status);

#endif
