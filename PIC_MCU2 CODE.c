unsigned char read_key();

unsigned char phoneActive=0;
unsigned char phonePicked=0;
unsigned char phoneScrollIndex=0;
unsigned int phoneScrollCounter=0;

unsigned char phoneServoActive=0;
unsigned int phoneServoCounter=0;
unsigned char phoneServoFrame=0;
unsigned char phoneMessagePending=0;

void lcd_pulse()
{
RE1_bit=1;
Delay_us(5);
RE1_bit=0;
Delay_us(100);
}

void lcd_cmd(unsigned char cmd)
{
RE0_bit=0;
PORTF=cmd;
lcd_pulse();
if(cmd==0x01||cmd==0x02)
Delay_ms(5);
else
Delay_ms(1);
}

void lcd_data(unsigned char dat)
{
RE0_bit=1;
PORTF=dat;
lcd_pulse();
Delay_ms(1);
}

void lcd_init()
{
RE0_bit=0;
RE1_bit=0;
Delay_ms(100);
PORTF=0x38;
lcd_pulse();
Delay_ms(10);
PORTF=0x38;
lcd_pulse();
Delay_ms(10);
PORTF=0x38;
lcd_pulse();
Delay_ms(10);
PORTF=0x0C;
lcd_pulse();
Delay_ms(2);
PORTF=0x06;
lcd_pulse();
Delay_ms(2);
PORTF=0x01;
lcd_pulse();
Delay_ms(5);
}

unsigned char read_key()
{
unsigned char key;
key=0;
TRISD=0xF0;
PORTD=0x01;
Delay_ms(1);
if(RD7_bit) key='/';
if(RD6_bit) key='9';
if(RD5_bit) key='8';
if(RD4_bit) key='7';

if(key==0)
{
PORTD=0x02;
Delay_ms(1);
if(RD7_bit) key='*';
if(RD6_bit) key='6';
if(RD5_bit) key='5';
if(RD4_bit) key='4';
}

if(key==0)
{
PORTD=0x04;
Delay_ms(1);
if(RD7_bit) key='-';
if(RD6_bit) key='3';
if(RD5_bit) key='2';
if(RD4_bit) key='1';
}

if(key==0)
{
PORTD=0x08;
Delay_ms(1);
if(RD7_bit) key='+';
if(RD6_bit) key='#';
if(RD5_bit) key='0';
if(RD4_bit) key='C';
}

PORTD=0x00;
return key;
}

void UART_SendOpen()
{
while(PIR1.TX1IF==0);
TXREG1='O';
}

void show_wrong()
{
lcd_cmd(0x01);
Delay_ms(5);
lcd_cmd(0x80);
lcd_data('W');
lcd_data('R');
lcd_data('O');
lcd_data('N');
lcd_data('G');
lcd_data(' ');
lcd_data('C');
lcd_data('O');
lcd_data('D');
lcd_data('E');

lcd_cmd(0xC0);
lcd_data('T');
lcd_data('R');
lcd_data('Y');
lcd_data(' ');
lcd_data('A');
lcd_data('G');
lcd_data('A');
lcd_data('I');
lcd_data('N');

PORTH.B0=1;
Delay_ms(2000);
PORTH.B0=0;

lcd_cmd(0x01);
Delay_ms(5);
lcd_cmd(0x80);
lcd_data('T');
lcd_data('e');
lcd_data('l');
lcd_data('e');
lcd_data('p');
lcd_data('h');
lcd_data('o');
lcd_data('n');
lcd_data('e');

lcd_cmd(0xC0);
lcd_data('c');
lcd_data('o');
lcd_data('d');
lcd_data(':');
}

void show_correct()
{
lcd_cmd(0x01);
Delay_ms(5);
lcd_cmd(0x80);
lcd_data('C');
lcd_data('O');
lcd_data('D');
lcd_data('E');
lcd_data(' ');
lcd_data('C');
lcd_data('O');
lcd_data('R');
lcd_data('R');
lcd_data('E');
lcd_data('C');
lcd_data('T');

lcd_cmd(0xC0);
lcd_data('O');
lcd_data('P');
lcd_data('E');
lcd_data('N');
lcd_data('I');
lcd_data('N');
lcd_data('G');
lcd_data('.');
lcd_data('.');
lcd_data('.');
}

void lcd2_pulse()
{
RH2_bit=1;
Delay_us(5);
RH2_bit=0;
Delay_us(100);
}

void lcd2_nibble(unsigned char value)
{
if(value==0)
{
RH3_bit=0;
RH4_bit=0;
RH5_bit=0;
RH6_bit=0;
}
else if(value==1)
{
RH3_bit=1;
RH4_bit=0;
RH5_bit=0;
RH6_bit=0;
}
else if(value==2)
{
RH3_bit=0;
RH4_bit=1;
RH5_bit=0;
RH6_bit=0;
}
else if(value==3)
{
RH3_bit=1;
RH4_bit=1;
RH5_bit=0;
RH6_bit=0;
}
else if(value==4)
{
RH3_bit=0;
RH4_bit=0;
RH5_bit=1;
RH6_bit=0;
}
else if(value==5)
{
RH3_bit=1;
RH4_bit=0;
RH5_bit=1;
RH6_bit=0;
}
else if(value==6)
{
RH3_bit=0;
RH4_bit=1;
RH5_bit=1;
RH6_bit=0;
}
else if(value==7)
{
RH3_bit=1;
RH4_bit=1;
RH5_bit=1;
RH6_bit=0;
}
else if(value==8)
{
RH3_bit=0;
RH4_bit=0;
RH5_bit=0;
RH6_bit=1;
}
else if(value==9)
{
RH3_bit=1;
RH4_bit=0;
RH5_bit=0;
RH6_bit=1;
}
else if(value==10)
{
RH3_bit=0;
RH4_bit=1;
RH5_bit=0;
RH6_bit=1;
}
else if(value==11)
{
RH3_bit=1;
RH4_bit=1;
RH5_bit=0;
RH6_bit=1;
}
else if(value==12)
{
RH3_bit=0;
RH4_bit=0;
RH5_bit=1;
RH6_bit=1;
}
else if(value==13)
{
RH3_bit=1;
RH4_bit=0;
RH5_bit=1;
RH6_bit=1;
}
else if(value==14)
{
RH3_bit=0;
RH4_bit=1;
RH5_bit=1;
RH6_bit=1;
}
else
{
RH3_bit=1;
RH4_bit=1;
RH5_bit=1;
RH6_bit=1;
}

lcd2_pulse();
}

void lcd2_cmd(unsigned char cmd)
{
unsigned char highNibble;
unsigned char lowNibble;

RH1_bit=0;

highNibble=cmd/16;
lowNibble=cmd-(highNibble*16);

lcd2_nibble(highNibble);
lcd2_nibble(lowNibble);

if(cmd==0x01||cmd==0x02)
Delay_ms(2);
}

void lcd2_data(unsigned char dat)
{
unsigned char highNibble;
unsigned char lowNibble;

RH1_bit=1;

highNibble=dat/16;
lowNibble=dat-(highNibble*16);

lcd2_nibble(highNibble);
lcd2_nibble(lowNibble);
}

void lcd2_init()
{
RH1_bit=0;
RH2_bit=0;
RH3_bit=0;
RH4_bit=0;
RH5_bit=0;
RH6_bit=0;

Delay_ms(100);

lcd2_nibble(3);
Delay_ms(10);
lcd2_nibble(3);
Delay_ms(10);
lcd2_nibble(3);
Delay_ms(10);
lcd2_nibble(2);
Delay_ms(10);

lcd2_cmd(0x28);
lcd2_cmd(0x0C);
lcd2_cmd(0x06);
lcd2_cmd(0x01);

Delay_ms(5);
}

void lcd2_unknown()
{
lcd2_cmd(0x01);
Delay_ms(2);

lcd2_cmd(0x80);

lcd2_data('u');
lcd2_data('n');
lcd2_data('k');
lcd2_data('n');
lcd2_data('o');
lcd2_data('w');
lcd2_data('n');
lcd2_data(' ');
lcd2_data('n');
lcd2_data('u');
lcd2_data('m');
lcd2_data('b');
lcd2_data('e');
lcd2_data('r');
}

void lcd2_message()
{
lcd2_cmd(0x01);
Delay_ms(2);

lcd2_cmd(0x80);

lcd2_data('Y');
lcd2_data('o');
lcd2_data('u');
lcd2_data(' ');
lcd2_data('f');
lcd2_data('o');
lcd2_data('u');
lcd2_data('n');
lcd2_data('d');
lcd2_data(' ');
lcd2_data('i');
lcd2_data('t');
lcd2_data('.');

lcd2_cmd(0xC0);

if(phoneScrollIndex==0)
{
lcd2_data('T');
lcd2_data('a');
lcd2_data('k');
lcd2_data('e');
lcd2_data(' ');
lcd2_data('i');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('E');
lcd2_data('a');
lcd2_data('s');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('c');
lcd2_data('o');
lcd2_data('r');
}
else if(phoneScrollIndex==1)
{
lcd2_data('a');
lcd2_data('k');
lcd2_data('e');
lcd2_data(' ');
lcd2_data('i');
lcd2_data('t');
lcd2_data('.');
lcd2_data(' ');
lcd2_data('E');
lcd2_data('a');
lcd2_data('s');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('c');
lcd2_data('o');
lcd2_data('r');
lcd2_data('r');
}
else if(phoneScrollIndex==2)
{
lcd2_data('k');
lcd2_data('e');
lcd2_data(' ');
lcd2_data('i');
lcd2_data('t');
lcd2_data('.');
lcd2_data(' ');
lcd2_data('E');
lcd2_data('a');
lcd2_data('s');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('c');
lcd2_data('o');
lcd2_data('r');
lcd2_data('r');
lcd2_data('i');
}
else if(phoneScrollIndex==3)
{
lcd2_data('e');
lcd2_data(' ');
lcd2_data('i');
lcd2_data('t');
lcd2_data('.');
lcd2_data(' ');
lcd2_data('E');
lcd2_data('a');
lcd2_data('s');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('c');
lcd2_data('o');
lcd2_data('r');
lcd2_data('r');
lcd2_data('i');
lcd2_data('d');
}
else if(phoneScrollIndex==4)
{
lcd2_data(' ');
lcd2_data('i');
lcd2_data('t');
lcd2_data('.');
lcd2_data(' ');
lcd2_data('E');
lcd2_data('a');
lcd2_data('s');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('c');
lcd2_data('o');
lcd2_data('r');
lcd2_data('r');
lcd2_data('i');
lcd2_data('d');
lcd2_data('o');
}
else
{
lcd2_data('i');
lcd2_data('t');
lcd2_data('.');
lcd2_data(' ');
lcd2_data('E');
lcd2_data('a');
lcd2_data('s');
lcd2_data('t');
lcd2_data(' ');
lcd2_data('c');
lcd2_data('o');
lcd2_data('r');
lcd2_data('r');
lcd2_data('i');
lcd2_data('d');
lcd2_data('o');
}
}

void CheckWardenSignal()
{
unsigned char received;

if(PIR1.RC1IF)
{
if(RCSTA1.OERR)
{
RCSTA1.CREN=0;
RCSTA1.CREN=1;
}

received=RCREG1;

if(received=='W')
{
phoneActive=1;
phonePicked=0;
phoneScrollIndex=0;
phoneScrollCounter=0;
phoneServoActive=0;
phoneServoCounter=0;
phoneServoFrame=0;
phoneMessagePending=0;
RJ0_bit=0;
PORTH.B0=0;
lcd2_unknown();
}
}
}

void RunPhoneSequence()
{
if(phoneActive==0)
return;

if(RH7_bit==1)
{
if(phonePicked==0&&phoneServoActive==0)
{
phoneServoActive=1;
phoneServoCounter=0;
phoneServoFrame=0;
phoneMessagePending=0;
}
}
}

void UpdatePhoneMessage()
{
if(phoneMessagePending==1)
{
phoneMessagePending=0;
phonePicked=1;
phoneScrollIndex=0;
phoneScrollCounter=0;
lcd2_message();
}

if(phoneActive==1&&phonePicked==1)
{
phoneScrollCounter++;

if(phoneScrollCounter>=500)
{
phoneScrollCounter=0;
phoneScrollIndex++;

if(phoneScrollIndex>5)
phoneScrollIndex=0;

lcd2_message();
}
}
}

void interrupt()
{
if(INTCON.TMR0IF)
{
TMR0H=0xFD;
TMR0L=0x8F;
INTCON.TMR0IF=0;

if(phoneServoActive==1)
{
if(phoneServoFrame<2)
RJ0_bit=1;
else
RJ0_bit=0;

phoneServoFrame++;

if(phoneServoFrame>=20)
phoneServoFrame=0;

phoneServoCounter++;

if(phoneServoCounter>=1000)
{
phoneServoCounter=1000;
phoneServoActive=1;
phoneMessagePending=1;
phonePicked=1;
}
}
else
{
RJ0_bit=0;
}
}
}

void main()
{
unsigned char key;
unsigned char old_key;
unsigned char codeStep;

ADCON1=0x0F;
CMCON=0x07;
PSPCON=0x00;

TRISE=0x00;
TRISF=0x00;
TRISD=0xF0;
TRISH=0x80;
TRISJ.B0=0;

TRISC6_bit=0;
TRISC7_bit=1;

TXSTA1=0x24;
RCSTA1=0x90;
BAUDCON1=0x00;
SPBRG1=129;

PORTE=0x00;
PORTF=0x00;
PORTD=0x00;
PORTH=0x00;
PORTJ=0x00;

PORTH.B0=0;
PORTH.B1=0;
PORTH.B2=0;
PORTH.B3=0;
PORTH.B4=0;
PORTH.B5=0;
PORTH.B6=0;

RJ0_bit=0;

T0CON=0x82;
TMR0H=0xFD;
TMR0L=0x8F;

INTCON.TMR0IF=0;
INTCON.TMR0IE=1;
INTCON.GIE=1;

lcd_init();
lcd2_init();

lcd_cmd(0x01);
Delay_ms(5);

lcd_cmd(0x80);
lcd_data('T');
lcd_data('e');
lcd_data('l');
lcd_data('e');
lcd_data('p');
lcd_data('h');
lcd_data('o');
lcd_data('n');
lcd_data('e');

lcd_cmd(0xC0);
lcd_data('c');
lcd_data('o');
lcd_data('d');
lcd_data('e');
lcd_data(':');

old_key=0;
codeStep=0;

phoneActive=0;
phonePicked=0;
phoneScrollIndex=0;
phoneScrollCounter=0;

phoneServoActive=0;
phoneServoCounter=0;
phoneServoFrame=0;
phoneMessagePending=0;

while(1)
{
CheckWardenSignal();

if(phoneActive==1)
{
RunPhoneSequence();
UpdatePhoneMessage();
}
else
{
key=read_key();

if(key!=0)
{
if(old_key==0)
{
Delay_ms(30);
key=read_key();

if(key!=0)
{
if(codeStep==0)
{
if(key=='3')
{
lcd_data('*');
codeStep=1;
}
else
{
show_wrong();
codeStep=0;
}
}
else if(codeStep==1)
{
if(key=='1')
{
lcd_data('*');
codeStep=2;
}
else
{
show_wrong();
codeStep=0;
}
}
else if(codeStep==2)
{
if(key=='2')
{
lcd_data('*');
codeStep=3;
}
else
{
show_wrong();
codeStep=0;
}
}
else if(codeStep==3)
{
if(key=='#')
{
lcd_data('*');
codeStep=4;
show_correct();
Delay_ms(500);
UART_SendOpen();
}
else
{
show_wrong();
codeStep=0;
}
}

old_key=key;
}
}
}
else
{
old_key=0;
}

Delay_ms(20);
}
}
}
