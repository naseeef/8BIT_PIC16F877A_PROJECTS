/*
 * File:   main_bmp280.c
 * Author: Naseef
 *
 * Created on 7 June, 2022, 10:59 AM
 */

#include <xc.h>
#include "p03a_bmp280.h"
#include "lcd4bit.h"

#define _XTAL_FREQ 20000000

long AA, AB, AC, AD, AE, AF, B0, B1, B2,B3, B5, 
        B6, B8, B9, BA, BB, BC, BD, BE; // BF
short AC1, AC2, AC3, MB, MC, MD, oss;
unsigned short AC4, AC5, AC6;
unsigned long B4, B7;
long UT, UP, X1, X2, T, P;



void main()
{
    LCD_init();
    i2c_init();
    
    show("BMP280");
    __delay_ms(1000);
    
    LCD_Command(0xc0);
    
    (i2c_read_byte(0x0000));

   
    while(1);
}
