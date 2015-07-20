// 
// 
// 

#include "SendReciveRemoteTemp.h"
//#include "MickeDebug.h"



#define Pin13LED  13

//#define RequestTimeout 5000



SendReciveRemoteTemp::SendReciveRemoteTemp(RS485WithErrChk* RS,byte numOfRemoteSensors,long requestSendIntervall)
:_rs485(RS),_numOfSensors(numOfRemoteSensors),_requestSendIntervall(requestSendIntervall)
{
	pinMode(Pin13LED,OUTPUT);
	_requestSentTime = millis()+_requestSendIntervall;
}

//Kollar om det finns mottaget data i RS485 Bufferten och parsar detta
//läser ur mom det tex är ett '@' för request el '$' för givar temperatur eller
// om det är ett err som mottagits
String SendReciveRemoteTemp::checkForRemoteData()
{
	byte recvBuf[10];
	String returnVal;
	//Hämta data från bufferten
	returnVal = reciveData(recvBuf);
	//Kolla om det inte har hämtats något data eller om
	//något blivit fel och returnera detta.
	if (returnVal == "" || returnVal == "err")
	{
		return(returnVal);
	}
	
	//Kolla om det är en request som mottagits
	if (char(recvBuf[0]) == '@')
	{
		//Skicka sensor data för alla sensorer en i taget.
		for (int i = 0;i < _numOfSensors; i++)
		{
			sendRemoteTemperature(i);
		}
		return(returnVal);
	}
	
	//Kolla om det är temperatur data som mottagits
	if (char(recvBuf[0]) == '$')
	{
		//Läs av vilken sensor det gäller
		byte recvSensIndex = char(recvBuf[1]);
		String sTemp;
		//Läs in den mottagna temperaturen
		for (int i = 2; i < 7; ++i)
		{
			sTemp += char(recvBuf[i]);
		}
		_remoteTemp[recvSensIndex] = sTemp.toFloat();
		return(sTemp);
	}
	
	return(returnVal);
	
}

//Returnerar true om requesten har skickats annars false
boolean SendReciveRemoteTemp::sendRemoteRequest(int sensorIndex)
{
		//Kolla om det redan finns en request på gång
		if (millis() < _requestSentTime + _requestSendIntervall)
		{
			return(false);
		}
		
		digitalWrite(Pin13LED,HIGH);
		//sätt ihop command strängen
		String cmdString = "@";
		cmdString += sensorIndex;
		//cmdString += "-432.1!";
		cmdString += "000000!";
		//skapa byte array och lägg in strängen i den
		byte byteBuf[10];
		cmdString.getBytes(byteBuf,sizeof byteBuf);
		//Skicka commandot till RS485
		//sendMessage(byteBuf,sizeof byteBuf);
		_rs485->sendMessage(byteBuf,sizeof byteBuf);
		//sätt request timern
		_requestSentTime = millis();
		digitalWrite(Pin13LED,LOW);
		return(true);
}

String SendReciveRemoteTemp::sendRemoteTemperature(byte sensorIndex)
{
			digitalWrite(Pin13LED,HIGH);
			String temp = String(_remoteTemp[sensorIndex]);
			
			//sätt ihop command strängen
			String cmdString = "$";
			cmdString += sensorIndex;
			//Lägg in temperaturen
			cmdString += temp;
			//Fyll på med 0:or
			for (int i = cmdString.length(); i < 8; ++i)
			{
				cmdString += "0";
			}
			cmdString += "!";
			
			//skapa byte array och lägg in strängen i den
			byte byteBuf[10];
			cmdString.getBytes(byteBuf,sizeof byteBuf);
			//Skicka commandot till RS485
			//sendMessage(byteBuf,sizeof byteBuf);
			_rs485->sendMessage(byteBuf,sizeof byteBuf);
			//sätt request timern
//			_requestSentTime = millis();
			digitalWrite(Pin13LED,LOW);
			return(cmdString);
}

//Hämtar data från RS485 bufferten och returnerar en till hyfsad sträng
//med dessa värden eller "error" om något gått fel.
String SendReciveRemoteTemp::reciveData(byte * recvBuf)
{
		byte recvTempBuf[10];
		byte recivedByteCount;
		String recvData;
		
		 //Kolla om det finns någon data i buffert annars hoppa ur
		 if (_rs485->available() == 0)
		 {
			 return("");
		 }

		//Hämta data från bufferten
		recivedByteCount = _rs485->recvMessage(recvTempBuf,sizeof recvTempBuf);
		
		//Kolla att sista tecknet är ett '!' om inte hoppa ur
		if (recvTempBuf[8] != char('!'))
		{
			return("err");
		}
		
		//Kolla att första tecknet är ett '@' eller '$'
		if (recvTempBuf[0] != char('@') && recvTempBuf[0] != char('$'))
		{
			return("err");
		}
		
		//Sätt ihop en sträng av de mottagna värderna
		for (int i = 0;i <= 8; i++) {
			recvBuf[i] = recvTempBuf[i];
			recvData += char(recvBuf[i]);
		}
		
		return(recvData);
		 
		 
	
}

void SendReciveRemoteTemp::setTemperatureToSend(byte sensorIndex,float temperature)
{
	_remoteTemp[sensorIndex] = temperature;
}

float SendReciveRemoteTemp::getRemoteTemperature(byte sensorIndex)
{
	return(_remoteTemp[sensorIndex]);
}




