#include "main.h"

extern struct FUNCTION FunData;

extern unsigned char         v_ad_flag;

extern unsigned int          v_speed;   

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
    
    for(ad=0; ad<FunData.Pad_Sel; ad++)
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

