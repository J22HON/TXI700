#include "main.h"

unsigned char   eeprom_add[2],
                 eeprom_data1[16],   
                 eeprom_data2[16],
                 eeprom_data3[16];  

unsigned long   total_page,
                 start_page,
                 end_page,
                 end_add;

unsigned long   g_delay_cnt;

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