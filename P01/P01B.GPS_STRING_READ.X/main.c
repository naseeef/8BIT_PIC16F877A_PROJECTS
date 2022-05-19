/*
 * File:   main.c
 * Author: Naseef
 *
 * Created on 19 May, 2022, 1:33 PM
 */

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 20000000

LCD_command(unsigned char cmd);
LCD_data(unsigned char data);
LCD_init();
unsigned char rx();
void ser_int();
void show(unsigned char *s);
void read_data();
void print_data();
void reset_dispaly();


unsigned char r[3], pv[3]="GPR";
int f, checkgps, h;
char a[95];
int i;

void main()
{
TRISD=0x00;
TRISC4 = TRISC5 = 0;
ser_int();
       
read_data();
LCD_init();
show("Connecting...");
__delay_ms(1000);
print_data();
__delay_ms(300);
reset_dispaly();
}

LCD_command(unsigned char cmd)
{
    PORTD = cmd;
    RC4 = 0;
    RC5 = 1;
    __delay_ms(10);
    RC5 = 0;
}
LCD_data(unsigned char data)
{
    PORTD = data;
    RC4 = 1;
    RC5 = 1;
    __delay_ms(10);
    RC5 = 0;
 }
LCD_init()
{
    LCD_command(0x38);        // enable 8 bit mode  chars 
    LCD_command(0x0E);       
    LCD_command(0x01);        // Clear Display
    LCD_command(0x06);
    LCD_command(0x80);        // Move the curs
}
unsigned char rx()
{
 while(!RCIF);
 RCIF=0;
 return RCREG;
}
void ser_int()
{
 RCSTA= 0b10010000;
 SPBRG=31;
}
void show(unsigned char *s)
{
  while(*s)
  {
    LCD_data(*s++);
  }
}
void read_data()
{
    for (int j=0; j<95;j++)
{
char gps = rx();
if (gps == '$')
{
    //RCIF=0;
    for(f=0;f<3;f++) {
        while(!RCIF);
        r[f]=RCREG;
      }
        r[f]='\0';
        
        //checkgps=strcmp(r,pv);
        
        for (f=0;pv[f]!='\0';f++)
        {
            if(r[f]!=pv[f])
            {
                checkgps=1;
                LCD_data('X');
                break;
            }
        }
        
         if(checkgps==0) {
             for(i=0;i<68;i++)
            {
                a[i]=rx();
            }
         a[i]='\0';
         break;
        }
}
}
}
void print_data()
{
    LCD_command(0x01);
    LCD_command(0x80);
    show("LON:");
    
        LCD_command(0x84);
        for(h=16;h<18;h++) {
          LCD_data(a[h]);
        }
        LCD_data('.');
        for(h=18;h<20;h++) {
          LCD_data(a[h]);
        }
        for(h=21;h<25;h++) {
          LCD_data(a[h]);
        }
        LCD_data(223);
        LCD_data(' ');
        LCD_data('N');
      
LCD_command(0xc0);
        show("LAT:");
        
        LCD_command(0xc4);
        for(h=29;h<31;h++) {
          LCD_data(a[h]);
        }
        LCD_data('.');
        for(h=31;h<33;h++) {
          LCD_data(a[h]);
        }
        for(h=34;h<38;h++) {
          LCD_data(a[h]);
        }
        LCD_data(223);
        LCD_data(' ');
        LCD_data('E');
}
void reset_dispaly()
{
    LCD_command(0x94);
    show("PRESS PUSH BUTTON");
    LCD_command(0xD4);
    show("FOR RESETTING DEVICE");
}
