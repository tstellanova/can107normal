/**
  ******************************************************************************
  * @file    systick.h
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   systickÍ·ÎÄ¼þ.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */

#ifndef __SYSTICK_H
#define __SYSTICK_H 

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f10x.h"

/* Private function prototypes -----------------------------------------------*/
void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






























