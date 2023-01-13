#include "main.h"

 extern struct FUNCTION FunData;
 extern struct KEY Key;

 extern unsigned char            ShortBeep;
 
void KEYPAD_Scan(void)
{    
    if(KEY_IN1_READ | KEY_IN2_READ | KEY_IN3_READ | KEY_IN4_READ | KEY_IN5_READ)
    {
        KEY1_RESET;
        KEY2_SET;
        KEY3_SET;

        if(!KEY_IN1_READ)
        {       
          while(!KEY_IN1_READ)     
          Key.PressFlg[10] = 1;
          if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[10] = 0;
                
        if(!KEY_IN2_READ)
        {
            while(!KEY_IN2_READ);
            Key.PressFlg[1] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[1] = 0;
        
        if(!KEY_IN3_READ)
        {
            while(!KEY_IN3_READ);
            Key.PressFlg[4] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[4] = 0;
        
        if(!KEY_IN4_READ)
        {
            while(!KEY_IN4_READ);
            Key.PressFlg[7] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[7] = 0;
        
        if(!KEY_IN5_READ)
        {
            while(!KEY_IN5_READ);
            Key.PressFlg[12] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[12] = 0;
        
        KEY1_SET;
        KEY2_RESET;
        KEY3_SET;

        if(!KEY_IN1_READ)
        {
            while(!KEY_IN1_READ);
            Key.PressFlg[11] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[11] = 0;
                
        if(!KEY_IN2_READ)
        {
            while(!KEY_IN2_READ);
            Key.PressFlg[2] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[2] = 0;
        
        if(!KEY_IN3_READ)
        {
            while(!KEY_IN3_READ);
            Key.PressFlg[5] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[5] = 0;
        
        if(!KEY_IN4_READ)
        {
            while(!KEY_IN4_READ);
            Key.PressFlg[8] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[8] = 0;
        
        if(!KEY_IN5_READ)
        {
            while(!KEY_IN5_READ);
            Key.PressFlg[0] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[0] = 0;
        
        KEY1_SET;
        KEY2_SET;
        KEY3_RESET;
                
        if(!KEY_IN2_READ)
        {
            while(!KEY_IN2_READ);
            Key.PressFlg[3] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[3] = 0;
        
        if(!KEY_IN3_READ)
        {
            while(!KEY_IN3_READ);
            Key.PressFlg[6] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[6] = 0;
        
        if(!KEY_IN4_READ)
        {
            while(!KEY_IN4_READ);
            Key.PressFlg[9] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[9] = 0;
        
        if(!KEY_IN5_READ)
        {
            while(!KEY_IN5_READ);
            Key.PressFlg[13] = 1;
            if(FunData.KeyBeep == 1)ShortBeep = 1; 
        }
        else Key.PressFlg[13] = 0;
    }         
} 

void Key_Clear(void)
{
  char i;
  
  for(i=0; i<14; i++)
  {
    Key.PressFlg[i]=0;
  }
}

void Setting_Mode(void)
{
    char Set_Step = 1;
    char Secret = 0;
    unsigned char exit_flag = 0;    

    Clear_Screen();
    
    while(!exit_flag)       //Menu select
    { 
       KEYPAD_Scan();
       
        if(Key.PressFlg[1])  
        {
          Key.PressFlg[1] = 0;
          if(!Secret)Secret = 1;
          else if(Secret==2)
          {
            Secret = 0;
            cal_mode();
          }
        }
        
        else if(Key.PressFlg[4])  
        {
          Key.PressFlg[4] = 0;
          if(Secret==1)Secret = 2;
        }
        
        else if(Key.PressFlg[7])  
        {
          Key.PressFlg[7] = 0;
          if(Secret==2)
          {
            Secret = 0;
            Secret_function();
          }
        }
        
        else if(Key.PressFlg[2]) 
        {
             Key.PressFlg[2] = 0;
             Set_Step--;
             if(Set_Step < 1) Set_Step = 5; 
        }
          
        else if (Key.PressFlg[8])
        {
            Key.PressFlg[8] = 0;
            Set_Step++;
            if(Set_Step > 5) Set_Step = 1;
        }
       
        else if (Key.PressFlg[13])
        {
            Key.PressFlg[13] = 0;
            switch(Set_Step)
            {
                case 1 :      
                    function();
                    Clear_Screen();
                    break;
                case 2 :
                    netid_mode();
                    Clear_Screen();
                    break;
                case 3 :
                    rtc_set();
                    Clear_Screen();
                    break;
                case 4 :
                    rtc_set();
                    Clear_Screen();
                    break;                    
                case 5 :
                    Clear_Screen();
                    return;
            }
        }
              
        else if (Key.PressFlg[12])
        {
            Key.PressFlg[12] = 0;
            Clear_Screen();
            return;
        }

        Print_Str6x8(Set_Step, 1, 1, " FUNCTION             ");
        Print_Str6x8(Set_Step, 1, 2, " PAIRING              ");
        Print_Str6x8(Set_Step, 1, 3, " DATE & TIME          ");
        Print_Str6x8(Set_Step, 1, 4, " TEST                 ");
        Print_Str6x8(Set_Step, 1, 5, " EXIT                 ");
        PrintArrow(68,7,0);
        PrintArrow(80,7,1);
        PrintArrow(94,7,2);
        Print_Str6x8(0xFF,110,7,"1/1");
    }
}

void MainKey(void)
{
  if(Key.PressFlg[0])      { Key.PressFlg[0]=0; }
  else if(Key.PressFlg[1]) { Key.PressFlg[1]=0; }
  else if(Key.PressFlg[2]) { Key.PressFlg[2]=0; heading_edit();}  
  else if(Key.PressFlg[3]) { Key.PressFlg[3]=0; Memory_Input(0);}
  else if(Key.PressFlg[4]) { Key.PressFlg[4]=0; Memory_Input(1);}
  else if(Key.PressFlg[5]) { Key.PressFlg[5]=0; }
  else if(Key.PressFlg[6]) { Key.PressFlg[6]=0; loadcell_test();}
  else if(Key.PressFlg[7]) { Key.PressFlg[7]=0; }
  else if(Key.PressFlg[8]) { Key.PressFlg[8]=0; Over_Weight();}
  else if(Key.PressFlg[9]) { Key.PressFlg[9]=0; Setting_Mode();}
  else if(Key.PressFlg[10]) { Key.PressFlg[10]=0; WeightModeSendToPad();}
  else if(Key.PressFlg[11]) { Key.PressFlg[11]=0; }
  else if(Key.PressFlg[12]) { Key.PressFlg[12]=0; }
  else if(Key.PressFlg[13]) { Key.PressFlg[13]=0; }  
}