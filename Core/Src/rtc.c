/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
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
#include "rtc.h"
#include "main.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#define RTCClockOutput_Enable

RTC_HandleTypeDef hrtc;

RTC_DateTypeDef sDate;
RTC_TimeTypeDef sTime;

extern struct KEY Key;

extern unsigned long     RealTime,
                          RealDate;

/* RTC init function */
void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0xA5A5)
  {
  /* USER CODE END Check_RTC_BKUP */
  
  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
 
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0xA5A5);
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void TimeRead(void)
{
    unsigned long temp;
    
    //RTC_CalendarShow(ShowTime, ShowDate);
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);       

    temp = sTime.Seconds;
    temp = sTime.Minutes * 100 + temp;
    temp = sTime.Hours * 10000 + temp;
    RealTime = temp;
    
    temp = sDate.Date;
    temp = sDate.Month * 100 + temp;
    temp = sDate.Year * 10000 + temp;
    RealDate = temp;
}

void rtc_set(void)
{
    unsigned char loop_out = 0;
    char bound = 0;
    unsigned int bound_count = 0;
    
    Clear_Screen();
    TimeRead();
    if(sDate.Year>99) sDate.Year = 0;
    if(sDate.Month>12) sDate.Month = 1;
    if(sDate.Date>31) sDate.Date = 1;
    if(sTime.Hours>23) sTime.Hours = 0;
    if(sTime.Minutes>59) sTime.Minutes = 0;
    if(sTime.Seconds>59) sTime.Seconds = 0;
    
    Print_Str6x8(0xFF,10,1,"Set Date and Time");
    
    mprintf(1, 3," Date : 20%02d. %02d. %02d", sDate.Year, sDate.Month, sDate.Date);
    mprintf(1, 4," Time : %02d : %02d ", sTime.Hours, sTime.Minutes);
    while(!loop_out)
    {
        KEYPAD_Scan();
        PrintArrow(68,7,0);
        PrintArrow(80,7,1);
        PrintArrow(94,7,2);
        PrintArrow(56,7,4);
        PrintArrow(44,7,3);
        Print_Str6x8(0xFF,110,7,"1/1");        
          
        if( Key.PressFlg[13] )
        {
          Clear_Screen();
          Key.PressFlg[13] = 0;
          loop_out = 1;

          HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
          
          HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
               
          
          mprintf(1, 5,"Date & Time SAVE!!");
          HAL_Delay(1000);
          Clear_Screen();
        }
                
        else if( Key.PressFlg[12])
        {
          Clear_Screen();
          Key.PressFlg[12] = 0;
          loop_out = 1;  
          break;
        }
    
        else if ( Key.PressFlg[2] )
        {
          Key.PressFlg[2] = 0;
          if(bound == 0)
          {
              sDate.Year++;
              if(sDate.Year>99) sDate.Year = 0;
          }
          else if(bound == 1)
          {
              sDate.Month++;
              if(sDate.Month>12) sDate.Month = 1;
          }
          else if(bound == 2)
          {
              sDate.Date++;
              if(sDate.Date>31) sDate.Date = 1;
          }
          else if(bound == 3)
          {
              sTime.Hours++;
              if(sTime.Hours>23) sTime.Hours = 0;
          }
          else if(bound == 4)
          {
              sTime.Minutes++;
              if(sTime.Minutes>59) sTime.Minutes = 0;
          }
          else if(bound == 5)
          {
              sTime.Seconds++;
              if(sTime.Seconds>59) sTime.Seconds = 0;
          }
        }
    
        else if( Key.PressFlg[4] )
        {
          Key.PressFlg[4] = 0;
          bound--;
          if(bound < 0) bound = 4;
          mprintf(1, 3," Date : 20%02d. %02d. %02d", sDate.Year, sDate.Month, sDate.Date);
          mprintf(1, 4," Time : %02d : %02d       ", sTime.Hours, sTime.Minutes);
        }

        else if ( Key.PressFlg[6] )
        {
          Key.PressFlg[6] = 0;
          bound++;
          if(bound > 4) bound = 0;
          mprintf(1, 3," Date : 20%02d. %02d. %02d", sDate.Year, sDate.Month, sDate.Date);
          mprintf(1, 4," Time : %02d : %02d       ", sTime.Hours, sTime.Minutes);
        }
    
        else if ( Key.PressFlg[8] )
        {
          Key.PressFlg[8] = 0;
          if(bound == 0)
          {
              sDate.Year--;
              if(sDate.Year<0) sDate.Year = 99;
          }
          else if(bound == 1)
          {
              sDate.Month--;
              if(sDate.Month<1) sDate.Month = 12;
          }
          else if(bound == 2)
          {
              sDate.Date--;
              if(sDate.Date<1) sDate.Date = 31;
          }
          else if(bound == 3)
          {
              sTime.Hours--;
              if(sTime.Hours<0) sTime.Hours = 23;
          }
          else if(bound == 4)
          {
              sTime.Minutes--;
              if(sTime.Minutes<0) sTime.Minutes = 59;
          }
        }
        
        if(bound_count <= 100)
        {
          if(bound<=2) mprintf(1, 3," Date : 20%02d. %02d. %02d", sDate.Year, sDate.Month, sDate.Date);
          else mprintf(1, 4," Time : %02d : %02d        ", sTime.Hours, sTime.Minutes);
          bound_count++;
        }
        else if(bound_count <= 300)
        {
          if     (bound==0) mprintf(1, 3," Date : 20  . %02d. ", sDate.Month, sDate.Date);
          else if(bound==1) mprintf(1, 3," Date : 20%02d.   . ", sDate.Year, sDate.Date);
          else if(bound==2) mprintf(1, 3," Date : 20%02d. %02d.   ", sDate.Year, sDate.Month);
          else if(bound==3) mprintf(1, 4," Time :    : %02d  ", sTime.Minutes);
          else if(bound==4) mprintf(1, 4," Time : %02d :    ", sTime.Hours);
          bound_count++;
        }
        else bound_count = 0;
    }
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/