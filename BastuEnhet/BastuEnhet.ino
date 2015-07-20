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

//Pin nummer konfigurationen som enheterna är
//inkopplade på
//#define LEDCONTROL_DATA_PIN 12
//#define LEDCONTROL_CLOCK_PIN 11
//#define LEDCONTROL_CS_PIN 10

#define ONE_WIRE_CONTROL_PIN 4

#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin
#define SSerialTxControl 3   //RS485 Direction control

#define RS485_SERIAL_COM_SPEED 4800
#define NUM_OF_REMOTE_SENSORS 3

//Definera samplings hastighet som data från
//frekvens ska hämtas
#define  READ_FREQVENCY 1000

//Skapa de globala objekten

OneWire oWire(ONE_WIRE_CONTROL_PIN);
MickeDallasTemperature mDallasTemp(&oWire);

RS485WithErrChk rs485 = RS485WithErrChk(SSerialRX,SSerialTX,SSerialTxControl);
SendReciveRemoteTemp sendRecvRTemp(&rs485,NUM_OF_REMOTE_SENSORS,5000);



//Behövs för att köra Freqcounter
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
	
	//Hämta temperatur från FreqCountern
	sjoTemp = GetTempFromFreqCounter(READ_FREQVENCY);
		
	//Hämta  temperatur från Dallas sensorn  
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
	
}

//Hämta temperatur från freqCouner
float GetTempFromFreqCounter(int readfreq)
{
	//Hämta temperatur från puls räknare
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
