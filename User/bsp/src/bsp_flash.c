/**********************************************************************************************
Copyright (C) 2020������þ٤�����˿Ƽ����޹�˾
��������:  ϵͳflash��д
��ǰ�汾:  1.0.0;
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
��������  
ʵ�ֹ��ܣ�  
���������  �ޣ�
���������  �ޣ�   
��������ֵ���ޣ�  
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
��������bspFlashWrite  
ʵ�ֹ��ܣ�flashд  
���������  �ޣ�
���������  �ޣ�   
��������ֵ��iNbrToWrite ������˫�ֽڶ���  
*********************************************************************************************/
int bspFlashWrite(u32 iAddress, u8 *buf, u32 len) 
{          
    u32 u32PageCanWrite = 0;//��ĳһҳ��,����д���ٸ��ֽ�
    u32 u32PageNum = 0;
    u32 offset = 0;
    u8 tmp[FLASH_PAGE_SIZE];
    u32 u32left = len;//Ҫд�������,��ʣ����ٸ��ֽ� 
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    
    FLASH_Unlock();
    
    u32PageNum = iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//������ַ
    offset = iAddress & (FLASH_PAGE_SIZE -1);
    u32PageCanWrite = FLASH_PAGE_SIZE - offset;//����ʣ��ռ��С 
    if(u32left <= u32PageCanWrite)//��ǰ�漴��д��,����Ҫ�����ҳ����
    {
        u32PageCanWrite = u32left;
    }
    /*������Ҫ��ҳд�Ĳ���
     * ��д��һҳ,��һҳ�Ƚ�����,��Ҫ��������
     * */    
    bspFlashRead(u32PageNum, tmp, FLASH_PAGE_SIZE);   //��������ҳ
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   
    FLASHStatus = FLASH_ErasePage(u32PageNum); //�������ҳ
    if(FLASHStatus != FLASH_COMPLETE)
    {
      return -1;
    } 
    memcpy(&tmp[offset],buf,u32PageCanWrite);
    Flash_Write_do(u32PageNum,tmp,FLASH_PAGE_SIZE);
    u32PageNum += FLASH_PAGE_SIZE;
    buf += u32PageCanWrite;
    u32left -= u32PageCanWrite;//����ʣ���ֽ�
    while( u32left > 0 ) 
    {
        if(u32left > FLASH_PAGE_SIZE) 
        {
          u32PageCanWrite=FLASH_PAGE_SIZE;//��һ����������д����
        }
        else
        {
          u32PageCanWrite = u32left;  //��һ����������д����
        }
        FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);   
        FLASHStatus = FLASH_ErasePage(u32PageNum); //�����������
        Flash_Write_do(u32PageNum,buf,FLASH_PAGE_SIZE);
        u32PageNum += FLASH_PAGE_SIZE;
        buf += u32PageCanWrite;
        u32left -= u32PageCanWrite;//����ʣ���ֽ�
     }
        
     FLASH_Lock();
     return len; 
}

/*********************************************************************************************
�������� bspFlashRead 
ʵ�ֹ��ܣ�  
���������  �ޣ�
���������  �ޣ�   
��������ֵ���ޣ�  
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
�������� bspJumpRead 
ʵ�ֹ��ܣ�  
���������  �ޣ�
���������  �ޣ�   
��������ֵ���ޣ�  
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
	
	u32PageNum = inputAddr & (~(FLASH_PAGE_SIZE -1 )) ;//������ַ
	FLASH_ErasePage(u32PageNum); //�������ҳ
	
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

