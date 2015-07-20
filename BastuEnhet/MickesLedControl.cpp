// 
// 
// 

#include "MickesLedControl.h"


MickesLedControl::MickesLedControl(int dataPin,int clkPin, int csPin, int numOfDev)
: LedControl(dataPin,clkPin,csPin,numOfDev)
{
	
}
void MickesLedControl::initLedDisplay(int addr,int displayIntensity)
{
	//Slå på displayen
	shutdown(addr,false);
	//Ställ ljusstyrkan
	setIntensity(addr,displayIntensity);
	//Rensa displayen
	clearDisplay(addr);
	
	//Kolla att allt är inkopplat som det ska genom att tända alla 8 segment.
	//En siffra i taget.
	for (int i;i<8;++i){
		setChar(addr,i,'8',true);
		delay(100);
	}
	
	//Rensa displayen igen
	clearDisplay(addr);
}

// void MickesLedControl::initLedDisplay()
// {
// 	int addr = 0;
// 	int displayIntensity = 5;
// 	//Slå på displayen
// 	shutdown(addr,false);
// 	//Ställ ljusstyrkan
// 	setIntensity(addr,displayIntensity);
// 	//Rensa displayen
// 	clearDisplay(addr);
// 	
// 	//Kolla att allt är inkopplat som det ska genom att tända alla 8 segment.
// 	//En siffra i taget.
// 	for (int i;i<8;++i){
// 		setChar(addr,i,'8',true);
// 		delay(100);
// 	}
// 	
// 	//Rensa displayen igen
// 	clearDisplay(addr);
// }

void MickesLedControl::showError(int addr,int startdigit)
{
	for (int i = startdigit;i < startdigit + 4;++i){
		setChar(addr,i,'-',false);
	}
}

//Skrver C
void MickesLedControl::setCelciusSign(int addr,int digit)
{
	//Skriver ut ett c tecken uppe i hörnet på det segment som valts.
	
	//Rensa bort eventuelt skräp på segmentet
	setChar(addr,digit,' ',false);
	//Skriv ut c tecknet i övre hörnet av segmentet.
	setLed(addr,digit,1,true); //Tänd segmentet Högst upp.
	setLed(addr,digit,6,true); //Tänd segmentet Uppe till vänster.
	setLed(addr,digit,7,true); //Tänd segmentet i mitten.
}

void MickesLedControl::showNo(int addr,int startdigit, int value)
{
	//Rensa siffrorna
	for (int i = startdigit;i<startdigit + 3;++i){
		setChar(addr,i,' ',false);
	}
	int digit = startdigit;
	//Skriv siffra
	setChar(addr,digit,char(value),false);
	//skriv o i nästa position
	++digit;
	setLed(addr,digit,7,true);
	setLed(addr,digit,4,true);
	setLed(addr,digit,3,true);
	setLed(addr,digit,5,true);
	setLed(addr,digit,0,true);
	++digit;
	//skriv n i nästa position
	setLed(addr,digit,7,true);
	setLed(addr,digit,3,true);
	setLed(addr,digit,5,true);
	
}

void MickesLedControl::showHi(int addr,int startdigit)
{
	//skriv några ' ' för att rensa displayen
	for (int i = 0;i<4;++i){
		setChar(addr,startdigit + i,' ',false);
	}
	
	//Skriv ett 'H'
	setChar(addr,startdigit + 2,'H',false);
	
	//Skriv ett 'I'
	setChar(addr,startdigit + 1,'1',false);

	//Skriv ett '!'
	setLed(addr,startdigit,2,true);
	setLed(addr,startdigit,0,true);
}

void MickesLedControl::showLo(int addr,int startdigit)
{
	//skriv några ' ' för att rensa displayen
	for (int i = 0;i<4;++i){
		setChar(addr,startdigit + i,' ',false);
	}
		
	//Skriv ett 'L'
	setLed(addr,startdigit +2,6,true);
	setLed(addr,startdigit +2,5,true);
	setLed(addr,startdigit +2,4,true);
		
		
	//Skriv ett 'o'
	setLed(addr,startdigit +1,7,true);
	setLed(addr,startdigit +1,4,true);
	setLed(addr,startdigit +1,3,true);
	setLed(addr,startdigit +1,5,true);

		
	//Skriv ett '!'
	setLed(addr,startdigit,2,true);
	setLed(addr,startdigit,0,true);
		
}

