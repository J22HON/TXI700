#include "main.h"

extern struct KEY Key;
extern struct FUNCTION FunData;

unsigned  long  ad_set_buf[2] = { 0,0 };
unsigned  long  set_weight[8][5] = { {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0} };
unsigned  long  back_adc=0;
unsigned  char  cal_no=0, k=0, pass_flag=0, cal_start=0;

extern unsigned char     v_ad_flag,
                         v_multi_cal,
                         v_minimum_division[2]; 

extern unsigned long    v_maximum_capacity[2],
                         v_e_resolution[2],
                         v_adc_org[2][5],
                         v_zero[2],
                         imsi_value,
                         v_temp_long;

extern float	         v_res_factor[2][5];
void cal_mode(void)
{
  static unsigned long temp_long;
 
RETRY:
    
  Clear_Screen();
  mprintf(1, 1, "    CALIBRTION MODE     "); 
  mprintf(1, 2, " STEP  : ");
  PrintArrow(68,7,0);
  PrintArrow(80,7,1);
  PrintArrow(94,7,2);
  Print_Str6x8(0xFF,110,7,"1/1");
  HAL_Delay(500);
    
  temp_long = v_multi_cal;
  
  while(1)
  {
    KEYPAD_Scan();
    
    if(!temp_long) temp_long = 1;
    
    if(Key.PressFlg[2])
    {
      Key.PressFlg[2] = 0;
      if(temp_long == 1) temp_long = 2;
      else if(temp_long == 2) temp_long = 5;
      else if(temp_long == 5) temp_long = 1;
      else temp_long = 1;
    }
    
    else if(Key.PressFlg[8])
    {
      Key.PressFlg[8] = 0;
      if(temp_long == 5) temp_long = 2;
      else if(temp_long == 2) temp_long = 1;
      else if(temp_long == 1) temp_long = 5;
      else temp_long = 1;      
    }
        
    else if(Key.PressFlg[13])
    {
      Key.PressFlg[13] = 0;
      if(temp_long)
      {
        v_multi_cal = temp_long;
      }
      else return;
      
      HAL_Delay(500);      
      break;
    }      
    
    mprintf(1, 2, " STEP  :      %d" ,temp_long );
  }  
  
  temp_long = 1;
  
  while(1)
  {
    KEYPAD_Scan();    
    
    if(Key.PressFlg[2])
    {
      Key.PressFlg[2] = 0;
      if(temp_long >= 2) temp_long = 1;
      else temp_long = temp_long + 1;
    }
    
    else if(Key.PressFlg[8])
    {
      Key.PressFlg[8] = 0;
      if(temp_long <= 1) temp_long = 2;
      else temp_long = temp_long - 1;        
    }
        
    else if(Key.PressFlg[13])
    {
      Key.PressFlg[13] = 0;
      if(temp_long)
      {
        cal_no = temp_long-1;
      }
      else return;
      
      HAL_Delay(500);
      break;
    }      
    
    mprintf(1, 3, " PAD   :      %d" ,temp_long );
  }  

  adc_initial();

  mprintf(1, 4, " CAPA  : "); HAL_Delay(700); 
  
  v_maximum_capacity[cal_no] = cal_number_long(v_maximum_capacity[cal_no], 0);
  
  mprintf(1, 5, " DIV   : "); HAL_Delay(700); 
  v_minimum_division[cal_no] = cal_number_long(v_minimum_division[cal_no],1);
  v_e_resolution[cal_no] = v_maximum_capacity[cal_no] / v_minimum_division[cal_no];
  
  zero_span_set(0);
  
  if(v_multi_cal==1) // 1???? ????
  {
    k=0;
    set_weight[cal_no][k] = cal_number_long(v_maximum_capacity[cal_no],2);
    if( (set_weight[cal_no][k] > v_maximum_capacity[cal_no]) || ((set_weight[cal_no][k]*100) < v_maximum_capacity[cal_no]) )
    {
      Clear_Screen();
      mprintf(1, 2, " C H 0 4 ");
      HAL_Delay(700); 
      goto RETRY; 
    }
    zero_span_set(1); 
    if(!pass_flag) goto RETRY;
  }
  //***********2 ???? ????************
  else if(v_multi_cal==2) // 2???? ????
  {
    for(k=0; k<2; k++)
    {
      set_weight[cal_no][k] = cal_number_long(v_maximum_capacity[cal_no]/2*(k+1),2);
      if( (set_weight[cal_no][k] > v_maximum_capacity[cal_no]) || ((set_weight[cal_no][k]*100) < v_maximum_capacity[cal_no]) )
      {
        Clear_Screen();
        mprintf(1, 2, " C H 0 4 ");
        HAL_Delay(700); 
        goto RETRY; 
      }
      zero_span_set(1);
      if(!pass_flag) goto RETRY;
    }    
  }
  //***********5 ???? ????************
  else if(v_multi_cal==5) // 5???? ????
  { 
    for(k=0; k<5; k++)
    {
      set_weight[cal_no][k] = cal_number_long(v_maximum_capacity[cal_no]/5*(k+1),2);
      if( (set_weight[cal_no][k] > v_maximum_capacity[cal_no]) || ((set_weight[cal_no][k]*100) < v_maximum_capacity[cal_no]) )
      {
        Clear_Screen();
        mprintf(1, 2, " C H 0 4 ");
        HAL_Delay(700); 
        goto RETRY; 
      }
      zero_span_set(1);
      if(!pass_flag) goto RETRY;
    }    
  }
  Clear_Screen();
  cal_write();
  mprintf(1, 2, " END ");
  HAL_Delay(700);
  Clear_Screen();
}

void zero_span_set(unsigned char flag)  // 0 : zero set, 1: span set 
{
  unsigned char j, exit_flag = 0;
  float  v_temp_float;
          
  pass_flag = 0; // Used for only zero calibration
  
  Clear_Screen();
  
  mprintf(1, 1, "    CALIBRTION MODE     "); 
  
  if(!cal_start)
  {
    if(flag) mprintf(1, 3, " L O A D ");
    else     mprintf(1, 2, " Z E R 0 ");
    HAL_Delay(1000);
  }
  do
  {
    KEYPAD_Scan();
    
    if( Key.PressFlg[13] || cal_start )
    {
      if(cal_start) Key.PressFlg[13] = 1;
      if(Key.PressFlg[13])
      {
        if(flag) mprintf(1, 3, " - - - - ");
        else mprintf(1, 2, " - - - - ");   
        imsi_value = 0;
        for(j=0; j<3; j++)  
        { 
          do v_temp_long = read_filtered_adc(cal_no); while(!v_ad_flag);
          imsi_value = imsi_value + v_temp_long;
        }
        
        v_temp_long = imsi_value / 3;
        if(flag) ad_set_buf[1] = v_temp_long;
        else     ad_set_buf[0] = v_temp_long;

        if(flag)
        {
          if(k==0) v_temp_long = (v_e_resolution[cal_no] * 5) / (unsigned long)((float)v_maximum_capacity[cal_no] / (float)set_weight[cal_no][k]);
          else     v_temp_long = (v_e_resolution[cal_no] * 5) / (unsigned long)((float)v_maximum_capacity[cal_no] / (float)(set_weight[cal_no][k]-set_weight[cal_no][k-1]));
          if(k!=0) ad_set_buf[0]=back_adc; 
 
          if((ad_set_buf[1] - ad_set_buf[0]) < v_temp_long) 
          { 
            Clear_Screen();
            mprintf(1, 2, " C H 0 5 ");
            HAL_Delay(700); 
            exit_flag = 1; 
          } 
          else
          { 
              v_temp_long = ad_set_buf[0];
              back_adc = ad_set_buf[1];
              if(k==0) v_temp_float = (float)(ad_set_buf[1] - v_temp_long) * ((float)v_maximum_capacity[cal_no] / (float)set_weight[cal_no][k]);
              else     v_temp_float = (float)(ad_set_buf[1] - v_temp_long) * ((float)v_maximum_capacity[cal_no] / (float)(set_weight[cal_no][k]-set_weight[cal_no][k-1]));

              v_res_factor[cal_no][k] = (float)(v_e_resolution[cal_no] * 20) / v_temp_float;
              if(k==0) 
              {
                v_adc_org[cal_no][k] = (unsigned long) ((float)v_temp_long * v_res_factor[cal_no][0]);
                v_zero[cal_no] = (unsigned long)v_temp_long;
              }
              else  v_adc_org[cal_no][k] = (unsigned long) v_temp_long;            
              pass_flag = 1;
           }
	        exit_flag=1;
         } // of if(flag)  
	      else exit_flag=1; // go to LOAD 
       } // if(sw_data==ENT_K)
    } // if(key_code & KEY_PRESSED)
  } while(!exit_flag);
}

unsigned int  cal_number_long(unsigned int init_value, unsigned char chat)
{
  unsigned int value = 0;
  char key_set = 0;
  char key_push = 0;
  
  value = init_value;
  
  while(1)
    {
        KEYPAD_Scan();        
        
        if(Key.PressFlg[0])       { Key.PressFlg[0] = 0; key_set = 0; key_push = 1;}
        else if(Key.PressFlg[1])  { Key.PressFlg[1] = 0; key_set = 1; key_push = 1;}
        else if(Key.PressFlg[2])  { Key.PressFlg[2] = 0; key_set = 2; key_push = 1;}
        else if(Key.PressFlg[3])  { Key.PressFlg[3] = 0; key_set = 3; key_push = 1;}
        else if(Key.PressFlg[4])  { Key.PressFlg[4] = 0; key_set = 4; key_push = 1;}
        else if(Key.PressFlg[5])  { Key.PressFlg[5] = 0; key_set = 5; key_push = 1;}
        else if(Key.PressFlg[6])  { Key.PressFlg[6] = 0; key_set = 6; key_push = 1;}
        else if(Key.PressFlg[7])  { Key.PressFlg[7] = 0; key_set = 7; key_push = 1;}
        else if(Key.PressFlg[8])  { Key.PressFlg[8] = 0; key_set = 8; key_push = 1;}
        else if(Key.PressFlg[9])  { Key.PressFlg[9] = 0; key_set = 9; key_push = 1;}
        else if(Key.PressFlg[12]) { Key.PressFlg[12] = 0; key_set = 0; value = 0; }
        else if(Key.PressFlg[13]) { Key.PressFlg[13] = 0;  return (value); }
             
        if( (!value) && key_push ) {value += key_set; key_push = 0;}
        else if(key_push == 1)
        {
            value *= 10;
            value += key_set;
            key_push = 0;
        }
        if( (!chat || chat == 2) && value > 999999) value = 0;
        else if( chat == 1 && value > 50) value = 0; 
        key_set = 0;
               
        if(!chat)               mprintf(1, 4, " CAPA  : %6d     " ,value);
        else if(chat==1)        mprintf(1, 5, " DIV   :     %2d     " ,value);
        else if(chat==2)        mprintf(1, 3, " SET-F  : %6d    " ,value);
    }
}