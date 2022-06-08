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
void print_up();

long aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b5,b6;
unsigned long b4,b7;
short ac1,ac2,ac3,oss=3;
unsigned short ac4;
long up,x1,x2,x3,p;


void main()
{
    LCD_init();
    bmp280_init();
    
    show("BMP280");
    __delay_ms(1000);
    while(1)
    {
        bmp280_getdata();
        print_checkdata();
        
        //Configure later
//        ac1 = ;
//        ac2 = ;
//        ac3 = ;
//        ac4 = ;
        

    }
}

void bmp280_getdata()
{
        aa=(bmp280_read_byte(0xAA));
        ab=(bmp280_read_byte(0xAB));
        ac=(bmp280_read_byte(0xAC));
        ad=(bmp280_read_byte(0xAD));
        ae=(bmp280_read_byte(0xAE));
        af=(bmp280_read_byte(0xAF));

        b0=(bmp280_read_byte(0xB0));
        b1=(bmp280_read_byte(0xB1));
        b2=(bmp280_read_byte(0xB2));
        b5=(bmp280_read_byte(0xB5));
}

void print_checkdata()
{
    LCD_Command(0xC0);
    LCD_Char(aa+0x30);
}

void print_up()
{
        bmp280_send_byte(0xf4,(0x34+(oss<<6)));
        __delay_ms(25);
        
        long ff6=(bmp280_read_byte(0xf6));
        long ff7=(bmp280_read_byte(0xf7));
        long ff8=(bmp280_read_byte(0xf8));
        up=(((ff6<<16)+(ff7<<8)+ff8)>>(8-oss));
}