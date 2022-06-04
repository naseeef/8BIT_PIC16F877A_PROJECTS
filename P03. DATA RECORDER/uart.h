void ser_int();
unsigned int tx(unsigned int);
unsigned char rx();


void ser_int()
{
 TXSTA= 0x20;
 RCSTA= 0b10010000;
 SPBRG=31;
}
 
unsigned int tx(unsigned int a)
{
 TXREG=a;
 while(!TXIF);
 TXIF = 0;
}
unsigned char rx()
{
 while(!RCIF);
 RCIF=0;
 return RCREG;
}

/*
 char b; 
TRISD=0x00;
TRISB=0X00;
LCD_init();
ser_int(); 
tx('a');
b = tx('a');
LCD_data (b);
__delay_ms(100);
 */