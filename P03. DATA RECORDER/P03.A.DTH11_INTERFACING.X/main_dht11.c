/*
 * File:   main_d2h11.c
 * Author: Naseef
 *
 * Created on 6 June, 2022, 11:44 AM
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
#define _XTAL_FREQ 20000000

void StartSignal2();
void CheckResponse2();
 unsigned char Check, T_byte1, T_byte2,
 RH_byte1, RH_byte2, Ch ;
 unsigned int Temp, RH, Sum ;
 
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
 void LCD_init (void)
{
    LCD_Command (0x02); 
    LCD_Command (0x28); //Send four bit initialization of LCD
    LCD_Command (0x80); //Cursor at Home position
    LCD_Command (0x0C); //Display On Cursor off
    LCD_Command (0x06); //Auto increment cursor
    LCD_Command (0x01);
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
void show(unsigned char *s)
{
    while(*s) {
        LCD_Char(*s++);
    }
}
void show_multidigits (unsigned int val)
{
    int remainder, digit1, digit2, digit3, digit4, result, result1;
    if (val<=9)
    {
       LCD_Char(val+0x30);
    }
    else if (val>=10 && val <100)
    {
        remainder = val % 10;
        digit1 = remainder;
        digit2 = val/10;
        LCD_Char(digit2+0x30);
        LCD_Char(digit1+0x30);
    }
    else if (val>=100 && val <1000)
    {
        remainder = val % 10;
        result1 = val /10;
        digit1 = remainder;
        remainder = result1%10;
        digit2 = remainder;
        digit3 = result1/10;
        LCD_Char(digit3+0x30);
        LCD_Char(digit2+0x30);
        LCD_Char(digit1+0x30);
    }
    else if (val>=1000 && val <9999)
    {
        remainder = val % 10;
        result1 = val /10;
        digit1 = remainder;
        remainder = result1%10;
        digit2 = remainder;
        result = result1/10;
        remainder = result%10;
        digit3=remainder;
        digit4 = result/10;
        LCD_Char(digit4+0x30);
        LCD_Char(digit3+0x30);
        LCD_Char(digit2+0x30);
        LCD_Char(digit1+0x30);
    }
}
 
void StartSignal(){
 TRISDbits.TRISD0 = 0; //Configure RD0 as output
 PORTDbits.RD0 = 0; //RD0 sends 0 to the sensor
 __delay_ms(18);
 PORTDbits.RD0 = 1; //RD0 sends 1 to the sensor
 __delay_us(30);
 TRISDbits.TRISD0 = 1; //Configure RD0 as input
 }
 //////////////////////////////
 void CheckResponse(){
 Check = 0;
 //__delay_us(40);
 if (PORTDbits.RD0 == 0)
 {
 __delay_us(80);
 if (PORTDbits.RD0 == 1) 
     Check = 1; 
 __delay_us(80);
 }
 }
 //////////////////////////////
 char ReadData(){
 char i, j;
 for(j = 0; j < 8; j++){
 while(!PORTDbits.RD0); //Wait until PORTD.F0 goes HIGH
 __delay_us(30);
 if(PORTDbits.RD0 == 0)
 i&= ~(1<<(7 - j)); //Clear bit (7-b)
 else {i|= (1 << (7 - j)); //Set bit (7-b)
 while(PORTDbits.RD0);} //Wait until PORTD.F0 goes LOW
 }
 return i;
 }
 //////////////////////////////
void main() {
    TRISB = 0x00;
    TRISC = 0X00;
 TRISDbits.TRISD0 = 0;
 
 LCD_init();
 LCD_Command(0x01); // clear LCD
 while(1){
 StartSignal2();
 CheckResponse2();
 if(Check == 1){
 RH_byte1 = ReadData();
 RH_byte2 = ReadData();
 T_byte1 = ReadData();
 T_byte2 = ReadData();
 Sum = ReadData();
 if(Sum == ((RH_byte1+RH_byte2+T_byte1+T_byte2) & 0XFF)){
 Temp = T_byte1;
 RH = RH_byte1;
 LCD_Command(0x80);
 show("Temp:");
 LCD_Command(0xC0);
 show("Humi:");
 LCD_Command(0x85);
 show_multidigits (Temp);
 LCD_Command(0xC5);
 show_multidigits(RH);
 }
 __delay_ms(1000);
 }
}
}

void StartSignal2(){
 TRISDbits.TRISD0 = 0; //Configure RD0 as output
 PORTDbits.RD0 = 0; //RD0 sends 0 to the sensor
 __delay_ms(25);
 PORTDbits.RD0 = 1; //RD0 sends 1 to the sensor
 __delay_us(30);
 TRISDbits.TRISD0 = 1; //Configure RD0 as input
 }
 //////////////////////////////
 void CheckResponse2(){
 Check = 0;
 __delay_us(40);
 if (PORTDbits.RD0 == 0)
 {
 __delay_us(80);
 if (PORTDbits.RD0 == 1){  
    __delay_us(50);
    Check = 1;
 }}

// while((!PORTDbits.RD0));
// while((PORTDbits.RD0));
// Check = 1;
 }