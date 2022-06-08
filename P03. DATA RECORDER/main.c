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
#include <math.h>
#include "lcd4bit.h"
#include "uart.h"
#include "dht11.h"
#include "ds1307.h"
#include "bmp280.h"

#define _XTAL_FREQ 20000000

void ADC_Init();
void print_serialnumber();
void print_analogvoltages();
void print_dht11data();
//RTC - DS1307
void rtc_getdata();
void rtc_lcd_data();
void rtc_terminal_data();
//BMP280
void bmp280_getdata();
void uncompensated_pressure();
void actual_pressure();
void calculate_altittude();
void bmp280_displaydata();
void bmp280_terminaldata();

unsigned int AV[4]; // to store 4 analog channel values (HERE AFTER REFERED AS POT values)
unsigned int sn=1; // declaration for serial number MAX=255;for more change the datatype
unsigned char message[3]; // array to store DTH11 output values
unsigned int humidity, temperature; // DTH11 variables

/*BMP280 Variables*/
long aa,ab,ac,ad,ae,af,b0,b1,b2,b3,b5,b6,up,x1,x2,x3,p;
unsigned long b4,b7;
short ac1,ac2,ac3,oss=3;
unsigned short ac4;
unsigned int hpa, altittude;
/*BMP280 Variable Ends*/

void main() 
{
    LCD_init();             //LCD Initialization
    ser_int();              //UART Initialization
    while (1)
    {
        LCD_Command(0x01); //clear LCD display after first a set of POT values

        /*---------------------SERIAL NUMBER--------------------------*/
        print_serialnumber();
        /*---------------------RTC BEGINS--------------------------*/
        rtc_int();
        rtc_getdata();
        rtc_lcd_data();
        rtc_terminal_data();
        /*---------------------RTC COMPLETES--------------------------*/
        /*---------------------POT BEGINS----------------------------*/
        ADC_Init ();       //ADC Initialization
        GO_nDONE=1;
        while(GO_nDONE==1);
        __delay_ms(10);
        LCD_Command(0xC0);
        print_analogvoltages();
        /*---------------------POT COMPLETES----------------------------*/

        /*---------------------DHT11 BEGINS----------------------------*/
        print_dht11data();
        /*---------------------DHT11 COMPLETES----------------------------*/
        
        /*---------------------BMP280 BEGINS----------------------------*/
        bmp280_init();  //BMP280 I2C Initialization 
        bmp280_getdata();  // Reading Registers for data
        uncompensated_pressure(); // Calculating uncompensated pressure
        
        ac1 = (aa<<8) + ab;
        ac2 = (ac<<8) + ad;
        ac3 = (ae<<8) + af;
        ac4 = (b0<<8) + b1;
        
        actual_pressure();
        //calculate_altittude();
        
        bmp280_displaydata();
        bmp280_terminaldata();
        /*---------------------BMP280 COMPLETES----------------------------*/
        
        tx(0x0d); // new after printing a set of values in virtual terminal
        __delay_ms(250); // delay for 1 second for RECORDING VALUES IN EVERY ONE SECOND
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

void print_serialnumber()
{
    tx_sn(sn); // fn for transmitting serial number to virtual terminal
    tx(')'); // ")" after every serial number    
}

void print_analogvoltages()
{
    for (unsigned char i=0;i<4;i++)
        {
            LCD_num(AV[i]);   //print POT values in LCD && SENDING POT values to split its digits
            LCD_Char(' ');   //space between two POT values
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
}

void print_dht11data()
{
        StartSignal();
        CheckResponse();
        if(Check == 1)
        {
            RH_byte1 = ReadData();
            RH_byte2 = ReadData();
            T_byte1 = ReadData();
            T_byte2 = ReadData();
            Sum = ReadData();
            if(Sum == ((RH_byte1+RH_byte2+T_byte1+T_byte2) & 0XFF))
            {
                Temp = T_byte1;
                RH = RH_byte1;
            }
        }        
        LCD_Command(0x94);
        show("Temp:");
        LCD_Command(0x99);
        show_multidigits (Temp);
        tx_sn(Temp);
        tx(',');
        LCD_Command(0x9D);
        show("Humi:");
        LCD_Command(0xA2);
        show_multidigits(RH);
        tx_sn(RH);
        tx(',');
}

void rtc_getdata()
{
    sec  =rtc_read_byte(0x00);
    min  =rtc_read_byte(0x01);
    hour =rtc_read_byte(0x02);
    day  =rtc_read_byte(0x03);
    date =rtc_read_byte(0x04);
    month=rtc_read_byte(0x05);
    year =rtc_read_byte(0x06);
}
void rtc_lcd_data()
{
        LCD_Command(0x80);
        LCD_Char(convup(hour));
        LCD_Char(convd(hour));
        LCD_Char(':');
        LCD_Char(convup(min));
        LCD_Char(convd(min));
        LCD_Char(':');
        LCD_Char(convup(sec));
        LCD_Char(convd(sec));
        
        LCD_Command(0x89);
        LCD_Char(convup(date));
        LCD_Char(convd(date));
        LCD_Char(':');
        LCD_Char(convup(month));
        LCD_Char(convd(month));
        LCD_Char(':');
        LCD_Char(convup(year));
        LCD_Char(convd(year));
        LCD_Char('/');
        LCD_Char(convup(day));
        LCD_Char(convd(day));
}
void rtc_terminal_data()
{
        tx(convup(hour));
        tx(convd(hour));
        tx(':');
        tx(convup(min));
        tx(convd(min));
        tx(':');
        tx(convup(sec));
        tx(convd(sec));
        
        tx(','); //to separate between time and date
        
        tx(convup(date));
        tx(convd(date));
        tx(':');
        tx(convup(month));
        tx(convd(month));
        tx(':');
        tx(convup(year));
        tx(convd(year));
        
        tx(','); //to separate between date and day
        tx(convup(day));
        tx(convd(day));  
        
        tx(',');  //to separate between day and first analog voltage
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
void actual_pressure()
{
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
        
        hpa = p/100; //pressure in hectopascal    
}
void calculate_altittude()
{
    altittude =  44330 * (1-(pow((hpa/1013.25),(1/5.255))));
}
void bmp280_displaydata()
{
    LCD_Command(0xD4);
    show("hPa:");
    LCD_Command(0xD9);
    show_multidigits(hpa);
    
    LCD_Command(0xDE);
    show("Alt:");
    LCD_Command(0xE3);
    show_multidigits(altittude);
}
void bmp280_terminaldata()
{
    tx_sn(hpa);
    tx_sn(altittude); 
}