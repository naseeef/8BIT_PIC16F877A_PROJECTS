void ser_int();
unsigned char tx(unsigned char);
unsigned char rx();


void ser_int()
{
 TXSTA= 0x20;
 RCSTA= 0b10010000;
 SPBRG=31;
}
 
unsigned char tx(unsigned char a)
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