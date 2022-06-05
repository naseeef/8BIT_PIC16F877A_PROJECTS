#define _XTAL_FREQ 20000000

//#define DHT11_Data_Pin_Direction  TRISDbits.TRISD0
//#define DHT11_Data_Pin   PORTDbits.RD0

// Variables for DHT11
unsigned char Humidity, Temp, RH_byte_1, RH_byte_2, Temp_byte_1, Temp_byte_2; 
unsigned char check_bit, Summation;

void dht11_init();
void find_response();
char read_dht11();

//void dht11_init()
//{
//    DHT11_Data_Pin_Direction= 0; //Configure RD0 as output
//    DHT11_Data_Pin = 0; //RD0 sends 0 to the sensor
//    __delay_ms(18);
//    DHT11_Data_Pin = 1; //RD0 sends 1 to the sensor
//    __delay_us(30);
//    DHT11_Data_Pin_Direction = 1; //Configure RD0 as input
// }

void dht11_init()
{
    TRISD = 0; //Configure RD0 as output
    PORTD = 0; //RD0 sends 0 to the sensor
    __delay_ms(18);
    PORTD = 0xFF; //RD0 sends 1 to the sensor
    __delay_us(30);
    TRISD = 0xFF; //Configure RD0 as input
 }

 void find_response()
{
    check_bit = 0;
    __delay_us(40);
//    if (DHT11_Data_Pin == 0)
    if (RD0 == 0)
    {
        __delay_us(80);
//        if (DHT11_Data_Pin == 1)
        if (RD0 == 1)
        {
            check_bit = 1;
        }          
        __delay_us(50);
    }
 }
 
char read_dht11()
{
    char data, for_count;
    for(for_count = 0; for_count < 8; for_count++)
    {
//        while(!DHT11_Data_Pin);
        while(!RD0);
        __delay_us(30);
//        if(DHT11_Data_Pin == 0)
        if(RD0 == 0)
        {
            data&= ~(1<<(7 - for_count)); //Clear bit (7-b)
        }
        else
        {
            data|= (1 << (7 - for_count)); //Set bit (7-b)
//            while(DHT11_Data_Pin);
            while(RD0);
        }
    }
    return data;
}