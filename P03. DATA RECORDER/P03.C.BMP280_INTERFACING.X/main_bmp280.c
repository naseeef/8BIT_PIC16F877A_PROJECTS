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

void bmp280_getdata();
void print_checkdata();

long AA, AB, AC, AD, AE, AF, B0, B1, B2,B3, B5, 
        B6, B8, B9, BA, BB, BC, BD, BE;// BF;
unsigned long B4, B7;
short AC1, AC2, AC3, MB, MC, MD, oss;
unsigned short AC4, AC5, AC6;
long UT, UP, X1, X2, T, P;



void main()
{
    LCD_init();
    i2c_init();
    
    show("BMP280");
    __delay_ms(1000);
    while(1)
    {
        bmp280_getdata();
        print_checkdata();
    }
}

void bmp280_getdata()
{
        AA=(i2c_read_byte(0xAA));
        AB=(i2c_read_byte(0xAB));
        AC=(i2c_read_byte(0xAC));
        AD=(i2c_read_byte(0xAD));
        AE=(i2c_read_byte(0xAE));
        AF=(i2c_read_byte(0xAF));

        B0=(i2c_read_byte(0xB0));
        B1=(i2c_read_byte(0xB1));
        B2=(i2c_read_byte(0xB2));
        B3=(i2c_read_byte(0xB3));
        B4=(i2c_read_byte(0xB4));
        B5=(i2c_read_byte(0xB5));
        B6=(i2c_read_byte(0xB6));
        B7=(i2c_read_byte(0xB7));
        B8=(i2c_read_byte(0xB8));
        B9=(i2c_read_byte(0xB9));
        BA=(i2c_read_byte(0xBA));
        BB=(i2c_read_byte(0xBB));
        BC=(i2c_read_byte(0xBC));
        BD=(i2c_read_byte(0xBD));
        BE=(i2c_read_byte(0xBE));
        BF=(i2c_read_byte(0xBF));
}

void print_checkdata()
{
    LCD_Command(0xC0);
    
    LCD_Char(AA+0x30);
    LCD_Char(AB+0x30);
    LCD_Char(AC+0x30);
    LCD_Char(AD+0x30);
    LCD_Char(AE+0x30);
    LCD_Char(AF+0x30);
    
    LCD_Command(0x94);
    LCD_Char(B0+0x30);
    LCD_Char(B1+0x30);
    LCD_Char(B2+0x30);
    LCD_Char(B3+0x30);
    
}