/**********************************************************************************************
Copyright (C) 2020，北京镁伽机器人科技有限公司
功能描述:  系统flash读写
当前版本:  1.0.0;
**********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp_flash.h"

#define DELAY_COUNT_MS 10402
#define DELAY_COUNT_US 10


void bspDelayMs(u32 milliSeconds)  
{
    u32 msCount;

    for (;milliSeconds != 0;milliSeconds--)
    {
        msCount = DELAY_COUNT_MS;
        for(; msCount!= 0;msCount--);
    }
}

void bspDelayUs(u32 microSeconds)  
{
    u32 msCount;

    for (;microSeconds != 0;microSeconds--)
    {
        msCount = DELAY_COUNT_US;
        for(; msCount!= 0;msCount--);
    }
}


/*********************************************************************************************
函数名：  
实现功能：  
输入参数：  无；
输出参数：  无；   
函数返回值：无；  
*********************************************************************************************/
u16 Flash_Write_do(u32 iAddress, u8 *buf, u16 u16Bytes) 
{
    u16 i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;
    
    if((u16Bytes & 0x01) != 0 )
    {
      u16Bytes = 0;
    }
    while((i < u16Bytes) && (FLASHStatus == FLASH_COMPLETE))
    {
		  FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(u16*)buf);
		  i = i+2;
		  iAddress = iAddress + 2;
		  buf = buf + 2;
    }
    
    return u16Bytes;
}

/*********************************************************************************************
函数名：bspFlashWrite  
实现功能：flash写  
输入参数：  无；
输出参数：  无；   
函数返回值：iNbrToWrite 必须是双字节对齐  
*********************************************************************************************/
int bspFlashWrite(u32 iAddress, u8 *buf, u32 len) 
{          
    u32 u32PageCanWrite = 0;//在某一页上,可以写多少个字节
    u32 u32PageNum = 0;
    u32 offset = 0;
    u8 tmp[FLASH_PAGE_SIZE];
    u32 u32left = len;//要写入的数据,还剩余多少个字节 
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    
    FLASH_Unlock();
    
    u32PageNum = iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//扇区地址
    offset = iAddress & (FLASH_PAGE_SIZE -1);
    u32PageCanWrite = FLASH_PAGE_SIZE - offset;//扇区剩余空间大小 
    if(u32left <= u32PageCanWrite)//当前面即可写完,不需要处理跨页操作
    {
        u32PageCanWrite = u32left;
    }
    /*对于需要跨页写的操作
     * 先写第一页,第一页比较特殊,需要单独处理
     * */    
    bspFlashRead(u32PageNum, tmp, FLASH_PAGE_SIZE);   //读出整个页
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   
    FLASHStatus = FLASH_ErasePage(u32PageNum); //擦除这个页
    if(FLASHStatus != FLASH_COMPLETE)
    {
      return -1;
    } 
    memcpy(&tmp[offset],buf,u32PageCanWrite);
    Flash_Write_do(u32PageNum,tmp,FLASH_PAGE_SIZE);
    u32PageNum += FLASH_PAGE_SIZE;
    buf += u32PageCanWrite;
    u32left -= u32PageCanWrite;//数据剩余字节
    while( u32left > 0 ) 
    {
        if(u32left > FLASH_PAGE_SIZE) 
        {
          u32PageCanWrite=FLASH_PAGE_SIZE;//下一个扇区还是写不完
        }
        else
        {
          u32PageCanWrite = u32left;  //下一个扇区可以写完了
        }
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   
        FLASHStatus = FLASH_ErasePage(u32PageNum); //擦除这个扇区
        Flash_Write_do(u32PageNum,buf,FLASH_PAGE_SIZE);
        u32PageNum += FLASH_PAGE_SIZE;
        buf += u32PageCanWrite;
        u32left -= u32PageCanWrite;//数据剩余字节
     }
        
     FLASH_Lock();
     return len; 
}

/*********************************************************************************************
函数名： bspFlashRead 
实现功能：  
输入参数：  无；
输出参数：  无；   
函数返回值：无；  
*********************************************************************************************/
u8 bspFlashRead(u32 iAddress, u8 *buf, u32 u32Bytes) 
{
        int i = 0;
        while(i < u32Bytes ) 
        {
           *(buf + i) = *(__IO u8*) iAddress++;
           i++;
        }
        return 0;
}

/*********************************************************************************************
函数名： bspJumpRead 
实现功能：  
输入参数：  无；
输出参数：  无；   
函数返回值：无；  
*********************************************************************************************/
u8 bspJumpRead(u32 inputAddr, u32 *readBuf, u32 *writeAddr) 
{
	u32 temp = 0;
	u32 iAddress = inputAddr;
	temp = *(__IO u32*) iAddress;
	if( temp == 0xFFFFFFFF )
	{
		temp = 0;
		bspFlashWrite(iAddress, (u8*)&temp, 4);
		*readBuf = *(__IO u32*)iAddress;
		*writeAddr = inputAddr+4;
		return 0;
	}	
	for(int i=1;i<500;i++)
	{		
		temp = *(__IO u32*) iAddress;
		if( temp == 0xFFFFFFFF )
		{
			*readBuf = *(__IO u32*) (iAddress-4);
			*writeAddr = iAddress;
			return 0;			
		}
		iAddress+=4;
	}
	*readBuf = *(__IO u32*)iAddress-4;
	*writeAddr = inputAddr;
	return 1;
}

u8 bspJumpEarse(u32 inputAddr)
{	
	u32 u32PageNum=0;
	
	FLASH_Unlock();
	
	u32PageNum = inputAddr & (~(FLASH_PAGE_SIZE -1 )) ;//扇区地址
	FLASH_ErasePage(u32PageNum); //擦除这个页
	
	FLASH_Lock();
	return 0;
}

u8 bspJumpWrite(u32 *inputAddr, u32 writeBuf, u32 startAddr, u32 cycleNum)
{	
	if(*inputAddr >= (startAddr+cycleNum*4))
	{
		bspJumpEarse(startAddr);
		*inputAddr = startAddr;		
	}
	bspFlashWrite(*inputAddr, (u8*)&writeBuf, 4);
	*inputAddr += 4;
	return 0;
}

