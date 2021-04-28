#include <stdio.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "CC1101_regs.h"
#include "CC1101.h"

#define bit_width 525e-6

void send_poweron(CC1101_t *cc1101)
{
	// it's hacking for generating power on pulse
	
	modemSetting(cc1101, 1/bit_width, false);
	updateModemSettings(cc1101);
	
	uint8_t txBuffer[20];
	for(int i=0; i<20; i++)
		txBuffer[i] = 0xff;
	
	RFSendPacket(cc1101, txBuffer, 7);
	nrf_delay_ms(220);
	RFSendPacket(cc1101, txBuffer, 5);
	nrf_delay_ms(118);
}

void send_poweron_2(CC1101_t *cc1101)
{
	// it's hacking for generating power on pulse
	
	modemSetting(cc1101, 1/bit_width, false);
	updateModemSettings(cc1101);
	
	uint8_t txBuffer[20];
	for(int i=0; i<20; i++)
		txBuffer[i] = 0xff;
	
	RFStartSendBuffer(cc1101, txBuffer, 7);
	nrf_delay_ms(30);
	RFSendTerminate(cc1101);
	nrf_delay_ms(220);
	RFStartSendBuffer(cc1101, txBuffer, 5);
	nrf_delay_ms(20);
	RFSendTerminate(cc1101);
	nrf_delay_ms(118);
}

void send_hormann_frame(CC1101_t *cc1101, uint8_t *buf, uint8_t len)
{
	// modemSetting(cc1101, 1/525e-6, false);
	// updateModemSettings(cc1101);
	
	RFSendPacket(cc1101, buf, len);
}

void send_hormann_frame_2(CC1101_t *cc1101, uint8_t *buf, uint8_t len, uint8_t repeat)
{
	RFStartSendBuffer(cc1101, buf, len);
	for(int i = 0; i < repeat; i++)
		RFSendBuffer(cc1101, buf, len);
	
	nrf_delay_ms(300);
	RFSendTerminate(cc1101);
}
