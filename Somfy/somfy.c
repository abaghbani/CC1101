#include <stdio.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "CC1101_regs.h"
#include "CC1101.h"

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

static void Send_bitone(CC1101_t *cc1101)
{
	nrf_gpio_pin_clear(cc1101->gd0_pin);
	nrf_delay_us(somfy_rts_interval_half);
	nrf_gpio_pin_set(cc1101->gd0_pin);
	nrf_delay_us(2*somfy_rts_interval_half);
}

static void Send_bitzero(CC1101_t *cc1101)
{
	nrf_gpio_pin_set(cc1101->gd0_pin);
	nrf_delay_us(somfy_rts_interval_half);
	nrf_gpio_pin_clear(cc1101->gd0_pin);
	nrf_delay_us(2*somfy_rts_interval_half);
}

void SendSomfyFrame_v2(CC1101_t *cc1101, uint8_t *buf)
{
	nrf_gpio_cfg_output(cc1101->gd0_pin);
	nrf_gpio_pin_clear(cc1101->gd0_pin);
	modemSetting(cc1101, 1/100e-6, false);
	updateModemSettings(cc1101);
	WriteStrobe_CC1101(cc1101, TI_CC1101_STX);				// Change state to TX, initiating, data transfer

	// send hardware sync (pulses of ca. double length)
	for (int8_t i = 0; i < 2; i++)
	{
		nrf_gpio_pin_set(cc1101->gd0_pin);
		nrf_delay_us(2*somfy_rts_interval);
		nrf_gpio_pin_clear(cc1101->gd0_pin);
		nrf_delay_us(2*somfy_rts_interval);
	}

	// send software sync (4 x symbol width high, half symbol width low)
	nrf_gpio_pin_set(cc1101->gd0_pin);
	nrf_delay_us(4*somfy_rts_interval);
	nrf_gpio_pin_clear(cc1101->gd0_pin);
	nrf_delay_us(somfy_rts_interval_half);

	// Send the user data
	for (int8_t i = 0; i < SOMFY_RTS_FRAME_SIZE; i++) {
		uint16_t mask = 0x80; // mask to send bits (MSB first)
		uint8_t d = buf[i];
		for (int8_t j = 0; j < 8; j++) {
			if ((d & mask) == 0) {
				Send_bitzero(cc1101);
			} else {
				Send_bitone(cc1101);
			}
			mask >>= 1; //get next bit
		}
	}

	// send inter-frame gap
	nrf_gpio_pin_clear(cc1101->gd0_pin);
	nrf_delay_us(100);
	WriteStrobe_CC1101(cc1101, TI_CC1101_SIDLE);		// Change state to IDLE
}

static uint8_t somfy_rts_calc_checksum(uint8_t *frame)
{
	uint8_t checksum = 0;
	for (int8_t i = 0; i < SOMFY_RTS_FRAME_SIZE; i++) {
		checksum = checksum ^ (frame[i]&0x0f) ^ ((frame[i] >> 4)&0x0f);
	}

	return (checksum & 0x0f);
}

void make_somfy_frame(uint8_t *buf, uint8_t ctrl, uint16_t rolling_code, uint32_t address)
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