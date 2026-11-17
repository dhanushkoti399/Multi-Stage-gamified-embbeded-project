unsigned char displayBuffer[4];
unsigned char displayIndex=0;
unsigned long totalSeconds=3000;
unsigned long selectedSeconds=3000;
unsigned int millisecondCounter=0;
unsigned int buzzerRemaining=0;
unsigned long uvRemaining=0;
unsigned char timerRunning=0;
unsigned char timerStarted=0;
unsigned char setMode=0;
unsigned char event45Done=0;
unsigned char event43Done=0;
unsigned char emergencyOn=0;
unsigned char cellAUnlocked=0;
unsigned char cellBUnlocked=0;
unsigned char potTriggered=0;
unsigned char cellBLeverLast=1;
unsigned char servoCounter=0;
unsigned char displayUpdateRequired=0;
unsigned char buttonLock=0;
unsigned char laundryOpen=0;
unsigned char motionDetected=0;
unsigned char extraServoActive=0;
unsigned char extraSwitchLast=1;
unsigned char ledSequenceActive=0;
unsigned char ledSequenceStep=0;
unsigned int ledSequenceCounter=0;
unsigned int ledBuzzerCounter=0;
unsigned char ledBuzzerActive=0;
unsigned char wardenServoActive=0;
unsigned char pressureWardenServoActive=0;
unsigned char wardenUnlockDone=0;
unsigned char wardenBeepActive=0;
unsigned char wardenBeepCount=0;
unsigned int wardenBeepCounter=0;
unsigned char wardenSignalSent=0;
unsigned char wardenSignalPending=0;
unsigned int waterValue=0;
unsigned int pressureValue=0;
unsigned int drainValue=0;
unsigned int calculatedPressure=0;
unsigned char pressureMaxDone=0;
unsigned char pressure30SignalSent=0;
unsigned char pressure30SignalPending=0;

unsigned char eastStage1Done=0;
unsigned char eastStage2Done=0;
unsigned char eastStage3Done=0;
unsigned char eastCorridorServoActive=0;
unsigned char eastCorridorDone=0;

const unsigned int PRESSURE_30PSI_ADC=614;
const unsigned int EAST_HG_MAX=900;

const unsigned char segmentCode[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void UpdateDisplay()
{
unsigned long value;
unsigned char mm;
unsigned char ss;

value=totalSeconds;
mm=value/60;
ss=value%60;

displayBuffer[0]=mm/10;
displayBuffer[1]=mm%10;
displayBuffer[2]=ss/10;
displayBuffer[3]=ss%10;
}

void DisplayScan()
{
PORTB=0x0F;
PORTD=segmentCode[displayBuffer[displayIndex]];

if(displayIndex==0)
PORTB.B0=0;
else if(displayIndex==1)
PORTB.B1=0;
else if(displayIndex==2)
PORTB.B2=0;
else
PORTB.B3=0;

displayIndex++;

if(displayIndex>=4)
displayIndex=0;
}

unsigned int ReadADCChannel(unsigned char channel)
{
unsigned int adcValue;

if(channel==0)
ADCON0=0x01;
else if(channel==9)
ADCON0=0x25;
else if(channel==10)
ADCON0=0x29;
else if(channel==11)
ADCON0=0x2D;
else if(channel==13)
ADCON0=0x35;
else if(channel==14)
ADCON0=0x39;
else if(channel==15)
ADCON0=0x3D;
else
ADCON0=0x01;

ADCON0.ADON=1;

Delay_us(50);

ADCON0.GO_DONE=1;

while(ADCON0.GO_DONE);

adcValue=((unsigned int)ADRESH*256)+ADRESL;

return adcValue;
}

unsigned int ReadPot()
{
return ReadADCChannel(0);
}

void SetLightsNormal()
{
LATA.B1=1;
LATA.B2=0;
LATA.B3=0;
}

void SetLightsUV()
{
LATA.B1=0;
LATA.B2=1;
LATA.B3=0;
}

void SetLightsEmergency()
{
LATA.B1=0;
LATA.B2=0;
LATA.B3=1;
}

void AllLightsOff()
{
LATA.B1=0;
LATA.B2=0;
LATA.B3=0;
}

void LEDsOff()
{
LATE.B0=0;
LATE.B1=0;
LATE.B2=0;
LATE.B3=0;
}

void GreenLEDsOff()
{
LATF.B0=0;
LATF.B1=0;
LATF.B2=0;
}

void EastCorridorLEDsOff()
{
LATF.B0=0;
LATF.B1=0;
LATF.B2=0;
}

void CheckEastCorridorHG()
{
unsigned int stage1Value;
unsigned int stage2Value;
unsigned int stage3Value;

if(eastCorridorDone==1)
return;

if(eastStage1Done==0)
{
stage1Value=ReadADCChannel(13);

if(stage1Value>=EAST_HG_MAX)
{
eastStage1Done=1;
LATF.B0=1;
}

return;
}

LATF.B0=1;

if(eastStage2Done==0)
{
stage2Value=ReadADCChannel(14);

if(stage2Value>=EAST_HG_MAX)
{
eastStage2Done=1;
LATF.B1=1;
}

return;
}

LATF.B1=1;

if(eastStage3Done==0)
{
stage3Value=ReadADCChannel(15);

if(stage3Value>=EAST_HG_MAX)
{
eastStage3Done=1;
LATF.B2=1;
}

return;
}

LATF.B2=1;

if((eastStage1Done==1)&&(eastStage2Done==1)&&(eastStage3Done==1))
{
eastCorridorServoActive=1;
eastCorridorDone=1;
timerRunning=0;
PORTH.B0=0;
}
}

void CheckPressure()
{
unsigned long averageValue;

waterValue=ReadADCChannel(9);
pressureValue=ReadADCChannel(10);
drainValue=ReadADCChannel(11);

averageValue=((unsigned long)waterValue+(unsigned long)pressureValue)/2;

if(drainValue>=averageValue)
calculatedPressure=0;
else
calculatedPressure=averageValue-drainValue;

if(calculatedPressure>1023)
calculatedPressure=1023;

if(calculatedPressure<256)
{
LATE.B0=1;
LATE.B1=0;
LATE.B2=0;
LATE.B3=0;
}
else if(calculatedPressure<512)
{
LATE.B0=1;
LATE.B1=1;
LATE.B2=0;
LATE.B3=0;
}
else if(calculatedPressure<768)
{
LATE.B0=1;
LATE.B1=1;
LATE.B2=1;
LATE.B3=0;
}
else
{
LATE.B0=1;
LATE.B1=1;
LATE.B2=1;
LATE.B3=1;
}

if(calculatedPressure>=PRESSURE_30PSI_ADC)
{
pressureWardenServoActive=1;

if(pressure30SignalSent==0)
{
pressure30SignalPending=1;
pressure30SignalSent=1;
}
}
else
{
pressureWardenServoActive=0;
pressure30SignalSent=0;
}

if(calculatedPressure>=1023)
{
if(pressureMaxDone==0)
{
pressureMaxDone=1;
}
}
else
{
pressureMaxDone=0;
}
}

void UART_SendWarden()
{
if(PIR1.TX1IF)
{
TXREG1='W';
wardenSignalSent=1;
wardenSignalPending=0;
}
}

void UART_SendPressure30()
{
if(PIR1.TX1IF)
{
TXREG1='W';
pressure30SignalSent=1;
pressure30SignalPending=0;
}
}

void StartLEDSequence()
{
ledSequenceActive=1;
ledSequenceStep=1;
ledSequenceCounter=0;
ledBuzzerCounter=0;
ledBuzzerActive=1;

wardenServoActive=0;
wardenUnlockDone=0;
wardenBeepActive=0;
wardenBeepCount=0;
wardenBeepCounter=0;
wardenSignalSent=0;
wardenSignalPending=0;

GreenLEDsOff();

LATE.B0=1;
LATE.B1=0;
LATE.B2=0;
LATE.B3=0;
}

void UpdateLEDSequence()
{
if(ledSequenceActive==0)
return;

ledSequenceCounter++;

if(ledBuzzerActive==1)
{
ledBuzzerCounter++;

if(ledBuzzerCounter<1000)
PORTH.B0=1;
else
{
ledBuzzerActive=0;
ledBuzzerCounter=0;
}
}

if(ledSequenceStep==1)
{
if(ledSequenceCounter>=3000)
{
ledSequenceStep=2;
ledSequenceCounter=0;
ledBuzzerCounter=0;
ledBuzzerActive=1;

LATE.B0=0;
LATE.B1=1;
LATE.B2=0;
LATE.B3=0;
}
}
else if(ledSequenceStep==2)
{
if(ledSequenceCounter>=3000)
{
ledSequenceStep=3;
ledSequenceCounter=0;
ledBuzzerCounter=0;
ledBuzzerActive=1;

LATE.B0=0;
LATE.B1=0;
LATE.B2=1;
LATE.B3=0;
}
}
else if(ledSequenceStep==3)
{
if(ledSequenceCounter>=3000)
{
ledSequenceStep=4;
ledSequenceCounter=0;
ledBuzzerCounter=0;
ledBuzzerActive=1;

LATE.B0=0;
LATE.B1=0;
LATE.B2=0;
LATE.B3=1;
}
}
else if(ledSequenceStep==4)
{
if(ledSequenceCounter>=4000)
{
ledSequenceActive=0;
ledSequenceCounter=0;

wardenServoActive=1;
wardenUnlockDone=1;
wardenBeepActive=1;
wardenBeepCount=0;
wardenBeepCounter=0;

if(wardenSignalSent==0)
wardenSignalPending=1;
}
}
}

void ResetEvents()
{
event45Done=0;
event43Done=0;
emergencyOn=0;
buzzerRemaining=0;
uvRemaining=0;
PORTH.B0=0;
AllLightsOff();
}

void StartTimer()
{
if(totalSeconds>0)
{
timerRunning=1;
timerStarted=1;

if(setMode==0)
{
if(emergencyOn==0)
SetLightsNormal();
}
}
}

void StopTimer()
{
timerRunning=0;
PORTH.B0=0;
}

void ResetTimer()
{
timerRunning=0;
timerStarted=0;
setMode=0;

totalSeconds=3000;
selectedSeconds=3000;

ResetEvents();

cellAUnlocked=0;
cellBUnlocked=0;
potTriggered=0;
cellBLeverLast=1;

laundryOpen=0;

extraServoActive=0;
extraSwitchLast=1;

ledSequenceActive=0;
ledSequenceStep=0;
ledSequenceCounter=0;
ledBuzzerCounter=0;
ledBuzzerActive=0;

wardenServoActive=0;
pressureWardenServoActive=0;
wardenUnlockDone=0;
wardenBeepActive=0;
wardenBeepCount=0;
wardenBeepCounter=0;
wardenSignalSent=0;
wardenSignalPending=0;

pressureMaxDone=0;

pressure30SignalSent=0;
pressure30SignalPending=0;

waterValue=0;
pressureValue=0;
drainValue=0;
calculatedPressure=0;

eastStage1Done=0;
eastStage2Done=0;
eastStage3Done=0;
eastCorridorServoActive=0;
eastCorridorDone=0;

EastCorridorLEDsOff();

PORTG.B3=0;
PORTG.B4=0;

PORTH.B1=0;
PORTH.B4=0;

LATE.B4=0;

SetLightsNormal();

displayUpdateRequired=1;
}

void IncreaseTime()
{
if(setMode==1)
{
if(totalSeconds<3600)
totalSeconds+=300;
}
else
{
if(totalSeconds<3600)
totalSeconds+=60;
}

if(totalSeconds>3600)
totalSeconds=3600;

selectedSeconds=totalSeconds;
displayUpdateRequired=1;
}

void DecreaseTime()
{
if(setMode==1)
{
if(totalSeconds>300)
totalSeconds-=300;
}
else
{
if(totalSeconds>300)
totalSeconds-=60;
}

if(totalSeconds<300)
totalSeconds=300;

selectedSeconds=totalSeconds;
displayUpdateRequired=1;
}

void EnterSetMode()
{
if(timerStarted==0)
{
setMode=1;
timerRunning=0;

totalSeconds=300;
selectedSeconds=300;

ResetEvents();

SetLightsUV();

displayUpdateRequired=1;
}
}

void ExitSetMode()
{
if(timerStarted==0)
{
setMode=0;
timerRunning=0;
selectedSeconds=totalSeconds;

ResetEvents();

AllLightsOff();

displayUpdateRequired=1;
}
}

void CheckPot()
{
unsigned int adcValue;

adcValue=ReadPot();

if(adcValue>=900)
{
if(potTriggered==0)
{
potTriggered=1;
cellAUnlocked=1;
}
}
else
{
potTriggered=0;
}
}

void CheckCellB()
{
unsigned char leverState;

leverState=PORTG.B0;

if(cellAUnlocked==1)
{
if(cellBUnlocked==0)
{
if((leverState==0)&&(cellBLeverLast==1))
cellBUnlocked=1;
}
}

cellBLeverLast=leverState;
}

void CheckPIR()
{
if(PORTG.B1==1)
{
motionDetected=1;
PORTH.B2=1;
PORTH.B3=0;
}
else
{
motionDetected=0;
PORTH.B2=0;
PORTH.B3=1;
}
}

void CheckExtraSwitch()
{
if(PORTG.B2==0)
{
extraServoActive=1;

if(extraSwitchLast==1)
StartLEDSequence();
}
else
{
extraServoActive=0;
}

extraSwitchLast=PORTG.B2;
}

void CheckValve()
{
if(PORTG.B2==0)
extraServoActive=1;
else
extraServoActive=0;
}

void UART1_Manual_Init()
{
TRISC.B6=0;
TRISC.B7=1;

TXSTA1=0x24;
RCSTA1=0x90;
BAUDCON1=0x00;
SPBRG1=129;
}

void CheckSlaveUART()
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

if(received=='O')
laundryOpen=1;
}
}

void CheckButtons()
{
if(buttonLock==1)
{
if(PORTC.B0==1&&PORTC.B1==1&&PORTC.B2==1&&PORTC.B3==1&&PORTC.B4==1&&PORTC.B5==1)
buttonLock=0;

return;
}

if(PORTC.B0==0)
{
buttonLock=1;
StartTimer();
return;
}

if(PORTC.B1==0)
{
buttonLock=1;
StopTimer();
return;
}

if(PORTC.B2==0)
{
buttonLock=1;
ResetTimer();
return;
}

if(PORTC.B3==0)
{
buttonLock=1;
IncreaseTime();
return;
}

if(PORTC.B4==0)
{
buttonLock=1;
DecreaseTime();
return;
}

if(PORTC.B5==0)
{
buttonLock=1;

if(timerStarted==0)
{
if(setMode==0)
EnterSetMode();
else
ExitSetMode();
}

return;
}
}

void interrupt()
{
if(INTCON.TMR0IF)
{
TMR0H=0xFD;
TMR0L=0x8F;
INTCON.TMR0IF=0;

DisplayScan();

servoCounter++;

if(servoCounter>=20)
servoCounter=0;

if(cellAUnlocked==1)
{
if(servoCounter<2)
PORTG.B3=1;
else
PORTG.B3=0;
}
else
PORTG.B3=0;

if(cellBUnlocked==1)
{
if(servoCounter<2)
PORTG.B4=1;
else
PORTG.B4=0;
}
else
PORTG.B4=0;

if(laundryOpen==1)
{
if(servoCounter<2)
PORTH.B1=1;
else
PORTH.B1=0;
}
else
PORTH.B1=0;

if(extraServoActive==1)
{
if(servoCounter<2)
PORTH.B4=1;
else
PORTH.B4=0;
}
else
PORTH.B4=0;

if((wardenServoActive==1)||(pressureWardenServoActive==1))
{
if(servoCounter<2)
LATE.B4=1;
else
LATE.B4=0;
}
else
{
LATE.B4=0;
}

if(eastCorridorServoActive==1)
{
if(servoCounter<2)
LATF.B3=1;
else
LATF.B3=0;
}
else
{
LATF.B3=0;
}

if(wardenBeepActive==1)
{
wardenBeepCounter++;

if(wardenBeepCount==0)
{
if(wardenBeepCounter<150)
PORTH.B0=1;
else
PORTH.B0=0;

if(wardenBeepCounter>=300)
{
wardenBeepCounter=0;
wardenBeepCount=1;
}
}
else if(wardenBeepCount==1)
{
if(wardenBeepCounter<150)
PORTH.B0=1;
else
PORTH.B0=0;

if(wardenBeepCounter>=300)
{
wardenBeepCounter=0;
wardenBeepCount=2;
}
}
else
{
wardenBeepActive=0;
PORTH.B0=0;
}
}
else if(ledSequenceActive==1&&ledBuzzerActive==1)
{
PORTH.B0=1;
}
else if(buzzerRemaining>0)
{
PORTH.B0=1;
buzzerRemaining--;
}
else
{
PORTH.B0=0;
}

if(uvRemaining>0)
{
LATA.B2=1;
uvRemaining--;
}

UpdateLEDSequence();

millisecondCounter++;

if(millisecondCounter>=1000)
{
millisecondCounter=0;

if(timerRunning)
{
if(totalSeconds>0)
{
totalSeconds--;
displayUpdateRequired=1;

if(totalSeconds==2700)
{
if(event45Done==0)
{
event45Done=1;
buzzerRemaining=3000;
uvRemaining=120000;
SetLightsUV();
}
}

if(totalSeconds==2580)
{
if(event43Done==0)
{
event43Done=1;
buzzerRemaining=3000;
uvRemaining=0;
emergencyOn=1;
SetLightsEmergency();
}
}
}
else
{
timerRunning=0;
buzzerRemaining=0;
uvRemaining=0;
PORTH.B0=0;
AllLightsOff();
displayUpdateRequired=1;
}
}
}
}
}

void main()
{
ADCON0=0x01;
ADCON1=0x00;
ADCON2=0xBE;
CMCON=0x07;

TRISA=0x01;
LATA=0x00;

TRISB=0x00;
PORTB=0x0F;

TRISC=0xFF;
PORTC=0x00;

TRISC.B0=1;
TRISC.B1=1;
TRISC.B2=1;
TRISC.B3=1;
TRISC.B4=1;
TRISC.B5=1;
TRISC.B6=0;
TRISC.B7=1;

TRISD=0x00;
PORTD=0x00;

TRISE=0x00;
LATE=0x00;

TRISF=0xF0;
LATF=0x00;

TRISG=0xFF;
PORTG=0x00;

TRISG.B0=1;
TRISG.B1=1;
TRISG.B2=1;
TRISG.B3=0;
TRISG.B4=0;
TRISG.B6=1;

PORTG.B3=0;
PORTG.B4=0;

TRISH=0xE0;
PORTH=0x00;

TRISH.B0=0;
TRISH.B1=0;
TRISH.B2=0;
TRISH.B3=0;
TRISH.B4=0;
TRISH.B5=1;
TRISH.B6=1;
TRISH.B7=1;

PORTH.B0=0;
PORTH.B1=0;
PORTH.B2=0;
PORTH.B3=1;
PORTH.B4=0;

TRISE.B4=0;
LATE.B4=0;

TRISF.B0=0;
TRISF.B1=0;
TRISF.B2=0;
TRISF.B3=0;

LATF.B0=0;
LATF.B1=0;
LATF.B2=0;
LATF.B3=0;

TRISF.B4=1;
TRISF.B5=1;
TRISF.B6=1;

UART1_Manual_Init();

T0CON=0x82;
TMR0H=0xFD;
TMR0L=0x8F;

INTCON.TMR0IF=0;
INTCON.TMR0IE=1;
INTCON.GIE=1;

totalSeconds=3000;
selectedSeconds=3000;
timerRunning=0;
timerStarted=0;
setMode=0;

cellAUnlocked=0;
cellBUnlocked=0;
potTriggered=0;
cellBLeverLast=1;

laundryOpen=0;
motionDetected=0;

extraServoActive=0;
extraSwitchLast=1;

ledSequenceActive=0;
ledSequenceStep=0;
ledSequenceCounter=0;
ledBuzzerCounter=0;
ledBuzzerActive=0;

wardenServoActive=0;
pressureWardenServoActive=0;
wardenUnlockDone=0;
wardenBeepActive=0;
wardenBeepCount=0;
wardenBeepCounter=0;
wardenSignalSent=0;
wardenSignalPending=0;

pressureMaxDone=0;

pressure30SignalSent=0;
pressure30SignalPending=0;

waterValue=0;
pressureValue=0;
drainValue=0;
calculatedPressure=0;

eastStage1Done=0;
eastStage2Done=0;
eastStage3Done=0;
eastCorridorServoActive=0;
eastCorridorDone=0;

buttonLock=0;

GreenLEDsOff();
EastCorridorLEDsOff();

ResetEvents();

SetLightsNormal();

UpdateDisplay();

while(1)
{
CheckSlaveUART();

if(pressure30SignalPending==1)
UART_SendPressure30();

if(wardenSignalPending==1&&wardenSignalSent==0)
UART_SendWarden();

CheckPot();
CheckCellB();
CheckPIR();
CheckValve();
CheckEastCorridorHG();
CheckPressure();
CheckButtons();

if(displayUpdateRequired)
{
displayUpdateRequired=0;
UpdateDisplay();
}
}
}