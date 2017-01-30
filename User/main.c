/**
  ******************************************************************************
  * @file    main.c
  * $Author: wdluo $
  * $Revision: 17 $
  * $Date:: 2012-07-06 11:16:48 +0800 #$
  * @brief   ������.
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_conf.h"

#include "stm32f10x.h"

#include "SysTick/systick.h" 
#include <stdlib.h>
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private variables ---------------------------------------------------------*/
uint8_t CAN1_RxRdy,CAN2_RxRdy;
uint16_t CAN1_Val_Tx,CAN1_Val_Rx,CAN2_Val_Tx,CAN2_Val_Rx;
CanTxMsg TxMessage;

/* Private function prototypes -----------------------------------------------*/
void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);
void CAN_Configuration(void);
void CanWriteData( CAN_TypeDef* CANx ,CanTxMsg *TxMessage );

/*******************************************************************************
* Function Name  : Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{
  uint16_t n;
  delay_init();//�δ�ʱ����ʼ��
  CAN_Configuration();//CAN��ʼ��
  USART_Configuration();//����1��ʼ��

  printf("\r\n***************www.viewtool.com/bbs******************************\r\n");
  printf("CAN-Bus Test \r\n");
  n=0;
  /* Infinite loop */
  while (1)
  {
    CAN1_Val_Tx = n;
    TxMessage.Data[0] = CAN1_Val_Tx>>8;
    TxMessage.Data[1] = CAN1_Val_Tx; 	
    CanWriteData(CAN1,&TxMessage);
    delay_ms (500);          /* delay 500 ms */

    if( CAN1_RxRdy == ENABLE )
    {
      CAN1_RxRdy = DISABLE;
      printf("CAN1 Receive Data: %d \r\n\r\n",CAN1_Val_Rx);
    }

    if( CAN2_RxRdy == ENABLE )
    {
      CAN2_RxRdy = DISABLE;
      TxMessage.Data[0] = CAN2_Val_Tx>>8;
      TxMessage.Data[1] = CAN2_Val_Tx; 	
      CanWriteData(CAN2,&TxMessage);
      printf("CAN2 Receive Data: %d \r\n",CAN2_Val_Rx);
    }
    n++;
    if(n==10000)
    {
      n=0;
    }
  }
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO ,ENABLE);
  /* CAN1 and CAN2 periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);						 
  /* Remap CAN1 and CAN2 GPIOs */
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE); 
  /* Configure CAN1 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure CAN1 pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure CAN2 RX pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure CAN2 pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); 						 
/**
 *  LED1 -> PA6 , LED2 -> PA7 
 */					 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  /* Enable CAN1 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable CAN2 RX0 interrupt IRQ channel */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : CAN_Configuration
* Description    : Configures the CAN
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CAN_Configuration(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  NVIC_Configuration();
  GPIO_Configuration();
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);

  CAN1_RxRdy = CAN2_RxRdy = DISABLE;
  CAN1_Val_Tx = CAN1_Val_Rx = CAN2_Val_Tx = CAN2_Val_Rx = 0;

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE; /* ʱ�䴥����ֹ, ʱ�䴥����CANӲ�����ڲ���ʱ����������ұ����ڲ���ʱ��� */
  CAN_InitStructure.CAN_ABOM = DISABLE; /* �Զ����߽�ֹ���Զ����ߣ�һ��Ӳ����ص�128��11������λ�����Զ��˳�����״̬��������Ҫ����趨������˳� */
  CAN_InitStructure.CAN_AWUM = DISABLE; /* �Զ����ѽ�ֹ���б�������ʱ���Զ��˳�����	*/
  CAN_InitStructure.CAN_NART = DISABLE; /* �����ش�, �������һֱ�����ɹ�ֹ������ֻ��һ�� */
  CAN_InitStructure.CAN_RFLM = DISABLE; /* ����FIFO����, 1--��������յ��µı���ժ��Ҫ��0--���յ��µı����򸲸�ǰһ����	*/
  CAN_InitStructure.CAN_TXFP = ENABLE;  /* �������ȼ�  0---�ɱ�ʶ������  1---�ɷ�������˳�����	*/
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; /* ģʽ	*/
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      /* ����ͬ������ֻ��canӲ�����ڳ�ʼ��ģʽʱ���ܷ�������Ĵ��� */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;      /* ʱ���1 */
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;      /* ʱ���2 */
  CAN_InitStructure.CAN_Prescaler = 5;          /* ������Ԥ��Ƶ�� */  
 
  /* �����ʼ��㷽�� */
  /* CANbps= Fpclk/((BRP+1)*((Tseg1+1)+(Tseg2+1)+1)  �˴�����Ϊ  CANbps=36000000/(45*(4+3+1))=1200kHz */   														  //�˴�Tseg1+1 = CAN_BS1_8tp
  /* ���ô���: Tseg1>=Tseg2  Tseg2>=tq; Tseg2>=2TSJW */						

  /*Initializes the CAN1  and CAN2 */
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_Init(CAN2, &CAN_InitStructure);

  /* CAN1 filter init */
  /* ����CAN������ */
  /* 32λ��Ӧ��id */
  /* stdid[10:0]��extid[17:0],ide,rtr	*/
  /* 16λ��Ӧ��id */
  /* stdid[10:0],ide,rtr,extid[17:15] */
  /* һ��ʹ������ģʽ	*/
  /* Ҫע�����fifo���մ������жϣ����о���fifo�ĸ����ȡ��һֱ��������һ�����ݣ� Ҫ�ͷŲ���ȡ��һ������ */
  /* ��ʹ�õ��ж��� */
  /* 1,����Ϣ�жϣ���fifo�Һ��ж� */
  /* 2,fifo���ж�	*/
  /* 3,fifo��֮��������Ϣ�����жϣ���fifo����ж�	*/
  CAN_FilterInitStructure.CAN_FilterNumber = 0;     /* ������1 */
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;  /* ����ģʽ */
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; /* 32λ */
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;  /* �����ĸ���Ϊ0, �����������κ�id */
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;  
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN2 filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* IT Configuration for CAN1 */  
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  /* IT Configuration for CAN2 */  
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}

/*******************************************************************************
* Function Name  : CanWriteData
* Description    : Can Write Date to CAN-BUS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CanWriteData( CAN_TypeDef* CANx ,CanTxMsg *TxMessage )
{
  /* transmit */
  TxMessage->StdId = 0xA5A5;     /* ���ñ�׼id  ע���׼id�����7λ����ȫ������(1)����11λ */
//TxMessage->ExtId = 0x00;       //������չid  ��չid��18λ
  TxMessage->RTR = CAN_RTR_DATA; /* ����Ϊ����֡ */
  TxMessage->IDE = CAN_ID_STD;   /* ʹ�ñ�׼id	*/
  TxMessage->DLC = 8;            /* ���ݳ���, can���Ĺ涨�������ݳ���Ϊ8�ֽ� */
 
  CAN_Transmit(CANx,TxMessage);  /* ���������Ϣ�����͵������0,1,2��û���������뷢��no_box */	
}

/*******************************************************************************
* Function Name  : CAN1_RX0_IRQHandler
* Description    : This function handles CAN1 RX0 interrupts 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);  /* �˺��������ͷ���������˵�,�ڷǱ�Ҫʱ,����Ҫ�Լ��ͷ� */

  CAN1_Val_Rx = RxMessage.Data[0] <<8 ;
  CAN1_Val_Rx += RxMessage.Data[1];

  CAN1_RxRdy = ENABLE;
}

/*******************************************************************************
* Function Name  : CAN2_RX0_IRQHandler
* Description    : This function handles CAN2 RX0 interrupts
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  CAN_Receive(CAN2,CAN_FIFO0, &RxMessage);  /* �˺��������ͷ���������˵�,�ڷǱ�Ҫʱ,����Ҫ�Լ��ͷ� */

  CAN2_Val_Rx = RxMessage.Data[0] <<8 ;
  CAN2_Val_Rx += RxMessage.Data[1];

  CAN2_Val_Tx = 10000-CAN2_Val_Rx;

  CAN2_RxRdy = ENABLE;
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

