#define _XTAL_FREQ 20000000

unsigned char Check, T_byte1, T_byte2,
RH_byte1, RH_byte2, Ch ;
unsigned Temp, RH, Sum ;
 
void StartSignal()
{
    TRISDbits.TRISD0 = 0; //Configure RD0 as output
    PORTDbits.RD0 = 0; //RD0 sends 0 to the sensor
    __delay_ms(25);
    PORTDbits.RD0 = 1; //RD0 sends 1 to the sensor
    __delay_us(30);
    TRISDbits.TRISD0 = 1; //Configure RD0 as input
 }
 //////////////////////////////
void CheckResponse()
{
    Check = 0;
    __delay_us(40);
    if (PORTDbits.RD0 == 0)
    {
        __delay_us(80);
        if (PORTDbits.RD0 == 1)
        {  
            __delay_us(50);
           Check = 1;
        }
    }
}
 //////////////////////////////
char ReadData()
{
    char i, j; 
    for(j = 0; j < 8; j++)
    {
        while(!PORTDbits.RD0); //Wait until PORTD.F0 goes HIGH
        __delay_us(30);
        if(PORTDbits.RD0 == 0)
        i&= ~(1<<(7 - j)); //Clear bit (7-b)
        else 
        {
            i|= (1 << (7 - j)); //Set bit (7-b)
            while(PORTDbits.RD0); //Wait until PORTD.F0 goes LOW
        } 
    }
    return i;
}