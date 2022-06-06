/*
 * File:   mian_ds1307.c
 * Author: Naseef
 *
 * Created on 6 June, 2022, 9:26 PM
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
#define _XTAL_FREQ 12000000

unsigned char sec,min,hour,day,date,month,year;
void rtc_int();
void rtc_start();
void rtc_stop();
void rtc_ack();
void rtc_nak();
void rtc_res();
void rtc_send(unsigned char a);
void rtc_send_byte(unsigned char addr,unsigned char data);
unsigned char rtc_read();
unsigned char rtc_read_byte(unsigned char addr);
void waitmssp();
unsigned char convup(unsigned char bcd);
unsigned char convd(unsigned char bcd);

void LCD_init();
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void show(unsigned char *s);

void main()
{
    LCD_init();
    
    show("Time:");
    LCD_Command(0xc0);
    show("Date:");
    rtc_int();
    while(1) {
        sec  =rtc_read_byte(0x00);
        min  =rtc_read_byte(0x01);
        hour =rtc_read_byte(0x02);
        day  =rtc_read_byte(0x03);
        date =rtc_read_byte(0x04);
        month=rtc_read_byte(0x05);
        year =rtc_read_byte(0x06);
        
        LCD_Command(0x85);
        LCD_Char(convup(hour));
        LCD_Char(convd(hour));
        LCD_Char(':');
        LCD_Char(convup(min));
        LCD_Char(convd(min));
        LCD_Char(':');
        LCD_Char(convup(sec));
        LCD_Char(convd(sec));
        
        LCD_Char(0xc5);
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
}       
    
void rtc_int()
{
    TRISC3=TRISC4=1;
    SSPCON=0x28;
    SSPADD= (((11059200/4)/100)-1);
}
void waitmssp()
{
    while(!SSPIF); // SSPIF is zero while TXion is progress    
    SSPIF=0;
}
void rtc_send(unsigned char a)
{
    SSPBUF=a;
    waitmssp();
    while(ACKSTAT);
}   
void rtc_send_byte(unsigned char addr,unsigned char data)
{
    rtc_start();
    rtc_send(0xD0);
    //rtc_send(addr>>8);
    rtc_send(addr);
    rtc_send(data);
    rtc_stop();
}
unsigned char rtc_read()
{
    RCEN=1;
    waitmssp();
    return SSPBUF;
}   
    
unsigned char rtc_read_byte(unsigned char addr)
{
    unsigned char rec;
L:  rtc_res();
    SSPBUF=0xD0;
    waitmssp();
    while(ACKSTAT){goto L;}
    //rtc_send(addr>>8);
    rtc_send(addr);
    rtc_res();
    rtc_send(0xD1);
    rec=rtc_read();
    rtc_nak();
    rtc_stop();
    return rec;
}
    
    
void rtc_start()
{
    SEN=1;
    waitmssp();
}
void rtc_stop()
{
    PEN=1;
    waitmssp();
}
void rtc_res()
{
    RSEN=1;
    waitmssp();
}
void rtc_ack()
{
    ACKDT=0;
    ACKEN=1;
    waitmssp();
}
void rtc_nak()
{
    ACKDT=1;
    ACKEN=1;
    waitmssp();
}
unsigned char convup(unsigned char bcd)
{ 
    return ((bcd>>4)+48);
}
unsigned char convd(unsigned char bcd)
{ 
    return ((bcd&0x0F)+48);
}


void LCD_Command (unsigned char cmnd)
{
    RC0 = 0;
    PORTB = cmnd;
    RC1 = 1;
    __delay_ms(10);
    RC1 = 0;
    __delay_ms(10);
    PORTB = (cmnd<<4);
    RC1 = 1;
    __delay_ms(10);
    RC1 = 0;
    __delay_ms(10);
}
void LCD_Char (unsigned char data)
{
    RC0 = 1;
    PORTB = data;
    RC1 = 1;
    __delay_ms(10);
    RC1 = 0;
    __delay_ms(10);
    PORTB = (data<<4);
    RC1 = 1;
    __delay_ms(10);
    RC1 = 0;
    __delay_ms(10);
}
void LCD_init (void)
{
    TRISBbits.TRISB7 =0;
    TRISBbits.TRISB6 =0;
    TRISBbits.TRISB5 =0;
    TRISBbits.TRISB4 =0;
    TRISCbits.TRISC0 =0;
    TRISCbits.TRISC1 =0;
    
    LCD_Command (0x02); 
    LCD_Command (0x28); //Send four bit initialization of LCD
    LCD_Command (0x80); //Cursor at Home position
    LCD_Command (0x0C); //Display On Cursor off
    LCD_Command (0x06); //Auto increment cursor
    LCD_Command (0x01);
    __delay_ms(10);
}
void show(unsigned char *s)
{
    while(*s) {
        LCD_Char(*s++);
    }
}
