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
void uncompensated_pressure();

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
        uncompensated_pressure();
        
        ac1 = aa + ab;
        ac2 = ac + ad;
        ac3 = ae + af;
        ac4 = b0 + b1;
        
        b6 = b5 - 4000;
        x1 = (b2*(b6*b6/4096))/2048;
        x2 = ac2*b6/2048;
        x3 = x1+x2;
        b3 = (((ac1*4+x3)<< oss)+ 2)/ 4; 
        x1 = ac3* b6 / 8192; 
        x2 = (b1 * (b6 *b6 / 4096)) / 65536;
        x3 =((x1+x2)+2)/4;
        b4 = ac4 * (unsigned long) (x3 + 32768)/ 32768;
        b7 = ((unsigned long)up - b3) * (50000 >> oss); 
        if (b7 < 0x80000000) 
        { 
            p = (b7* 2)/ b4;
        } 
        else 
        { 
            p = (b7 / b4)* 2;
        }
        x1 =(p/256)*(p/256);
        x1 = (x1 * 3038)/65536;
        x2 = (-7357 * p) / 65536;
        p=p+(x1+x2+3791)/16; // pressure in pascal
        unsigned int hpa = p/1000; //pressure in hectopascal
        
         LCD_Command(0xC0);
         show_multidigits(hpa);
        
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

void uncompensated_pressure()
{
        bmp280_send_byte(0xf4,(0x34+(oss<<6)));
        __delay_ms(25);
        
        long ff6=(bmp280_read_byte(0xf6));
        long ff7=(bmp280_read_byte(0xf7));
        long ff8=(bmp280_read_byte(0xf8));
        up=(((ff6<<16)+(ff7<<8)+ff8)>>(8-oss));
}