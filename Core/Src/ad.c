#include "main.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

 extern struct FUNCTION FunData;
 extern struct KEY Key;

 extern unsigned char             ShortBeep,
                                   Time01Start,
                                   Time02Start,
                                   PowerOn,
                                   v_ad_flag,                         
                                   BatteryLevel,
                                   StableReadyFlg,
                                   PressCalFlg,
                                   over_flag[2],
                                   stable_flag[2],
                                   count[2],
                                   v_az_scope,
                                   imsi,
                                   v_multi_cal,
                                   v_cal_flag,
                                   v_minimum_division[2],
                                   cal_confirm;                                                                                                 
  
 extern unsigned long             Time01Count,
                                   Time02Count,
                                   ShortBeepCount,
                                   RealTime,
                                   RealDate,                         
                                   v_temp_long,
                                   StableCount,
                                   v_adc_org[2][5],
                                   diff[2],
                                   diff1[2],
                                   v_zero[2],
                                   v_e_resolution[2],
                                   v_over_weight;
                                            
 extern unsigned short           zero_count[2],
                                  v_gravity_factor1,
                                  v_gravity_factor2;
  
 extern long                     BattOffSet,
                                  prev_adc1[2],
                                  v_adc1_buf,
                                  v_adc3_buf,
                                  v_e_value[2];
                         
  
 extern unsigned int             v_speed;   
 
 extern float                    v_res_factor[2][5];                         
 
void chip_select(unsigned char enable, unsigned char ad)
{
  if(enable)
  {
    if(ad==0)      { ADC_CS1_LO; ADC_CS2_HI;}
    else if(ad==1) { ADC_CS1_HI; ADC_CS2_LO;}    
  }
  else { ADC_CS1_HI; ADC_CS2_HI;}
}

long read_filtered_adc(unsigned char k)
{
    unsigned char i, j, v;
    unsigned long read_data=0;
    
    read_data = 0;
    v_ad_flag = 0;
    
    chip_select(1, k);
    for(i=0; i<10; i++)	{} // Necessary delay time
    if(ADC_MISO) { chip_select(0, 0); return(0); }

    v=0x5C;
    for(i=0; i<8; i++)
    {
        if(v & 0x80)ADC_MOSI_HI ;
        else         ADC_MOSI_LO;
        ADC_SCK_LO;
        interval();
        ADC_SCK_HI;
        v = v << 1;
    }
    
    for(i=0; i<3; i++)
    {
        for(j=0; j<8; j++)
        {
            ADC_SCK_LO;
            read_data = read_data << 1;
            if (ADC_MISO) read_data = read_data | 0x00000001;
            ADC_SCK_HI;
        }
    }
    read_data= read_data >> 3;
    chip_select(0, 0);
    v_ad_flag = 1;
    return(read_data);
}

void adc_initial(void)
{
    unsigned char ad, i, j, temp_char;
    unsigned char adc_set_data[8]= { 0x08,0x0C,0x40,0x00,  //mode
    0x10,0x00,0x02,0x18,  //configuration
    };
    unsigned int  v_speed;
    
    // communications : 0000 1000 (mode resister select)
    // mode_1         : 0000 1100 (internal clock)
    // mode_2         : 0000 0000
    // mode_3         : 0000 0000 (4805/10Hz)
    // communications : 0001 0000 (configuration resister select)
    // configration_1 : 0000 0000 (chop disable)
    // configration_2 : 0000 0010 (channel 3-4)
    // configration_3 : 0001 1000 (buffer, unipolar, chop enable)
    // communications : 0011 0000 (offset resister select)
    // offset1        : 0000 0000 (offset1)
    // offset2        : 0000 0010 (offset2)
    // offset3        : 0001 1000 (offset3)
    
    if(FunData.Filter_Degree==1)  v_speed=40;      //120/sec
    else if(FunData.Filter_Degree==2)  v_speed=80;      //60/sec
    else if(FunData.Filter_Degree==3)  v_speed=96;      //50/sec
    else if(FunData.Filter_Degree==4)  v_speed=480;     //10/sec
    else if(FunData.Filter_Degree==5)  v_speed=640;     //7.5/sec
    else if(FunData.Filter_Degree==6)  v_speed=1023;    //4.7/sec
    else if(FunData.Filter_Degree==8)  {v_speed=640; adc_set_data[5]= adc_set_data[5] | 0x80;}   //1.875/sec chop enable
    else if(FunData.Filter_Degree==9)  {v_speed=1023; adc_set_data[5]= adc_set_data[5] | 0x80;}  //1.175/sec chop enable
    
    //1,2,5,16,32,40,80, 96,480,640, 1023, 
    
    //v_speed = 40;// adc_set_data[5]=0x80;
    adc_set_data[2] = adc_set_data[2] | (unsigned char)(v_speed>>8);
    adc_set_data[3] = adc_set_data[3] | (unsigned char)(v_speed&0x00FF);
    adc_set_data[7] = adc_set_data[7] | 7; // gain 128
    
    for(ad=0; ad<2; ad++)
    {
      chip_select(1, ad);
      ADC_SCK_HI;
      ADC_MOSI_HI;
      HAL_Delay(1);
      for(i=0; i<80; i++)
      {	
          ADC_SCK_LO; interval(); ADC_SCK_HI; 
      }
      ADC_MOSI_LO;
      HAL_Delay(100);
      for(j=0; j<8; j++)
      {
          temp_char=adc_set_data[j]; 
          for(i=0; i<8; i++)
          {
              if(temp_char & 0x80) ADC_MOSI_HI;
              else  ADC_MOSI_LO;
              ADC_SCK_LO;
              interval();
              ADC_SCK_HI;
              temp_char = temp_char << 1;
          }
      }
      chip_select(0, 0);
    }
}

void multi_gap(unsigned long comp_value1, unsigned long comp_value2, unsigned char k, unsigned char j)
{
  unsigned char p;
  unsigned long gap;


  if(comp_value1>=comp_value2) // multi calibraion ????
  { 
          if(k==1) gap = comp_value1-comp_value2; 
          else     gap = (unsigned long)((float)(comp_value1-comp_value2) / v_res_factor[j][0]);
          
          for(p=1 ; p<5 ; p++) 
                  v_adc_org[j][p] = v_adc_org[j][p]+gap;
  }
  else
  {
          if(k==1) gap = comp_value2-comp_value1; 
          else     gap = (unsigned long)((float)(comp_value2-comp_value1) / v_res_factor[j][0]);

          for(p=1 ; p<5 ; p++) 
                  v_adc_org[j][p] = v_adc_org[j][p]-gap;
  }
}
 
void Over_Weight(void)
{
  char key_set = 0;
  char key_push = 0;
  unsigned int bound_count = 0;  
  
  Clear_Screen();  
  mprintf(10, 2,"  OVER WEIGHT    ");  
  mprintf(10, 3,"  MAX : 65,000 KG");
  
  v_over_weight = eeprom_2byte_read(V_OVER_WEIGHT);
  
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
      else if(Key.PressFlg[12]) { Key.PressFlg[12] = 0; key_set = 0; v_over_weight = 0; }
      else if(Key.PressFlg[13]) 
      { 
        Key.PressFlg[13] = 0; 
        eeprom_2byte_write(V_OVER_WEIGHT, v_over_weight); 
        Clear_Screen();
        HAL_Delay(100);
        break; 
      }
           
      if( (!v_over_weight) && key_push ) {v_over_weight += key_set; key_push = 0;}
      else if(key_push == 1)
      {
          v_over_weight *= 10;
          v_over_weight += key_set;
          key_push = 0;
      }           
      key_set = 0;                     
      
      if(v_over_weight>65000)v_over_weight=0;
      
      if(bound_count <= 800)
      {
        mprintf(10, 5," WEIGHT : %d", v_over_weight);        
        bound_count++;
      }
      else if(bound_count <= 1200)
      {
        mprintf(10, 5," WEIGHT :           ");
        bound_count++;
      }
      else bound_count = 0;
  }  
}

void LoadCell_Cheak(void)
{
  unsigned char i=0, pass_start=0;
  
RETRY:
  
  if(FunData.Mode == 4)
  {
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
  }
}