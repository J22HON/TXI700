#include "main.h"
#include "dma.h"
#include "usart.h"

  extern UART_HandleTypeDef hlpuart1;
  extern UART_HandleTypeDef huart2;
  extern UART_HandleTypeDef huart3;

  extern struct FUNCTION FunData;
  extern struct KEY Key;

  extern DMA_CircularBuffer Uart1Rx, Uart2Rx, Uart3Rx;

  extern unsigned char          ShortBeep,
                                 Time01Start,
                                 Time02Start,
                                 PowerOn,
                                 v_ad_flag,
                                 v_multi_cal,
                                 v_minimum_division[2],
                                 BatteryLevel,                                 
                                 cal_confirm,
                                 stable_flag[2],
                                 over_flag[2],
                                 count[2],
                                 v_az_scope,
                                 imsi,
                                 v_target,
                                 Cal_ad_flag,
                                 step_ch[2],
                                 Sleep[16],
                                 chck_R_L_flag,
                                 v_cal_flag,
                                 difr_flag,
                                 excess_flag,
                                 UartTxBuf[TXBUFFERSIZE],
                                 Uart1TxBuf[TXBUFFERSIZE],
                                 Uart2TxBuf[TXBUFFERSIZE],
                                 UartRxBuf[200],
                                 Uart2RxBuf[200],
                                 v_car_id[12],
                                 v_item[10],
                                 p_head[120],
                                 step,
                                 ErrorFlg,
                                 OverFlg,
                                 disp_sel,
                                 one_time,
                                 NextStep,
                                 AutoEnterKeyFlg,
                                 v_stable_check,
                                 AutoPrintStep,
                                 AutoPrintCheck,
                                 v_stable_flag,
                                 com1_flag,
                                 com2_flag;                                                     
  
  extern unsigned long          Time01Count,
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
                                 diff1[2],
                                 GetCurrRecvCount,          
                                 GetPreRecvCount,
                                 CmdWaitCount,
                                 v_over_weight,
                                 chck_R_L_t,
                                 chck_R_L_tm,
                                 inmtn_lmt_tm,
                                 v_rs_flag1,
                                 rs_err,
                                 wim_error;    
 
  extern unsigned short         zero_count[2],
                                 s_delay;
  
  extern long                    BattOffSet,
                                  prev_adc1[2],
                                  v_adc1_buf, 
                                  v_adc3_buf,                                  
                                  v_axle_value1[12],
                                  v_axle_value2[12],
                                  v_e_value[24],
                                  v_axle_sum,
                                  v_sum_value,
                                  v_auto_weight_stable_time_count,
                                  v_auto_print_time_count;
                                 
  extern unsigned int            v_speed,
                                  hlpuart1RxCnt,
                                  ErrorFlgCount;   
  
  extern float                   v_res_factor[2][5];
  
  unsigned char                  change_step,
                                  clr_m_flag=0,
                                  step_number=0,
                                  pad_no=0,
                                  in_pad=0;

void Wireless_Normal_Mode(void)
{      
  if(FunData.Weigh_In_Motion==2) chack_R_L_step();
  
  else
  {	
    chck_R_L_t=0;
    chck_R_L_tm=0; 
    inmtn_lmt_tm=0;
  }

  if(clr_m_flag) 
  {
    clr_m_flag=0;
    inmotion_sum_prt();
  }
  
  if(!FunData.Weigh_In_Motion)v_auto_weight_stable_time_check();
  
  s_delay = 0;
  
  Uart1TxBuf[0] = 0x21;
  Uart1TxBuf[1] = 49+pad_no;
  
  if(FunData.Pad_Type == TXDI)
  {
    if(Sleep[pad_no] == 2 || Sleep[pad_no] == 3) {Uart1TxBuf[2] = 0x5B;}  
    else Uart1TxBuf[2] = 0x5D;
    if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 3)!= HAL_OK) Error_Handler();
    HAL_Delay(300); 
  }
  else
  {
    if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 2)!= HAL_OK) Error_Handler();
    HAL_Delay(300);
  }
  
  while(v_rs_flag1==0)
  {
    Cmd_Recv_Check(&Uart1Rx);
    //if(Uart1Rx.Buf[11] == 0x0A) {v_rs_flag1 = 1;}
    
    if(FunData.Pad_Type == TXDI)
    {
      if(s_delay>50 && (Sleep[pad_no] < 2 || Sleep[pad_no] == 4))
      { 
          rs_err++; 
          if(rs_err>50) 
          { 
              rs_err=0; 
              if(FunData.Weigh_In_Motion==2)	difr_flag=1; 
              else				difr_flag=0; 
              //rf_error(pad_no+1); 
              if(FunData.Wireless_Output)	stt_out('Q');
          } 
          return;
      }
      else if((s_delay > 50) && (Sleep[pad_no] == 2 || Sleep[pad_no] == 3))
      {
          pad_no++;
          if(pad_no>=FunData.Pad_Sel)	pad_no=0;
          return;
      }
    }
    else if(FunData.Pad_Type == TXD)
    {
      if(s_delay>50)
      { 
          rs_err++; 

          if(rs_err>200) 
          { 
              rs_err=0;
              if(FunData.Weigh_In_Motion==2)	difr_flag=1; 
              else				difr_flag=0; 
              //rf_error(pad_no+1); 
              if(FunData.Wireless_Output)	stt_out('Q');
          } 
          return;
      }
    }    
  }

  if(wim_error==0)
  {
    if(FunData.Wireless_Output)			
    {
        rf_send(v_sum_value, ErrorFlg);
        if(FunData.Pad_Sel==4) tf_each_send(in_pad,v_e_value[in_pad]);
    }
    //if(v_rs_mode==1)		each_send(in_pad, v_e_value[in_pad]);	
    //else if(v_rs_mode==3)	sum_send(v_sum_value);	
  }
  
  rs_err=0;
  
  if(FunData.Weigh_In_Motion==2)
  {
      if(difr_flag == 1)
      {
        if( step_ch[0] != step_ch[1] )
        {
            difr_flag=0;
            chck_R_L_tm=chck_R_L_t+1;
        }
        else if( (step_ch[0]==0)&&(step_ch[1]==0) )
        {
            difr_flag=0;
            chck_R_L_tm=0; 
            inmtn_lmt_tm=0;
        }
      }
  }
  
  if( (Uart1Rx.Buf[2]=='A')||(Uart1Rx.Buf[2]=='B') )	
  {
    if(Uart1Rx.Buf[2]=='A'){ wprintf(" FAST"); if(FunData.Wireless_Output){ stt_out('B'); }}
    else		 { wprintf(" SLO"); if(FunData.Wireless_Output){ stt_out('A'); }}
    wim_error=1;
  }
  
  else
  {
    ;
  }
  
  Buf_init(1);
  
     
}

void Wired_Normal_Mode(void)
{
  unsigned char i;
  
  for(i=0; i<8; i++) 
  { 
      stable_flag[i]=0;
      count[i]=0; 
      over_flag[i]=0;
  }
  
  v_az_scope = FunData.Auto_Zero * 20;
  imsi = v_multi_cal;
  
  KEYPAD_Scan();
  
  //READ A/D
  for(i=0; i<2; i++)
  {         
    do v_temp_long = read_filtered_adc(i); while(!v_ad_flag);
    
    v_adc1_buf = (long) ( ((float) v_temp_long) * v_res_factor[i][0] );

    if(imsi==5)
    {
      if(v_temp_long >= v_adc_org[i][1]) 
      { 
        v_adc1_buf = (long)((float) v_adc_org[i][1] * v_res_factor[i][0]) 
                   + (long)((float)(v_temp_long - v_adc_org[i][1]) * v_res_factor[i][1]);    	 

        if(v_temp_long >= v_adc_org[i][2])
        { 
          v_adc1_buf = (long)((float) v_adc_org[i][1] * v_res_factor[i][0]) 
                     + (long)((float)(v_adc_org[i][2] - v_adc_org[i][1]) * v_res_factor[i][1]) 
                     + (long)((float)(    v_temp_long - v_adc_org[i][2]) * v_res_factor[i][2]); 
                             
          if(v_temp_long >= v_adc_org[i][3]) 
          { 
            v_adc1_buf = (long)((float) v_adc_org[i][1] * v_res_factor[i][0]) 
                       + (long)((float)(v_adc_org[i][2] - v_adc_org[i][1]) * v_res_factor[i][1]) 
                       + (long)((float)(v_adc_org[i][3] - v_adc_org[i][2]) * v_res_factor[i][2]) 
                       + (long)((float)(    v_temp_long - v_adc_org[i][3]) * v_res_factor[i][3]); 
                             
            if(v_temp_long >= v_adc_org[i][4])
            { 
                    v_adc1_buf = (long)((float) v_adc_org[i][1] * v_res_factor[i][0]) 
                               + (long)((float)(v_adc_org[i][2] - v_adc_org[i][1]) * v_res_factor[i][1]) 
                               + (long)((float)(v_adc_org[i][3] - v_adc_org[i][2]) * v_res_factor[i][2]) 
                               + (long)((float)(v_adc_org[i][4] - v_adc_org[i][3]) * v_res_factor[i][3]) 
                               + (long)((float)(    v_temp_long - v_adc_org[i][4]) * v_res_factor[i][4]); 
            }
          }
        }
      }
    }
    else if(imsi==2)
    {
      if(v_temp_long >= v_adc_org[i][1]) 
      { 
        v_adc1_buf = (long)((float)v_adc_org[i][1]*v_res_factor[i][0]) 
                   + (long)((float)(v_temp_long - v_adc_org[i][1]) * v_res_factor[i][1]);   	 
      }
    }

    //********************* STABLE CHECK ***************************/
    if ( v_adc1_buf > v_adc_org[i][0]) 	diff1[i] = v_adc1_buf      - v_adc_org[i][0];
    else                               	diff1[i] = v_adc_org[i][0] - v_adc1_buf;

    if ( v_adc1_buf > prev_adc1[i] ) 	diff[i]  = v_adc1_buf   - prev_adc1[i];
    else                             	diff[i]  = prev_adc1[i] - v_adc1_buf;

    if(!v_cal_flag)
    {	
      if(FunData.Auto_Zero)
      {
        if ( (diff1[i]<=v_az_scope) && stable_flag[i] )
        {
          if( zero_count[i] > 1000 )
          {
                  zero_count[i] = 0;
                  multi_gap(v_adc1_buf, v_adc_org[i][0], 0, i);
                  v_adc_org[i][0]=v_adc1_buf;
          } 
        }
        else  zero_count[i]=0;       
      }
    }
          
    if(diff[i] > 9) count[i] = 1;
          
    if(count[i] < (2+FunData.Stable))
    {
      if(diff[i] <= 9) count[i]++; 
      else                     
      { 
        stable_flag[i]=0; 
        count[i]=1;
      }
    }
    else
    {
      stable_flag[i]=1;
      count[i] = 1;
    }
          
    prev_adc1[i] = v_adc1_buf;

    // Calcurate Weight	  
    v_adc3_buf = v_adc1_buf - v_adc_org[i][0];   

    if(v_adc3_buf>=0) v_e_value[i] = (v_adc3_buf + 10) / 20;	// ???? ??
    else              v_e_value[i] = (v_adc3_buf - 10) / 20;    

    if(!v_cal_flag)
    {
      if(v_adc3_buf>=0)
      {
        if( v_adc3_buf > ((v_e_resolution[i]+9)*20) ) over_flag[i]=1;	// OVER
        else over_flag[i]=0; 									
      }
    }
    else  over_flag[i]=0;
          
    v_e_value[i] = v_e_value[i] * (unsigned long)v_minimum_division[i];   
  }          
}

void WeightModeSendToPad(void)
{
  if(FunData.Pad_Sel == 2 && FunData.Func_Key == 0)
  {
    FunData.Weigh_In_Motion++;
    if(FunData.Weigh_In_Motion>2)FunData.Weigh_In_Motion=0;
    eeprom_1byte_write(WEIGH_IN_MOTION, FunData.Weigh_In_Motion);
    HAL_Delay(40);
    
    if(FunData.Weigh_In_Motion==0) 
    {
      Uart1TxBuf[0] = 0x21;
      Uart1TxBuf[1] = 'S';
      Uart1TxBuf[2] = 'T';
      Uart1TxBuf[3] = 'M';
      Uart1TxBuf[4] = 'O';
      Uart1TxBuf[5] = 'D';
      Uart1TxBuf[6] = 'E';
      Uart1TxBuf[7] = 'I';
      Uart1TxBuf[8] = 'I';
      Uart1TxBuf[9] = 0x0D;
      Uart1TxBuf[10] = 0x0A;
      if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
      {
        Error_Handler();
      }
      HAL_Delay(40);   
      ShortBeep = 1;
      HAL_Delay(40);
    }
    
    else if(FunData.Weigh_In_Motion==1)
    {
      Uart1TxBuf[0] = 0x21;
      Uart1TxBuf[1] = 'S';
      Uart1TxBuf[2] = 'T';
      Uart1TxBuf[3] = 'M';
      Uart1TxBuf[4] = 'O';
      Uart1TxBuf[5] = 'D';
      Uart1TxBuf[6] = 'E';
      Uart1TxBuf[7] = 'W';
      Uart1TxBuf[8] = 'W';
      Uart1TxBuf[9] = 0x0D;
      Uart1TxBuf[10] = 0x0A;
      if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
      {
        Error_Handler();
      }
      HAL_Delay(40); 
      ShortBeep = 1;
      HAL_Delay(40);
    }
    
    else if(FunData.Weigh_In_Motion==2)
    {
      Uart1TxBuf[0] = 0x21;
      Uart1TxBuf[1] = 'S';
      Uart1TxBuf[2] = 'T';
      Uart1TxBuf[3] = 'M';
      Uart1TxBuf[4] = 'O';
      Uart1TxBuf[5] = 'D';
      Uart1TxBuf[6] = 'E';
      Uart1TxBuf[7] = 'P';
      Uart1TxBuf[8] = 'P';
      Uart1TxBuf[9] = 0x0D;
      Uart1TxBuf[10] = 0x0A;
      if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
      {
        Error_Handler();
      }
      HAL_Delay(40); 
      ShortBeep = 1;
      HAL_Delay(40);
    }    
  }

}

void chack_R_L_step(void)
{
  if(chck_R_L_flag==1)
  {
    chck_R_L_flag=0;  
    step_ch[0]=step_ch[1]=0;
    chck_R_L_tm=0; 
    inmtn_lmt_tm=0; 
    wprintf("MISS"); 
    bbik();
    clr_motion(); 
    HAL_Delay(700);
  }
}

void clr_motion(void)
{
  unsigned char i;	
	
  v_axle_sum=0; 
  step=0; 
  change_step=0; 
	
  if(FunData.Pad_Type == TXD || FunData.Pad_Type == TXDI)
  {
      for(i=0; i<3; i++) 
      { 
        Uart1TxBuf[0] = 0x21;
        Uart1TxBuf[1] = 'S';
        Uart1TxBuf[2] = 'T';
        Uart1TxBuf[3] = 'M';
        Uart1TxBuf[4] = 'O';
        Uart1TxBuf[5] = 'D';
        Uart1TxBuf[6] = 'E';
        Uart1TxBuf[7] = 'C';
        Uart1TxBuf[8] = 'C';
        Uart1TxBuf[9] = 0x0D;
        Uart1TxBuf[10] = 0x0A;
        if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
        {
          Error_Handler();
        }
        HAL_Delay(40);
      }
  }	
  Buf_init(1);
}

void inmotion_sum_prt(void)
{
  unsigned char i=0;

  if(FunData.Weigh_In_Motion) 
  { 
    if(FunData.Pad_Type == TXD || FunData.Pad_Type == TXDI)
    {
      for(i=0; i<3; i++) 
      { 
        Uart1TxBuf[0] = 0x21;
        Uart1TxBuf[1] = 'S';
        Uart1TxBuf[2] = 'T';
        Uart1TxBuf[3] = 'M';
        Uart1TxBuf[4] = 'O';
        Uart1TxBuf[5] = 'D';
        Uart1TxBuf[6] = 'E';
        Uart1TxBuf[7] = 'C';
        Uart1TxBuf[8] = 'C';
        Uart1TxBuf[9] = 0x0D;
        Uart1TxBuf[10] = 0x0A;
        if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
        {
          Error_Handler();
        }
        HAL_Delay(40);
      }
    }
  }
  
  SUM_PRT_Lamp(SUM);
 
  v_axle_sum=0;

  step=step_number;

  if(step>=1)
  {
    if(FunData.Pad_Sel<=2)
    { 
      for(i=0; i<step; i++)	
      {
        v_axle_sum = v_axle_sum+ v_axle_value1[i]+v_axle_value2[i]; 
      }
    }
  }
  else 
  {
    v_axle_sum = v_sum_value;							  
  }

  wprintf(" %5ld",v_axle_sum);
  HAL_Delay(1000);

  while(1)
  {
    if(FunData.Wireless_Output) rf_send(v_axle_sum, ErrorFlg);
    ShortBeep = 1;

    if(FunData.Pad_Sel<=2) step=step_number;
    else
    {
      step = FunData.Pad_Sel/2;
            
      v_axle_value1[0]=v_e_value[0]; 
      v_axle_value1[1]=v_e_value[2]; 
      v_axle_value1[2]=v_e_value[4];
      v_axle_value1[3]=v_e_value[6]; 
      v_axle_value1[4]=v_e_value[8]; 
      v_axle_value1[5]=v_e_value[10];
      v_axle_value1[6]=v_e_value[12];
      v_axle_value1[7]=v_e_value[14];
      v_axle_value1[8]=v_e_value[16];
      v_axle_value1[9]=v_e_value[18];
      v_axle_value1[10]=v_e_value[20];
      v_axle_value1[11]=v_e_value[22];

      v_axle_value2[0]=v_e_value[1];
      v_axle_value2[1]=v_e_value[3];
      v_axle_value2[2]=v_e_value[5];
      v_axle_value2[3]=v_e_value[7]; 
      v_axle_value2[4]=v_e_value[9]; 
      v_axle_value2[5]=v_e_value[11];
      v_axle_value2[6]=v_e_value[13];
      v_axle_value2[7]=v_e_value[15];
      v_axle_value2[8]=v_e_value[17];
      v_axle_value2[9]=v_e_value[19];
      v_axle_value2[10]=v_e_value[21];
      v_axle_value2[11]=v_e_value[23];
    }  	
         
    if(FunData.Over_Enable==1)
    {
      excess_sig();
      if(excess_flag) {ErrorFlg = 1; OverFlg = 1;}
      else ErrorFlg = 2;
      if(FunData.Wireless_Output)
      {
        rf_send(v_axle_sum, ErrorFlg);
        HAL_Delay(30);
        rf_send(v_axle_sum, ErrorFlg);
        HAL_Delay(30);
        rf_send(v_axle_sum, ErrorFlg);
      }
    }
   
    if(FunData.Print_Enable) 
    { 
      one_time=0;
     // print_out(2);
      one_time=1;

     // if(FunData.Print_Copies) print_out(2); 
     // data_to_flash();
    }

    for(i=0; i<10; i++) 
    { 
      v_car_id[i]=0xFF; 
      v_item[i]=0xFF;
    }
    
    for(i=0; i<12; i++) 
    {
      v_axle_value1[i]=0;
      v_axle_value2[i]=0;
    }
          
    v_axle_sum=0;

    break;
  }

    disp_sel=0;
	
    if(FunData.Weigh_In_Motion)
    {
      if(FunData.Pad_Type == TXD || FunData.Pad_Type == TXDI)
      {
        for(i=0; i<3; i++) 
        { 
          Uart1TxBuf[0] = 0x21;
          Uart1TxBuf[1] = 'S';
          Uart1TxBuf[2] = 'T';
          Uart1TxBuf[3] = 'M';
          Uart1TxBuf[4] = 'O';
          Uart1TxBuf[5] = 'D';
          Uart1TxBuf[6] = 'E';
          Uart1TxBuf[7] = 'C';
          Uart1TxBuf[8] = 'C';
          Uart1TxBuf[9] = 0x0D;
          Uart1TxBuf[10] = 0x0A;
          if(HAL_UART_Transmit_DMA(&hlpuart1, (unsigned char*)Uart1TxBuf, 11)!= HAL_OK)
          {
            Error_Handler();
          }
          HAL_Delay(40);
        }
      }
    }

    step=0; 
    change_step=0;
}

void excess_sig(void)
{
  unsigned char i;
  
  if(FunData.Over_Enable && v_over_weight)											
  {
    if(v_over_weight<v_axle_sum)
    {
      excess_flag=1;
      for(i=0; i<3; i++) 
      {
        bbik();
        HAL_Delay(500);
      }
    }
  }
}

void v_auto_weight_stable_time_check(void)  
{
  if(FunData.Auto_Stable && (FunData.Pad_Sel==2) && (v_sum_value>100) && !NextStep)
  {
    if(v_auto_weight_stable_time_count >= (long)((long)FunData.Auto_Stable*25))  //1sec ? 25
    {
      AutoEnterKeyFlg=1;
      NextStep=1;
      v_auto_weight_stable_time_count=0;
      v_auto_print_time_count=0;
      v_stable_check=0;
      AutoPrintStep=1;
    }
    if(v_stable_flag)v_stable_check=1;   
    if(v_stable_check) v_auto_weight_stable_time_count++;
    else v_auto_weight_stable_time_count=0;
  }
  else if(NextStep && (v_sum_value<=100))
  {
    NextStep=0;
    v_auto_weight_stable_time_count=0;
    v_stable_check=0;
  }
  if((v_sum_value<=100) && AutoPrintStep || (step >=12))
  {
    v_auto_print_time_count ++;
    if((v_auto_print_time_count >= (long)((long)FunData.Auto_Print * 75)) ||  (step >=12))
    {
      AutoPrintCheck = 1;
      NextStep = 0;
      AutoPrintStep =0;
      v_auto_print_time_count = 0;
    }
  }
}