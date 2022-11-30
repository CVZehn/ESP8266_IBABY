
#include "dht12z.h"


#if 0
void delayMicroseconds(1)		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 1;
	while (--i);
}

/*
void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 16;
	while (--i);
}
*/

void delayMicroseconds(us_10)		//@11.0592MHz
{
	unsigned char i;

	i = 35;
	while (--i);
}


void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 15;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}


void Delay2ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 29;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}
#endif

//	pinMode(sda_pin, OUTPUT);
//	digitalWrite(sda_pin, LOW);
DHT12_z::DHT12_z(uint8_t sda, uint8_t scl)
{
    sda_pin = sda;
    scl_pin = scl;
}

void DHT12_z::IICstart()
{
	digitalWrite(sda_pin, HIGH);
	delayMicroseconds(us_10); //IIC ?????? 0~100Hz
	digitalWrite(scl_pin, HIGH);
	delayMicroseconds(us_10);
	digitalWrite(sda_pin, LOW);
	delayMicroseconds(us_10);
	digitalWrite(scl_pin, LOW);
}

void DHT12_z::IICstop()
{
	digitalWrite(sda_pin, LOW);
	delayMicroseconds(us_10);
	digitalWrite(scl_pin, HIGH);
	delayMicroseconds(us_10);
	digitalWrite(sda_pin, HIGH);
	delayMicroseconds(us_10);
}

uint8_t DHT12_z::IICrespons()
{
	uint8_t i=0,error=0;//?????? WaitACK,?????? SDA
	//HDCSDA_Input();
	pinMode(sda_pin, INPUT_PULLUP);
	delayMicroseconds(us_10);
	digitalWrite(scl_pin, HIGH);
	delayMicroseconds(us_10);
	ACK_ret = 0;
	while(((digitalRead(sda_pin))!=0)&&i<50)i++;
	if(i>45)
	{
		error = 1;
		ACK_ret = 1;
	}
	digitalWrite(scl_pin, LOW);//SCK:0 ???? SDA
	delayMicroseconds(us_10);
	pinMode(sda_pin, OUTPUT);
	return error;
}


void DHT12_z::IICSendACK(uint8_t ack)
{
	if(ack == 0)
	{
		digitalWrite(sda_pin, LOW);
	}
	else
	{
		digitalWrite(sda_pin, HIGH);
	}
	delayMicroseconds(us_10);
	digitalWrite(scl_pin, HIGH);
	delayMicroseconds(us_10);
	digitalWrite(scl_pin, LOW);
	delayMicroseconds(us_10);
	digitalWrite(sda_pin, HIGH);
}


void DHT12_z::IICwrite_byte(uint8_t date)
{
	uint8_t i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		digitalWrite(scl_pin, LOW);
	// delay(10);
		if((temp&0x80)==0x80)
		{
			digitalWrite(sda_pin, HIGH);
		}
		else
		{
			digitalWrite(sda_pin, LOW);
		}
		delayMicroseconds(us_10);
		digitalWrite(scl_pin, HIGH);
		delayMicroseconds(us_10);
		temp=temp<<1;
	}
	digitalWrite(scl_pin, LOW);
	delayMicroseconds(us_10);
	// digitalWrite(sda_pin, HIGH);
	// delay(10);
}


uint8_t DHT12_z::IICread_byte(void)
{
	uint8_t i,k;
	pinMode(sda_pin, INPUT_PULLUP);
	delayMicroseconds(us_10);
	for(i=0;i<8;i++)
	{
		digitalWrite(scl_pin, HIGH);
		delayMicroseconds(us_10);
		k <<= 1;
		if(digitalRead(sda_pin))//==0x04
		{
			k |= 0x01;
		}
		digitalWrite(scl_pin, LOW);//SCK:0 ???? SDA
		delayMicroseconds(us_10);
	}
	pinMode(sda_pin, OUTPUT);
	return k;
}


uint16_t DHT12_z::Read_Rg(uint8_t AddRg)
{
	uint8_t errRe,ReadDatSH[3];
	uint16_t ReDat;
	IICstart();
	IICwrite_byte(V4_I2C_ADDR<<1);// Add+W
	delayMicroseconds(1);
	IICrespons();
	IICwrite_byte(0xD2);
	IICrespons();
	IICwrite_byte(AddRg);
	IICrespons();
	delayMicroseconds(us_10);
	IICstart();
	IICwrite_byte(V4_I2C_ADDR<<1|0x01);// Add+R
	errRe=IICrespons();
	//if(ACK_ret == 0)
	//{
	ReadDatSH[0] = IICread_byte();
	IICSendACK(ACK);
	ReadDatSH[1] = IICread_byte();
	IICSendACK(ACK);
	ReadDatSH[2] = IICread_byte();
	IICSendACK(NACK);
	//}
	IICstop();
	errRe = SHT3x_CheckCrc(ReadDatSH,2);
	if(errRe==0)
	{
		ReDat = ReadDatSH[0];
	}else
	{
		ReDat = 0xff;
	}
	return ReDat;
}

//Èí¸´Î»
void DHT12_z::ResetMD()
{
	IICstart();
	delay(1);
	IICwrite_byte(V4_I2C_ADDR<<1);// Add+W
	delay(1);
	delayMicroseconds(1);
	IICrespons();
	IICwrite_byte(0x30);
	IICrespons();
	IICwrite_byte(0xA2);
	IICrespons();
	IICstop();
}





void DHT12_z::DHTC12_MInit()
{
	
	pinMode(sda_pin, OUTPUT);
	pinMode(scl_pin, OUTPUT);
	digitalWrite(sda_pin, HIGH);
	delayMicroseconds(2000); //10us »òÕß²»ÓÃ

	ResetMD();
	delayMicroseconds(us_10); //10us »òÕß²»ÓÃ
	HumA = Read_Rg(8);
	HumA = (HumA<<8)|Read_Rg(9);
	HumB = Read_Rg(10);
	HumB = (HumB<<8)|Read_Rg(11);
}



uint8_t DHT12_z::ReadDHTC12_M(float_t *tem,uint16_t *Hum)
{
	uint8_t i,errRe,ReadDatSH[6],CalCRC[3],errorflag;
	s16 TemBuf;
	long CapBuf;//s32
	IICstart();
	IICwrite_byte(V4_I2C_ADDR<<1);// Add+W
	delayMicroseconds(1);
	IICrespons();
	IICwrite_byte(0x2c);//µ¥×Ö½Ú¶ÁÈ¡¼Ä´æÆ÷  Ö¸Áî D2xx
	IICrespons();
	IICwrite_byte(0x10);
	IICrespons();
	IICstop();
	delay(2);//2ms SCL¿ÕÏÐ×îÉÐ¡ 1ms
	for(i=0;i<5;i++)//²éÑ¯5´Î¿´²âÍê½á¹û
	{
		delay(1);//1ms
		delay(1);
		delay(1);
		delay(1);
		IICstart();
		IICwrite_byte(V4_I2C_ADDR<<1|0x01);// Add+R
		errRe=IICrespons();
			if(errRe ==0)
		break;//²âÁ¿Íê³É
		else
		IICstop();
	}
	if(errRe == 0)
	{
		ReadDatSH[0] = IICread_byte();
		IICSendACK(ACK);
		ReadDatSH[1] = IICread_byte();
		IICSendACK(ACK);
		ReadDatSH[2] = IICread_byte();
		IICSendACK(ACK);
		ReadDatSH[3] = IICread_byte();
		IICSendACK(ACK);
		ReadDatSH[4] = IICread_byte();
		IICSendACK(ACK);
		ReadDatSH[5] = IICread_byte();
		IICSendACK(NACK);
		IICstop();
	}
	else
	{
		Serial.println("dht12 read err");
	}
	CalCRC[0] = ReadDatSH[0];
	CalCRC[1] = ReadDatSH[1];
	CalCRC[2] = ReadDatSH[2];
	errorflag = SHT3x_CheckCrc(CalCRC,2);
	if(errorflag==0)
	{
		TemBuf = (uint16_t)ReadDatSH[0]<<8|(ReadDatSH[1]);
		TemBuf = 400+TemBuf/25.6;//*10 ½á¹û*10±¶ 286 ¼´ 28.6¡æ
		*tem = TemBuf;
	}
	else
	{
		Serial.println("crc err");
	}
	CalCRC[0] = ReadDatSH[3];
	CalCRC[1] = ReadDatSH[4];
	CalCRC[2] = ReadDatSH[5];
	errorflag |= SHT3x_CheckCrc(CalCRC,2);
	if(errorflag==0)
	{
	CapBuf = (uint16_t)ReadDatSH[3]<<8|(ReadDatSH[4]);
	CapBuf = (CapBuf-HumB)*600/(HumA-HumB)+300;
	//20¡æÎª5¸öÊª¶Èµã  ¼´1¡æÎª0.25¸öÊª¶Èµã  0.1¡æÎª0.025
	CapBuf = CapBuf+ 25*(TemBuf-250)/100;
	if(CapBuf>1000)
		CapBuf = 999;
	else if(CapBuf<0)
		CapBuf = 0;
	*Hum = (uint16_t)CapBuf;//Í¬Ñù½á¹û*10
	}
	return errorflag;	
}


uint8_t DHT12_z::SHT3x_CheckCrc(uint8_t Data[], uint8_t nbrOfBytes) //¸úSHT30Ò»ÑùÖ»ÊÇ½«CRCÖµµ¹»Øµ½Dataºó·µ»Ø
//==============================================================================
{
	uint8_t crc = 0xff; //0
	uint8_t byteCtr,Bit;
	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
	{ crc ^= (Data[byteCtr]);
		for (Bit = 8; Bit > 0; --Bit)
		{ if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
		else crc = (crc << 1);
		}
	}
	if (crc != Data[nbrOfBytes])
	{
	Data[nbrOfBytes] = crc;
	return 0x01;
	}
	else return 0;
}





