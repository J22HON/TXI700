#include "main.h"

extern struct KEY Key;
extern struct FUNCTION FunData;

 unsigned char   eeprom_add[2],
                 eeprom_data1[16],   
                 eeprom_data2[16],
                 eeprom_data3[16];  

 unsigned long   total_page,
                 start_page,
                 end_page,
                 end_add;

 unsigned long   g_delay_cnt;

 extern unsigned char             v_ad_flag,
                                   v_multi_cal,
                                   v_minimum_division[2],
                                   p_head[120];  

 extern unsigned long             v_maximum_capacity[2],
                                   v_e_resolution[2],
                                   v_adc_org[2][5],
                                   v_zero[2],
                                   imsi_value,
                                   v_temp_long;
 
 extern unsigned short            v_gravity_factor1,
                                   v_gravity_factor2; 

 extern float                     v_res_factor[2][5];
 
void eeprom_4byte_write(unsigned long addr, unsigned long data)
{
    unsigned long temp;
    
    for(temp=0; temp<4; ++temp)
    {    
        eeprom_data2[temp] = 0;
    }
    temp=data;
    eeprom_data2[3]=temp/0x1000000;
    eeprom_data2[2]=(temp%0x1000000)/0x10000;
    eeprom_data2[1]=(temp%0x10000)/0x100;
    eeprom_data2[0]=temp%0x100;
    
    sw_i2c_addr_write(1, addr, eeprom_data2, 4);
    HAL_Delay(10);
}

unsigned long eeprom_4byte_read(unsigned long addr)
{
    unsigned long temp;
    for(temp=0; temp<4;++temp)
    {
        eeprom_data1[temp] = 0;
    }
    sw_i2c_addr_read(1, addr, eeprom_data1, 4);
    
    temp=0;
    temp=eeprom_data1[3]*0x1000000;
    temp=temp+(eeprom_data1[2]*0x10000);
    temp=temp+(eeprom_data1[1]*0x100);
    temp=temp+eeprom_data1[0];
    return temp;
}

void eeprom_2byte_write(unsigned long addr, unsigned int data)
{
    unsigned int temp;
    
    for(temp=0; temp<2;++temp)
    {
        eeprom_data2[temp] = 0;
    }
    temp=data;
    eeprom_data2[1]=(temp%0x10000)/0x100;
    eeprom_data2[0]=temp%0x100;
    
    sw_i2c_addr_write(1, addr, eeprom_data2, 2);
    HAL_Delay(10);
}

unsigned int eeprom_2byte_read(unsigned long addr)
{
    unsigned int temp;
    
    for(temp=0; temp<2;++temp)
    {
        eeprom_data1[temp] = 0;
    }
    sw_i2c_addr_read(1, addr, eeprom_data1, 2);
    
    temp=0;
    temp=eeprom_data1[1]*0x100;
    temp=temp+eeprom_data1[0];
    return temp;
}

void eeprom_1byte_write(unsigned long addr, unsigned char data1)
{
    unsigned char temp;
    
    temp=data1;
    eeprom_data2[0]=temp;
    
    sw_i2c_addr_write(1, addr, eeprom_data2, 1);
    HAL_Delay(10);
}

unsigned char eeprom_1byte_read(unsigned long addr)
{
    unsigned char temp;
    eeprom_data1[0]=0;
    sw_i2c_addr_read(1, addr, eeprom_data1, 1);
    
    temp=0;
    temp=eeprom_data1[0];
    return temp;
}

void eeprom_float_write(unsigned long addr, float data00)
{  
    unsigned long temp;
    for(temp=0; temp<4;++temp)
    {
        eeprom_data2[temp] = 0;
    }
    temp=(unsigned long)(data00*100000000.0);
    eeprom_data2[3]=temp/0x1000000;
    eeprom_data2[2]=(temp%0x1000000)/0x10000;
    eeprom_data2[1]=(temp%0x10000)/0x100;
    eeprom_data2[0]=temp%0x100;
    
    sw_i2c_addr_write(1, addr, eeprom_data2, 4);
    HAL_Delay(10);
}

float eeprom_float_read(unsigned long addr)
{
    unsigned long temp;
    float temp1;
    for(temp=0; temp<4;++temp)
    {
        eeprom_data1[temp] = 0;
    }
    sw_i2c_addr_read(1, addr, eeprom_data1, 4);
    
    temp=0;
    temp=eeprom_data1[3]*0x1000000;
    temp=temp+(eeprom_data1[2]*0x10000);
    temp=temp+(eeprom_data1[1]*0x100);
    temp=temp+eeprom_data1[0];
    
    temp1=(float)(temp/100000000.0);
    return temp1;
}

unsigned long sw_i2c_read(unsigned char stop, unsigned char *buf, unsigned long maxlen)
{
    unsigned long i;
    
    i2c_start();
    
    i2c_send_byte(0xA1);
    
    if(!i2c_recv_ack())   // ID check
    {
        i2c_stop();
        return(RT_NO_ACK);
    }
    
    for(i=0;i<maxlen;++i)
    {
        buf[i] = i2c_recv_byte();
        if(i!=maxlen-1){i2c_send_ack();}
        else{i2c_send_no_ack();}
    }
    if(stop)
    {
        i2c_stop();      
    }
    return(RT_SUCCESS);
}

unsigned long sw_i2c_write(unsigned char stop, unsigned char *buf, unsigned long maxlen)
{
    unsigned long i;
    
    i2c_start();
    
    i2c_send_byte(0xA0);
    
    if(!i2c_recv_ack())   // ID check
    {
        i2c_stop();
        return(RT_NO_ACK);
    }
    
    for(i=0;i<maxlen;++i)
    {
        i2c_send_byte(buf[i]);
        if(!i2c_recv_ack())
        {
            i2c_stop();
            return( RT_NO_ACK2);
        }
    }
    if(stop)
    {
        i2c_stop();      
    }
    return(RT_SUCCESS);
}

unsigned long sw_i2c_addr_write(unsigned char stop, unsigned long addr, unsigned char *buf, unsigned long maxlen)
{
    unsigned long i;
    unsigned char add[3];
    
    
    //GPIO_ResetBits(GPIOB, GPIO_Pin_5);  // eeprom WP low(allow)
    
    //i=addr%0x10000;  //address 2byte
    //add[1]=i/0x100;  //address 2byte
    //add[2]=i%0x100;  //address 2byte
    add[1]=addr;
    add[0]=addr/0x100;;  //address 1byte
    
    i2c_start();
    
    i2c_send_byte(0xA0 | (add[0]<<1 & 0x0E));
    
    if(!i2c_recv_ack())   // ID check
    {
        i2c_stop();
        return(RT_NO_ACK);
    }
    //for(i=0;i<2;++i)    //address 2byte
    for(i=0;i<1;++i)    //address 1byte
    {
        i2c_send_byte(add[i+1]);
        if(!i2c_recv_ack())
        {
            i2c_stop();
            return( RT_NO_ACK2);
        }
    }
    for(i=0;i<maxlen;++i)
    {
        i2c_send_byte(buf[i]);
        if(!i2c_recv_ack())
        {
            i2c_stop();
            return(RT_NO_ACK2);
        }
    }
    if(stop)
    {
        i2c_stop();      
    }
    HAL_Delay(5);  
    //GPIO_SetBits(GPIOB, GPIO_Pin_5);  // eeprom WP high(protect)
    return(RT_SUCCESS);
}

unsigned long sw_i2c_addr_read(unsigned long stop, unsigned long addr, unsigned char *buf, unsigned long maxlen1)
{
    unsigned long i;
    unsigned char add[3];
    
    //GPIO_ResetBits(GPIOB, GPIO_Pin_5);  // eeprom WP low(allow)
    
    //i=addr%0x10000;   //address 2byte
    //add[1]=i/0x100;   //address 2byte
    //add[2]=i%0x100;   //address 2byte
    add[1]=addr;
    add[0]=addr/0x100;  //address 1byte
    
    i2c_start();
    
    i2c_send_byte(0xA0 | (add[0]<<1 & 0x0E));
    
    if(!i2c_recv_ack())   // ID check
    {
        i2c_stop();
        return(RT_NO_ACK);
    }
    
    //for(i=0;i<2;++i)  //address 2byte
    for(i=0;i<1;++i)  //address 1byte
    {
        i2c_send_byte(add[i+1]);
        if(!i2c_recv_ack())
        {
            i2c_stop();
            return( RT_NO_ACK2);
        }
    }
    
    i2c_start();
    
    i2c_send_byte(0xA0 | (add[0]<<1 & 0x0E) | 0x01);
    
    if(!i2c_recv_ack())   // ID check
    {
        i2c_stop();
        return(RT_NO_ACK);
    }
    
    for(i=0;i<maxlen1;++i)
    {
        buf[i] = i2c_recv_byte();
        if(i!=maxlen1-1){i2c_send_ack();}
        else{i2c_send_no_ack();}
    }
    if(stop)
    {
        i2c_stop();      
    }
    //GPIO_SetBits(GPIOB, GPIO_Pin_5);  // eeprom WP high(protect)
    return(RT_SUCCESS);
}

void i2c_start(void)
{
    SDA_HI;  interval();
    SCL_HI;  interval();
    SDA_LO;  interval();
}

void i2c_stop(void)
{
    SCL_LO;  interval();
    SDA_LO;  interval();
    SCL_HI;  interval();
    SDA_HI;  interval();
}

void interval(void)
{
    int k;
    g_delay_cnt = 50;
    for(k=0;k<g_delay_cnt;k++)
    {
        ;
    }
}

void i2c_send_no_ack(void)
{
    SCL_LO;    interval();
    SDA_HI;    interval();
    SCL_HI;    interval();
    SCL_LO;    interval();
    return;
}

void i2c_send_ack(void)
{
    SCL_LO;  interval();
    SDA_LO;  interval();
    SCL_HI;  interval();
    SCL_LO;  interval();
    return;
}

unsigned char i2c_recv_ack(void)
{
    unsigned char ack;
    SCL_LO;  interval();
    SDA_HI; interval();
    SCL_HI; interval();
    
    ack = !sda_value();
    
    SCL_LO;
    
    return(ack);
}

unsigned char sda_value(void)
{
    return(HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin));
}

void i2c_send_byte(unsigned char data)
{
    unsigned long bit;
    int i;
    
    for (i = 0; i < 8; ++i)
    {
        //bit = (data & (0x80 >> i)) ? TRUE : FALSE;	/* MSB first */
        if(data & (0x80 >> i)) bit = 1;
        else bit = 0;
        SCL_LO; interval();
        bit ? SDA_HI : SDA_LO; interval();
        SCL_HI; interval(); 
    }
    return;
}

unsigned char i2c_recv_byte(void)
{
    unsigned char data;
    unsigned char bit;
    int i;
    
    SDA_HI;
    data = 0;
    for (i = 0; i < 8; ++i)
    {
        SCL_LO;	interval();
        SCL_HI;	interval();
        bit = sda_value();	interval();
        
        data |= ((bit ? 0x80 : 0x00) >> i);
    }
    return data;
}


void function(void)
{
    unsigned int bound_count = 0;
    unsigned int value = 1;
    char key_set = 0;
    char key_push = 0;
    
RESET :  
  
    Clear_Screen();
    Print_Str6x8(0xFF,10,1,"FuncTion Set");
    if(FunData.Mode==3) mprintf(1, 2,"  - Wireless -   ");
    else if(FunData.Mode==4) mprintf(1, 2,"  - Wired -   ");
  
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
        else if(Key.PressFlg[13]) 
        { 
          Key.PressFlg[13] = 0;
          if(!value)
          {
            function_read();
            function_range();
            Clear_Screen();
            break;
          }
          title_Function(value); 
          break;
        }
             
        if( (!value) && key_push ) {value += key_set; key_push = 0;}
        else if(key_push == 1)
        {
            value *= 10;
            value += key_set;
            key_push = 0;
        }
        if(value > 20) value = 0;
        key_set = 0;
                       
        if(bound_count <= 800)
        {
          mprintf(1, 4," FuncTion : %02d", value);
          bound_count++;
        }
        else if(bound_count <= 1200)
        {
          mprintf(1, 4," FuncTion :      ", value);
          bound_count++;
        }
        else bound_count = 0;
    }
    
    if(value)
    {
      number_long(value);    
      
      if(FunData.Mode == 3)
      {
        if(value<18) value++;
        else value = 0;
        goto RESET;
      }
      
      else if(FunData.Mode == 4)
      {
        if(value<18) value++;
        else value = 0;
        goto RESET;
      }
    }
}

void title_Function(unsigned char set_num)
{  
  Clear_Screen();
  
  if(FunData.Mode == 3)
  {
    switch(set_num)
    {
        case 1:
            mprintf(1, 2,"  Number of Scales to");
            mprintf(1, 3,"  be Linked Wireless ");
            break;
        
        case 2:
            mprintf(1, 2,"  Stable Condition  ");
            break;

        case 3:
            mprintf(1, 2,"  Weigh-In-Motion   ");
            break;
            
        case 4:
            mprintf(1, 2,"  Excess Enable     ");
            break;

        case 5:
            mprintf(1, 2,"  Print Format      ");
            break;

        case 6:
            mprintf(1, 2,"  Number of Copies  ");
            break;

        case 7:
            mprintf(1, 2,"  Line Feed         ");
            break;
            
        case 8:
            mprintf(1, 2,"  Enable Printer    ");
            break;          
            
        case 9:
            mprintf(1, 2,"  Data Format       ");  
            break;  
            
        case 10:
            mprintf(1, 2,"  External Wireless ");
            mprintf(1, 3,"  Output            ");
            break;                   
            
        case 11:
            mprintf(1, 2,"  Power Off Sync    ");
            break;    
            
        case 12:
            mprintf(1, 2,"  Automatic Measuring");
            mprintf(1, 3,"  (A-WIM)            ");       
            break;    
            
        case 13:
            mprintf(1, 2,"  Auto Weighing      ");
            mprintf(1, 3,"  Stable Time        ");
            break;          
            
        case 14:
            mprintf(1, 2,"  Auto Print Time    ");
            break;   
            
        case 15:
            mprintf(1, 2,"  Pad Type Used      ");
            break;  
            
        case 16:
            mprintf(1, 2,"  Power Saving Mode  ");
            mprintf(1, 3,"  Display Used       ");
            break;  
            
        case 17:
            mprintf(1, 2,"  Print Density      ");
            break;
            
        case 18:
            mprintf(1, 2,"  Function Key Set   ");
            break;

        default:
            break;          
    }  
  }
  
  else if(FunData.Mode == 4)
  {
    switch(set_num)
    {
        case 1:
            mprintf(1, 2,"  Number of Scales to");
            mprintf(1, 3,"  be Linked Wireless ");
            break;
        
        case 2:
            mprintf(1, 2,"  Stable Condition  ");
            break;

        case 3:
            mprintf(1, 2,"  Weigh-In-Motion   ");
            break;
            
        case 4:
            mprintf(1, 2,"  Excess Enable     ");
            break;

        case 5:
            mprintf(1, 2,"  Print Format      ");
            break;

        case 6:
            mprintf(1, 2,"  Number of Copies  ");
            break;

        case 7:
            mprintf(1, 2,"  Line Feed         ");
            break;
            
        case 8:
            mprintf(1, 2,"  Enable Printer    ");
            break;          
            
        case 9:
            mprintf(1, 2,"  Data Format       ");  
            break;  
            
        case 10:
            mprintf(1, 2,"  External Wireless ");
            mprintf(1, 3,"  Output            ");
            break;         
        
        case 11:
            mprintf(1, 2,"  Auto Zero          ");
            mprintf(1, 3,"  Condition Set      ");
            break;
            
        case 12:
            mprintf(1, 2,"  Automatic Measuring");
            mprintf(1, 3,"  (A-WIM)            ");       
            break;    
            
        case 13:
            mprintf(1, 2,"  Auto Weighing      ");
            mprintf(1, 3,"  Stable Time        ");
            break;          
            
        case 14:
            mprintf(1, 2,"  Auto Print Time    ");
            break;       
            
        case 15:
            mprintf(1, 2,"  Adjustment        ");
            mprintf(1, 3,"  The Hold Speed    ");
            break;   
            
        case 16:
            mprintf(1, 2,"  Initialization    ");
            mprintf(1, 3,"  Hold Weight       ");
            break;  
            
        case 17:
            mprintf(1, 2,"  Print density      ");
            break;  
        
        case 18:
            mprintf(1, 2,"  Adjustment          ");
            mprintf(1, 2,"  The Speed of        ");
            mprintf(1, 4,"  Weight Change       ");
            break;            
            
        default:
            break;          
    }
  }
}

void number_long(unsigned char chat1)
{
  unsigned int value=0;
  char key_set = 0;
  char key_push = 0;
  unsigned int bound_count = 0;
  
  if(FunData.Mode==3)
  {
    if(chat1==1) value = eeprom_1byte_read(PAD_SEL); 
    else if(chat1==2) value = eeprom_1byte_read(STABLE); 
    else if(chat1==3) value = eeprom_1byte_read(WEIGH_IN_MOTION); 
    else if(chat1==4) value = eeprom_1byte_read(OVER_ENABLE);
    else if(chat1==5) value = eeprom_1byte_read(PRINT_FORM);
    else if(chat1==6) value = eeprom_1byte_read(PRINT_COPIES);
    else if(chat1==7) value = eeprom_1byte_read(LINEFEED);
    else if(chat1==8) value = eeprom_1byte_read(PRINT_ENABLE);
    else if(chat1==9) value = eeprom_1byte_read(DATA_FORMAT);
    else if(chat1==10) value = eeprom_1byte_read(WIRELESS_OUTPUT);
    else if(chat1==11) value = eeprom_1byte_read(OFF_SYNS);
    else if(chat1==12) value = eeprom_1byte_read(AUTO_MEASURING);
    else if(chat1==13) value = eeprom_1byte_read(AUTO_STABLE);
    else if(chat1==14) value = eeprom_1byte_read(AUTO_PRINT);
    else if(chat1==15) value = eeprom_1byte_read(PAD_TYPE);
    else if(chat1==16) value = eeprom_1byte_read(DISPLAY_USED);
    else if(chat1==17) value = eeprom_1byte_read(PRINT_DENSITY);
    else if(chat1==18) value = eeprom_1byte_read(FUNC_KEY);
  }
  
  else if(FunData.Mode==4)
  {
    if(chat1==1) value = eeprom_1byte_read(PAD_SEL);    
    else if(chat1==2) value = eeprom_1byte_read(STABLE); 
    else if(chat1==3) value = eeprom_1byte_read(WEIGH_IN_MOTION); 
    else if(chat1==4) value = eeprom_1byte_read(OVER_ENABLE);
    else if(chat1==5) value = eeprom_1byte_read(PRINT_FORM);
    else if(chat1==6) value = eeprom_1byte_read(PRINT_COPIES);
    else if(chat1==7) value = eeprom_1byte_read(LINEFEED);
    else if(chat1==8) value = eeprom_1byte_read(PRINT_ENABLE);
    else if(chat1==9) value = eeprom_1byte_read(DATA_FORMAT);
    else if(chat1==10) value = eeprom_1byte_read(WIRELESS_OUTPUT);
    else if(chat1==11) value = eeprom_1byte_read(AUTO_ZERO); 
    else if(chat1==12) value = eeprom_1byte_read(AUTO_MEASURING);
    else if(chat1==13) value = eeprom_1byte_read(AUTO_STABLE);
    else if(chat1==14) value = eeprom_1byte_read(AUTO_PRINT);
    else if(chat1==15) value = eeprom_1byte_read(HOLD_SPEED);
    else if(chat1==16) value = eeprom_1byte_read(HOLD_ZERO);
    else if(chat1==17) value = eeprom_1byte_read(PRINT_DENSITY);
    else if(chat1==18) value = eeprom_1byte_read(FILTER_DEGREE); 
  }
  
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
      else if(Key.PressFlg[13]) 
      { 
        Key.PressFlg[13] = 0;  
        
        if(FunData.Mode==3)
        {
          if(chat1==1) eeprom_1byte_write(PAD_SEL ,value); 
          else if(chat1==2) eeprom_1byte_write(STABLE ,value); 
          else if(chat1==3) eeprom_1byte_write(WEIGH_IN_MOTION ,value); 
          else if(chat1==4) eeprom_1byte_write(OVER_ENABLE ,value);
          else if(chat1==5) eeprom_1byte_write(PRINT_FORM ,value);
          else if(chat1==6) eeprom_1byte_write(PRINT_COPIES ,value);
          else if(chat1==7) eeprom_1byte_write(LINEFEED ,value);
          else if(chat1==8) eeprom_1byte_write(PRINT_ENABLE ,value);
          else if(chat1==9) eeprom_1byte_write(DATA_FORMAT ,value);
          else if(chat1==10) eeprom_1byte_write(WIRELESS_OUTPUT ,value);
          else if(chat1==11) eeprom_1byte_write(OFF_SYNS ,value);
          else if(chat1==12) eeprom_1byte_write(AUTO_MEASURING ,value);
          else if(chat1==13) eeprom_1byte_write(AUTO_STABLE ,value);
          else if(chat1==14) eeprom_1byte_write(AUTO_PRINT ,value);
          else if(chat1==15) eeprom_1byte_write(PAD_TYPE ,value);
          else if(chat1==16) eeprom_1byte_write(DISPLAY_USED ,value);
          else if(chat1==17) eeprom_1byte_write(PRINT_DENSITY ,value);
          else if(chat1==18) eeprom_1byte_write(FUNC_KEY ,value);
        }
        
        else if(FunData.Mode==4)
        {
          if(chat1==1) eeprom_1byte_write(PAD_SEL ,value);    
          else if(chat1==2) eeprom_1byte_write(STABLE ,value); 
          else if(chat1==3) eeprom_1byte_write(WEIGH_IN_MOTION ,value); 
          else if(chat1==4) eeprom_1byte_write(OVER_ENABLE ,value);
          else if(chat1==5) eeprom_1byte_write(PRINT_FORM ,value);
          else if(chat1==6) eeprom_1byte_write(PRINT_COPIES ,value);
          else if(chat1==7) eeprom_1byte_write(LINEFEED ,value);
          else if(chat1==8) eeprom_1byte_write(PRINT_ENABLE ,value);
          else if(chat1==9) eeprom_1byte_write(DATA_FORMAT ,value);
          else if(chat1==10) eeprom_1byte_write(WIRELESS_OUTPUT ,value);
          else if(chat1==11) eeprom_1byte_write(AUTO_ZERO ,value); 
          else if(chat1==12) eeprom_1byte_write(AUTO_MEASURING ,value);
          else if(chat1==13) eeprom_1byte_write(AUTO_STABLE ,value);
          else if(chat1==14) eeprom_1byte_write(AUTO_PRINT ,value);
          else if(chat1==15) eeprom_1byte_write(HOLD_SPEED ,value);
          else if(chat1==16) eeprom_1byte_write(HOLD_ZERO ,value);
          else if(chat1==17) eeprom_1byte_write(PRINT_DENSITY ,value);
          else if(chat1==18) eeprom_1byte_write(FILTER_DEGREE ,value); 
        }
        break; 
      }
           
      if( (!value) && key_push ) {value += key_set; key_push = 0;}
      else if(key_push == 1)
      {
          value *= 10;
          value += key_set;
          key_push = 0;
      }
      
      if(FunData.Mode == 3)
      {
        if( chat1 ==1 && (value > 16))value = 0;
        else if( (chat1==2 || chat1==7 || chat1==17) && (value > 9))value = 0;
        else if( (chat1==3 || chat1==4 || chat1==15) && (value > 2))value = 0;
        else if( (chat1==6 || chat1==8 || chat1==9 || chat1==10 || chat1==11 || 
                  chat1==13 || chat1==14 || chat1==16 || chat1==18) && (value > 1))value = 0;
        else if( chat1==5 && (value > 6))value = 0;
        else if( chat1==12 && (value > 30 || value < 5))value = 0;          
      }
      
      else if(FunData.Mode == 4)
      {
        if( (chat1==2 || chat1==7 || chat1==11 || chat1==13 || chat1==14 || chat1==15 
                      || chat1==16 || chat1==17 || chat1 ==18) && (value > 9))value = 0;
        else if( (chat1==6 || chat1==8 || chat1==9 || chat1==10) && (value > 1))value = 0;
        else if( (chat1==1 || chat1==4 || chat1==3) && (value > 2))value = 0;  
        else if(  chat1==5 && (value > 6))value = 0;
        else if(  chat1==12 && (value > 30 || value < 5))value = 0;
      }
            
      key_set = 0;                     
            
      if(bound_count <= 800)
      {
        mprintf(1, 5,"          %d", value);
        bound_count++;
      }
      else if(bound_count <= 1200)
      {
        mprintf(1, 5,"              ");
        bound_count++;
      }
      else bound_count = 0;
  }
}

void function_read(void)
{    
  unsigned char i;
  
    FunData.Mode = eeprom_1byte_read(MODE);
    FunData.Pad_Sel = eeprom_1byte_read(PAD_SEL);
    FunData.Stable = eeprom_1byte_read(STABLE);    
    FunData.Weigh_In_Motion = eeprom_1byte_read(WEIGH_IN_MOTION);
    FunData.Over_Enable = eeprom_1byte_read(OVER_ENABLE);
    FunData.Print_Form = eeprom_1byte_read(PRINT_FORM);
    FunData.Print_Copies = eeprom_1byte_read(PRINT_COPIES);
    FunData.LineFeed = eeprom_1byte_read(LINEFEED);
    FunData.Print_Enable = eeprom_1byte_read(PRINT_ENABLE);
    FunData.Data_Format = eeprom_1byte_read(DATA_FORMAT);
    FunData.Wireless_Output = eeprom_1byte_read(WIRELESS_OUTPUT);
    FunData.Off_Syns = eeprom_1byte_read(OFF_SYNS);
    FunData.Auto_Measuring = eeprom_1byte_read(AUTO_MEASURING);
    FunData.Auto_Stable = eeprom_1byte_read(AUTO_STABLE);
    FunData.Auto_Print = eeprom_1byte_read(AUTO_PRINT);
    FunData.Pad_Type = eeprom_1byte_read(PAD_TYPE);
    FunData.Display_Used = eeprom_1byte_read(DISPLAY_USED);  
    FunData.Print_Density = eeprom_1byte_read(PRINT_DENSITY);  
    FunData.Func_Key = eeprom_1byte_read(FUNC_KEY);
    FunData.Filter_Degree = eeprom_1byte_read(FILTER_DEGREE);
    FunData.Hold_Speed = eeprom_1byte_read(HOLD_SPEED);
    FunData.Hold_Zero = eeprom_1byte_read(HOLD_ZERO);
    FunData.KeyBeep = 1;
    
    for(i=0; i<20; i++)p_head[i] = eeprom_1byte_read(P_HEAD);
}

void function_write(void)
{    
    eeprom_1byte_write(MODE, FunData.Mode); 
    eeprom_1byte_write(PAD_SEL, FunData.Pad_Sel);          
    eeprom_1byte_write(STABLE, FunData.Stable);          
    eeprom_1byte_write(WEIGH_IN_MOTION, FunData.Weigh_In_Motion);  
    eeprom_1byte_write(OVER_ENABLE, FunData.Over_Enable);  
    eeprom_1byte_write(PRINT_FORM, FunData.Print_Form);  
    eeprom_1byte_write(PRINT_COPIES, FunData.Print_Copies);  
    eeprom_1byte_write(LINEFEED, FunData.LineFeed);  
    eeprom_1byte_write(PRINT_ENABLE, FunData.Print_Enable);  
    eeprom_1byte_write(DATA_FORMAT, FunData.Data_Format);  
    eeprom_1byte_write(WIRELESS_OUTPUT, FunData.Wireless_Output);  
    eeprom_1byte_write(OFF_SYNS, FunData.Off_Syns);  
    eeprom_1byte_write(AUTO_MEASURING, FunData.Auto_Measuring);  
    eeprom_1byte_write(AUTO_STABLE, FunData.Auto_Stable);  
    eeprom_1byte_write(AUTO_PRINT, FunData.Auto_Print);  
    eeprom_1byte_write(PAD_TYPE, FunData.Pad_Type);  
    eeprom_1byte_write(DISPLAY_USED, FunData.Display_Used);   
    eeprom_1byte_write(PRINT_DENSITY, FunData.Print_Density);
    eeprom_1byte_write(FUNC_KEY, FunData.Func_Key); 
    eeprom_1byte_write(FILTER_DEGREE, FunData.Filter_Degree);  
    eeprom_1byte_write(HOLD_SPEED, FunData.Hold_Speed);  
    eeprom_1byte_write(HOLD_ZERO, FunData.Hold_Zero);  
}

void function_range(void)
{
   if(FunData.Mode || FunData.Mode!=4) FunData.Mode = 3;
   if(FunData.Pad_Sel<1 || FunData.Pad_Sel>16) FunData.Pad_Sel = 2;
   if(FunData.Stable<1 || FunData.Stable>9) FunData.Stable = 2;
   if(FunData.Weigh_In_Motion>2) FunData.Weigh_In_Motion = 0;
   if(FunData.Over_Enable>2) FunData.Over_Enable = 0; 
   if(FunData.Print_Form>9) FunData.Print_Form = 0; 
   if(FunData.Print_Copies>1) FunData.Print_Copies = 0;     
   if(FunData.LineFeed>9) FunData.LineFeed = 5;     
   if(FunData.Print_Enable>1) FunData.Print_Enable = 1;     
   if(FunData.Data_Format>1) FunData.Data_Format = 0;    
   if(FunData.Wireless_Output>1) FunData.Wireless_Output = 0;      
   if(FunData.Off_Syns>1) FunData.Off_Syns = 1;    
   if(FunData.Auto_Measuring<5 || FunData.Auto_Measuring>30) FunData.Auto_Measuring = 10;    
   if(FunData.Auto_Stable>9) FunData.Auto_Stable = 0;
   if(FunData.Auto_Print>9) FunData.Auto_Print = 2;    
   if(FunData.Pad_Type>2) FunData.Pad_Type = 0;
   if(FunData.Display_Used>1) FunData.Display_Used = 0; 
   if(FunData.Print_Density>9) FunData.Print_Density = 4;
   if(FunData.Func_Key>1) FunData.Func_Key = 0;
   if(FunData.Filter_Degree<1 || FunData.Filter_Degree>9) FunData.Filter_Degree = 5;
   if(FunData.Hold_Speed<1 || FunData.Hold_Speed>9) FunData.Hold_Speed = 5;
   if(FunData.Hold_Zero<1 || FunData.Hold_Zero>9) FunData.Hold_Zero = 5;
    
    function_write();
    HAL_Delay(10);
}

void function_reset(void)
{
    FunData.Mode = 3;
    FunData.Pad_Sel = 2;
    FunData.Stable = 2;
    FunData.Weigh_In_Motion = 0;
    FunData.Over_Enable = 0; 
    FunData.Print_Form = 0; 
    FunData.Print_Copies = 0;     
    FunData.LineFeed = 5;     
    FunData.Print_Enable = 1;     
    FunData.Data_Format = 0;    
    FunData.Wireless_Output = 0;      
    FunData.Off_Syns = 1;    
    FunData.Auto_Measuring = 10;    
    FunData.Auto_Stable = 0;
    FunData.Auto_Print = 2;    
    FunData.Pad_Type = 0;
    FunData.Display_Used = 0; 
    FunData.Print_Density = 4;
    FunData.Func_Key = 0;
    FunData.Filter_Degree = 5;
    FunData.Hold_Speed = 5;
    FunData.Hold_Zero = 5;
    
    function_write();
    HAL_Delay(10);
}

void cal_write(void)
{
   unsigned char i, j;
   
   for(j=0; j<2; j++)
   {
      for(i=0; i<5; i++)
      {        
        eeprom_float_write(V_RES_FACTOR+(j*20)+(i*4), v_res_factor[j][i]);         	         
        eeprom_4byte_write(V_ADC_ORG+(j*20)+(i*4), v_adc_org[j][i]);
      } 
   }

   for(i=0; i<2; i++)
   {
        eeprom_4byte_write(V_ZERO+(i*4), v_zero[i]);
        eeprom_4byte_write(V_MAXIMUM_CAPACITY+(i*4), v_maximum_capacity[i]);                 
        eeprom_4byte_write(V_E_RESOLUTION+(i*4), v_e_resolution[i]);        
        eeprom_1byte_write(V_MINIMUM_DIVISION+i, v_minimum_division[i]);
    }    
    eeprom_1byte_write(V_MULTI_CAL, v_multi_cal); 
}

void cal_read(void)
{
   unsigned char i, j;
   
   for(j=0; j<2; j++)
   {
      for(i=0; i<5; i++)
      {        
        v_res_factor[j][i] =  eeprom_float_read(V_RES_FACTOR+(j*20)+(i*4));
        v_adc_org[j][i] =  eeprom_4byte_read(V_ADC_ORG+(j*20)+(i*4));
      } 
   }  
   
   for(i=0; i<2; i++)
   {
        v_zero[i] = eeprom_4byte_read(V_ZERO+(i*4));
        v_maximum_capacity[i] = eeprom_4byte_read(V_MAXIMUM_CAPACITY+(i*4));
                         
        v_e_resolution[i] = eeprom_4byte_read(V_E_RESOLUTION+(i*4));        
        v_minimum_division[i] = eeprom_1byte_read(V_MINIMUM_DIVISION+i);
    }   
   v_multi_cal = eeprom_1byte_read(V_MULTI_CAL);
}

void Secret_function(void)
{
    char Set_Step;    
  
    Clear_Screen();
    
    Print_Str6x8(0xFF,10,1,"Secret Set");    
    PrintArrow(68,7,0);
    PrintArrow(80,7,1);
    PrintArrow(94,7,2);
    Print_Str6x8(0xFF,110,7,"1/1");
  
    Set_Step = eeprom_1byte_read(MODE);
  
    while(1)
    {
      KEYPAD_Scan();  
        
        if(Key.PressFlg[2])
        {
            Key.PressFlg[2] = 0;
            Set_Step--;
            if(Set_Step < 3) Set_Step = 5;
        }
          
        else if(Key.PressFlg[8])
        {
            Key.PressFlg[8] = 0;
            Set_Step++; 
            if(Set_Step > 5) Set_Step = 3;
        }
        
        else if(Key.PressFlg[12])
        {
            Clear_Screen();
            Key.PressFlg[12] = 0;
            return;
        }
        
        else if(Key.PressFlg[13])
        {
            Clear_Screen();
            Key.PressFlg[13] = 0;            
            if(Set_Step==5)
            {
              function_reset();
              mprintf(1, 2,"  - R E S E T -   ");              
              HAL_Delay(1000);
              mprintf(1, 2,"                  ");
              HAL_Delay(1000);
              mprintf(1, 2,"  - R E S E T -   ");              
              HAL_Delay(1000);
              mprintf(1, 2,"                  ");
              HAL_Delay(1000);
              Clear_Screen();
              return;
            }
            FunData.Mode = Set_Step;
            eeprom_1byte_write(MODE, FunData.Mode);
            break;
        }
      
        Print_Str6x8(Set_Step, 1, 3, " WIRELESS         ");
        Print_Str6x8(Set_Step, 1, 4, " WIRED            ");
        Print_Str6x8(Set_Step, 1, 5, " RESET            ");
    }        
}