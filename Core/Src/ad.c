#include "main.h"
#include "stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

 extern struct FUNCTION FunData;
 extern struct AD_DATA AdData;
 extern struct CAL Calibration;

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
                                   v_minimum_division[2];                                                                                                 
  
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
                                   v_e_resolution[2];
                                            
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

void normal_mode(void)
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
/*
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
    }*/
          
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