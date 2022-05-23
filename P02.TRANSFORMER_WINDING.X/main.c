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

//https://www.youtube.com/watch?v=7odL_O1rgBQ

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
void lookuptable();
void LCD_num(unsigned int val);
void display_data();
void attach_sizes();
void winiding_time_check();
void rotation();
void result_display();

unsigned char keypad[4][4]={{'7','8','9','/'},{'4','5','6','*'},{'1','2','3','-'},{'C','0','=','+'}};
unsigned char rowloc,colloc;
//unsigned char coil_size, bobbin_size; 
unsigned char no_turns, turn_time;
unsigned char b1,b2,c1,c2,n1,n2; // from fetch_sizes()
unsigned char bs, cs, nt; // for attach_sizes()
unsigned char winding_time;


struct lut { unsigned char b_s; unsigned char c_s; unsigned char n_t; unsigned char t_t;}; 
//b_s=bobbin size, c_s=coil size, n_t=no. of turns, t_t= turn time

void main()
{
    TRISB=0;
    TRISC=0;
    LCD_init();
    keyinit();

    while(1)
    {
        fetch_sizes();
        lookuptable();
        attach_sizes();
        display_data();
        winding_time = nt*turn_time;
        winiding_time_check();
        
    unsigned char winding_time0  = winding_time;
    while(winding_time0 != 0)
    {
        unsigned char wt = winding_time0 % 10;
        LCD_dat(wt);
        winding_time0 = winding_time0 / 10;
    }
    while(1);
        
//        if (winding_time > 0)
//        {
//            rotation();
//            result_display();
//        }
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
    LCD_cmd(0x01);
    LCD_cmd(0x80);
    show("Enter the Bobbin Size:");
        b1=key();
        LCD_dat(b1);
        b2=key();
        LCD_dat(b2);
    __delay_ms(100);
    
    LCD_cmd(0x01);
    LCD_cmd(0x80);
    show("Enter the Coil Size:");
        c1=key();
        LCD_dat(c1);
        c2=key();
        LCD_dat(c2);
    __delay_ms(100);
    
    LCD_cmd(0x01);
    LCD_cmd(0x80);
    show("Enter the No. Turns:");
        n1=key();
        LCD_dat(n1);
        n2=key();
        LCD_dat(n2);
    __delay_ms(100);
}

void lookuptable()
{
    
    /*struct lut lut1 = {17, 10, 2.4, 1.0};
    struct lut lut2 = {17, 15, 2.0, 0.8};
    struct lut lut3 = {17, 20, 1.6, 0.6};
    struct lut lut4 = {23, 10, 4.5, 1.8};
    struct lut lut5 = {23, 15, 3.7, 1.5};
    struct lut lut6 = {23, 20, 3.0, 1.2};
    struct lut lut7 = {45, 10, 8.7, 3.5};
    struct lut lut8 = {45, 15, 7.2, 3.1};
    struct lut lut9 = {45, 20, 5.8, 2.7};
    if (bobbin_size == 17 && coil_size == 10)
    {
         no_turns = lut1.n_t;
         turn_time = lut1.t_t;
    }*/
        
    if (b1 == '1')
    {
        if (c1 == '1')
            {
                if (c2 == '0')
                {
                    turn_time = '9';
                }
                else
                {
                    turn_time = '8';
                }
            }
        else
            {
                turn_time = '7';
            }
    }
    else if (b1 == '2')
    {
        if (c1 == '1')
            {
                if (c2 == '0')
                {
                    turn_time = '6';
                }
                else
                {
                    turn_time = '5';
                }
            }
        else
            {
                turn_time = '4';
            }
    }
    else if (b1 == '4')
    {
        if (c1 == '1')
            {
                if (c2 == '0')
                {
                    turn_time = '7';
                }
                else
                {
                    turn_time = '2';
                }
            }
        else
            {
                turn_time = '1';
            }
    }
    else
    {
        LCD_cmd(0x01);
        LCD_cmd(0xC0);
        show("Invalid Data Input!!");  
    }
        
}

void display_data()
{
        LCD_cmd(0x01);
        LCD_cmd(0x80);
        show("Entered Data: ");
        
        LCD_cmd(0xC0);
        show("Bobbin Size: ");
        LCD_cmd(0xCC);
        LCD_dat(b1);
        LCD_dat(b2);
        
        
        LCD_cmd(0x94);
        show("Coil Size: ");
        LCD_cmd(0xA0);
        LCD_dat(c1);
        LCD_dat(c2);
        
        LCD_cmd(0xD4);
        show("Total No. Turns: ");
        LCD_cmd(0xE4);
        LCD_dat(n1);
        LCD_dat(n2);
        
        __delay_ms(1000);
         
}

void attach_sizes()
{
    bs = b1*10 + b2;
    cs = c1*10 + c2;
    nt = n1*10 + n2; 
}

void winiding_time_check()
{
        if (winding_time > 0)
        {
            RB2 = 1;
        }
}

void rotation()
{
    LCD_cmd(0x01);
    LCD_cmd(0x80);
    show("Winding Starts");
    RB4 = 1; // Relay ON
    for (int i=0; i<winding_time; i++)
    {
        __delay_ms(1000);
    }
    RB1 = 1; // Buzzer
    RB4 = 0; //Relay OFF
}

void result_display()
{
    LCD_cmd(0x01);
    LCD_cmd(0x80);
    show("Winding Completes");
    LCD_cmd(0x94);
    show("Total Time Taken:");
    LCD_cmd(0xD4);
    
    unsigned char winding_time0  = winding_time;
    while(winding_time0 != 0)
    {
        unsigned char wt = winding_time0 % 10;
        LCD_dat(wt);
        winding_time0 = winding_time0 / 10;
    }
    
}