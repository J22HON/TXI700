#include "main.h"

extern struct KEY Key;
extern struct FUNCTION FunData;
extern struct CAL Calibration;

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
                                   v_minimum_division[2];  

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
    char Set_Step;
    
RESET :   
  
    Clear_Screen();
    
    Print_Str6x8(0xFF,10,1,"FuncTion Set");    
    
    Set_Step = eeprom_1byte_read(MODE);
  
    while(1)
    {
      KEYPAD_Scan();  
        
        if(Key.PressFlg[2])
        {
            Key.PressFlg[2] = 0;
            Set_Step--;
            if(Set_Step < 3) Set_Step = 4;
        }
          
        else if(Key.PressFlg[8])
        {
            Key.PressFlg[8] = 0;
            Set_Step++; 
            if(Set_Step > 4) Set_Step = 3;
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
            FunData.Mode = Set_Step;
            eeprom_1byte_write(MODE, FunData.Mode);
            break;
        }
      
        Print_Str6x8(Set_Step, 1, 3, " Wireless         ");
        Print_Str6x8(Set_Step, 1, 4, " Wired            ");
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
          if(!value)
          {
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
          mprintf(1, 3," FuncTion : %02d", value);
          bound_count++;
        }
        else if(bound_count <= 1200)
        {
          mprintf(1, 3," FuncTion :      ", value);
          bound_count++;
        }
        else bound_count = 0;
    }
    
    if(value)
    {
      number_long(value);    
      if(value<17) value++;
      else value = 1;
      goto RESET;
    }
}

void title_Function(unsigned char set_num)
{  
  Clear_Screen();
  
  switch(set_num)
  {
      case 1:
          mprintf(1, 2,"  Number of scales to");
          mprintf(1, 3," be linked wireless  ");
          break;
      
      case 2:
          mprintf(1, 2,"  Stable condition  ");
          break;

      case 3:
          mprintf(1, 2,"  Weigh-In-Motion   ");
          break;
          
      case 4:
          mprintf(1, 2,"  Excess Enable     ");
          break;

      case 5:
          mprintf(1, 2,"  Print format      ");
          break;

      case 6:
          mprintf(1, 2,"  Number of copies  ");
          break;

      case 7:
          mprintf(1, 2,"  Line feed         ");
          break;
          
      case 8:
          mprintf(1, 2,"  Enable Printer    ");
          break;          
          
      case 9:
          mprintf(1, 2,"  Data format for   ");
          mprintf(1, 3," auxiliary display  ");
          break;  
          
      case 10:
          mprintf(1, 2,"  External wireless ");
          mprintf(1, 3," output             ");
          break;                   
          
      case 11:
          mprintf(1, 2,"  Power off sync    ");
          break;    
          
      case 12:
          mprintf(1, 2,"  Automatic measuring");
          mprintf(1, 3," (A-WIM)             ");       
          break;    
          
      case 13:
          mprintf(1, 2,"  Auto weighing      ");
          mprintf(1, 3," stable time         ");
          break;          
          
      case 14:
          mprintf(1, 2,"  Auto print time    ");
          break;   
          
      case 15:
          mprintf(1, 2,"  Pad type used      ");
          break;  
          
      case 16:
          mprintf(1, 2,"  Power saving mode  ");
          mprintf(1, 3,"  Display used       ");
          break;  
          
      case 17:
          mprintf(1, 2,"  Print density      ");
          break;

      default:
          break;          
  }  
}

void number_long(unsigned char chat1)
{
  unsigned int value=0;
  char key_set = 0;
  char key_push = 0;
  unsigned int bound_count = 0;
  
  if(chat1==1) value = eeprom_1byte_read(FUNCTION01); 
  else if(chat1==2) value = eeprom_1byte_read(FUNCTION02); 
  else if(chat1==3) value = eeprom_1byte_read(FUNCTION03); 
  else if(chat1==4) value = eeprom_1byte_read(FUNCTION04);
  else if(chat1==5) value = eeprom_1byte_read(FUNCTION05);
  else if(chat1==6) value = eeprom_1byte_read(FUNCTION06);
  else if(chat1==7) value = eeprom_1byte_read(FUNCTION07);
  else if(chat1==8) value = eeprom_1byte_read(FUNCTION08);
  else if(chat1==9) value = eeprom_1byte_read(FUNCTION09);
  else if(chat1==10) value = eeprom_1byte_read(FUNCTION10);
  else if(chat1==11) value = eeprom_1byte_read(FUNCTION11);
  else if(chat1==12) value = eeprom_1byte_read(FUNCTION12);
  else if(chat1==13) value = eeprom_1byte_read(FUNCTION13);
  else if(chat1==14) value = eeprom_1byte_read(FUNCTION14);
  else if(chat1==15) value = eeprom_1byte_read(FUNCTION15);
  else if(chat1==16) value = eeprom_1byte_read(FUNCTION16);
  else if(chat1==17) value = eeprom_1byte_read(FUNCTION17);
  else chat1 = 1;

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
        if(chat1==1) eeprom_1byte_write(FUNCTION01, value);          
        else if(chat1==2) eeprom_1byte_write(FUNCTION02, value);          
        else if(chat1==3) eeprom_1byte_write(FUNCTION03, value);  
        else if(chat1==4) eeprom_1byte_write(FUNCTION04, value);  
        else if(chat1==5) eeprom_1byte_write(FUNCTION05, value);  
        else if(chat1==6) eeprom_1byte_write(FUNCTION06, value);  
        else if(chat1==7) eeprom_1byte_write(FUNCTION07, value);  
        else if(chat1==8) eeprom_1byte_write(FUNCTION08, value);  
        else if(chat1==9) eeprom_1byte_write(FUNCTION09, value);  
        else if(chat1==10) eeprom_1byte_write(FUNCTION10, value);  
        else if(chat1==11) eeprom_1byte_write(FUNCTION11, value);  
        else if(chat1==12) eeprom_1byte_write(FUNCTION12, value);  
        else if(chat1==13) eeprom_1byte_write(FUNCTION13, value);  
        else if(chat1==14) eeprom_1byte_write(FUNCTION14, value);  
        else if(chat1==15) eeprom_1byte_write(FUNCTION15, value);  
        else if(chat1==16) eeprom_1byte_write(FUNCTION16, value);  
        else if(chat1==17) eeprom_1byte_write(FUNCTION17, value);  
        else if(chat1==18) eeprom_1byte_write(FUNCTION18, value);  
        else if(chat1==19) eeprom_1byte_write(FUNCTION19, value);  
        else if(chat1==20) eeprom_1byte_write(FUNCTION20, value); 
        function_reset();
        break; 
      }
           
      if( (!value) && key_push ) {value += key_set; key_push = 0;}
      else if(key_push == 1)
      {
          value *= 10;
          value += key_set;
          key_push = 0;
      }
      if(chat1==1 && ( value < 0 || value > 16)) value = 0;
      else if( chat1==2 && (value > 9 || value < 0)) value = 0;
      else if( (chat1==3 || chat1==4 || chat1==15) && (value > 2 || value < 0)) value = 0;
      else if( (chat1==5 || chat1==7 || chat1==17) && (value > 9 || value < 0)) value = 0;
      else if( (chat1==6 || chat1==8 || chat1==9 || chat1==10 || chat1==11 || 
                 chat1==13 || chat1==14 || chat1==16) && (value > 1 || value < 0)) value = 0;
      else if( chat1==12 && (value > 30 || value < 5)) value = 0;          
      key_set = 0;                     
      
      if(bound_count <= 800)
      {
        if(chat1==1 || chat1==12)      mprintf(1, 5,"         %02d", value);
        else                         mprintf(1, 5,"          %d ", value);
        bound_count++;
      }
      else if(bound_count <= 1200)
      {
        if(chat1==1 || chat1==12)      mprintf(1, 5,"             ");
        else                         mprintf(1, 5,"             ");
        bound_count++;
      }
      else bound_count = 0;
  }
}

void function_read(void)
{    
    FunData.Mode = eeprom_1byte_read(MODE);
    FunData.Pad_Sel = eeprom_1byte_read(FUNCTION01);
    FunData.Stable = eeprom_1byte_read(FUNCTION02);    
    FunData.Weigh_In_Motion = eeprom_1byte_read(FUNCTION03);
    FunData.Over_Enable = eeprom_1byte_read(FUNCTION04);
    FunData.Print_Form = eeprom_1byte_read(FUNCTION05);
    FunData.Print_Copies = eeprom_1byte_read(FUNCTION06);
    FunData.LineFeed = eeprom_1byte_read(FUNCTION07);
    FunData.Print_Enable = eeprom_1byte_read(FUNCTION08);
    FunData.Data_Format = eeprom_1byte_read(FUNCTION09);
    FunData.Wireless_Output = eeprom_1byte_read(FUNCTION10);
    FunData.Off_Syns = eeprom_1byte_read(FUNCTION11);
    FunData.Auto_Measuring = eeprom_1byte_read(FUNCTION12);
    FunData.Auto_Stable = eeprom_1byte_read(FUNCTION13);
    FunData.Auto_Print = eeprom_1byte_read(FUNCTION14);
    FunData.Pad_Type = eeprom_1byte_read(FUNCTION15);
    FunData.Display_Used = eeprom_1byte_read(FUNCTION16);  
    FunData.Print_Density = eeprom_1byte_read(FUNCTION17);  
}

void function_write(void)
{    
    eeprom_1byte_write(FUNCTION01, FunData.Pad_Sel);          
    eeprom_1byte_write(FUNCTION02, FunData.Stable);          
    eeprom_1byte_write(FUNCTION03, FunData.Weigh_In_Motion);  
    eeprom_1byte_write(FUNCTION04, FunData.Over_Enable);  
    eeprom_1byte_write(FUNCTION05, FunData.Print_Form);  
    eeprom_1byte_write(FUNCTION06, FunData.Print_Copies);  
    eeprom_1byte_write(FUNCTION07, FunData.LineFeed);  
    eeprom_1byte_write(FUNCTION08, FunData.Print_Enable);  
    eeprom_1byte_write(FUNCTION09, FunData.Data_Format);  
    eeprom_1byte_write(FUNCTION10, FunData.Wireless_Output);  
    eeprom_1byte_write(FUNCTION11, FunData.Off_Syns);  
    eeprom_1byte_write(FUNCTION12, FunData.Auto_Measuring);  
    eeprom_1byte_write(FUNCTION13, FunData.Auto_Stable);  
    eeprom_1byte_write(FUNCTION14, FunData.Auto_Print);  
    eeprom_1byte_write(FUNCTION15, FunData.Pad_Type);  
    eeprom_1byte_write(FUNCTION16, FunData.Display_Used);  
    eeprom_1byte_write(FUNCTION17, FunData.Print_Density);  
}

void function_reset(void)
{
          if(FunData.Mode<3 || FunData.Mode>4) FunData.Mode = 3;
    else if(FunData.Pad_Sel<1 || FunData.Pad_Sel>16) FunData.Pad_Sel = 2;
    else if(FunData.Stable<1 || FunData.Stable>9) FunData.Stable = 2;
    else if(FunData.Weigh_In_Motion<0 || FunData.Weigh_In_Motion>2) FunData.Weigh_In_Motion = 0;
    else if(FunData.Over_Enable<0 || FunData.Over_Enable>2) FunData.Over_Enable = 0; 
    else if(FunData.Print_Form<0 || FunData.Print_Form>9) FunData.Print_Form = 0; 
    else if(FunData.Print_Copies<0 || FunData.Print_Copies>1) FunData.Print_Copies = 0;     
    else if(FunData.LineFeed<0 || FunData.LineFeed>9) FunData.LineFeed = 5;     
    else if(FunData.Print_Enable<0 || FunData.Print_Enable>1) FunData.Print_Enable = 1;     
    else if(FunData.Data_Format<0 || FunData.Data_Format>1) FunData.Data_Format = 0;    
    else if(FunData.Wireless_Output<0 || FunData.Wireless_Output>1) FunData.Wireless_Output = 0;      
    else if(FunData.Off_Syns<0 || FunData.Off_Syns>1) FunData.Off_Syns = 1;    
    else if(FunData.Auto_Measuring<5 || FunData.Auto_Measuring>30) FunData.Auto_Measuring = 10;    
    else if(FunData.Auto_Stable<0 || FunData.Auto_Stable>9) FunData.Auto_Stable = 0;
    else if(FunData.Auto_Print<0 || FunData.Auto_Print>9) FunData.Auto_Print = 2;    
    else if(FunData.Pad_Type<0 || FunData.Pad_Type>2) FunData.Pad_Type = 0;
    else if(FunData.Display_Used<0 || FunData.Display_Used>1) FunData.Display_Used = 0; 
    else if(FunData.Print_Density<0 || FunData.Print_Density>9) FunData.Print_Density = 4;
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