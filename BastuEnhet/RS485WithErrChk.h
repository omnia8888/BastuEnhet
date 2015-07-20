// RS485WithErrChk.h



#ifndef _RS485WITHERRCHK_h
#define _RS485WITHERRCHK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif




class RS485WithErrChk : public SoftwareSerial
{
 public:
	RS485WithErrChk(int RX_recivepin,int TX_transmitpin,int TX_controlpin); 
	
//	void InitComminucation(long as portSpeed);
	long getPortSpeed();

	void InitRs485ComPort(long portSpeed);
	void ReciveMode();
	void TransmitMode();
	
// 	void sendMsg (WriteCallback fSend,
// 	const byte * data, const byte length);
	void sendMessage(byte * msgContent, const byte lengtOfmsg);
	
	byte recvMessage(byte * msgContent,const byte lenghtOfmsg, long timeout=500);
	

 private:
	void sendComplemented (byte what);
	
	static byte calcCRC8(const byte *addr, byte len);
	
	int _txcontrolpin;
	
	long _portSpedd;



};



#endif
