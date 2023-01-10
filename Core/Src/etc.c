#include "main.h"

extern struct KEY Key;

extern unsigned char        ShortBeep,
                             v_car_id[12],
                             v_item[10],
                             p_head[120];

void Memory_Input(unsigned char pick)
{
  unsigned char v_alpha=0, v_in=0, i, toggle=0, id_digit=0, v_current=0;
  unsigned char id_back[10];
  unsigned long delay_timer;
  
  Clear_Screen();
  for(i=0; i<10; i++) id_back[i]=0xFF;
  if(!pick)mprintf(1, 2,"  VEHICLE NUMBER    ");
  else if(pick)mprintf(1, 2,"  ITEM NUMBER    ");
  
  while(1)
  {
     KEYPAD_Scan(); 
     
     if(id_digit > 10) { bbik(); Key_Clear(); HAL_Delay(100); id_digit--;}

     if(v_alpha)
     {
	if(Key.PressFlg[1]) 
	{
	  Key.PressFlg[1]=0;
          if(v_current!=1) { toggle=0; v_in=0; } 
          if(!toggle) { if(v_in) id_back[id_digit-1] = '.'; else id_back[id_digit] = '.'; toggle=1; }
          else        { if(v_in) id_back[id_digit-1] = '-'; else id_back[id_digit] = '-'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=1;
	}
        else if(Key.PressFlg[2]) 
	{
	  Key.PressFlg[2]=0;
          if(v_current!=2) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'A'; else id_back[id_digit] = 'A'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'B'; else id_back[id_digit] = 'B'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'C'; else id_back[id_digit] = 'C'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=2;
	}
        else if(Key.PressFlg[3]) 
	{
	  Key.PressFlg[3]=0;
          if(v_current!=3) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'D'; else id_back[id_digit] = 'D'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'E'; else id_back[id_digit] = 'E'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'F'; else id_back[id_digit] = 'F'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=3;
	}
        else if(Key.PressFlg[4]) 
	{
	  Key.PressFlg[4]=0;
          if(v_current!=4) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'G'; else id_back[id_digit] = 'G'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'H'; else id_back[id_digit] = 'H'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'I'; else id_back[id_digit] = 'i'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=4;
	} 
        else if(Key.PressFlg[5]) 
	{
	  Key.PressFlg[5]=0;
          if(v_current!=5) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'J'; else id_back[id_digit] = 'J'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'K'; else id_back[id_digit] = 'K'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'L'; else id_back[id_digit] = 'L'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=5;
	}           
        else if(Key.PressFlg[6]) 
	{
	  Key.PressFlg[6]=0;
          if(v_current!=6) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'M'; else id_back[id_digit] = 'M'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'N'; else id_back[id_digit] = 'N'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'O'; else id_back[id_digit] = 'O'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=6;
	}                   
        else if(Key.PressFlg[7]) 
	{
	  Key.PressFlg[7]=0;
          if(v_current!=7) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'P'; else id_back[id_digit] = 'P'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'Q'; else id_back[id_digit] = 'Q'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'R'; else id_back[id_digit] = 'R'; toggle=3; }
          else if(toggle==3) { if(v_in) id_back[id_digit-1] = 'S'; else id_back[id_digit] = 'S'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=7;
	}         
        else if(Key.PressFlg[8]) 
	{
	  Key.PressFlg[8]=0;
          if(v_current!=8) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'T'; else id_back[id_digit] = 'T'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'U'; else id_back[id_digit] = 'U'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'V'; else id_back[id_digit] = 'V'; toggle=3; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=8;
	}
        else if(Key.PressFlg[9]) 
	{
	  Key.PressFlg[9]=0;
          if(v_current!=9) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'W'; else id_back[id_digit] = 'W'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'X'; else id_back[id_digit] = 'X'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'Y'; else id_back[id_digit] = 'Y'; toggle=3; }
          else if(toggle==3) { if(v_in) id_back[id_digit-1] = 'Z'; else id_back[id_digit] = 'Z'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=9;
	}        
        else if(Key.PressFlg[10]){ Key.PressFlg[10] = 0; if(v_alpha)v_alpha=0; else v_alpha = 1;   }
        else if(Key.PressFlg[11]){ Key.PressFlg[11] = 0; v_in=0; id_back[id_digit] = ' '; id_digit++; }
        else if(Key.PressFlg[12]){ Key.PressFlg[12] = 0; if(!id_digit) bbik(); else { v_in=0;  id_digit--; id_back[id_digit]=0xFF; }}
        else if(Key.PressFlg[13])
       { 
         Key.PressFlg[13] = 0; 
         if(!pick)
         {
           if(id_digit) { for(i=0; i<10; i++) v_car_id[i]=id_back[i]; }
           else         { for(i=0; i<10; i++) v_car_id[i]=0xFF; }
         }
         else
         {
           if(id_digit) { for(i=0; i<10; i++) v_item[i]=id_back[i]; }
           else         { for(i=0; i<10; i++) v_item[i]=0xFF; }
         }
         Clear_Screen();
         break;
       }
     }
     
     else
     {
       if(id_digit>10) { bbik(); Key_Clear(); id_digit--;} 
       delay_timer=0; v_in=0;
       if(Key.PressFlg[0]) { Key.PressFlg[0] = 0; v_in=0; id_back[id_digit] = 0; id_digit++; }
       else if(Key.PressFlg[1]) { Key.PressFlg[1] = 0; v_in=0; id_back[id_digit] = 1; id_digit++; }
       else if(Key.PressFlg[2]) { Key.PressFlg[2] = 0; v_in=0; id_back[id_digit] = 2; id_digit++; }
       else if(Key.PressFlg[3]) { Key.PressFlg[3] = 0; v_in=0; id_back[id_digit] = 3; id_digit++; }
       else if(Key.PressFlg[4]) { Key.PressFlg[4] = 0; v_in=0; id_back[id_digit] = 4; id_digit++; }
       else if(Key.PressFlg[5]) { Key.PressFlg[5] = 0; v_in=0; id_back[id_digit] = 5; id_digit++; } 
       else if(Key.PressFlg[6]) { Key.PressFlg[6] = 0; v_in=0; id_back[id_digit] = 6; id_digit++; }
       else if(Key.PressFlg[7]) { Key.PressFlg[7] = 0; v_in=0; id_back[id_digit] = 7; id_digit++; }
       else if(Key.PressFlg[8]) { Key.PressFlg[8] = 0; v_in=0; id_back[id_digit] = 8; id_digit++; }
       else if(Key.PressFlg[9]) { Key.PressFlg[9] = 0; v_in=0; id_back[id_digit] = 9; id_digit++; }      
       else if(Key.PressFlg[10]){ Key.PressFlg[10] = 0; if(v_alpha)v_alpha=0; else v_alpha = 1;   }
       else if(Key.PressFlg[11]){ Key.PressFlg[11] = 0; v_in=0; id_back[id_digit] = ' '; id_digit++; }
       else if(Key.PressFlg[12]){ Key.PressFlg[12] = 0; if(!id_digit) bbik(); else { v_in=0;  id_digit--; id_back[id_digit]=0xFF; }}
       else if(Key.PressFlg[13])
       { 
         Key.PressFlg[13] = 0; 
         if(!pick)
         {
           if(id_digit) { for(i=0; i<10; i++) v_car_id[i]=id_back[i]; }
           else         { for(i=0; i<10; i++) v_car_id[i]=0xFF; }
         }
         else
         {
           if(id_digit) { for(i=0; i<10; i++) v_item[i]=id_back[i]; }
           else         { for(i=0; i<10; i++) v_item[i]=0xFF; }
         }
         Clear_Screen();
         break;
       }
     }
           
      if(v_in)
      {
        delay_timer++;
        if(v_alpha) mprintf(1, 6," WRITE ");
        if(delay_timer>1000){delay_timer=0; v_in=0; toggle=0; }
      }
      else mprintf(1, 6,"       ");
      if(v_alpha) mprintf(1, 7," ENGLISH ");
      else if(!v_alpha) mprintf(1, 7," NUMBER  ");
      
      if(!id_digit) mprintf(1, 4,"                      ");
      else if(id_digit==1){if(id_back[0]<10)mprintf(20, 4,"%d  ",id_back[0]); else mprintf(20, 4,"%c  ",id_back[0]);}     
      else if(id_digit==2){if(id_back[1]<10)mprintf(26, 4,"%d  ",id_back[1]); else mprintf(26, 4,"%c  ",id_back[1]);}                  
      else if(id_digit==3){if(id_back[2]<10)mprintf(32, 4,"%d  ",id_back[2]); else mprintf(32, 4,"%c  ",id_back[2]);} 
      else if(id_digit==4){if(id_back[3]<10)mprintf(38, 4,"%d  ",id_back[3]); else mprintf(38, 4,"%c  ",id_back[3]);} 
      else if(id_digit==5){if(id_back[4]<10)mprintf(44, 4,"%d  ",id_back[4]); else mprintf(44, 4,"%c  ",id_back[4]);} 
      else if(id_digit==6){if(id_back[5]<10)mprintf(50, 4,"%d  ",id_back[5]); else mprintf(50, 4,"%c  ",id_back[5]);} 
      else if(id_digit==7){if(id_back[6]<10)mprintf(56, 4,"%d  ",id_back[6]); else mprintf(56, 4,"%c  ",id_back[6]);} 
      else if(id_digit==8){if(id_back[7]<10)mprintf(62, 4,"%d  ",id_back[7]); else mprintf(62, 4,"%c  ",id_back[7]);} 
      else if(id_digit==9){if(id_back[8]<10)mprintf(68, 4,"%d  ",id_back[8]); else mprintf(68, 4,"%c  ",id_back[8]);} 
      else if(id_digit==10){if(id_back[9]<10)mprintf(74, 4,"%d  ",id_back[9]); else mprintf(74, 4,"%c  ",id_back[9]);} 
  }
}

void heading_edit(void)
{
  unsigned char v_alpha=0, v_in=0, i, toggle=0, id_digit=0, v_current=0;
  unsigned char id_back[20];
  unsigned long delay_timer;
 
  Clear_Screen();
  for(i=0; i<20; i++) id_back[i]=0xFF;
  mprintf(1, 2,"  TITLE    ");

  while(1)
  {
     KEYPAD_Scan(); 
     
     if(id_digit > 20) { bbik(); Key_Clear(); HAL_Delay(100); id_digit--;}

     if(v_alpha)
     {
	if(Key.PressFlg[1]) 
	{
	  Key.PressFlg[1]=0;
          if(v_current!=1) { toggle=0; v_in=0; } 
          if(!toggle) { if(v_in) id_back[id_digit-1] = '.'; else id_back[id_digit] = '.'; toggle=1; }
          else        { if(v_in) id_back[id_digit-1] = '-'; else id_back[id_digit] = '-'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=1;
	}
        else if(Key.PressFlg[2]) 
	{
	  Key.PressFlg[2]=0;
          if(v_current!=2) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'A'; else id_back[id_digit] = 'A'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'B'; else id_back[id_digit] = 'B'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'C'; else id_back[id_digit] = 'C'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=2;
	}
        else if(Key.PressFlg[3]) 
	{
	  Key.PressFlg[3]=0;
          if(v_current!=3) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'D'; else id_back[id_digit] = 'D'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'E'; else id_back[id_digit] = 'E'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'F'; else id_back[id_digit] = 'F'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=3;
	}
        else if(Key.PressFlg[4]) 
	{
	  Key.PressFlg[4]=0;
          if(v_current!=4) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'G'; else id_back[id_digit] = 'G'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'H'; else id_back[id_digit] = 'H'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'I'; else id_back[id_digit] = 'i'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=4;
	} 
        else if(Key.PressFlg[5]) 
	{
	  Key.PressFlg[5]=0;
          if(v_current!=5) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'J'; else id_back[id_digit] = 'J'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'K'; else id_back[id_digit] = 'K'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'L'; else id_back[id_digit] = 'L'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=5;
	}           
        else if(Key.PressFlg[6]) 
	{
	  Key.PressFlg[6]=0;
          if(v_current!=6) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'M'; else id_back[id_digit] = 'M'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'N'; else id_back[id_digit] = 'N'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'O'; else id_back[id_digit] = 'O'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=6;
	}                   
        else if(Key.PressFlg[7]) 
	{
	  Key.PressFlg[7]=0;
          if(v_current!=7) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'P'; else id_back[id_digit] = 'P'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'Q'; else id_back[id_digit] = 'Q'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'R'; else id_back[id_digit] = 'R'; toggle=3; }
          else if(toggle==3) { if(v_in) id_back[id_digit-1] = 'S'; else id_back[id_digit] = 'S'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=7;
	}         
        else if(Key.PressFlg[8]) 
	{
	  Key.PressFlg[8]=0;
          if(v_current!=8) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'T'; else id_back[id_digit] = 'T'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'U'; else id_back[id_digit] = 'U'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'V'; else id_back[id_digit] = 'V'; toggle=3; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=8;
	}
        else if(Key.PressFlg[9]) 
	{
	  Key.PressFlg[9]=0;
          if(v_current!=9) { toggle=0; v_in=0; } 
          if(!toggle)        { if(v_in) id_back[id_digit-1] = 'W'; else id_back[id_digit] = 'W'; toggle=1; }
          else if(toggle==1) { if(v_in) id_back[id_digit-1] = 'X'; else id_back[id_digit] = 'X'; toggle=2; }
          else if(toggle==2) { if(v_in) id_back[id_digit-1] = 'Y'; else id_back[id_digit] = 'Y'; toggle=3; }
          else if(toggle==3) { if(v_in) id_back[id_digit-1] = 'Z'; else id_back[id_digit] = 'Z'; toggle=0; }
          if(!v_in) id_digit++;
          delay_timer=0; v_in=1; v_current=9;
	}        
        else if(Key.PressFlg[10]){ Key.PressFlg[10] = 0; if(v_alpha)v_alpha=0; else v_alpha = 1;   }
        else if(Key.PressFlg[11]){ Key.PressFlg[11] = 0; v_in=0; id_back[id_digit] = ' '; id_digit++; }
        else if(Key.PressFlg[12]){ Key.PressFlg[12] = 0; if(!id_digit) bbik(); else { v_in=0;  id_digit--; id_back[id_digit]=0xFF; }}
        else if(Key.PressFlg[13])
       { 
         Key.PressFlg[13] = 0; 
         if(id_digit) { for(i=0; i<20; i++) p_head[i]=id_back[i]; }
         else         { for(i=0; i<20; i++) p_head[i]=0xFF; }        
         Clear_Screen();
         break;
       }
     }
     
     else
     {
       if(id_digit>20) { bbik(); Key_Clear(); id_digit--;} 
       delay_timer=0; v_in=0;
       if(Key.PressFlg[0]) { Key.PressFlg[0] = 0; v_in=0; id_back[id_digit] = 0; id_digit++; }
       else if(Key.PressFlg[1]) { Key.PressFlg[1] = 0; v_in=0; id_back[id_digit] = 1; id_digit++; }
       else if(Key.PressFlg[2]) { Key.PressFlg[2] = 0; v_in=0; id_back[id_digit] = 2; id_digit++; }
       else if(Key.PressFlg[3]) { Key.PressFlg[3] = 0; v_in=0; id_back[id_digit] = 3; id_digit++; }
       else if(Key.PressFlg[4]) { Key.PressFlg[4] = 0; v_in=0; id_back[id_digit] = 4; id_digit++; }
       else if(Key.PressFlg[5]) { Key.PressFlg[5] = 0; v_in=0; id_back[id_digit] = 5; id_digit++; } 
       else if(Key.PressFlg[6]) { Key.PressFlg[6] = 0; v_in=0; id_back[id_digit] = 6; id_digit++; }
       else if(Key.PressFlg[7]) { Key.PressFlg[7] = 0; v_in=0; id_back[id_digit] = 7; id_digit++; }
       else if(Key.PressFlg[8]) { Key.PressFlg[8] = 0; v_in=0; id_back[id_digit] = 8; id_digit++; }
       else if(Key.PressFlg[9]) { Key.PressFlg[9] = 0; v_in=0; id_back[id_digit] = 9; id_digit++; }      
       else if(Key.PressFlg[10]){ Key.PressFlg[10] = 0; if(v_alpha)v_alpha=0; else v_alpha = 1;   }
       else if(Key.PressFlg[11]){ Key.PressFlg[11] = 0; v_in=0; id_back[id_digit] = ' '; id_digit++; }
       else if(Key.PressFlg[12]){ Key.PressFlg[12] = 0; if(!id_digit) bbik(); else { v_in=0;  id_digit--; id_back[id_digit]=0xFF; }}
       else if(Key.PressFlg[13])
       { 
         Key.PressFlg[13] = 0; 
         if(id_digit) { for(i=0; i<20; i++) p_head[i]=id_back[i]; }
         else         { for(i=0; i<20; i++) p_head[i]=0xFF; }        
         Clear_Screen();
         break;
       }
     }
           
      if(v_in)
      {
        delay_timer++;
        if(v_alpha) mprintf(1, 6," WRITE ");
        if(delay_timer>1000){delay_timer=0; v_in=0; toggle=0; }
      }
      else mprintf(1, 6,"       ");
      if(v_alpha) mprintf(1, 7," ENGLISH ");
      else if(!v_alpha) mprintf(1, 7," NUMBER  ");
      
      if(!id_digit) mprintf(1, 4,"                      ");
      if(id_digit==9) mprintf(1, 5,"                      ");
      if(id_digit==1){if(id_back[0]<10)mprintf(20, 4,"%d  ",id_back[0]); else mprintf(20, 4,"%c  ",id_back[0]);}     
      else if(id_digit==2){if(id_back[1]<10)mprintf(26, 4,"%d  ",id_back[1]); else mprintf(26, 4,"%c  ",id_back[1]);}                  
      else if(id_digit==3){if(id_back[2]<10)mprintf(32, 4,"%d  ",id_back[2]); else mprintf(32, 4,"%c  ",id_back[2]);} 
      else if(id_digit==4){if(id_back[3]<10)mprintf(38, 4,"%d  ",id_back[3]); else mprintf(38, 4,"%c  ",id_back[3]);} 
      else if(id_digit==5){if(id_back[4]<10)mprintf(44, 4,"%d  ",id_back[4]); else mprintf(44, 4,"%c  ",id_back[4]);} 
      else if(id_digit==6){if(id_back[5]<10)mprintf(50, 4,"%d  ",id_back[5]); else mprintf(50, 4,"%c  ",id_back[5]);} 
      else if(id_digit==7){if(id_back[6]<10)mprintf(56, 4,"%d  ",id_back[6]); else mprintf(56, 4,"%c  ",id_back[6]);} 
      else if(id_digit==8){if(id_back[7]<10)mprintf(62, 4,"%d  ",id_back[7]); else mprintf(62, 4,"%c  ",id_back[7]);} 
      else if(id_digit==9){if(id_back[8]<10)mprintf(68, 4,"%d  ",id_back[8]); else mprintf(68, 4,"%c  ",id_back[8]);} 
      else if(id_digit==10){if(id_back[9]<10)mprintf(74, 4,"%d  ",id_back[9]); else mprintf(74, 4,"%c  ",id_back[9]);} 
      else if(id_digit==11){if(id_back[10]<10)mprintf(20, 5,"%d  ",id_back[10]); else mprintf(20, 5,"%c  ",id_back[10]);}     
      else if(id_digit==12){if(id_back[11]<10)mprintf(26, 5,"%d  ",id_back[11]); else mprintf(26, 5,"%c  ",id_back[11]);}                  
      else if(id_digit==13){if(id_back[12]<10)mprintf(32, 5,"%d  ",id_back[12]); else mprintf(32, 5,"%c  ",id_back[12]);} 
      else if(id_digit==14){if(id_back[13]<10)mprintf(38, 5,"%d  ",id_back[13]); else mprintf(38, 5,"%c  ",id_back[13]);} 
      else if(id_digit==15){if(id_back[14]<10)mprintf(44, 5,"%d  ",id_back[14]); else mprintf(44, 5,"%c  ",id_back[14]);} 
      else if(id_digit==16){if(id_back[15]<10)mprintf(50, 5,"%d  ",id_back[15]); else mprintf(50, 5,"%c  ",id_back[15]);} 
      else if(id_digit==17){if(id_back[16]<10)mprintf(56, 5,"%d  ",id_back[16]); else mprintf(56, 5,"%c  ",id_back[16]);} 
      else if(id_digit==18){if(id_back[17]<10)mprintf(62, 5,"%d  ",id_back[17]); else mprintf(62, 5,"%c  ",id_back[17]);} 
      else if(id_digit==19){if(id_back[18]<10)mprintf(68, 5,"%d  ",id_back[18]); else mprintf(68, 5,"%c  ",id_back[18]);} 
      else if(id_digit==20){if(id_back[19]<10)mprintf(74, 5,"%d  ",id_back[19]); else mprintf(74, 5,"%c  ",id_back[19]);} 
  }
	
}

void bbik(void)
{
  ShortBeep = 1;
  HAL_Delay(200);
  ShortBeep = 1;
  HAL_Delay(200);
  ShortBeep = 1;
  HAL_Delay(200);
}
