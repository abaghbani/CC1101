#include <stdio.h>
#include <string.h>
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "CC1101_regs.h"
#include "CC1101.h"

const uint16_t somfy_rts_interval = 1240; // symbol width in us -> ca. 828 Hz data rate
const uint16_t somfy_rts_interval_half = somfy_rts_interval/2;
const uint8_t SOMFY_RTS_FRAME_SIZE = 7;

void SendPreamble(CC1101_t *cc1101)
{
	// it's hacking for generate preamble of somfy_rts
	modemSetting(cc1101, 1/302e-6, false);
	updateModemSettings(cc1101);
	uint8_t txBuffer[] = {0xff, 0x00, 0xff, 0x00, 0xff, 0xff};
	RFSendPacket(cc1101, txBuffer, 6);
}

void SendPayload(CC1101_t *cc1101, uint8_t *buf)
{
	modemSetting(cc1101, 1/604e-6, true);
	updateModemSettings(cc1101);
	RFSendPacket(cc1101, buf, 7);
}

void SendSomfyFrame(CC1101_t *cc1101, uint8_t *buf)
{
	SendPreamble(cc1101);
	SendPayload(cc1101, buf);
}

void Send_bitone(CC1101_t *cc1101)
{
	nrf_gpio_pin_clear(cc1101->gd0_pin);
	nrf_delay_us(somfy_rts_interval_half);
	nrf_gpio_pin_set(cc1101->gd0_pin);
	nrf_delay_us(2*somfy_rts_interval_half);
}

void Send_bitzero(CC1101_t *cc1101)
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
