/*
 * File:   main.c
 * Author: Naseef
 *
 * Created on 3 June, 2022, 2:31 PM
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
#include "lcd4bit.h"
#include "uart.h"
#define _XTAL_FREQ 20000000

void ADC_Init();
//void display_analog(unsigned int AC0,unsigned int AC1,unsigned int AC2,unsigned int AC3);

unsigned int AV[4];

void main() 
{
    TRISB =0x00;
    TRISC =0x00;
    
    LCD_init();                                     //LCD Initialization
    ser_int();                                      //UART Initialization
    while (1)
    {
        ADC_Init ();                                    //ADC Initialization
        GO_nDONE=1;
        while(GO_nDONE==1);
        __delay_ms(10);
        
        for (int i=0;i<4;i++)
        {
            LCD_num(AV[i]);
            tx('a');
            __delay_ms(300);
        }
        LCD_Command(0x01);
        __delay_ms(800);
    }
}

void ADC_Init ()
{
    ADCON0 = 0x85; //Channel 0 selected, FOSC/64 selected, ADON = 1; GO/DONE = 1;
    ADCON1 = 0xC0; // Right Justified, ADCS2 = 1.
    AV[0]= ((ADRESH<<8)+ADRESL);
   
    __delay_ms(10);
    ADCON0 = 0x8D;
    ADCON1 = 0xC0; // Right Justified, ADCS2 = 1.
    AV[1]= ((ADRESH<<8)+ADRESL);
   
    __delay_ms(10);
    ADCON0 = 0x95;
    ADCON1 = 0xC0; // Right Justified, ADCS2 = 1.
    AV[2]= ((ADRESH<<8)+ADRESL);
   
    __delay_ms(10);
    ADCON0 = 0x9D;
    ADCON1 = 0xC0; // Right Justified, ADCS2 = 1.
    AV[3]= ((ADRESH<<8)+ADRESL);
    
}

//void display_analog(unsigned int AC0,unsigned int AC1,unsigned int AC2,unsigned int AC3)
/*{

        LCD_Command (0xC0);
        LCD_num(AC0);
        LCD_Command (0xC5);
        LCD_num(AC0);
        LCD_Command (0xCA);
        LCD_num(AC0);
        LCD_Command (0xCF);
        LCD_num(AC0);
}*/