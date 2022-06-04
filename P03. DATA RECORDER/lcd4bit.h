void LCD_init(void);
void LCD_Command(unsigned char cmnd);
void LCD_num(long int val);
void LCD_Char(unsigned char data);
void show(unsigned char *s);
unsigned char intdigits_chardigits(unsigned int a, unsigned int b, unsigned int c);

unsigned char avv[3];

#define _XTAL_FREQ 20000000

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
    LCD_Command (0x02); 
    LCD_Command (0x28); //Send four bit initialization of LCD
    LCD_Command (0x80); //Cursor at Home position
    LCD_Command (0x0C); //Display On Cursor off
    LCD_Command (0x06); //Auto increment cursor
    LCD_Command (0x01);
    __delay_ms(10);
}
void LCD_num (long int val)
{
    int calb_val = 200;
    unsigned int remainder, digit1, digit2, digit3, result, 
    result1;
    result = ((val*100)/calb_val);
    remainder = result% 10;
    result1 = result /10;
    digit1 = remainder;
    remainder = result1%10;
    digit2 = remainder;
    remainder = (result1/10);
    digit3=remainder;
    LCD_Char(digit3+0x30);
    LCD_Char('.');
    LCD_Char(digit2+0x30);
    LCD_Char(digit1+0x30);
    
    intdigits_chardigits(digit3,digit2,digit1);
}
    
void show(unsigned char *s)
{
    while(*s) {
        LCD_Char(*s++);
    }
}

unsigned char intdigits_chardigits(unsigned int a, unsigned int b, unsigned int c)
{
    avv[0] = (unsigned char)a;
    avv[1] = (unsigned char)b;
    avv[2] = (unsigned char)c;
}