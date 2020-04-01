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
	volatile uint8_t Status;		// 计数器状态
	volatile uint8_t Mode;			// 计数器模式
	volatile uint8_t Flag;			// 定时到达标志
	volatile uint32_t Count;		// 计数器计数值
	volatile uint32_t PreLoad;		// 计数器预装值
	void (*s_TIM_CallBack)(void);	// 计数器回调函数
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
