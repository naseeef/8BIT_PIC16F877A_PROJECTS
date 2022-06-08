/*
 * File:   main_bmp280.c
 * Author: Naseef
 *
 * Created on 7 June, 2022, 10:59 AM
 */

#include <xc.h>
#include "p03a_bmp280.h"
#include "lcd4bit.h"

#define _XTAL_FREQ 12000000

void bmp280_getdata();
void print_checkdata();

long aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b5,b6,b8,b9,ba,bb,bc,bd,be,bf;
unsigned long b4,b7;
short ac1,ac2,ac3,mb,mc,md,oss=3;
unsigned short ac4,ac5,ac6;
long ut,up,x1,x2,p,t;


void main()
{
    LCD_init();
    i2c_init();
    
    show("BMP280");
    __delay_ms(1000);
    while(1)
    {
        //bmp280_getdata();
        //print_checkdata();
        
        
        i2c_send_byte(0xf4,(0x34+(oss<<6)));
        __delay_ms(25);
        
        long ff6=(i2c_read_byte(0xf6));
        long ff7=(i2c_read_byte(0xf7));
        long ff8=(i2c_read_byte(0xf8));
        up=(((ff6<<16)+(ff7<<8)+ff8)>>(8-oss));
        
        LCD_Command(0xC0);
        //LCD_Char((up)+0x30);
        
        show_multidigits((up/100));
        show_multidigits((up%100));
//        show_multidigits((up%10));
        __delay_ms(1000);

    }
}

void bmp280_getdata()
{
        aa=(i2c_read_byte(0xAA));
        ab=(i2c_read_byte(0xAB));
        ac=(i2c_read_byte(0xAC));
        ad=(i2c_read_byte(0xAD));
        ae=(i2c_read_byte(0xAE));
        af=(i2c_read_byte(0xAF));

        b0=(i2c_read_byte(0xB0));
        b1=(i2c_read_byte(0xB1));
        b2=(i2c_read_byte(0xB2));
        b3=(i2c_read_byte(0xB3));
        b4=(i2c_read_byte(0xB4));
        b5=(i2c_read_byte(0xB5));
        b6=(i2c_read_byte(0xB6));
        b7=(i2c_read_byte(0xB7));
        b8=(i2c_read_byte(0xB8));
        b9=(i2c_read_byte(0xB9));
        ba=(i2c_read_byte(0xBA));
        bb=(i2c_read_byte(0xBB));
        bc=(i2c_read_byte(0xBC));
        bd=(i2c_read_byte(0xBD));
        be=(i2c_read_byte(0xBE));
        bf=(i2c_read_byte(0xBF));
}

void print_checkdata()
{
    LCD_Command(0xC0);
    LCD_Char(aa+0x30);
    LCD_Char(ab+0x30);
    LCD_Char(ac+0x30);
    LCD_Char(ad+0x30);
    LCD_Char(ae+0x30);
    LCD_Char(af+0x30);
    
    LCD_Command(0x94);
    LCD_Char(b0+0x30);
    LCD_Char(b1+0x30);
    LCD_Char(b2+0x30);
    LCD_Char(b3+0x30);
    
}