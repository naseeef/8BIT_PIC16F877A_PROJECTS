#define DHT22_PIN   PORTDbits.RD0
#define DHT22_PIN_DIR   TRISDbits.TRISD0

#define _XTAL_FREQ 20000000

uint8_t dht22_read_byte();
void dht22_read(uint16_t *dht22_humi, int16_t *dht22_temp);

uint8_t dht22_read_byte()
{
  uint8_t i = 8, dht22_byte = 0;
  while(i--)
  {
    while( !DHT22_PIN );
    __delay_us(40);
    if( DHT22_PIN )
    {
      dht22_byte |= (1 << i);   // set bit i
      while( DHT22_PIN );
    }
  }
  return(dht22_byte);
}

void dht22_read(uint16_t *dht22_humi, int16_t *dht22_temp)
{
  // send start signal
  DHT22_PIN     = 0;   // connection pin output low
  DHT22_PIN_DIR = 0;   // configure connection pin as output
  __delay_ms(25);        // wait 25 ms
  DHT22_PIN     = 1;   // connection pin output high
  __delay_us(30);        // wait 30 us
  DHT22_PIN_DIR = 1;   // configure connection pin as input
  // check sensor response
  while( DHT22_PIN );
  while(!DHT22_PIN );
  while( DHT22_PIN );
  // read data
  *dht22_humi = dht22_read_byte();  // read humidity byte 1
  *dht22_humi = (*dht22_humi << 8) | dht22_read_byte();  // read humidity byte 2
  *dht22_temp = dht22_read_byte();  // read temperature byte 1
  *dht22_temp = (*dht22_temp << 8) | dht22_read_byte();  // read temperature byte 2
  dht22_read_byte();               // read checksum (skipped)
  if(*dht22_temp & 0x8000)// if temperature is negative
  {
    *dht22_temp &= 0x7FFF;
    *dht22_temp *= -1;
  }
}