// 
// 
// 

#include "TemperatureDisplay.h"

TemperatureDisplay::TemperatureDisplay(int displaynumber,int addr,int startdigit, MickesLedControl MLCobject)
: _displaynum(displaynumber),_displayaddr(addr),_startdigit(startdigit), MLC(MLCobject)
{

}

void TemperatureDisplay::showTemperature(float temperature)
{
	int decimal = 0;
	int one = 0;
	int ten = 0;
	boolean isnegative = false;
		
	//kolla om värdet över 99 och i sådant fall visa "Hi!"
	//på displayen
	if (temperature>99){
		MLC.showHi(_displayaddr,_startdigit);
		return;
	};
	//kolla om värdet är under -99 och i sådantfall visa "Lo!"
	if (temperature<-99){
		MLC.showLo(_displayaddr,_startdigit);
		return;
	};
	
	//Kolla om värdet är negativt
	if (temperature<0){
		isnegative = true;
		//gör om temperaturen till ett positivt tal
		temperature=temperature*-1;
	}
	//Delaupp talet i enskilda siffror
	
	int v = temperature * 10;
	decimal=v%10;
    v=v/10;
    one=v%10;
    v=v/10;
    ten=v;
	
	//Skriv ut en celcius symbol
	MLC.setCelciusSign(_displayaddr,_startdigit);
	
	//kolla om värdet överstiger -10 isådant fall
	//flytta fram startpositionen annars inte
	int digitposition = _startdigit;
	if (!isnegative) {
		++digitposition;
	}
	else if(temperature<10){
		++digitposition;
	}
	
	//Skriv ut siffrorna
	MLC.setTemperatureDigit(_displayaddr,digitposition,char(decimal),false);
	++digitposition;
	MLC.setTemperatureDigit(_displayaddr,digitposition,char(one),true);
	++digitposition;
	//kolla om det är en nolla och i sådant fall inte skriva ut den
	if (ten==0){
		MLC.setTemperatureDigit(_displayaddr,digitposition,char(' '),false);
	}
	else{
		MLC.setTemperatureDigit(_displayaddr,digitposition,char(ten),false);
	}

	
	//Skriv ut minustecken om temperaturen är under 0
	if (isnegative){
		//kolla vart minustecknet skall sitta beroende på det visade värdets längd
		if (ten==0){ 
			MLC.setTemperatureDigit(_displayaddr,digitposition,'-',false);
			//Rensa sista tecknet så inget blir kvar
			++digitposition;
			MLC.setTemperatureDigit(_displayaddr,digitposition,char(' '),false);
		}
		else{
			++digitposition;
			MLC.setTemperatureDigit(_displayaddr,digitposition,'-',false);
		}
	}
	
	
}

void TemperatureDisplay::showDisplayNumber()
{
	MLC.showNo(_displayaddr,_startdigit,_displaynum);
}

void TemperatureDisplay::PrintC()
{
	MLC.setCelciusSign(0,_startdigit);
}

void TemperatureDisplay::SetStartPosition(int startpos)
{
	_startdigit = startpos;
}

void TemperatureDisplay::SetDisplayNum(int displaynum)
{
	_displaynum = displaynum;
}

int TemperatureDisplay::GetDisplayNum()
{
	return(_displaynum);
}


