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
#include <stdlib.h>
#include <time.h>
#define _XTAL_FREQ 20000000

void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_init();
unsigned char rx();
void ser_int();
void show(unsigned char *s);
void read_data();
void print_data();
void refresh_text();
void corner_cases(int number);
void cloudy();
void low_battery();
int printRandoms(int lower, int upper, int count);
int random_number(int l, int u, int c);

unsigned char r[3], pv[3]="GPR";
char a[95];
int random_case, reset_counter = 0;

void main()
{
/*DO NOT EDIT BELOW STATEMENTS*/
TRISD=0x00;
TRISB=0x00;
TRISC3 = 1;
TRISC4 = TRISC5 = 0;
ser_int();
LCD_init();
read_data();
/*DO NOT EDIT ABOVE STATEMENTS*/

    show("WELCOME...");
    __delay_ms(200);
    LCD_command(0x01);
    A:
    LCD_command(0x80);
    show("Connecting...");
    __delay_ms(500);

    /*random_case = random_number(0, 1, 1);
    corner_cases(random_case);*/
    
//print_data();
corner_cases(1);
int seven[11] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xC0};

    while(1)
    {
        if (RC3 == 1)
        {
            reset_counter += 1;
            PORTB = seven[reset_counter];
            while(reset_counter <= 10)
            {
                LCD_command(0x01);
                LCD_command(0x80);
                show("Resetting...");
                __delay_ms(200);
                LCD_command(0x01);
                goto A;   
            }
            low_battery();
            //return 0;
        }
    }
}

void LCD_command(unsigned char cmd)
{
    PORTD = cmd;
    RC4 = 0;
    RC5 = 1;
    __delay_ms(10);
    RC5 = 0;
}
void LCD_data(unsigned char data)
{
    PORTD = data;
    RC4 = 1;
    RC5 = 1;
    __delay_ms(10);
    RC5 = 0;
 }
void LCD_init()
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
    int i, f, flag=0;
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
        
        //flag=strcmp(r,pv);
        
        for (f=0;pv[f]!='\0';f++)
        {
            if(r[f]!=pv[f])
            {
                flag += 1;
                LCD_data('X');
                break;
            }
        }
        
         if(flag==0) {
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
    int h;
    
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
    
    refresh_text();
}
void refresh_text()
{
    LCD_command(0x94);
    show("PRESS PUSH BUTTON");
    LCD_command(0xD4);
    show("FOR RESTTING DEVICE");
}
void corner_cases(int number)
{
    switch(number)
    {
        case 0:
            cloudy();
            break;
        case 1:
            print_data();
            break;
    }
}
void cloudy()
{
    LCD_command(0x80);
    show("!CLOUDY ENVIRONMENT!");
    LCD_command(0xC0);
    show("!!CAN'T FETCH DATA!!");
    __delay_ms(250);
    LCD_command(0x94);
    show(" ");
    LCD_command(0xD4);
    show("---TRY  RESETTING---"); 
}
void low_battery()
{
    LCD_command(0x01);
    LCD_command(0x80);
    show("-!!!BATTERY LOW!!!-");
    __delay_ms(100);
    LCD_command(0xC0);
    show("-!!!BATTERY LOW!!!-");
    LCD_command(0xD4);
    show("RECHARGE IMMEDIATELY");
    __delay_ms(300);
    LCD_command(0x01);
    LCD_command(0xC0);
    show("SWITCHING OFF");
    __delay_ms(100);
    show(".");
    __delay_ms(100);
    show(".");
    __delay_ms(100);
    show(".");
    __delay_ms(500);
    LCD_command(0x01);
    while(1);
}
int printRandoms(int lower, int upper, int count)
{
    int z;
    for (z = 0; z < count; z++) {
        int num = (rand() % (upper - lower + 1)) + lower;
        return num;
    }
}
int random_number(int l, int u, int c)
{
    int lower = l, upper = u, count = c, rn;
  
    // Use current time as 
    // seed for random generator
    srand(time(0));
  
    rn = printRandoms(lower, upper, count);
  
    return rn;
}