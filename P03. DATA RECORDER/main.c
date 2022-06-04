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

unsigned int AV[4]; // to store 4 analog channel values (HERE AFTER REFERED AS POT values)
unsigned int sn=1; // declaration for serial number MAX=255;for more change the datatype
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
       
        tx_sn(sn); // fn for transmitting serial number to virtual terminal
        tx(')'); // ")" after every serial number
        for (unsigned char i=0;i<4;i++)
        {
            LCD_num(AV[i]);   //print POT values in LCD && SENDING POT values to split its digits
            LCD_Char(" ");   //space between two POT values
            for (unsigned char j=0;j<3;j++)
            {
                tx((avv[j]+0x30)); //SPLITED POT value digits in avv[] && print POT values in virtual terminal
                if (j == 0)
                {
                    tx('.'); // "." between volt and millivolt in virtual terminal
                }
                else if (j == 2)
                {
                    tx(','); // "," between two POT voltages in virtual terminal
                }
            }
            // HERE j for loop completes printing one POT value in virtual terminal
            __delay_ms(100);
        }
        // HERE i for loop completes printing 4 POT values in LCD
        tx(0x0d); // new after printing a set of values in virtual terminal
        LCD_Command(0x01); //clear LCD display after first a set of POT values
        __delay_ms(1000); // delay for 1 second for RECORDING VALUES IN EVERY ONE SECOND
        sn += 1; // increment serial number after every set of reading
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