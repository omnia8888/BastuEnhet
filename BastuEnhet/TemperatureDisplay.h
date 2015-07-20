// TemperatureDisplay.h
#include "LedControl.h"
#include "MickesLedControl.h"

#ifndef _TEMPERATUREDISPLAY_h
#define _TEMPERATUREDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class TemperatureDisplay
{
 protected:


 public:
	TemperatureDisplay(int displaynumber,int addr,int startdigit, MickesLedControl MLCobject);
	
	void showTemperature(float temperature);
	
	void showDisplayNumber();
	
	void SetDisplayNum(int displaynum);
	int GetDisplayNum();
	void SetStartPosition(int startpos);
	void PrintC();
	
	
 private:
	int _displaynum;
	int _startdigit;
	int _displayaddr;
	MickesLedControl MLC;
};



#endif

