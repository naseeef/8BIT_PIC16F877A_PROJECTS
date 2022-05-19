/*
 * File:   main.c
 * Author: Naseef
 *
 * Created on 18 May, 2022, 10:31 PM
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
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 20000000
#define delay for(int i=0;i<1000;i++)

char namegps[7], name1gps[7] = "PS",gpsdat[7];
char msggps ; 
char checkgps;
int h;
unsigned char f;

LCD_command(unsigned char cmd)
{
    PORTD = cmd;
    RC0 = 0;
    
    RC1 = 1;
    delay;
    RC1 = 0;
}
LCD_data(unsigned char data)
{
    PORTD = data;
    RC0 = 1;

    RC1 = 1;
    delay;
    RC1 = 0;
 }
LCD_init()
{
    TRISC0=0;
    TRISC1=0;
    TRISD=0;
    LCD_command(0x38);        // enable 8 bit mode  chars 
    LCD_command(0x0e);       
    LCD_command(0x06);        
    LCD_command(0x80);        
}
void show(unsigned char *s)
{
  while(*s)
  {
    LCD_data(*s++);
  }
}

void serial_init()
{
  //TRISC6=TRISC7=1;
  //TXSTA=0b00100010;
  RCSTA=0b10010000;
  SPBRG=31;
  //TXIF=0;
  RCIF=0;
}

void interrupt ser()
{
  if(RCIF) 
  {
    msggps=RCREG;
    
    if(msggps=='G')
    {
        RC2 = 1;
      RCIE=0;
      RCIF=0;
 
      for(f=0;f<=1;f++) {
        while(!RCIF);
        namegps[f]=RCREG;
      }
        checkgps=strcmp(namegps,name1gps);
         if(checkgps==0) {
            RC3 = 1;
           for(f=0;f<=6;f++) {
            while(!RCIF);
            gpsdat[f]=RCREG;      
        }
 
        LCD_command(0xc4);
        for(h=1;h<=6;h++) {
          LCD_data(gpsdat[h]);
        }
      }
      RCIF=0;
    }  
  }
}  

void main()
{
    
    TRISD=0;
    TRISC2 = 0, TRISC3=0, TRISC4=0;
    INTCON=0b11000000;
    PIE1=0b00100000;
    LCD_init();
    LCD_command(0x80);
    show("LON:");
    LCD_command(0xc0);
    show("LAT:");
     
    while(1){
        serial_init();
        __delay_ms(500);
    }
}