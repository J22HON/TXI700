#include "main.h"

extern struct KEY Key;

extern unsigned char     v_ad_flag;

void loadcell_test(void)
{
  unsigned long temp_long=1;
  unsigned char cal_no;
 
  Clear_Screen();
  
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
      
      Clear_Screen();
      HAL_Delay(500);
      break;
    }      
    
    mprintf(1, 3, " Pad_Number  :  %d" ,temp_long );
  } 
                                  
  while(1)
  {
    KEYPAD_Scan();
    
    do
    { 
        temp_long = read_filtered_adc(cal_no);
    } 
    while(v_ad_flag==0);

    if(( temp_long < 10) || (temp_long > 2000000l))	// LOAD CELL ERROR 
    {
      mprintf(1, 3, " C H 0 2          ");
    } 
    else mprintf(1, 3, "     AD : %d",temp_long/5);

    if(Key.PressFlg[13])
    {
       Key.PressFlg[13] = 0;
       Clear_Screen();
       break;         
    }
  }
}