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

