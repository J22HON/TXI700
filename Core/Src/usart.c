/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_lpuart_rx;
extern DMA_HandleTypeDef hdma_lpuart_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

extern DMA_CircularBuffer Uart1Rx, Uart2Rx;

extern struct FUNCTION FunData;
extern struct KEY Key;

extern unsigned char               UartTxBuf[TXBUFFERSIZE],
                                    Uart1TxBuf[TXBUFFERSIZE],
                                    Uart2TxBuf[TXBUFFERSIZE],
                                    UartRxBuf[200],
                                    Uart2RxBuf[200],
                                    ShortBeep,
                                    v_rs_flag1,
                                    com1_flag,
                                    com2_flag,
                                    v_stable_flag;

extern unsigned int                hlpuart1RxCnt;

extern unsigned long               GetCurrRecvCount,          
                                    GetPreRecvCount,
                                    CmdWaitCount;     

/* LPUART1 init function */

void MX_LPUART1_UART_Init(void)
{

  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 38400;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */

  /* USER CODE END LPUART1_MspInit 0 */
    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**LPUART1 GPIO Configuration
    PC0     ------> LPUART1_RX
    PC1     ------> LPUART1_TX
    */
    GPIO_InitStruct.Pin = UART1_RX_Pin|UART1_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* LPUART1 DMA Init */
    /* LPUART_RX Init */
    hdma_lpuart_rx.Instance = DMA2_Channel7;
    hdma_lpuart_rx.Init.Request = DMA_REQUEST_4;
    hdma_lpuart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_lpuart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_lpuart_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_lpuart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_lpuart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_lpuart_rx.Init.Mode = DMA_CIRCULAR;
    hdma_lpuart_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_lpuart_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_lpuart_rx);

    /* LPUART_TX Init */
    hdma_lpuart_tx.Instance = DMA2_Channel6;
    hdma_lpuart_tx.Init.Request = DMA_REQUEST_4;
    hdma_lpuart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_lpuart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_lpuart_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_lpuart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_lpuart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_lpuart_tx.Init.Mode = DMA_NORMAL;
    hdma_lpuart_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_lpuart_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_lpuart_tx);

  /* USER CODE BEGIN LPUART1_MspInit 1 */

  /* USER CODE END LPUART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = UART2_TX_Pin|UART2_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Request = DMA_REQUEST_2;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Channel7;
    hdma_usart2_tx.Init.Request = DMA_REQUEST_2;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = UART3_TX_Pin|UART3_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Request = DMA_REQUEST_2;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Request = DMA_REQUEST_2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();

    /**LPUART1 GPIO Configuration
    PC0     ------> LPUART1_RX
    PC1     ------> LPUART1_TX
    */
    HAL_GPIO_DeInit(GPIOC, UART1_RX_Pin|UART1_TX_Pin);

    /* LPUART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
  /* USER CODE BEGIN LPUART1_MspDeInit 1 */

  /* USER CODE END LPUART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, UART2_TX_Pin|UART2_RX_Pin);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOC, UART3_TX_Pin|UART3_RX_Pin);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

unsigned long Cmd_Recv_Check(DMA_CircularBuffer *cb)
{
  unsigned char chk;

  GetCurrRecvCount =  DMA_Get_Cnt(cb);
  chk=0;
  if( GetCurrRecvCount==GetPreRecvCount)
  {
    if(GetCurrRecvCount!=0)
    {
      DMA_CB_Read(cb,UartRxBuf,GetCurrRecvCount);
      UartRxBuf[GetCurrRecvCount]=0;
      chk = 1;
      GetPreRecvCount=GetCurrRecvCount;
      CmdWaitCount = 0;  
    }
  }
  //else
  {
    GetPreRecvCount=GetCurrRecvCount;
  }

  
   if(chk)
  {       

        
        if(UartRxBuf[GetCurrRecvCount -1] == 0x0A) {v_rs_flag1 = 1;} 
            
   }
  
  return 0;
}

//****************************************************
// DMA Circular Buffer 
//****************************************************

void DMA_CB_Init(DMA_CircularBuffer *cb, unsigned char * buffer ,unsigned long size,unsigned long * pDMA_Cnt ) 
{
  cb->size  = size ; /* include empty elem */
  cb->start = pDMA_Cnt;
  cb->end   = 0;
  cb->elems = buffer;
}

unsigned long DMA_Get_Cnt(DMA_CircularBuffer *cb)
{
  unsigned long temp_cnt;
  #if 1  
  if(*(cb->start)!=cb->size)
  {
    temp_cnt= cb->size - *(cb->start);
  }
  else
  {
    temp_cnt = 0;
  }
  #endif
  if((temp_cnt)>=cb->end)
  {
    return ( temp_cnt  -  cb->end);
  }
  else
  {
    return ((temp_cnt + cb->size)  -  cb->end);   	
  }	
}

void DMA_CB_Read(DMA_CircularBuffer *cb, unsigned char* data, unsigned long cnt)
{
  for(int k=0;k<cnt;k++)
  {	
    data[k]=  cb->elems[cb->end];
    
    if((cb->end+1)>= cb->size)
    {    	cb->end = 0;	}
    else
    {cb->end ++;}	
  }
}

void DMA_CB_PreRead(DMA_CircularBuffer *cb, unsigned char* data, unsigned long cnt)
{
  
  unsigned long temp_end;
  
  temp_end = cb->end;
  for(int k=0;k<cnt;k++)
  {	
    data[k]=  cb->elems[temp_end];
    
    if((temp_end+1)>= cb->size)
    {    	temp_end = 0;	}
    else
    {temp_end ++;}	
  }
}

void netid_mode(void)
{
    static unsigned char low, high, sum, rx_count_back; 
      char i;
      
      Buf_init(1);
      while(hlpuart1.TxXferCount);
      Clear_Screen();                 
      KEYPAD_Scan(); 
      mprintf(1, 1, "     - PAIRING - ");
      mprintf(1, 3, "        RFSET ");
              
      while(1)
      {
          
        if(Uart1Rx.Buf[7] == (unsigned char)((Uart1Rx.Buf[5] + Uart1Rx.Buf[6] + 0x82))) break;
        if(!hlpuart1.TxXferCount)
        {          
          Buf_init(1);
          Uart1TxBuf[0]=0x1F;  
          Uart1TxBuf[1]=0x12; 
          Uart1TxBuf[2]=0x00; 
          Uart1TxBuf[3]=0x1F+0x12; // write        
          if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 4)!= HAL_OK) Error_Handler();
          HAL_Delay(20);   
        }
        HAL_Delay(20); 
      }
      
      if(Uart1Rx.Buf[7] == (unsigned char)((Uart1Rx.Buf[5] + Uart1Rx.Buf[6] + 0x82)))
      {
      hlpuart1.hdmarx->Instance->CNDTR = RXBUFFERSIZE;      
      hlpuart1RxCnt = hlpuart1.hdmarx->Instance->CNDTR;
      rx_count_back = hlpuart1RxCnt;      
      HAL_Delay(50);                    
                  
        low = Uart1Rx.Buf[5];
        high = Uart1Rx.Buf[6];
        sum = Uart1Rx.Buf[7];
        
        Uart1TxBuf[0]=0x12;  
        Uart1TxBuf[1]=0x00; 
        Uart1TxBuf[2]=0x00; 
        Uart1TxBuf[3]=0x12; // write
        if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 4)!= HAL_OK) Error_Handler();

        HAL_Delay(20);            
        mprintf(1, 5, "     Press Enter     ");
          while(1)
          {
            KEYPAD_Scan();
            
            if(Key.PressFlg[13])
            {
              Key.PressFlg[13] = 0;
                  
              // id send
              
              for(i=0; i<5; i++)
              {
                Uart1TxBuf[0] = 0x21;
                Uart1TxBuf[1] = 'S';
                Uart1TxBuf[2] = 'T';
                Uart1TxBuf[3] = low;
                Uart1TxBuf[4] = high;
                Uart1TxBuf[5] = sum;

                Uart1TxBuf[6] = '0';
                Uart1TxBuf[7] = '0';
                Uart1TxBuf[8] = '0'; 
                Uart1TxBuf[9] = 0x0D;  
                Uart1TxBuf[10] = 0x0A; 
                if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
                {
                  Error_Handler();
                }
                HAL_Delay(40);
                
                Uart1TxBuf[0] = 0x12;
                Uart1TxBuf[1] = low;
                Uart1TxBuf[2] = high;
                Uart1TxBuf[3] = low+high+0x12;
                if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 4)!= HAL_OK)
                {
                  Error_Handler();
                }
                HAL_Delay(40);
              }
              
              HAL_Delay(40);
              Clear_Screen();
              break;
            }
            
            else if(Key.PressFlg[12])
            {
              Key.PressFlg[12] = 0;
              return;
            }              
        }
      }
      
                   
        mprintf(1, 5, "          END         ");
        HAL_Delay(500);
        Clear_Screen();
}

void Buf_init(char uart)
{
    unsigned long i;
    
    if(uart == 1)
    {
        for(i=0; i<RXBUFFERSIZE; i++){Uart1Rx.Buf[i] = 0;}
        for(i=0; i<TXBUFFERSIZE; i++){Uart1TxBuf[i] = 0;}
        v_rs_flag1=0;
    }
    else if(uart == 2)
    {
        for(i=0; i<RXBUFFERSIZE; i++){Uart2Rx.Buf[i] = 0;}
        for(i=0; i<TXBUFFERSIZE; i++){Uart2TxBuf[i] = 0;}
    }
    
    else if(uart == 0) for(i=0; i<200; i++){UartRxBuf[i] = 0;}
}

void rf_send(long imsi_value, unsigned char error_flg) //Send to TF-600
{
  unsigned char i, temp, buf[5];
  unsigned long rs_value;

  HAL_Delay(5);
	
  Uart1TxBuf[0] = 0x21;
  Uart1TxBuf[1] = 'S'; 									
  Uart1TxBuf[2] = 'D';						
  temp = 0;
  /*
  if(OverFlg || excess_flag) temp = error_flg | 0x80;
    else temp = error_flg;
    if(com1_flag) temp = temp + 0x40; //division
    if(lampdsp[7])  temp |= 0x08;  //MINUS
    if(lampdsp[14]) temp |= 0x04;  //ZERO*/
    
  Uart1TxBuf[3] = temp;							

  rs_value=imsi_value;
  
  for(i=5; i>0; i--)
  { 
          buf[i] = (unsigned char)(rs_value /10000); 		
          rs_value = rs_value % 10000;
          rs_value = rs_value * 10;
  }
  
  Uart1TxBuf[4] = buf[5]+'0';
  Uart1TxBuf[5] = buf[4]+'0';
  Uart1TxBuf[6] = buf[3]+'0';
  Uart1TxBuf[7] = buf[2]+'0';
  Uart1TxBuf[8] = buf[1]+'0';
  Uart1TxBuf[8] = 0x0D; 								
  Uart1TxBuf[9] = 0x0A; 									
  if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 10)!= HAL_OK) Error_Handler();
  HAL_Delay(40);
}

void stt_out(unsigned char onebyte)
{
  unsigned char i;

  HAL_Delay(5);

  Uart1TxBuf[0] = 0x21;
  Uart1TxBuf[1] = 'S'; 			
  Uart1TxBuf[2] = 'D';			
  Uart1TxBuf[3] = 0;				

  for(i=0; i <5; i++) Uart1TxBuf[i+4] = onebyte;
  
  Uart1TxBuf[9] = 0x0D;				
  Uart1TxBuf[10] = 0x0A;				
  if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK) Error_Handler();
  HAL_Delay(20);
}

void tf_each_send(unsigned char pad, long rs_weight)
{
    unsigned char rs;
    unsigned char send_buffer[11];
    unsigned char state = 0;

    send_buffer[0] = 'X';
    if(pad!=0xFF) 
    {
        pad++;
    	send_buffer[1] = pad;
    }
    else          
    {
        send_buffer[1] = 'T';
    }

    if(rs_weight<0) //v_sign_flag
    { 
        state |= 0x08;
        rs_weight = rs_weight*-1;
    }
    if(v_stable_flag)  state |= 0x10;
    if(com1_flag) state += 0x80;
    else if(com2_flag) state += 0x40;
    send_buffer[2] = state;
    for(rs=7; rs>0; rs--)        
    { 
        send_buffer[rs+3] = (unsigned char)(rs_weight/1000000)+'0';
        rs_weight = rs_weight %1000000;
        rs_weight = rs_weight * 10;
    }
    send_buffer[10] = 0x0A;	
    
    Uart1TxBuf[0] = 0x21;
    for(rs=0; rs<11; rs++) Uart1TxBuf[rs+1]=(send_buffer[rs]);
    if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 12)!= HAL_OK) Error_Handler();
     HAL_Delay(30);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/