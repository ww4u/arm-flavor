//--<include>--

#include "main.h"

extern volatile uint32_t s_uiHour;
extern volatile uint32_t hourAddr;
volatile uint32_t cleanHour;

pumpPara pump[PUMP_LIMIT];
pumpTimer pumptim[PUMP_LIMIT];

//--<Function>--
void initPara(void)
{
	uint8_t temp = 0;	
	
	bspJumpRead(TIME_ADDR, (u32*)&s_uiHour, (u32*)&hourAddr);
	
	bspFlashRead(RATIO_ADDR, (u8*)pump, sizeof(pump[0])*PUMP_LIMIT);
	if(pump[0].pumpRatio==0xFFFFFFFF && pump[0].pumpLength==0xFFFFFFFF)
	{
		memset(pump,0,sizeof(pump[0])*PUMP_LIMIT);
		for (u8 i = 0; i < PUMP_LIMIT; i++)
		{
			pump[i].pumpRatio = 500;
			pump[i].pumpLength = 2000;
		}
		bspFlashWrite(RATIO_ADDR, (u8*)pump, sizeof(pump[0])*PUMP_LIMIT);
	}	
	for (u8 i = 1; i < PUMP_LIMIT; i++) 
    {		
		temp = bspJumpRead(TOTAL_ADDR + (i-1)*FLASH_PAGE, &pump[i].weightTatol, &pump[i].weightTatolAddr);
		if(temp){
			bspJumpEarse(TOTAL_ADDR + (i-1)*FLASH_PAGE);
			pump[i].weightTatolAddr = TOTAL_ADDR + (i-1)*FLASH_PAGE;
			bspJumpWrite(&pump[i].weightTatolAddr,pump[i].weightTatol,TOTAL_ADDR + (i-1)*FLASH_PAGE,500);
		}
    }	
}

u8 pumpParaSave(void)
{
	bspFlashWrite(RATIO_ADDR, (u8*)pump, sizeof(pump[0])*PUMP_LIMIT);
	return 0;
}

u8 pumpStatusGet(int index)
{
	return pumptim[index].workStatus;
}

void pumpStatusSet(int index, u8 status)
{
	pumptim[index].workStatus = status;
}


void pumpWork(int index, unsigned long ms_time)
{
	bsp_StartTimer(index, ms_time);
	bsp_LedOn(index);
}

void pumpSecond(int index, int sec_time)
{
    pumpWork(index, (unsigned long) sec_time*1000);
}

void pumpMinute(int index, int min_time)
{
    pumpWork(index, (unsigned long) min_time*60*1000);

}

void pumpRun(int index, uint8_t run_state)
{
    bsp_StopTimer(index);
    if(run_state == 1){    
        bsp_LedOn(index);
    }
    else{
        bsp_LedOff(index);
    }
}

void pumpClean(void)
{
    for(int i=1; i<PUMP_LIMIT; i++){
        pumpMinute( i, PUMPMINUTE );
        //delay(200);
    }
	pump[0].storageHour = bsp_hourReturn();
	pumpParaSave();
    //minute.recondTime = 0;
    //saveLong( RECOND_ADDR + 4, minute.recondTime); 
    //delay(50); 
}

