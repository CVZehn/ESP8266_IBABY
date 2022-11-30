
#ifndef DHT12Z_h
#define DHT12Z_h

#include "Arduino.h"

#define V4_I2C_ADDR 0x44
#define ACK 0
#define NACK 1

#define MIN_ELAPSED_TIME 2000
#define us_10  9 

// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

class DHT12_z {
public:
    DHT12_z(uint8_t sda, uint8_t scl);
    void IICstart();
    void IICstop();
    uint8_t IICrespons();
    void IICSendACK(uint8_t ack);
    void IICwrite_byte(uint8_t date);
    uint16_t Read_Rg(uint8_t AddRg);
    void ResetMD();
    uint8_t ReadDHTC12_M(float_t *tem,uint16_t *Hum);
    uint8_t SHT3x_CheckCrc(uint8_t Data[], uint8_t nbrOfBytes);
    uint8_t IICread_byte(void);
    void DHTC12_MInit();

private:
    uint8_t ACK_ret;
    
    uint8_t sda_pin = 5;
    uint8_t scl_pin = 4;
    uint16_t HumA;
    uint16_t HumB;

    uint16_t POLYNOMIAL = 0x131; //P(x)=x^8+x^5+x^4+1 = 100110001
};


#endif