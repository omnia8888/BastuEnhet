// MickeDallasTemperature.h



#ifndef _MICKEDALLASTEMPERATURE_h
#define _MICKEDALLASTEMPERATURE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

static DeviceAddress sensoraddress[16];

class MickeDallasTemperature :private DallasTemperature
{

 public:
	MickeDallasTemperature(OneWire*);
	
	void InitSensors();
	float getSensorTempC(int sensorindex);
	
 private:
  OneWire* _onewire;
  int _numberofsensors;
};



#endif

