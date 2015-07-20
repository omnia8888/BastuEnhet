// 
// 
// 
#include "OneWire.h"
#include "DallasTemperature.h"
#include "MickeDallasTemperature.h"

DeviceAddress sensor0 = {0x28, 0x91, 0x3E, 0xD0, 0x05, 0x00, 0x00, 0x8A };



MickeDallasTemperature::MickeDallasTemperature(OneWire* _onewire)
: DallasTemperature(_onewire)
{

}

// Initsierar och strtar upp sensorna och räknar dem
// Loopar sedan igenom den och tar fram sensor adresserna
// och sparar ned dem
void MickeDallasTemperature::InitSensors()
{
	
	//starta data insamlingen frånsensorna
	begin();
	
	//ställ in upplösningen. Rekomenderat värde är 10.
	setResolution(12);
	
	//hämta data för alla sensorer.
	//behövs för att bla. kunna räkna dem
	requestTemperatures();
	
	//Räkna antalet sensorer
	_numberofsensors = getDeviceCount();
	
	//Loppa igen om alla inkopplade sensorer
	//och sparar undan sensoradresserna
	for (int i = 0;i < _numberofsensors;i++){
	  boolean b = getAddress(sensoraddress[i],i);
	}
	float f;
	f = getTempC(sensoraddress[0]);
	return;	
}

//Uppdaterar och hämtar temperatur från en givare med
//motsvarande nummer från sensor indexet på den
//inkopplade givarna
float MickeDallasTemperature::getSensorTempC(int sensorindex)
{
	//Uppdatera temperaturer från givarna
	requestTemperatures();
	//hämta temperatur från en av givara
	float temperature = getTempC(sensoraddress[sensorindex]);
	return(temperature);			
}


