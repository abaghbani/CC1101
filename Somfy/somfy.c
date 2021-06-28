#include <stdio.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "CC1101_regs.h"
#include "CC1101.h"
#include "nrfx_timer.h"
#include "timer.h"
#include "somfy.h"

/*
https://pushstack.wordpress.com/somfy-rts-protocol/
Parameters
Frequency: 433.42 Mhz
Modulation: ASK/OOK
Encoding: Manchester code, rising edge = 1, falling edge = 0
Payload Data length: 56 bit
Symbol Width: 1208 us

preamble : Repeating sequence of n symbol "11110000" (~2416us high and 2416us low). When a button is pressed n=2, repeating frames have n=7
sync : 4550 us high, followed by 1/2 symbol width low(~604 us)
data: data is Manchester encoded, 1=rising edge, 0=falling edge. One bit is encoded per symbol(~1208 us)
Inter-frame gap: 30415 us of silence after last symbol. (Note: This means that if last bit = 0, then the line will be silent 1/2 symbol width longer)
*/

const uint16_t somfy_rts_interval = 1240; // symbol width in us -> ca. 828 Hz data rate
const uint16_t somfy_rts_interval_half = somfy_rts_interval/2;
const uint8_t SOMFY_RTS_FRAME_SIZE = 7;

void SendPowerOn(CC1101_t *cc1101)
{
	// it's hacking for generate preamble of somfy_rts
	modemSetting(cc1101, 1/302e-6, false);
	updateModemSettings(cc1101);
	uint8_t txBuffer[] = {0xff, 0xff, 0xff, 0xff, 0xff};
	RFSendPacket(cc1101, txBuffer, 5);
}

static void SendPreamble(CC1101_t *cc1101, uint8_t preamble_num)
{
	// it's hacking for generate preamble of somfy_rts
	modemSetting(cc1101, 1/302e-6, false);
	updateModemSettings(cc1101);
	uint8_t txBuffer[2*preamble_num+2];
	for(uint8_t i = 0; i<preamble_num; i++)
	{
		txBuffer[2*i] = 0x7f;
		txBuffer[2*i+1] = 0x80;
	}
	txBuffer[2*preamble_num] = 0x7f;
	txBuffer[2*preamble_num+1] = 0xff;

	RFSendPacket(cc1101, txBuffer, 2*preamble_num+2);
}

static void SendPayload(CC1101_t *cc1101, uint8_t *buf)
{
	modemSetting(cc1101, 1/604e-6, true);
	updateModemSettings(cc1101);
	uint8_t txBuf[SOMFY_RTS_FRAME_SIZE];
	for (int8_t i = 0; i < SOMFY_RTS_FRAME_SIZE; i++)
	{
		txBuf[i] = ~buf[i];
	}
	RFSendPacket(cc1101, txBuf, 3);
	RFSendPacket(cc1101, txBuf+3*(sizeof(txBuf[0])), SOMFY_RTS_FRAME_SIZE-3);
}

void SendSomfyFrame(CC1101_t *cc1101, uint8_t preamble_num, uint8_t *buf)
{
	modemSetting(cc1101, 1/620e-6, false);
	updateModemSettings(cc1101);
	
	uint8_t txBuffer[preamble_num+1+2*SOMFY_RTS_FRAME_SIZE];
	for(uint8_t i = 0; i<preamble_num; i++)
	{
		txBuffer[i] = 0xf0;
	}
	txBuffer[preamble_num] = 0xfe;

	for (int8_t i = 0; i < SOMFY_RTS_FRAME_SIZE; i++)
	{
		txBuffer[preamble_num+1+2*i] = 0;
		txBuffer[preamble_num+1+2*i+1] = 0;
		for(int8_t j = 7; j>=4; j--)
		{
			if((buf[i]&(1<<j)) != 0)
				txBuffer[preamble_num+1+2*i] |= 1<<(2*(j%4));
			else
				txBuffer[preamble_num+1+2*i] |= 2<<(2*(j%4));
		}
		for(int8_t j = 3; j>=0; j--)
		{
			if((buf[i]&(1<<j)) != 0)
				txBuffer[preamble_num+1+2*i+1] |= 1<<(2*(j%4));
			else
				txBuffer[preamble_num+1+2*i+1] |= 2<<(2*(j%4));
		}
	}
	RFSendPacket(cc1101, txBuffer, preamble_num+1+2*SOMFY_RTS_FRAME_SIZE);
}

void SendSomfyFrame_old(CC1101_t *cc1101, uint8_t preamble_num, uint8_t *buf)
{
	SendPreamble(cc1101, preamble_num);
	SendPayload(cc1101, buf);
}

static uint8_t somfy_rts_calc_checksum(uint8_t *frame)
{
	uint8_t checksum = 0;
	for (int8_t i = 0; i < SOMFY_RTS_FRAME_SIZE; i++) {
		checksum = checksum ^ (frame[i]&0x0f) ^ ((frame[i] >> 4)&0x0f);
	}

	return (checksum & 0x0f);
}

void somfy_make_frame(uint8_t *buf, uint8_t ctrl, uint16_t rolling_code, uint32_t address)
{	
	// create somfy frame from the given input
	// 0   |    1     |   2     3    | 4   5   6
	// key | ctrl+cks | rolling_code | address

	// buf[0] = 0xa0 + (rolling_code & 0x0f);
	buf[0] = 0xa0;
	buf[1] = (ctrl<<4) & 0xf0;
	buf[2] = (uint8_t)(rolling_code >> 8);
	buf[3] = (uint8_t)(rolling_code);
	buf[4] = (uint8_t)(address >> 16);
	buf[5] = (uint8_t)(address >> 8);
	buf[6] = (uint8_t)(address);

	// calculate checksum
	buf[1] |= somfy_rts_calc_checksum(buf);
	
	// "encryption"
	for (uint8_t i = 1; i < SOMFY_RTS_FRAME_SIZE; i++)
	{
		buf[i] = buf[i] ^ buf[i-1];
	}
}

void somfy_send_frame()
{
	uint8_t tx_buf[20];
	// somfy bit_width is 604us, so timer tick should be set to this number (as precise as possible)
			
	// power on
	timer_send_duration(true, 20);		// 12ms high
	timer_send_duration(false, 33);		// 20ms low

	// sync
	for(uint8_t i=0; i<2; i++)
	{
		tx_buf[i] = 0xf0;
	}
	tx_buf[2] = 0xfe;
	timer_send_data(tx_buf, 3, true);
	
	// payload
	uint16_t rolling_counter = 0x1553;
	uint32_t address = 0x7a724a; // room-1 of unit-304
	somfy_make_frame(tx_buf, move_up, rolling_counter, address);
	timer_send_data(tx_buf, 7, true);
	timer_send_duration(false, 50);	// 30 ms low

	// sync
	for(uint8_t i=0; i<7; i++)
	{
		tx_buf[i] = 0xf0;
	}
	tx_buf[7] = 0xfe;
	timer_send_data(tx_buf, 8, true);
	
	// payload
	somfy_make_frame(tx_buf, move_up, rolling_counter, address);
	timer_send_data(tx_buf, 7, true);
	timer_send_duration(false, 5);	// a short delay to end packet
}