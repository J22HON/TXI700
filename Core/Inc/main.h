/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
  
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUZZER_Pin GPIO_PIN_13
#define BUZZER_GPIO_Port GPIOC
#define UART1_RX_Pin GPIO_PIN_0
#define UART1_RX_GPIO_Port GPIOC
#define UART1_TX_Pin GPIO_PIN_1
#define UART1_TX_GPIO_Port GPIOC
#define AD_MISO_Pin GPIO_PIN_2
#define AD_MISO_GPIO_Port GPIOC
#define AD_MOSI_Pin GPIO_PIN_3
#define AD_MOSI_GPIO_Port GPIOC
#define KEY_IN1_Pin GPIO_PIN_0
#define KEY_IN1_GPIO_Port GPIOA
#define KEY_IN2_Pin GPIO_PIN_1
#define KEY_IN2_GPIO_Port GPIOA
#define UART2_TX_Pin GPIO_PIN_2
#define UART2_TX_GPIO_Port GPIOA
#define UART2_RX_Pin GPIO_PIN_3
#define UART2_RX_GPIO_Port GPIOA
#define KEY_IN3_Pin GPIO_PIN_4
#define KEY_IN3_GPIO_Port GPIOA
#define KEY_IN4_Pin GPIO_PIN_5
#define KEY_IN4_GPIO_Port GPIOA
#define KEY_IN5_Pin GPIO_PIN_6
#define KEY_IN5_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_7
#define KEY1_GPIO_Port GPIOA
#define UART3_TX_Pin GPIO_PIN_4
#define UART3_TX_GPIO_Port GPIOC
#define UART3_RX_Pin GPIO_PIN_5
#define UART3_RX_GPIO_Port GPIOC
#define ADC1_Pin GPIO_PIN_0
#define ADC1_GPIO_Port GPIOB
#define LCD_WR_Pin GPIO_PIN_1
#define LCD_WR_GPIO_Port GPIOB
#define LCD_DATA_Pin GPIO_PIN_2
#define LCD_DATA_GPIO_Port GPIOB
#define ADC_SCK_Pin GPIO_PIN_10
#define ADC_SCK_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_11
#define LCD_CS_GPIO_Port GPIOB
#define ADC_CS2_Pin GPIO_PIN_12
#define ADC_CS2_GPIO_Port GPIOB
#define ADC_CS1_Pin GPIO_PIN_13
#define ADC_CS1_GPIO_Port GPIOB
#define DIMMING_Pin GPIO_PIN_14
#define DIMMING_GPIO_Port GPIOB
#define OLED_SDIN_Pin GPIO_PIN_15
#define OLED_SDIN_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_6
#define LCD_EN_GPIO_Port GPIOC
#define OLED_SCLK_Pin GPIO_PIN_7
#define OLED_SCLK_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_8
#define OLED_DC_GPIO_Port GPIOC
#define OLED_RES_Pin GPIO_PIN_9
#define OLED_RES_GPIO_Port GPIOC
#define OLED_CS_Pin GPIO_PIN_8
#define OLED_CS_GPIO_Port GPIOA
#define BLE_EN_Pin GPIO_PIN_9
#define BLE_EN_GPIO_Port GPIOA
#define V5_EN_Pin GPIO_PIN_10
#define V5_EN_GPIO_Port GPIOA
#define PSW_Pin GPIO_PIN_15
#define PSW_GPIO_Port GPIOA
#define POFF_Pin GPIO_PIN_10
#define POFF_GPIO_Port GPIOC
#define LCD_IRQ_Pin GPIO_PIN_12
#define LCD_IRQ_GPIO_Port GPIOC
#define ZIGBEE_EN_Pin GPIO_PIN_2
#define ZIGBEE_EN_GPIO_Port GPIOD
#define OLED_EN_Pin GPIO_PIN_3
#define OLED_EN_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_5
#define KEY2_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define KEY1_SET                HAL_GPIO_WritePin(KEY1_GPIO_Port, KEY1_Pin, GPIO_PIN_SET)
#define KEY1_RESET              HAL_GPIO_WritePin(KEY1_GPIO_Port, KEY1_Pin, GPIO_PIN_RESET)

#define KEY2_SET                HAL_GPIO_WritePin(KEY2_GPIO_Port, KEY2_Pin, GPIO_PIN_SET)
#define KEY2_RESET              HAL_GPIO_WritePin(KEY2_GPIO_Port, KEY2_Pin, GPIO_PIN_RESET)

#define KEY3_SET                HAL_GPIO_WritePin(KEY3_GPIO_Port, KEY3_Pin, GPIO_PIN_SET)
#define KEY3_RESET              HAL_GPIO_WritePin(KEY3_GPIO_Port, KEY3_Pin, GPIO_PIN_RESET)

#define KEY_IN1_READ            HAL_GPIO_ReadPin(KEY_IN1_GPIO_Port, KEY_IN1_Pin)
#define KEY_IN2_READ            HAL_GPIO_ReadPin(KEY_IN2_GPIO_Port, KEY_IN2_Pin)
#define KEY_IN3_READ            HAL_GPIO_ReadPin(KEY_IN3_GPIO_Port, KEY_IN3_Pin)
#define KEY_IN4_READ            HAL_GPIO_ReadPin(KEY_IN4_GPIO_Port, KEY_IN4_Pin)
#define KEY_IN5_READ            HAL_GPIO_ReadPin(KEY_IN5_GPIO_Port, KEY_IN5_Pin)

#define ADC_MOSI_HI             HAL_GPIO_WritePin(AD_MOSI_GPIO_Port, AD_MOSI_Pin, GPIO_PIN_SET)
#define ADC_MOSI_LO             HAL_GPIO_WritePin(AD_MOSI_GPIO_Port, AD_MOSI_Pin, GPIO_PIN_RESET)

#define ADC_SCK_HI              HAL_GPIO_WritePin(ADC_SCK_GPIO_Port, ADC_SCK_Pin, GPIO_PIN_SET)
#define ADC_SCK_LO              HAL_GPIO_WritePin(ADC_SCK_GPIO_Port, ADC_SCK_Pin, GPIO_PIN_RESET)

#define ADC_CS1_HI              HAL_GPIO_WritePin(ADC_CS1_GPIO_Port, ADC_CS1_Pin, GPIO_PIN_SET)
#define ADC_CS1_LO              HAL_GPIO_WritePin(ADC_CS1_GPIO_Port, ADC_CS1_Pin, GPIO_PIN_RESET)

#define ADC_CS2_HI              HAL_GPIO_WritePin(ADC_CS2_GPIO_Port, ADC_CS2_Pin, GPIO_PIN_SET)
#define ADC_CS2_LO              HAL_GPIO_WritePin(ADC_CS2_GPIO_Port, ADC_CS2_Pin, GPIO_PIN_RESET)
  
#define ADC_MISO                HAL_GPIO_ReadPin(AD_MISO_GPIO_Port, AD_MISO_Pin)

#define BUZZER_ON               HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET)
#define BUZZER_OFF              HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET)

#define LCD_CS_HI               HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_CS_LO               HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)

#define LCD_WR_HI               HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define LCD_WR_LO               HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)

#define LCD_DATA_HI             HAL_GPIO_WritePin(LCD_DATA_GPIO_Port, LCD_DATA_Pin, GPIO_PIN_SET)
#define LCD_DATA_LO             HAL_GPIO_WritePin(LCD_DATA_GPIO_Port, LCD_DATA_Pin, GPIO_PIN_RESET)

#define LCD_DIMMING_ON          HAL_GPIO_WritePin(DIMMING_GPIO_Port, DIMMING_Pin, GPIO_PIN_SET)
#define LCD_DIMMING_OFF         HAL_GPIO_WritePin(DIMMING_GPIO_Port, DIMMING_Pin, GPIO_PIN_RESET)

#define LCD_ON                  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET)
#define LCD_OFF                 HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET)

#define LCD_IRQ_HI              HAL_GPIO_WritePin(LCD_IRQ_GPIO_Port, LCD_IRQ_Pin, GPIO_PIN_SET)
#define LCD_IRQ_LO              HAL_GPIO_WritePin(LCD_IRQ_GPIO_Port, LCD_IRQ_Pin, GPIO_PIN_RESET)

#define OLED_CS_HI              HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)
#define OLED_CS_LO              HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)

#define OLED_SCLK_HI            HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin, GPIO_PIN_SET)
#define OLED_SCLK_LO            HAL_GPIO_WritePin(OLED_SCLK_GPIO_Port, OLED_SCLK_Pin, GPIO_PIN_RESET)

#define OLED_DC_HI              HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_DC_LO              HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)

#define OLED_RES_HI             HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)
#define OLED_RES_LO             HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)

#define OLED_SDIN_HI            HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, GPIO_PIN_SET)
#define OLED_SDIN_LO            HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, GPIO_PIN_RESET)

#define OLED_ON                 HAL_GPIO_WritePin(OLED_EN_GPIO_Port, OLED_EN_Pin, GPIO_PIN_SET)
#define OLED_OFF                HAL_GPIO_WritePin(OLED_EN_GPIO_Port, OLED_EN_Pin, GPIO_PIN_RESET)

#define BACKLIGHT_ON            HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
#define BACKLIGHT_OFF           HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);

#define PSW_READ                HAL_GPIO_ReadPin(PSW_GPIO_Port, PSW_Pin)

#define POFF_ON                 HAL_GPIO_WritePin(POFF_GPIO_Port, POFF_Pin, GPIO_PIN_SET)
#define POFF_OFF                HAL_GPIO_WritePin(POFF_GPIO_Port, POFF_Pin, GPIO_PIN_RESET)

#define ZIGBEE_ON               HAL_GPIO_WritePin(ZIGBEE_EN_GPIO_Port, ZIGBEE_EN_Pin, GPIO_PIN_SET)
#define ZIGBEE_OFF              HAL_GPIO_WritePin(ZIGBEE_EN_GPIO_Port, ZIGBEE_EN_Pin, GPIO_PIN_RESET)

#define BLE_ON                  HAL_GPIO_WritePin(BLE_EN_GPIO_Port, BLE_EN_Pin, GPIO_PIN_SET)
#define BLE_OFF                 HAL_GPIO_WritePin(BLE_EN_GPIO_Port, BLE_EN_Pin, GPIO_PIN_RESET)

#define LCD_ON                  HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET)
#define LCD_OFF                 HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET)

#define V5_ON                   HAL_GPIO_WritePin(V5_EN_GPIO_Port, V5_EN_Pin, GPIO_PIN_SET)
#define V5_OFF                  HAL_GPIO_WritePin(V5_EN_GPIO_Port, V5_EN_Pin, GPIO_PIN_RESET)

#define SDA_HI                  HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET)
#define SDA_LO                  HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET)

#define SCL_HI                  HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET)
#define SCL_LO                  HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET)

#define WRITE_DEVICE_ADDRESS    0xa0 
#define READ_DEVICE_ADDRESS     0xa1

#define RT_SUCCESS              0
#define RT_NO_ACK               1
#define RT_NO_ACK2              2

#define ALL     0
#define ZIGBEE  1
#define PC      3

#define RXBUFFERSIZE	        200
#define TXBUFFERSIZE		200


struct KEY {
    char PressFlg[14];
};

typedef struct {
    unsigned long    size;   /* maximum number of elements           */
    unsigned long    *start;  /* index of oldest element              */
    unsigned long    end;    /* index at which to write new element  */
    unsigned char    *elems;  /* vector of elements                   */
    unsigned char     Buf[RXBUFFERSIZE];
    unsigned char     UartCount;
} DMA_CircularBuffer;

struct FUNCTION {
    char Filter_Degree;
    char Pad_Sel;
    char Stable;
    char Weigh_In_Motion;
    char Over_Enable;
    char Print_Form;
    char Print_Copies;
    char LineFeed;
    char Print_Enable;
    char Data_Format;
    char Wireless_Output;
    char Off_Syns;
    char Auto_Measuring;
    char Auto_Stable;
    char Auto_Print;
    char Pad_Type;
    char Display_Used;
    char Print_Density;
    char KeyBeep;
};


// EEP
void eeprom_4byte_write(unsigned long addr, unsigned long data);
unsigned long eeprom_4byte_read(unsigned long addr);
void eeprom_2byte_write(unsigned long addr, unsigned int data);
unsigned int eeprom_2byte_read(unsigned long addr);
void eeprom_1byte_write(unsigned long addr, unsigned char data1);
unsigned char eeprom_1byte_read(unsigned long addr);
void eeprom_float_write(unsigned long addr, float data00);
float eeprom_float_read(unsigned long addr);
unsigned long sw_i2c_read(unsigned char stop, unsigned char *buf, unsigned long maxlen);
unsigned long sw_i2c_write(unsigned char stop, unsigned char *buf, unsigned long maxlen);
unsigned long sw_i2c_addr_write(unsigned char stop, unsigned long addr, unsigned char *buf, unsigned long maxlen);
unsigned long sw_i2c_addr_read(unsigned long stop, unsigned long addr, unsigned char *buf, unsigned long maxlen1);
void i2c_start(void);
void i2c_stop(void);
void interval(void);
void i2c_send_no_ack(void);
void i2c_send_ack(void);
unsigned char i2c_recv_ack(void);
unsigned char sda_value(void);
void i2c_send_byte(unsigned char data);
unsigned char i2c_recv_byte(void);
void function(void);
void title_Function(unsigned char set_num);
void number_long(unsigned char chat);
void function_read(void);
void function_write(void);
void function_reset(void);
void cal_write(void);

// DISPLAY
void Write_Command(unsigned char Command);
void Write_Data(unsigned char Data);
void OLED_All_Fill(unsigned char bmp_data);
void OLED_Fill(unsigned char bmp_data, unsigned char select);
void Oled_Gotoxy(unsigned char x_pos, unsigned char y_pos);
void Clear_Screen(void);
void Batt_Lamp(unsigned char x_pos, unsigned char y_pos, unsigned char Per);
void SBatt_Lamp(unsigned char y_pos, unsigned char Per);
void OLED_Initialize(void);
void Print_Str6x8(unsigned char Invert, unsigned char x_pos, unsigned char y_pos, char *ch);
int mprintf(const char x_pos, const char y_pos, const char* format, ...) ;
void PrintArrow(unsigned char x_pos, unsigned char y_pos, unsigned char ch);
void LOGO_Print(unsigned char select);
void Perforated_Line(void);
int wprintf(const char* format, ...);
void Print_Str18x40(unsigned char x_pos, unsigned char y_pos, char *ch);
void Print_Dot4x40(unsigned char x_pos, unsigned char y_pos, unsigned char chr);
void Print_Text18x40(unsigned char x_pos, unsigned char y_pos, unsigned char ch);
void Print_Num18x40(unsigned char x_pos, unsigned char y_pos, unsigned char ch);
void Bar_Graph(unsigned long max_capa, unsigned long set_weight, unsigned long weight_value, unsigned char division);
int square(unsigned char value, unsigned char num);

// UART
void DMA_CB_Init(DMA_CircularBuffer *cb, unsigned char * buffer ,unsigned long size,unsigned long * pDMA_Cnt );
unsigned long DMA_Get_Cnt(DMA_CircularBuffer *cb);
void DMA_CB_Read(DMA_CircularBuffer *cb, unsigned char* data, unsigned long cnt);
void DMA_CB_PreRead(DMA_CircularBuffer *cb, unsigned char* data, unsigned long cnt);

// AD
void chip_select(unsigned char enable, unsigned char ad);
long read_filtered_adc(unsigned char k);
void adc_initial(void);

// KEY
void KEYPAD_Scan(void);

// RTC
void TimeRead(void);
void rtc_set(void);

// CAL
void cal_mode(void);
void zero_span_set(unsigned char flag); 
unsigned int  cal_number_long(unsigned int init_value, unsigned char chat);

// TEST
void loadcell_test(void);

#define FUNCTION01        1
#define FUNCTION02        2
#define FUNCTION03        3
#define FUNCTION04        4
#define FUNCTION05        5
#define FUNCTION06        6
#define FUNCTION07        7
#define FUNCTION08        8
#define FUNCTION09        9
#define FUNCTION10        10
#define FUNCTION11        11
#define FUNCTION12        12
#define FUNCTION13        13
#define FUNCTION14        14
#define FUNCTION15        15
#define FUNCTION16        16
#define FUNCTION17        17
#define FUNCTION18        18
#define FUNCTION19        19
#define FUNCTION20        20

#define V_RES_FACTOR             100  // 4*5*2
#define V_ADC_ORG                140  // 4*5*2
#define V_ZERO                   180  // 4*2
#define V_MAXIMUM_CAPACITY       188  // 4*2
#define V_E_RESOLUTION           196  // 4*2
#define V_MINIMUM_DIVISION       204  // 1*2
#define V_MULTI_CAL              206  



void SystemClock_Config(void);
  
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

