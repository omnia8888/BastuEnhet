// 
// 
// 
#include "SoftwareSerial.h"
#include "RS485WithErrChk.h"


#define RS485Transmit    HIGH
#define RS485Receive     LOW

const byte STX = '\2';
const byte ETX = '\3';

RS485WithErrChk::RS485WithErrChk(int RX_recivepin,int TX_transmitpin,int TX_controlpin)
: SoftwareSerial(RX_recivepin,TX_transmitpin)
{
	_txcontrolpin = TX_controlpin;
	pinMode(TX_controlpin,OUTPUT);
}

// void RS485WithErrChk::InitComminucation(long as portSpeed)
// {
// 	
// 	//Ställ in mottagnings läge
// 	ReciveMode();
// 	//ställ in kommunikations hastigheten
// 	begin(portSpeed);
// 	
// }

long RS485WithErrChk::getPortSpeed()
{
	return(_portSpedd);
}

void RS485WithErrChk::InitRs485ComPort(long portSpeed)
{
	_portSpedd = portSpeed;
	//ställ in kommunikations hastigheten
	begin(portSpeed);
	//Ställ in mottagnings läge
	ReciveMode();
	//Pausa så att porten hinner initsieras	
	delay(100);
}

void RS485WithErrChk::ReciveMode()
{
	digitalWrite(_txcontrolpin,RS485Receive);
	delay(1);
}

void RS485WithErrChk::TransmitMode()
{
	digitalWrite(_txcontrolpin,RS485Transmit);
	delay(1);
}

void RS485WithErrChk::sendMessage(byte * msgContent, const byte lengtOfmsg)
{
	  TransmitMode();
	  write (STX);  // STX
	  for (byte i = 0; i < lengtOfmsg; i++)
	  sendComplemented (msgContent [i]);
	  write (ETX);  // ETX
	  sendComplemented (calcCRC8 (msgContent, lengtOfmsg));	
	  delayMicroseconds (660);
	  ReciveMode();
}

byte RS485WithErrChk::recvMessage(byte * msgContent,const byte lenghtOfmsg, long timeout)
{
	unsigned long start_time = millis ();
	
	bool have_stx = false;

	// variables below are set when we get an STX
	bool have_etx;
	byte input_pos;
	bool first_nibble;
	byte current_byte;

	while (millis () - start_time < timeout)
	{
		if (available () > 0)
		{
			byte inByte = read ();
			
			switch (inByte)
			{
				
				case STX:   // start of text
				have_stx = true;
				have_etx = false;
				input_pos = 0;
				first_nibble = true;
				start_time = millis ();  // reset timeout period
				break;
				
				case ETX:   // end of text
				have_etx = true;
				break;
				
				default:
				// wait until packet officially starts
				if (!have_stx)
				break;
				
				// check byte is in valid form (4 bits followed by 4 bits complemented)
				if ((inByte >> 4) != ((inByte & 0x0F) ^ 0x0F) )
				return 0;  // bad character
				
				// convert back
				inByte >>= 4;
				
				// high-order nibble?
				if (first_nibble)
				{
					current_byte = inByte;
					first_nibble = false;
					break;
				}  // end of first nibble
				
				// low-order nibble
				current_byte <<= 4;
				current_byte |= inByte;
				first_nibble = true;
				
				// if we have the ETX this must be the CRC
				if (have_etx)
				{
					if (calcCRC8 (msgContent, input_pos) != current_byte)
					return 0;  // bad crc
					return input_pos;  // return received length
				}  // end if have ETX already
				
				// keep adding if not full
				if (input_pos < lenghtOfmsg)
				msgContent [input_pos++] = current_byte;
				else
				return 0;  // overflow
				break;
				
			}  // end of switch
		}  // end of incoming data
	} // end of while not timed out
	
	return 0;  // timeout
}

void RS485WithErrChk::sendComplemented(byte what)
{
		byte c;

		// first nibble
		c = what >> 4;
		write ((c << 4) | (c ^ 0x0F));

		// second nibble
		c = what & 0x0F;
		write ((c << 4) | (c ^ 0x0F));
}

byte RS485WithErrChk::calcCRC8(const byte *addr, byte len)
{
		byte crc = 0;
	while (len--)
	{
		byte inbyte = *addr++;
		for (byte i = 8; i; i--)
		{
			byte mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix)
			crc ^= 0x8C;
			inbyte >>= 1;
		}  // end of for
	}  // end of while
	return crc;
}  // end of crc8


