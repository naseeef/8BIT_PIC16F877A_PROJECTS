void ser_int();
unsigned char tx(unsigned char);
unsigned char rx();
void tx_sn(unsigned int val); // fn for transmitting integer values to UART


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

void tx_sn (unsigned int val)
{
    int remainder, digit1, digit2, digit3, digit4, result, result1;
    if (val<=9)
    {
       tx(val+0x30);
    }
    else if (val>=10 && val <100)
    {
        remainder = val % 10;
        digit1 = remainder;
        digit2 = val/10;
        tx(digit2+0x30);
        tx(digit1+0x30);
    }
    else if (val>=100 && val <1000)
    {
        remainder = val % 10;
        result1 = val /10;
        digit1 = remainder;
        remainder = result1%10;
        digit2 = remainder;
        digit3 = result1/10;
        tx(digit3+0x30);
        tx(digit2+0x30);
        tx(digit1+0x30);
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
        tx(digit4+0x30);
        tx(digit3+0x30);
        tx(digit2+0x30);
        tx(digit1+0x30);
    }
}