/*
 * File:   main.c
 * Author: Naseef
 *
 * Created on 21 May, 2022, 7:47 PM
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
#define _XTAL_FREQ 20000000

#define rs RC2
#define en RC3
#define R1 RD0
#define R2 RD1
#define R3 RD2
#define R4 RD3
#define C1 RD4
#define C2 RD5
#define C3 RD6
#define C4 RD7

void LCD_init();
void LCD_cmd(unsigned char a);
void LCD_dat(unsigned char b);
void show(unsigned char *s);
unsigned char key();
void keyinit();
void fetch_sizes();
void lut();
int fn_winding_time(int a, int b, int c);
void LCD_num(unsigned int val);
void display_data();

unsigned char keypad[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};
unsigned char rowloc,colloc;
int coil_size, bobbin_size, no_turns, turn_time, winding_time;

void main()
{
    unsigned int i;
    TRISB=0;
    TRISC=0;
    PORTD = 0;
    LCD_init();
    keyinit();
    unsigned char b;
    
    while(1)
    {
        fetch_sizes();
        lut();
        winding_time = fn_winding_time(bobbin_size, turn_time, no_turns);
        //winding_time = (const int)winding_time;
        display_data();
        LCD_cmd(0x01);
        LCD_cmd(0x80);       
        show("Winding Starts");
        RB0 = 1; // Relay ON
        //__delay_ms(winding_time);
        __delay_ms(3000);
        RB0 = 0; //Relay OFF
        RB1 = 1; // Buzzer
        LCD_cmd(0x01);
        LCD_cmd(0x80);
        show("Winding Completes");
        LCD_cmd(0x94);
        show("Total Time Taken:");
        LCD_cmd(0xD4);
        LCD_num((winding_time/60));
        while(1);
    }
}

void LCD_init()
{
    LCD_cmd(0x02);
    LCD_cmd(0x28);
    LCD_cmd(0x0e);
    LCD_cmd(0x06);
    LCD_cmd(0x80);
}

void LCD_cmd(unsigned char cmd)
{
    rs=0; 
   __delay_ms(10);
   PORTC =(cmd);
   en=1;
   __delay_ms(10);
   en=0;
   __delay_ms(10);
   PORTC=(cmd<<4);
   rs =0;
   en =1;
   __delay_ms(10);
   en =0;
   __delay_ms(10);
}

void LCD_dat(unsigned char data)
{
    rs =1; 
   __delay_ms(10);
   PORTC =(data);
   en=1;
   __delay_ms(10);
   en=0;
   __delay_ms(10);
   PORTC=(data<<4);
   rs =1;
   en=1;
   __delay_ms(10);
   en=0;
   __delay_ms(10);
}

void show(unsigned char *s)
{
 while(*s) 
    {
        LCD_dat(*s++);
    }
}

void keyinit()
{
    TRISD=0XF0;
    PORTD=0X00;
}

unsigned char key()
{
    PORTD=0X00;
    while(C1&&C2&&C3&&C4);
        {
        while(!C1||!C2||!C3||!C4) 
            {
            __delay_ms(100); 

            R1=0;
            R2=R3=R4=1;
            if(!C1||!C2||!C3||!C4) 
                {
                    rowloc=0;
                    break;
                }
            R2=0;
            R1=R3=R4=1;
            if(!C1||!C2||!C3||!C4) 
                {
                    rowloc=1;
                    break;
                }
            R3=0;
            R2=R1=R4=1;
            if(!C1||!C2||!C3||!C4) 
                {
                    rowloc=2;
                    break;
                }
            R4=0;
            R2=R3=R1=1;
            if(!C1||!C2||!C3||!C4)
                {
                    rowloc=3;
                    break;
                }

            }
       }
    
    if(C1==0&&C2!=0&&C3!=0&&C4!=0)
    colloc=0;
    else if(C1!=0&&C2==0&&C3!=0&&C4!=0)
    colloc=1;
    else if(C1!=0&&C2!=0&&C3==0&&C4!=0)
    colloc=2;
    else if(C1!=0&&C2!=0&&C3!=0&&C4==0)
    colloc=3;
    while(C1==0||C2==0||C3==0||C4==0);
    
    return (keypad[rowloc][colloc]);
}

void fetch_sizes()
{
    int i;
    
    LCD_cmd(0x80);
    show("Enter the Coil Size:");
    for(i=0;i<2;i++)
    {
        coil_size = coil_size*10+key();
    }
    __delay_ms(1000);
    
    LCD_cmd(0x80);
    show("Enter the Bobbin Size:");
    for(i=0;i<1;i++)
    {
        bobbin_size = bobbin_size*10+key();
    }
    __delay_ms(1000);
    
    LCD_cmd(0x80);
    show("Enter the No. Turns:");
    for(i=0;i<2;i++)
    {
        no_turns = no_turns*10+key();
    }  
    __delay_ms(1000);
}

void lut()
{
    if (bobbin_size == 1)
    {
        turn_time = 100;
    }
    else if (bobbin_size == 2)
    {
        turn_time = 125;
    }
    else if (bobbin_size == 3)
    {
        turn_time = 150;
    }    
}

int fn_winding_time(int a, int b, int c)
{
    int d = a*b*c;
    return d;
}

void LCD_num (unsigned int val)
{
    int remainder, digit1, digit2, digit3, digit4, result, result1;
    if (val<=9)
    {
        LCD_dat (val+0x30);
    }
    else if (val>=10 && val <100)
    {
        remainder = val % 10;
        digit1 = remainder;
        digit2 = val/10;
        LCD_dat(digit2+0x30);
        LCD_dat(digit1+0x30);
    }
    else if (val>=100 && val <1000)
    {
        remainder = val % 10;
        result1 = val /10;
        digit1 = remainder;
        remainder = result1%10;
        digit2 = remainder;
        digit3 = result1/10;
        LCD_dat(digit3+0x30);
        LCD_dat(digit2+0x30);
        LCD_dat(digit1+0x30);
    }
    else if (val>=1000 && val <9999)
    {
        remainder = val % 10;
        result1 = val /10;
        digit1 = remainder;
        remainder = result1%10;
        digit2 = remainder;
        result = result1/10;
        remainder = result%10;
        digit3=remainder;
        digit4 = result/10;
        LCD_dat(digit4+0x30);
        LCD_dat(digit3+0x30);
        LCD_dat(digit2+0x30);
        LCD_dat(digit1+0x30);
    }
}

void display_data()
{
        LCD_cmd(0x01);
        LCD_cmd(0xC0);
        show("Coil Size: ");
        LCD_cmd(0xCB);
        LCD_num(coil_size);
        LCD_cmd(0x94);
        show("Bobbin Size: ");
        LCD_cmd(0x9F);
        LCD_num(bobbin_size);
        LCD_cmd(0xD4);
        show("No. Turns: ");
        LCD_cmd(0xDF);
        LCD_num(no_turns);
        __delay_ms(1000);
}