// MickesLedControl.h
#include "LedControl.h"

#ifndef _MICKESLEDCONTROL_h
#define _MICKESLEDCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class MickesLedControl : LedControl
{
 protected:


 public:
    /* 
    * Create a new controller 
    * Params :
    * dataPin		pin on the Arduino where data gets shifted out
    * clockPin		pin for the clock
    * csPin		pin for selecting the device 
    * numDevices	maximum number of devices that can be controlled
    */
	MickesLedControl(int dataPin,int clkPin,int csPin, int numOfDev);
 
	
	/*
	* disables the shutdown (power saving) mode for the device
	* by setting it to false.
	* Sets the brightness of the display.
	* Switches all Leds on the display off.
	* Runs a test where all segments is turned on
	* one by on from digit 0 to 7
	* Params:
	* addr		the address of the display to control
	* intensity	the brightness of the display. (0..15)
*/
	void initLedDisplay(int addr,int DisplayIntensity=5);
	void initLedDisplay();
	
	/* 
    * Display a small 'c' character in upper part
	* on a 7-Segment display.
    * Params:
    * addr	address of the display
    * digit	the position of the character on the display (0..7)
    */
	void setCelciusSign(int addr,int digit);
	

	
	void setTemperatureDigit(int addr,int digit, char value, boolean dp){
		setChar(addr,digit,value,dp);
	}
	
	void showError(int addr,int startdigit);
	
	void showNo(int addr, int startdigit, int value);
	
	void showHi(int addr, int startdigit);
	void showLo(int addr, int startdigit);
	
};

//extern MickesLedControl MickesLedControl;

#endif

