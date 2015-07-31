#include <LedControl.h>
#include <SoftwareSerial.h>
#include <DallasTemperature.h>
#include <OneWire.h>

//Micke egna bibliotek
#include "MickeDallasTemperature.h"
#include "RS485WithErrChk.h"
#include "SendReciveRemoteTemp.h"
/*
 * BastuEnhet.ino
 *
 * Created: 5/15/2015 4:13:04 PM
 * Author: Micke
 */ 

//Pin nummer konfigurationen som enheterna �r
//inkopplade p�
//#define LEDCONTROL_DATA_PIN 12
//#define LEDCONTROL_CLOCK_PIN 11
//#define LEDCONTROL_CS_PIN 10

#define ONE_WIRE_CONTROL_PIN 4

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin
#define SSerialTxControl 3   //RS485 Direction control

#define RS485_SERIAL_COM_SPEED 4800
#define NUM_OF_REMOTE_SENSORS 3

//Definera samplings hastighet som data fr�n
//frekvens ska h�mtas
#define  READ_FREQVENCY 1000

//Definerar hur ofta det l�gsta v�rdet fr�n sj� temperaturen ska l�sas av.
#define FREQTEMP_LOW_VALUE_UPDATE_INTERVALL 5000

//Skapa de globala objekten

OneWire oWire(ONE_WIRE_CONTROL_PIN);
MickeDallasTemperature mDallasTemp(&oWire);

RS485WithErrChk rs485 = RS485WithErrChk(SSerialRX,SSerialTX,SSerialTxControl);
SendReciveRemoteTemp sendRecvRTemp(&rs485,NUM_OF_REMOTE_SENSORS,5000);

float lowestfreqTemp = 0;
float currentFreqTemp = 0;
long lastFreqTempUpdate = 0;

//Beh�vs f�r att k�ra Freqcounter
//interupten nedan
volatile unsigned long firstPulseTime;
volatile unsigned long lastPulseTime;
volatile unsigned long numPulses;

void isr()
{
	unsigned long now = micros();
	if (numPulses == 0)
	{
		firstPulseTime = now;
	}
	else
	{
		lastPulseTime = now;
	}
	++numPulses;
}

void setup()
{
	Serial.begin(9600);
	  /* add setup code here, setup code runs once when the processor starts */
	Serial.println("Init Sensors..");
	
	mDallasTemp.InitSensors();
	
	rs485.InitRs485ComPort(RS485_SERIAL_COM_SPEED);
}

void loop()
{
	float sjoTemp;
	float bastuTemp;
	float uteTemp;
	
	//H�mta temperatur fr�n FreqCountern
	sjoTemp = GetLowestTempFromFreqCounter();
		
	//H�mta  temperatur fr�n Dallas sensorn  
	bastuTemp = mDallasTemp.getSensorTempC(0);
	uteTemp = mDallasTemp.getSensorTempC(1);

	Serial.print("Sjo Temperatur = ");
	Serial.println(sjoTemp);
	Serial.print("Bastu Temperatur = ");
	Serial.println(bastuTemp);
	Serial.print("Ute Temperatur = ");
	Serial.println(uteTemp);
	
	sendRecvRTemp.setTemperatureToSend(0,sjoTemp);
	sendRecvRTemp.setTemperatureToSend(2,bastuTemp);
	sendRecvRTemp.setTemperatureToSend(1,uteTemp);
	
	String sTemp;
	sTemp = sendRecvRTemp.checkForRemoteData();
	Serial.print("Recived request = ");
	Serial.println(sTemp);
}

float GetLowestTempFromFreqCounter()
{
	float tempFromFreqCount;
	//Kolla om den k�rs f�r f�rsta g�ngen i s�dant fall h�mta och returnera
	//en temperatur p� direktent
	if (lastFreqTempUpdate == 0) {
		//Updatera timern
		Serial.println("Setting freqTempStartValue");
		lastFreqTempUpdate = millis();
		tempFromFreqCount = GetTempFromFreqCounter(READ_FREQVENCY);
		lowestfreqTemp = tempFromFreqCount;
		return lowestfreqTemp;
	}
	
	if (lastFreqTempUpdate + FREQTEMP_LOW_VALUE_UPDATE_INTERVALL > millis()) {
		Serial.println("Reading FreqLowVal only");
		//Tr�skel v�rdet f�r updaterings intervallet har INTE uppn�tts,
		//s� l�gsta freqCount temperaturen ska bara l�sas av
		tempFromFreqCount = GetTempFromFreqCounter(READ_FREQVENCY);
		if (tempFromFreqCount < lowestfreqTemp) {
		//om det �r l�gst sparas undan.
			lowestfreqTemp = tempFromFreqCount;
		}
	}
	else {
		//Nytt l�gsta v�rde skall s�ttas
		Serial.println("Updating freqTemp Low Value");
		currentFreqTemp = lowestfreqTemp;
		lastFreqTempUpdate = millis();
	}
	return currentFreqTemp;
}

//H�mta temperatur fr�n freqCouner
float GetTempFromFreqCounter(int readfreq)
{
	//H�mta temperatur fr�n puls r�knare
	int freq ;
	// Measure the frequency over the specified sample time in milliseconds, returning the frequency in Hz
	numPulses = 0;                      // prime the system to start a new reading
	attachInterrupt(0, isr, RISING);    // enable the interrupt
	delay(readfreq);
	detachInterrupt(0);
	freq = (numPulses < 2) ? 0 : (1000000UL * (numPulses - 1))/(lastPulseTime - firstPulseTime);
	float temperatur = freq;
	return(temperatur/10);
}
