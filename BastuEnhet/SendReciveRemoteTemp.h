// SendReciveRemoteTemp.h

#ifndef _SENDRECIVEREMOTETEMP_h
#define _SENDRECIVEREMOTETEMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SoftwareSerial.h>
#include "RS485WithErrChk.h"

class SendReciveRemoteTemp
{
 public:
	SendReciveRemoteTemp(RS485WithErrChk*,byte numOfRemoteSensors, long requestSendIntervall);
	
	String checkForRemoteData();
	
	boolean sendRemoteRequest(int sensorIndex);
	
	String sendRemoteTemperature(byte sensorIndex);
	
	String reciveData(byte * recvBuf);
	
	void setTemperatureToSend(byte sensorIndex,float temperature);
	float getRemoteTemperature(byte sensorIndex);
	

	
 
 private:
	RS485WithErrChk* _rs485;
	byte _numOfSensors;
	long _requestSendIntervall;
	
	float _remoteTemp[10];
	
	long _requestSentTime;

};



#endif

