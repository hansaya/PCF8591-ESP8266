/*
 * Written By Hans
 * This is my first version of the driver for PCF8591
 *
 */
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "c_types.h"

#include "user_interface.h"
#include "PCF8591.h"
#include "i2c/i2c.h"

#define PCF8591 0x90 // I2C bus address
// control bytes for reading individual ADCs
#define ADC0 0x00
#define ADC1 0x01
#define ADC2 0x02
#define ADC3 0x03

uint8 ADC0Data, ADC1Data, ADC2Data, ADC3Data;
#define DEBUG

///////////////////////////////////////////////////////////////
//PCF8591
///////////////////////////////////////////////////////////////
uint8 * ICACHE_FLASH_ATTR
readAllAnalog()
{
	char temp[128];
	#ifdef DEBUG
	uart0_sendStr("ADC Read started\r\n");
	#endif

	static uint8 data[4];

	data[0] = ADC0Read();
	data[1] = ADC1Read();
	data[2] = ADC2Read();
	data[3] = ADC3Read();

	#ifdef DEBUG
	os_sprintf(temp,"Value#:  ; %d ; %d ; %d ; %d \r\n\r\n",data[0],data[1],data[2],data[3]);
	uart0_sendStr(temp);
	#endif

    return data;
}


uint8 ICACHE_FLASH_ATTR ADC0Read(){
	i2c_init();
    i2c_start();
	i2c_writeByte(PCF8591);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 0 \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_writeByte(ADC0);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("failed at send data ack \n");
		#endif
		i2c_stop();
		return -1;
	}
   i2c_stop();

   i2c_start();
   uint8 readAdd = PCF8591; //shift the address
   readAdd = readAdd +0x1;				//add the read bit

   i2c_writeByte(readAdd);
   if (!i2c_check_ack())
   {
	#ifdef DEBUG
	uart0_sendStr("data not ack 001 \n");
	#endif
	   i2c_stop();
	   return -1;
   }
   ADC0Data=i2c_readByte();//read LSB
   i2c_send_ack(0);           //NACK READY FOR STOP
   i2c_stop();
   return ADC0Data;

}
uint8 ICACHE_FLASH_ATTR ADC1Read(){
	i2c_init();
	i2c_start();
    i2c_writeByte(PCF8591);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 1 \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_writeByte(ADC1);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("failed at send data ack \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_stop();

	i2c_start();
	uint8 readAdd = PCF8591; //shift the address
	readAdd = readAdd +0x1;				//add the read bit

	i2c_writeByte(readAdd);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 11 \n");
		#endif
		i2c_stop();
		return -1;
	}
	ADC1Data=i2c_readByte();//read LSB
	i2c_send_ack(0);           //NACK READY FOR STOP
	i2c_stop();
	return ADC1Data;
}
uint8 ICACHE_FLASH_ATTR ADC2Read(){
	i2c_init();
	i2c_start();
	i2c_writeByte(PCF8591);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 2 \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_writeByte(ADC2);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("failed at send data ack \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_stop();

	i2c_start();
	uint8 readAdd = PCF8591; //shift the address
	readAdd = readAdd +0x1;				//add the read bit

	i2c_writeByte(readAdd);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 22 \n");
		#endif
		i2c_stop();
		return -1;
	}
	ADC2Data=i2c_readByte();//read LSB
	i2c_send_ack(0);           //NACK READY FOR STOP
	i2c_stop();

	return ADC2Data;

}
uint8 ICACHE_FLASH_ATTR ADC3Read(){
	i2c_init();
	i2c_start();
	i2c_writeByte(PCF8591);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 3 \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_writeByte(ADC3);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("failed at send data ack \n");
		#endif
		i2c_stop();
		return -1;
	}
	i2c_stop();

	i2c_start();
	uint8 readAdd = PCF8591; //shift the address
	readAdd = readAdd +0x1;				//add the read bit

	i2c_writeByte(readAdd);
	if (!i2c_check_ack())
	{
		#ifdef DEBUG
		uart0_sendStr("data not ack 3 \n");
		#endif
		i2c_stop();
		return -1;
	}
	ADC3Data=i2c_readByte();//read LSB
	i2c_send_ack(0);           //NACK READY FOR STOP
	i2c_stop();
	return ADC3Data;
}
