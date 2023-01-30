#include "main.h"

extern unsigned char    scan_year,       
                         scan_month,
                         scan_date,
                         s_buffer[81],
                         v_car_id[12],
                         v_item[10],
                         scan_step;
                         
extern unsigned int     scan_byte,
                         v_byte_addr;      

extern struct           FUNCTION FunData;
extern struct           KEY Key;

extern unsigned short   scan_no;

extern RTC_DateTypeDef sDate;

void scan_mode(void)
{
  unsigned char scan_temp, j, chi;
  unsigned int  i;
	
  static unsigned char	UsbSave, DualSave;

  mprintf(1, 1,"      1 - 5 SCAN DATA ");
	
  while(1)
  {   
      KEYPAD_Scan();

      if(Key.PressFlg[1])											
      { 
          Key.PressFlg[1] = 0;
          TimeRead();
          HAL_Delay(700);
          clock_disp();
          scan_year=sDate.Year;
          scan_month=sDate.Month;
          scan_date=sDate.Date;		
          mprintf(1, 5,"      1 - PRINT ");
              
          while(1)
          {                                                                                                                   
            if(Key.PressFlg[10] || Key.PressFlg[12] || Key.PressFlg[0])
            {
              if(Key.PressFlg[10]) DualSave = 1;						
              else if(Key.PressFlg[0]) {UsbSave = 1; DualSave = 0;}		
              else {UsbSave = 0; DualSave = 0;}					
              Key_Clear(); 							
                    
              for(i=0; i<2500; i++)
              {
                 scan_byte = 81*i+5002; scan_temp = eeprom_1byte_read(scan_byte);
                 if( (scan_temp==255)|| (v_byte_addr==(scan_byte-2)) )
                 {
                    mprintf(1, 7,"          END   "); 
                    HAL_Delay(600); 
                    return; 
                  }      
                  if(scan_temp==scan_date)
                  {
                      scan_byte = 81*i+5001; scan_temp = eeprom_1byte_read(scan_byte);
                      if(scan_temp==scan_month)
                      {
                          scan_byte = 81*i+5000; scan_temp = eeprom_1byte_read(scan_byte);
                          if(scan_temp==scan_year)
                          {
                              for(j=0; j<81; j++)
                              {
                                scan_byte = j+81*i+5000;
                                s_buffer[j] = eeprom_1byte_read(scan_byte);
                              }                            
                              StepCheck();
                              //if(DualSave) {ScanUsbOut(); ScanPrintOut();}		
                              //else if(UsbSave) ScanUsbOut();				
                              //else ScanPrintOut();					
                              //scan_send();  
                          }
                      }
                  } 
               } 
                    
                mprintf(1, 7,"          END   "); 
                HAL_Delay(600); 
                return;
            }
            
            else if(Key.PressFlg[13]) 
            { 
               Key.PressFlg[13] = 0;
               return;
            }                
          }	
      }
      
      else if(Key.PressFlg[2])									
      {
          Key.PressFlg[2] = 0;
          //scan_no = number_sno();
          mprintf(1, 5,"      2 - PRINT ");
              
          while(1)
          {                     
              if(Key.PressFlg[10] || Key.PressFlg[12] || Key.PressFlg[0])
              {
                  if(Key.PressFlg[10]) DualSave = 1;						
                  else if(Key.PressFlg[0]) {UsbSave = 1; DualSave = 0;}		
                  else {UsbSave = 0; DualSave = 0;}					
                  Key_Clear(); 					

                  for(i=0; i<2500; i++)
                  {
                     scan_byte = 81*i+5006; scan_temp = eeprom_1byte_read(scan_byte);
                     if( (scan_temp==255)|| (v_byte_addr==(scan_byte-6)) )
                     {
                        mprintf(1, 7,"          END   "); 
                        HAL_Delay(600); 
                        return; 
                      }      
                      if(scan_temp==scan_no%100)
                      {
                          scan_byte = 81*i+5005; scan_temp = eeprom_1byte_read(scan_byte);
                          if(scan_temp==scan_no/100)
                          {                        
                              for(j=0; j<81; j++)
                              {
                                scan_byte = j+81*i+5005;
                                s_buffer[j] = eeprom_1byte_read(scan_byte);
                              }
                              
                              StepCheck();
                              //if(DualSave) {ScanUsbOut(); ScanPrintOut();}	
                              //else if(UsbSave) ScanUsbOut();			
                              //else ScanPrintOut();				
                              //scan_send();  
                           }
                       }
                    } 
                                               
                 mprintf(1, 7,"          END   "); 
                 HAL_Delay(600); 
                 return;
              }
              
              else if(Key.PressFlg[13]) 
              { 
                 Key.PressFlg[13] = 0;
                 return;
              }        
          }
      }
      
        else if(Key.PressFlg[3])									
        { 
            Key.PressFlg[3] = 0;
            Memory_Input(0);
            mprintf(1, 5,"      3 - PRINT ");
                
            while(1)
            {   
                if(Key.PressFlg[10] || Key.PressFlg[12] || Key.PressFlg[0])
                {
                    if(Key.PressFlg[10]) DualSave = 1;						
                    else if(Key.PressFlg[0]) {UsbSave = 1; DualSave = 0;}		
                    else {UsbSave = 0; DualSave = 0;}					
                    Key_Clear();
                        
                    for(i=0; i<2500; i++)
                    {
                        scan_byte = 81*i+16; // 10 check
                        scan_temp = eeprom_1byte_read(scan_byte);
                        
                        if( ( (scan_temp==255)||(v_byte_addr==(scan_byte-16))) ) 		
                        { 
                            mprintf(1, 7,"          END   "); 
                            HAL_Delay(600); 
                            return; 
                        }

                        if(scan_temp==v_car_id[9]) 
                        {
                            scan_byte = 81*i+15; // 9 check
                            scan_temp = eeprom_1byte_read(scan_byte);
                                
                            if(scan_temp==v_car_id[8]) 
                            {
                                scan_byte = 81*i+14; // 8 check
                                scan_temp = eeprom_1byte_read(scan_byte);
                                    
                                if(scan_temp==v_car_id[7]) 
                                {
                                    scan_byte = 81*i+13; // 7 check
                                    scan_temp = eeprom_1byte_read(scan_byte);

                                    if(scan_temp==v_car_id[6]) 
                                    {
                                        scan_byte = 81*i+12; // 6 check
                                        scan_temp = eeprom_1byte_read(scan_byte);

                                        if(scan_temp==v_car_id[5]) 
                                        {
                                            scan_byte = 81*i+11; // 5 check
                                            scan_temp = eeprom_1byte_read(scan_byte);

                                            if(scan_temp==v_car_id[4]) 
                                            {
                                                scan_byte = 81*i+10; // 4 check
                                                scan_temp = eeprom_1byte_read(scan_byte);

                                                if(scan_temp==v_car_id[3]) 
                                                {
                                                    scan_byte = 81*i+9; // 3 check
                                                    scan_temp = eeprom_1byte_read(scan_byte);

                                                    if(scan_temp==v_car_id[2]) 
                                                    {
                                                        scan_byte = 81*i+8; // 2 check
                                                        scan_temp = eeprom_1byte_read(scan_byte);

                                                        if(scan_temp==v_car_id[1]) 
                                                        {
                                                            scan_byte = 81*i+7; // 1 check
                                                            scan_temp = eeprom_1byte_read(scan_byte);

                                                            if(scan_temp==v_car_id[0]) 
                                                            {
                                                                for(chi=0; chi<81; chi++)
                                                                {
                                                                        scan_byte = chi + 81*i;
                                                                        s_buffer[chi] = eeprom_1byte_read(scan_byte);
                                                                }
                                                                    
                                                                //StepCheck();
                                                                //if(DualSave) {ScanUsbOut(); ScanPrintOut();}
                                                                //else if(UsbSave) ScanUsbOut();
                                                                //else ScanPrintOut();
                                                                //scan_send();
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                            
                    }
                    
                    for(i=0; i<10; i++) v_car_id[i]=0xFF;     
                    mprintf(1, 7,"          END   "); 
                    HAL_Delay(600); 
                    return; 
                } 
                else if(Key.PressFlg[13]) 
                { 
                   Key.PressFlg[13] = 0;
                   return;
                }                              
                    
            }
        }
        else if(Key.PressFlg[4])										
        { 
           Key.PressFlg[4] = 0;
           Memory_Input(1);
           mprintf(1, 5,"      4 - PRINT ");
                
            while(1)
            {                                    
                if(Key.PressFlg[10] || Key.PressFlg[12] || Key.PressFlg[0])
                {
                    if(Key.PressFlg[10]) DualSave = 1;						
                    else if(Key.PressFlg[0]) {UsbSave = 1; DualSave = 0;}		
                    else {UsbSave = 0; DualSave = 0;}					
                    Key_Clear();					

                    for(i=0; i<2500; i++)
                    {
                        scan_byte = 81*i+26; // 20 check
                        scan_temp = eeprom_1byte_read(scan_byte);

                        if( ( (scan_temp==255)||(v_byte_addr==(scan_byte-26))) ) 		
                        { 
                            mprintf(1, 7,"          END   "); 
                            HAL_Delay(600); 
                            return; 
                        }

                        if(scan_temp==v_item[9])
                        {
                            scan_byte = 81*i+25; // 9 check
                            scan_temp = eeprom_1byte_read(scan_byte);

                            if(scan_temp==v_item[8]) 
                            {
                                scan_byte = 81*i+24; // 8 check
                                scan_temp = eeprom_1byte_read(scan_byte);

                                if(scan_temp==v_item[7]) 
                                {
                                    scan_byte = 81*i+23; // 7 check
                                    scan_temp = eeprom_1byte_read(scan_byte);

                                    if(scan_temp==v_item[6]) 
                                    {
                                        scan_byte = 81*i+22; // 6 check
                                        scan_temp = eeprom_1byte_read(scan_byte);

                                        if(scan_temp==v_item[5]) 
                                        {
                                            scan_byte = 81*i+21; // 5 check
                                            scan_temp = eeprom_1byte_read(scan_byte);

                                            if(scan_temp==v_item[4]) 
                                            {
                                                scan_byte = 81*i+20; // 4 check
                                                scan_temp = eeprom_1byte_read(scan_byte);

                                                if(scan_temp==v_item[3]) 
                                                {
                                                    scan_byte = 81*i+19; // 3 check
                                                    scan_temp = eeprom_1byte_read(scan_byte);

                                                    if(scan_temp==v_item[2]) 
                                                    {
                                                        scan_byte = 81*i+18; // 2 check
                                                        scan_temp = eeprom_1byte_read(scan_byte);

                                                        if(scan_temp==v_item[1]) 
                                                        {
                                                            scan_byte = 81*i+17; // 1 check
                                                            scan_temp = eeprom_1byte_read(scan_byte);

                                                            if(scan_temp==v_item[0]) 
                                                            {
                                                                for(chi=0; chi<81; chi++)
                                                                {
                                                                    scan_byte = chi + 81*i;
                                                                    s_buffer[chi] = eeprom_1byte_read(scan_byte);
                                                                }

                                                                //StepCheck();
                                                                //if(DualSave) {ScanUsbOut(); ScanPrintOut();}
                                                                //else if(UsbSave) ScanUsbOut();
                                                                //else ScanPrintOut();        
                                                                //scan_send();
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }                           
                    }
                        
                    for(i=0; i<10; i++) v_item[i]=0xFF;
                    mprintf(1, 7,"          END   "); 
                    HAL_Delay(600); 
                    return;
                } 
                else if(Key.PressFlg[13]) 
                { 
                   Key.PressFlg[13] = 0;
                   return;
                }                                   
            }
        }
        else if(Key.PressFlg[5])										
        {
            Key.PressFlg[5] = 0;
            mprintf(1, 5,"      5 - PRINT ");

            while(1)
            {   
                if(Key.PressFlg[10] || Key.PressFlg[12] || Key.PressFlg[0])
                {
                    if(Key.PressFlg[10]) DualSave = 1;						
                    else if(Key.PressFlg[0]) {UsbSave = 1; DualSave = 0;}		
                    else {UsbSave = 0; DualSave = 0;}					
                    Key_Clear();								//key¡Æ¨£ Reset
                        
                    for(i=0; i<2500; i++)
                    {                           
                        scan_byte = 81*i;                        
                        scan_temp = eeprom_1byte_read(scan_byte);

                        if( (scan_temp==255)||( (scan_temp==255)||(v_byte_addr==scan_byte)) ) 	
                        {
                            mprintf(1, 7,"          END   "); 
                            HAL_Delay(600); 
                            return; 
                        }

                        for(chi=0; chi<81; chi++)
                        {
                            scan_byte = chi + 81*i;
                            s_buffer[chi] = eeprom_1byte_read(scan_byte);
                        }
                        
                        //StepCheck();
                        //if(DualSave) {ScanUsbOut(); ScanPrintOut();}
                        //else if(UsbSave) ScanUsbOut();
                        //else ScanPrintOut();	
                        //scan_send();  
                    }                        
                    mprintf(1, 7,"          END   "); 
                    HAL_Delay(600); 
                    return; 
                } 
                else if(Key.PressFlg[13]) 
                { 
                   Key.PressFlg[13] = 0;
                   return;
                }                           
            }
        }                            
    } 
}

void StepCheck(void)
{
         if((s_buffer[31]==0xFF)&&(s_buffer[32]==0xFF)) 	scan_step=1;
    else if((s_buffer[35]==0xFF)&&(s_buffer[36]==0xFF)) 	scan_step=2;
    else if((s_buffer[39]==0xFF)&&(s_buffer[40]==0xFF)) 	scan_step=3;
    else if((s_buffer[43]==0xFF)&&(s_buffer[44]==0xFF)) 	scan_step=4;
    else if((s_buffer[47]==0xFF)&&(s_buffer[48]==0xFF)) 	scan_step=5;
    else if((s_buffer[51]==0xFF)&&(s_buffer[52]==0xFF)) 	scan_step=6;
    else if((s_buffer[55]==0xFF)&&(s_buffer[56]==0xFF)) 	scan_step=7;
    else if((s_buffer[59]==0xFF)&&(s_buffer[60]==0xFF)) 	scan_step=8;
    else if((s_buffer[63]==0xFF)&&(s_buffer[64]==0xFF)) 	scan_step=9;
    else if((s_buffer[67]==0xFF)&&(s_buffer[68]==0xFF)) 	scan_step=10;
    else if((s_buffer[71]==0xFF)&&(s_buffer[72]==0xFF)) 	scan_step=11;
    else                                                	scan_step=12;
}