#ifndef   _BSP_FLASH_H_
#define   _BSP_FLASH_H_

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((u16)0x800)
  
#else
  #define FLASH_PAGE_SIZE    ((u16)0x400)
#endif

#define FLASH_START_ADDR   0x08000000

#if defined(STM32F10X_XL)
#define FLASH_END_ADDR    0x080C0000/*768K*/
#else
#define FLASH_END_ADDR   0x08070000 /*512K*/
#endif



s32 bspFlashWrite(u32 iAddress, u8 *buf, u32 u32bytes);
u8 bspFlashRead(u32 iAddress, u8 *buf, u32 u32Bytes);
u8 bspJumpRead(u32 inputAddr, u32 *readBuf, u32 *writeAddr);
u8 bspJumpWrite(u32 *inputAddr, u32 writeBuf, u32 startAddr, u32 cycleNum);
u8 bspJumpEarse(u32 inputAddr);

#endif

