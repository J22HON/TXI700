#include "main.h"
#include "display_font.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

void Write_Command(unsigned char Command)
{
    unsigned char i=0;
    
    OLED_DC_LO;
    OLED_CS_LO;       
    for(i=0; i<8; i++)
    {
        OLED_SCLK_LO;
        
        if(Command & 0x80) OLED_SDIN_HI;
        else               OLED_SDIN_LO;
        OLED_SCLK_HI;
        Command = Command << 1;
    }
    OLED_CS_HI;
}

void Write_Data(unsigned char Data)
{
    unsigned char i=0;
    
    OLED_DC_HI;
    OLED_CS_LO;       
    for(i=0; i<8; i++)
    {
        OLED_SCLK_LO;
        if(Data & 0x80) OLED_SDIN_HI;
        else            OLED_SDIN_LO;
        OLED_SCLK_HI;        
        Data = Data << 1;
    }
    OLED_CS_HI;
}

void OLED_All_Fill(unsigned char bmp_data) // Fill all the bmp_data on the screen
{                                                      
   unsigned char x_pos = 0;
   unsigned char page = 0;
    
    Write_Command(0xB0 + 0); // Set page start address command
    Write_Command(0x00);        // Set lower column start address command
    Write_Command(0x10);        // Set higher column start address command
    
   for(page = 0; page <= 7; page++) 
   {    
      Write_Command(0xB0 + page); // Set page start address command
      Write_Command(0x00);        // Set lower column start address command
      Write_Command(0x10);        // Set higher column start address command
       
      for(x_pos = 0; x_pos <= 129; x_pos++)    
      {                                    
         Write_Data(bmp_data);
      }                    
   }    
}

void OLED_Fill(unsigned char bmp_data, unsigned char select) // Fill all the bmp_data on the screen 
{                                                      
   unsigned char x_pos = 0;
   unsigned char page = 0;
    
    Write_Command(0xB0 + 0); // Set page start address command
    Write_Command(0x00);        // Set lower column start address command
    Write_Command(0x10);        // Set higher column start address command
    if(select)
    {
        for(x_pos = 0; x_pos <= 113; x_pos++)    
        {                                    
            Write_Data(bmp_data);
        }  
    }
    else
    {
        for(x_pos = 0; x_pos <= 129; x_pos++)    
        {                                    
            Write_Data(bmp_data);
        }  
    }

   for(page = 1; page <= 7; page++) 
   {    
      Write_Command(0xB0 + page); // Set page start address command
      Write_Command(0x00);        // Set lower column start address command
      Write_Command(0x10);        // Set higher column start address command
       
      for(x_pos = 0; x_pos <= 129; x_pos++)    
      {                                    
         Write_Data(bmp_data);
      }                    
   }    
}

void Oled_Gotoxy(unsigned char x_pos, unsigned char y_pos)  //x,y setting
{                                    
   Write_Command((0xB0 + y_pos));  //Set_Page_Start_Address_CMD
   Write_Command(((x_pos & 0x0F) | 0x00));  //Set_Lower_Column_Start_Address_CMD
   Write_Command((((x_pos & 0xF0) >> 0x04) | 0x10));   //Set_Higher_Column_Start_Address_CMD
} 

void Clear_Screen(void) // Fill all the 0x00 on the screen
{ 
    OLED_Fill(0x00, ALL);
}

void Batt_Lamp(unsigned char x_pos, unsigned char y_pos, unsigned char Per)
{
    unsigned char i = 0;
    Oled_Gotoxy(x_pos, y_pos);
    
    for(i=0; i<16; i++)
    {
        Write_Data(Batt16x8[Per][i]);  
    }
    
}

void SBatt_Lamp(unsigned char y_pos, unsigned char Per)
{
    unsigned char i = 0;
    Oled_Gotoxy(118, y_pos);
    
    for(i=0; i<12; i++)
    {
        Write_Data(Batt12x8[Per][i]);  
    }    
}

void OLED_Initialize(void)
{
    OLED_ON;
    OLED_CS_LO;
    OLED_SCLK_HI;
    OLED_RES_LO;
    OLED_RES_HI;

    Write_Command(0xae);

    Write_Command(0xa1);    //segment remap

    Write_Command(0xda);    //common pads hardware: alternative
    Write_Command(0x12);

    Write_Command(0xc8);    //common output scan direction:com0~com63

    Write_Command(0xa8);    //multiplex ration mode:63
    Write_Command(0x3f);

    Write_Command(0xd5);    //display divide ratio/osc. freq. mode
    Write_Command(0x50);    //

    Write_Command(0x81);    //contrast control 
    Write_Command(0x3F);    // 

    Write_Command(0xd9);	   //set pre-charge period
    Write_Command(0xf1);	   //

    Write_Command(0x20);    //Set Memory Addressing Mode
    Write_Command(0x02);    //page addressing mode

    Write_Command(0xdb);    //VCOM deselect level mode

    Write_Command(0x20);	   //set Vvcomh

    Write_Command(0xad);    //master configuration 

    Write_Command(0x8e);    //external VCC supply

    Write_Command(0xa4);    //out follows RAM content

    Write_Command(0xa6);    //set normal display

    Write_Command(0x81);    //Brightness
    Write_Command(0xCF);

    Write_Command(0xaf);

    OLED_Fill(0x00, ALL); //OLED CLR
}

void Print_Str6x8(unsigned char Invert, unsigned char x_pos, unsigned char y_pos, char *ch) // 8x6 Setting mode string display
{ 
   unsigned char chr = 0; 
   unsigned char i = 0; 
   unsigned char j = 0;  
 
    while(ch[j] != '\0') 
    {      
       // if(ch[j]==0x7f) ch[j] = ' ';
        
        chr = (ch[j] - 32); 
 
        Oled_Gotoxy(x_pos, y_pos);
        for(i = 0; i < 6; i++)      
        {            
            if(Invert == y_pos) // if(y_pos == Invert) --> FONT Invert
            {
                Write_Data(FONT6x8[chr][i] ^ 0xFF);  
            }
            else
            {
                Write_Data(FONT6x8[chr][i]);  
            }
        }
        j++; 
        x_pos += 6; 
    } 
} 

int mprintf(const char x_pos, const char y_pos, const char* format, ...) 
{ 
    char *buf; 
    va_list ap; 
    int i;
    
    buf = (char*) malloc(30); 
    va_start(ap, format); 
    i=vsprintf(buf,format,ap); 
    va_end(ap); 
    
    Print_Str6x8(0xff, x_pos, y_pos, buf); 
    free(buf);

    return i; 
}

void PrintArrow(unsigned char x_pos, unsigned char y_pos, unsigned char ch)
{
    unsigned char i = 0;
    
    Oled_Gotoxy(x_pos, y_pos);
    for(i = 0; i < 9; i++) Write_Data(Arrow9x8[ch][i]);
}

void LOGO_Print(unsigned char select)
{
    unsigned char i = 0;
    unsigned char x_pos=26;
    unsigned char y_pos=1;
    
 
        Oled_Gotoxy(x_pos+7, y_pos);
        for(i=0; i<68; i++) Write_Data(TMTLOGO[i]);
        
        Oled_Gotoxy(x_pos+7, y_pos+1);
        for(i=0; i<68; i++) Write_Data(TMTLOGO[i+68]);
        
        Oled_Gotoxy(x_pos+7, y_pos+2);
        for(i=0; i<68; i++) Write_Data(TMTLOGO[i+136]);
}

void Perforated_Line(void)
{
    unsigned char i = 0;
    
    Oled_Gotoxy(0, 1);
    for(i=0; i<64; i++)
    {
        Write_Data(0x10);
        Write_Data(0x00);
    }

    Oled_Gotoxy(0, 6);
    for(i=0; i<64; i++)
    {
        Write_Data(0x10);
        Write_Data(0x00);
    }
}

int wprintf(const char* format, ...)
{
    char *buf;
    va_list ap;
    int i;
    
    buf = (char*) malloc(30); 
    va_start(ap, format); 
    i=vsprintf(buf,format,ap); 
    va_end(ap); 
    Print_Str18x40(2, 2, buf); 
    free(buf);

    return i; 
}

void Print_Str18x40(unsigned char x_pos, unsigned char y_pos, char *ch)
{ 
   unsigned char chr = 0; 
   unsigned char j = 0;  
 
    while(ch[j] != '\0')
    {     
        if(ch[j] == '.')
        {
            x_pos -= 4;
            Oled_Gotoxy(x_pos, y_pos);
            Print_Dot4x40(x_pos, y_pos,0);
            x_pos += 4;
            j++;
        }
        else if(ch[j] == ',') // Dot delete
        {        
            x_pos -= 4;
            Oled_Gotoxy(x_pos, y_pos);
            Print_Dot4x40(x_pos, y_pos,1);
            x_pos += 4;
            j++;
        
        }
        else if(ch[j] == ' ')
        {
            Oled_Gotoxy(x_pos, y_pos);
            Print_Num18x40(x_pos, y_pos, 10);
            x_pos += 22;
            j++;
        }
        else if( (ch[j] == '-') || ((ch[j]>=0x41) && (ch[j]<=0x5A)) )
        {
            Oled_Gotoxy(x_pos, y_pos);
            
            Print_Text18x40(x_pos, y_pos, ch[j]);
            x_pos += 22;
            j++;
        }
        else
        {
            chr = (ch[j] - 48); 
            Oled_Gotoxy(x_pos, y_pos);  
            Print_Num18x40(x_pos, y_pos, chr);
            x_pos += 22; 
            j++;
        }
    } 
}

void Print_Dot4x40(unsigned char x_pos, unsigned char y_pos, unsigned char chr)
{
    unsigned char i=0, j=0;
    
    for(j=0; j<5; j++)
    {
        Oled_Gotoxy(x_pos, y_pos+j);
        for(i=0; i<4; i++)
        {
            Write_Data(DOT4x40[chr][i+(j*4)]);
        }
    }
}

void Print_Text18x40(unsigned char x_pos, unsigned char y_pos, unsigned char ch) // 18x40TEXT FONT
{ 
    uint8_t i = 0, j = 0; 
    if(ch=='C') ch = 0;
    else if(ch=='H') ch = 1;
    else if(ch=='O') ch = 2;
    else if(ch=='V') ch = 3;
    else if(ch=='E') ch = 4;
    else if(ch=='R') ch = 5;
    else if(ch=='L') ch = 6;
    else if(ch=='D') ch = 7;
    else if(ch=='A') ch = 8;
    else if(ch=='-') ch = 9;
    
    for(j=0; j<5; j++)
    {
        Oled_Gotoxy(x_pos, y_pos+j); 
        for(i = 0; i < 18; i++) Write_Data(TEXT18x40[ch][i+j*18]);  
    }
    
}

void Print_Num18x40(unsigned char x_pos, unsigned char y_pos, unsigned char ch) // 18x40NUM FONT
{ 
    unsigned char i = 0, j = 0; 
    
    for(j=0; j<5; j++)
    {
        Oled_Gotoxy(x_pos, y_pos+j); 
        for(i = 0; i < 18; i++) Write_Data(NUM18x40[ch][i+j*18]);  
    }
}

void Bar_Graph(unsigned long max_capa, unsigned long set_weight, unsigned long weight_value, unsigned char division)
{
    unsigned char i = 0;
    unsigned char x_pos = 3;
    unsigned char y_pos = 7;
    unsigned char set_weight_per = 0;
    unsigned char value_per = 0;
    double value = 0; 
    //sEEPROM_t* pEEPROM = NULL;
    
    //pEEPROM = sEEPROM_getptr(); //EEPROM struct adress call
    value = weight_value/square(10,division);
    value_per = (unsigned char)((value/(float)max_capa)*100);
    set_weight_per = (unsigned char) (((float)set_weight/(float)max_capa)*100 );
    if( (max_capa==0) || (set_weight==0) )
    {
        Oled_Gotoxy(x_pos, y_pos);
        Write_Data(0x00);   //blank
        Write_Data(0x3C);   //start fill
        for(i = 0; i < 102; i++) Write_Data(0x24); // blank fill
        Write_Data(0x3C);   // end fill
        return;
    }
    if(value < set_weight)  
    {
        //if(pEEPROM->v_buzzer) buzzer_set(0);
        set_weight_per = (unsigned char)(((float)set_weight/(float)max_capa)*100);
        Oled_Gotoxy(x_pos, y_pos);
        Write_Data(0x00);   //blank
        Write_Data(0x3C);   //start fill
        for(i = 0; i < value_per; i++) Write_Data(0x3C); //middle fill
        for(i = 0; i < (set_weight_per-value_per); i++) Write_Data(0x24); //blank fill
        Write_Data(0xA4);   //arrow
        Write_Data(0xE4);   //arrow
        Write_Data(0xA4);   //arrow
        for(i = 0; i < (102-set_weight_per); i++) Write_Data(0x24); // blank fill
        Write_Data(0x3C);   // end fill
    }
    else if(value == set_weight)
    {
        //if(pEEPROM->v_buzzer) buzzer_set(0);
        Oled_Gotoxy(x_pos, y_pos);
        Write_Data(0x00);   //blank
        Write_Data(0x3C);   //start fill
        for(i = 0; i < value_per; i++) Write_Data(0x3C); //middle fill
        Write_Data(0xBC);   //arrow
        Write_Data(0xE4);   //arrow
        Write_Data(0xA4);   //arrow
        for(i = 0; i < (102-set_weight_per); i++) Write_Data(0x24); // blank fill
        Write_Data(0x3C);   // end fill
    }
    else if( (value > set_weight) && (value < max_capa))
    {
        //if(pEEPROM->v_buzzer) buzzer_set(WARNING);
        Oled_Gotoxy(x_pos, y_pos);
        Write_Data(0x00);   //blank
        Write_Data(0x3C);   //start fill
        for(i = 0; i < set_weight_per; i++) Write_Data(0x3C); //middle fill
        Write_Data(0xBC);   //arrow
        Write_Data(0xFC);   //arrow
        Write_Data(0xBC);   //arrow
        for(i = 0; i < (value_per-set_weight_per); i++) Write_Data(0x3C); //fill
        for(i = 0; i < (102-value_per); i++) Write_Data(0x24); // blank fill
        Write_Data(0x3C);   // end fill
    }
    else 
    {
        //if(pEEPROM->v_buzzer) buzzer_set(WARNING);
    }
    Write_Data(0x00);
}

int square(unsigned char value, unsigned char num)
{
    unsigned char i=0;
    unsigned long result=0;
    
    result=value;
    if(num==0) return 1;
    for(i=0; i<(num-1); i++)
    {
        result = result*value;
    }
    return result;
}

void lamp_off(void)
{
    Stable_Lamp(OFF);
    Hold_Lamp(OFF);
    ZIGBEE_Lamp(OFF);
    BLE_Lamp(OFF);
    STATE_Lamp(0);
    Zero_Lamp(OFF);
}

void lamp_on(void)
{
    Stable_Lamp(ON);
    Hold_Lamp(ON);
    ZIGBEE_Lamp(ON);
    BLE_Lamp(ON);
    STATE_Lamp(0);
    Zero_Lamp(ON);    
}

void Zero_Lamp(uint8_t k)
{
    if(k) Print_Str6x8(0xFF, 1, 0, "ZERO"); 
    else  Print_Str6x8(0xFF, 1, 0 ,"    ");
}

void ZIGBEE_Lamp(uint8_t k)
{
    if(k) Print_Str6x8(0xFF, 61, 0, "Z"); 
    else  Print_Str6x8(0xFF, 61, 0 ," ");
}

void BLE_Lamp(uint8_t k)
{
    if(k) Print_Str6x8(0xFF, 73, 0, "B"); 
    else  Print_Str6x8(0xFF, 73, 0 ," ");
}

void STATE_Lamp(uint8_t k)
{    
    if(k==A_WIM) Print_Str6x8(0xFF, 85, 0 ,"A");
    else if(k==W_WIM) Print_Str6x8(0xFF, 85, 0 ,"W");
    else Print_Str6x8(0xFF, 85, 0, "S"); 
}

void Hold_Lamp(uint8_t k)
{    
    if(k==PEAK)            Print_Str6x8(0xFF, 31, 0, "PEAK");
    else if((k==HOLD) || (k==ON)) Print_Str6x8(0xFF, 31, 0, "HOLD");
    else if(k==OFF)        Print_Str6x8(0xFF, 31, 0, "    ");
}

void Stable_Lamp(uint8_t k)
{
    uint8_t i=0;
    
    Oled_Gotoxy(95, 0);
    for(i = 0; i <9 ; i++) Write_Data(Stable9x8[k][i]);    
}

void Print_Str6x8up(uint8_t x_pos, uint8_t y_pos, char *ch) // 8x6 Setting mode string display
{ 
   uint8_t chr = 0;
   uint8_t i = 0;
   uint8_t j = 0;
   
    while(ch[j] != '\0') 
    {      
        if( (ch[j]>='0') && (ch[j]<='9') ) chr = (ch[j] - 0x30); 
        else if(ch[j]=='.') chr = 10;
        else if(ch[j]=='%') chr = 11;
        else if(ch[j]=='f') chr = 12;
        else if(ch[j]=='L') chr = 13;
        else if(ch[j]=='R') chr = 14;
        else if(ch[j]=='r') chr = 15;
        else if(ch[j]==' ') chr = 16;
        else if(ch[j]=='-') chr = 17;
 
        Oled_Gotoxy(x_pos, y_pos);
        for(i = 0; i < 6; i++)      
        {            
            Write_Data(UPFONT6x8[chr][i]);
        }
        j++; 
        x_pos += 6; 
    } 
} 
