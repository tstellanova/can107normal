/**
  ******************************************************************************
  * @file    usart.h
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   usartÍ·ÎÄ¼þ.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */

#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include <stm32f10x.h>

void Rcc_Configuration(void);
void UsartGPIO_Configuration(void);
void usart_Configuration(void);
void UsartGPIO_CTRT_Configuration(void);
void USART_CTRT_Configuartion(void);

#endif /*_USART_H*/
