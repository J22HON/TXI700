/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "lptim.h"
#include "usart.h"
#include "rtc.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/

#define RTCClockOutput_Enable

/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
  void SystemClock_Config(void);

  struct FUNCTION FunData;
  struct KEY Key;
  struct CAL Calibration;
  struct UNIT Unit;
  struct PAN_ID Zigbee;
  struct AD_DATA AdData;
  struct STATE State;
  struct WEIGHT Weight;
  
  extern RTC_HandleTypeDef hrtc;
  extern RTC_DateTypeDef sDate;
  extern RTC_TimeTypeDef sTime;
  
  DMA_CircularBuffer Uart1Rx, Uart2Rx, Uart3Rx;
  
 unsigned char                  ShortBeep,
                                 Time01Start,
                                 Time02Start,
                                 PowerOn,
                                 v_ad_flag,
                                 v_multi_cal,
                                 v_minimum_division[2],
                                 BatteryLevel,
                                 pass_start,
                                 cal_confirm,
                                 stable_flag[2],
                                 over_flag[2],
                                 count[2],
                                 v_az_scope,
                                 imsi,
                                 v_target,
                                 Cal_ad_flag,
                                 i,
                                 v_cal_flag;                            
  
 unsigned long                  Time01Count,
                                 Time02Count,
                                 ShortBeepCount,
                                 RealTime,
                                 RealDate,                                                                                                                             
                                 v_temp_long,
                                 first_count,
                                 SysTicFlg,
                                 StableCount,
                                 v_adc_org[2][5],
                                 v_maximum_capacity[2],
                                 v_e_resolution[2],
                                 v_zero[2],
                                 imsi_value,
                                 diff[2],
                                 diff1[2];    
 
  unsigned short                 zero_count[2];
  
  long                            BattOffSet,
                                  prev_adc1[2],
                                  v_adc1_buf,
                                  v_adc3_buf,
                                  v_e_value[2];
                                 
  unsigned int                   v_speed;   
  
  float                          v_res_factor[2][5];
  
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  
  BUZZER_OFF;
  POFF_ON;     // Power ON
  
  LCD_DIMMING_OFF; 
  LCD_OFF;
  ADC_CS1_HI;
  ADC_CS2_HI;
  
  FunData.Filter_Degree = 9;
  adc_initial();
    
  OLED_Initialize();
  LOGO_Print(0);
  mprintf(43,6," TXI-700");
  mprintf(43,7,"VER T100");
  HAL_Delay(1500);
    
  HAL_RTC_WaitForSynchro(&hrtc);
  
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_LPTIM1_Init();
  MX_LPTIM2_Init();
   
  MX_LPUART1_UART_Init();
  if (HAL_UART_Receive_DMA(&hlpuart1, (unsigned char*)Uart1Rx.Buf, RXBUFFERSIZE) != HAL_OK)  //rx dma zigbee
  {
    Clear_Screen();
    mprintf(1, 3, "          ERR-U1      ");
    Error_Handler();
  }
  DMA_CB_Init(&Uart1Rx,Uart1Rx.Buf,RXBUFFERSIZE,(unsigned long *)&(hlpuart1.hdmarx->Instance->CNDTR) );
  
  MX_USART2_UART_Init();
  if (HAL_UART_Receive_DMA(&huart2, (unsigned char*)Uart2Rx.Buf, RXBUFFERSIZE) != HAL_OK)  //rx dma ble
  {
    Clear_Screen();
    mprintf(1, 3, "          ERR-U2      ");
    Error_Handler();
  }
  DMA_CB_Init(&Uart2Rx,Uart2Rx.Buf,RXBUFFERSIZE,(unsigned long *)&(huart2.hdmarx->Instance->CNDTR) ); 
  
  MX_USART3_UART_Init();
  if (HAL_UART_Receive_DMA(&huart3, (unsigned char*)Uart3Rx.Buf, RXBUFFERSIZE) != HAL_OK)  //rx dma print
  {
    Clear_Screen();
    mprintf(1, 3, "          ERR-U3      ");
    Error_Handler();
  }
  DMA_CB_Init(&Uart3Rx,Uart3Rx.Buf,RXBUFFERSIZE,(unsigned long *)&(huart3.hdmarx->Instance->CNDTR) );   
  
  MX_RTC_Init();
  MX_TIM15_Init();
  MX_USB_DEVICE_Init();
  ShortBeep = 1;
  PowerOn = 1;
  FunData.KeyBeep = 1;
  
  function_read();
  function_reset();  
  cal_read();  
      
  Clear_Screen();
  
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
RETRY:
  
  for(i=0; i<2; i++)
  {
    do v_temp_long = read_filtered_adc(i); while(v_ad_flag==0);

    if(( v_temp_long < 100) || (v_temp_long > 1000000l))	// LOAD CELL ERROR
    { 
      Battery_check(); 
      mprintf(1, 3, " C H 0 2 ");
      pass_start=0; 
      HAL_Delay(700); 
    } 
    else
    {
      pass_start++;
      multi_gap(v_temp_long, v_zero[i], 1, i);
      do v_temp_long = read_filtered_adc(i); while(v_ad_flag==0);      
      v_adc_org[i][0] = (unsigned long) ( ((float) v_temp_long) * v_res_factor[i][0] );
    }
  }
  
  if(cal_confirm==0) 
  { 
    if(pass_start!=2) 
    {
        pass_start=0; 
        goto RETRY; 
    }
  }
  
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    KEYPAD_Scan();
    Battery_check();      
    TimeRead();
    
    mprintf(1, 3," Date  : 20%02d. %02d. %02d", sDate.Year, sDate.Month, sDate.Date);
    mprintf(1, 4," Time  : %02d : %02d       ", sTime.Hours, sTime.Minutes);
          
    if(BatteryLevel >= 95)      Batt_Lamp(114, 0, 4); 
    else if(BatteryLevel >= 75) Batt_Lamp(114, 0, 3);
    else if(BatteryLevel >= 50) Batt_Lamp(114, 0, 2);
    else if(BatteryLevel >= 25) Batt_Lamp(114, 0, 1);
    else if(BatteryLevel >= 10)  Batt_Lamp(114, 0, 0);
    else
    {
      Batt_Lamp(114, 0, 1); HAL_Delay(50); Batt_Lamp(114, 0, 0); HAL_Delay(50);
    }
    
    if(Key.PressFlg[0])      { Key.PressFlg[0]=0; }
    else if(Key.PressFlg[1]) { Key.PressFlg[1]=0; }
    else if(Key.PressFlg[2]) { Key.PressFlg[2]=0; }  
    else if(Key.PressFlg[3]) { Key.PressFlg[3]=0; }
    else if(Key.PressFlg[4]) { Key.PressFlg[4]=0; }
    else if(Key.PressFlg[5]) { Key.PressFlg[5]=0; }
    else if(Key.PressFlg[6]) { Key.PressFlg[6]=0; loadcell_test();}
    else if(Key.PressFlg[7]) { Key.PressFlg[7]=0; cal_mode();}
    else if(Key.PressFlg[8]) { Key.PressFlg[8]=0; rtc_set();}
    else if(Key.PressFlg[9]) { Key.PressFlg[9]=0; function();}
    else if(Key.PressFlg[10]) { Key.PressFlg[10]=0; }
    else if(Key.PressFlg[11]) { Key.PressFlg[11]=0; }
    else if(Key.PressFlg[12]) { Key.PressFlg[12]=0; }
    else if(Key.PressFlg[13]) { Key.PressFlg[13]=0; }   
    
    normal_mode();
    
    mprintf(1, 5, " Wt1   : %d kg ",v_e_value[0]);
    mprintf(1, 6, " Wt2   : %d kg ",v_e_value[1]);
    
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART2 
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_LPTIM2
                              |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;
  PeriphClkInit.Lptim2ClockSelection = RCC_LPTIM2CLKSOURCE_PCLK;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
