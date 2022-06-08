#define write_cmd 0xEE
#define read_cmd 0xEF

void bmp280_init();
void bmp280_start();
void bmp280_stop();
void bmp280_restart();
void bmp280_ack();
void bmp280_nak();
void waitmssp();
void bmp280_send(unsigned char dat);
void bmp280_send_byte(unsigned char addr,unsigned char count);
unsigned char bmp280_read();
unsigned char bmp280_read_byte(unsigned char addr);
void bmp280_init()
{
    TRISC3=TRISC4=1;
    SSPCON=0x28;                    //SSP Module as Master
    SSPADD=((11059200/4)/100)-1;    //Setting Clock Speed, My PCLK = 11.0592MHz
}
void bmp280_start()
{
    SEN=1;
    waitmssp();
}
void bmp280_stop()
{
    PEN=1;
    waitmssp();
}
void bmp280_restart()
{
    RSEN=1;
    waitmssp();
}
void bmp280_ack()
{
    ACKDT=0;
    ACKEN=1;
    waitmssp();
}
void bmp280_nak()
{
    ACKDT=1;
    ACKEN=1;
    waitmssp();
}
void waitmssp()
{
    while(!SSPIF);
    SSPIF=0;
}
void bmp280_send(unsigned char dat)
{
L1: SSPBUF=dat;
    waitmssp();
    while(ACKSTAT){bmp280_restart;goto L1;}
}
void bmp280_send_byte(unsigned char addr,unsigned char count)
{
    bmp280_start();
    bmp280_send(write_cmd);
    bmp280_send(addr);
    bmp280_send(count);
    bmp280_stop();
}
unsigned char bmp280_read()
{
    RCEN=1;
    waitmssp();
    return SSPBUF;
}   
unsigned char bmp280_read_byte(unsigned char addr)
{
    unsigned char rec;
L:  bmp280_restart();
    SSPBUF=write_cmd;
    waitmssp();
    while(ACKSTAT){goto L;}
    bmp280_send(addr);
    bmp280_restart();
    bmp280_send(read_cmd);
    rec=bmp280_read();
    bmp280_nak();
    bmp280_stop();
    return rec;
}