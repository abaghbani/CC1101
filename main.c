#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrfx_timer.h"
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "spi_driver.h"
#include "CC1101_regs.h"
#include "CC1101.h"
#include "hormann.h"

#include "app_util_platform.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void spi_initialization(Spi_t *spi)
{
	if(spi_init(spi))
	{
		NRF_LOG_INFO("SPI initialization is done, SPI started...");
	}
}

void cc1101_initialization(CC1101_t * cc1101)
{
	uint8_t status = Init_CC1101(cc1101);

	NRF_LOG_INFO("CC1101 initialization is done, Chip stsus = %x", status);
	NRF_LOG_INFO("chip part number = %x", ReadStatus_CC1101(cc1101, TI_CC1101_PARTNUM));
	NRF_LOG_INFO("chip version = %x", ReadStatus_CC1101(cc1101, TI_CC1101_VERSION));
}

void hormann_command(CC1101_t * cc1101)
{
/*

0. 1. 1. 0. 1. 1. 0. 1. = 6d
1. 0. 1. 1. 0. 1. 1. 0. = b6
1. 0. 0. 1. 0. 0. 1. 0. = 92
0. 1. 0. 0. 1. 0. 0. 1. = 49
1. 0. 1. 1. 0. 1. 0. 0. = b4
1. 1. 0. 1. 0. 0. 1. 1. = d3
0. 1. 0. 0. 1. 0. 0. 1. = 49
1. 0. 1. 1. 0. 1. 0. 0. = b4
1. 1. 0. 1. 0. 0. 1. 0. = d2
0. 1. 0. 0. 1. 0. 0. 1. = 49
1. 0. 1. 1. 0. 1. 1. 0. = b6
1. 1. 0. 1. 0. 0. 1. 1. = d3
0. 1. 1. 0. 1. 1. 0. 1. = 6d
1. 0. 1. 1. 0. 1. 0. 0. = b4
1. 1. 0. 1. 0. 0. 1. 0. = d2
0. 1. 0. 0. 1. 0. 0. 1. = 49
1. 0. 1. 1. 0. 1  1  1  = b7
*/

	uint8_t txBuffer[17+4] = {0xed, 0xb6, 0x92, 0x49, 0xb4, 0xd3, 0x49, 0xb4, 0xd2, 0x49, 0xb6, 0xd3, 0x6d, 0xb4, 0xd2, 0x49, 0xb7, 0xff, 0xff, 0xff, 0xff};

	send_poweron(cc1101);

	for(int i = 0; i < 20; i++)
	{
		send_hormann_frame(cc1101, txBuffer, 17+4);
		//nrf_delay_ms(13.5);		
	}
	//send_hormann_frame(cc1101, txBuffer, 17);
}

void hormann_command_v2(CC1101_t * cc1101)
{
	uint8_t txBuffer[17+4] = {0xed, 0xb6, 0x92, 0x49, 0xb4, 0xd3, 0x49, 0xb4, 0xd2, 0x49, 0xb6, 0xd3, 0x6d, 0xb4, 0xd2, 0x49, 0xb7, 0xff, 0xff, 0xff, 0xff};

	send_poweron_2(cc1101);
	send_hormann_frame_2(cc1101, txBuffer, 17+4, 20);
}


const uint8_t test_array[] = {1,0,1,0,1,1,0,0,1,1,0,0};

void timer_test_event_handler(nrf_timer_event_t event_type, void* p_context)
{
    // static uint32_t i;

    switch (event_type)
    {
        case NRF_TIMER_EVENT_COMPARE0:
			nrf_gpio_pin_toggle(46);
			nrf_gpio_pin_toggle(47);
			/*
			if(test_array[(i++) % 12]==1)
			{
				nrf_gpio_pin_set(46);
				nrf_gpio_pin_set(47);
			}
			else
			{
				nrf_gpio_pin_clear(46);
				nrf_gpio_pin_clear(47);
			}*/
            break;

        default:
            //Do nothing.
            break;
    }
}

void timer_init(nrfx_timer_t *TIMER)
{
    // uint32_t time_ms = 300; //Time(in miliseconds) between consecutive compare events.
    uint32_t time_us = 300+3; //Time(in microseconds) between consecutive compare events.
    uint32_t time_ticks;

    // nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
    nrfx_timer_config_t timer_cfg = {
		.frequency          = NRF_TIMER_FREQ_2MHz,
		.mode               = NRF_TIMER_MODE_TIMER,
		.bit_width          = NRF_TIMER_BIT_WIDTH_16,
		.interrupt_priority = 6,
		.p_context          = NULL
	};
	
    if(nrfx_timer_init(TIMER, &timer_cfg, timer_test_event_handler) != NRFX_SUCCESS)
		NRF_LOG_INFO("Error: timer init is failed");

    // time_ticks = nrfx_timer_ms_to_ticks(TIMER, time_ms);
    time_ticks = nrfx_timer_us_to_ticks(TIMER, time_us);
	NRF_LOG_INFO("timer init: tick number : %d", time_ticks);
    nrfx_timer_extended_compare(TIMER, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}

int main(void)
{
	
	bsp_board_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS);
	nrf_gpio_cfg_output(46);
	nrf_gpio_cfg_output(47);
	nrf_gpio_pin_clear(46);
	nrf_gpio_pin_clear(47);

	
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();

	// timer init
	nrfx_timer_t timer_0 = NRFX_TIMER_INSTANCE(0);
	timer_init(&timer_0);

	// spi init
	Spi_t spi_init;
	spi_init.spi = NRF_SPI0;
	spi_init.cs_pin = SPI_SS_PIN;
	spi_init.clk_pin = SPI_SCK_PIN;
	spi_init.mosi_pin = SPI_MOSI_PIN;
	spi_init.miso_pin = SPI_MISO_PIN;
	spi_init.int_enabled = false;
	spi_initialization(&spi_init);

	// cc1101 init
	CC1101_t cc1101_init;
	cc1101_init.spi = &spi_init;
	cc1101_init.gd0_pin = CC1101_GDO0_PIN;
	cc1101_init.gd2_pin = CC1101_GDO2_PIN;
	cc1101_init.spi_cs_pin = CC1101_CS_PIN;
	// Hormann - OOK @ 868.3MHz baud_rate= 2100us
	uint8_t paTable[] = {0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00};
	cc1101_init.pa_table = paTable;
	RF_SETTINGS rfSettings =
	{
		0x0e,   // FSCTRL1   Frequency synthesizer control.
		0x00,   // FSCTRL0   Frequency synthesizer control.
		0x21,   // FREQ2     Frequency control word, high byte.
		0x65,   // FREQ1     Frequency control word, middle byte.
		0xf1,   // FREQ0     Frequency control word, low byte.
		0xf6,   // MDMCFG4   Modem configuration.
		0x43,   // MDMCFG3   Modem configuration.
		0x38,   // MDMCFG2   Modem configuration.
		0x00,   // MDMCFG1   Modem configuration.
		0xF8,   // MDMCFG0   Modem configuration.
		0x00,   // CHANNR    Channel number.
		0x00,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
		0xB6,   // FREND1    Front end RX configuration.
		0x11,   // FREND0    Front end TX configuration.
		0x07,   // MCSM2     Main Radio Control State Machine configuration.
		0x30,   // MCSM1     Main Radio Control State Machine configuration.
		0x18,   // MCSM0     Main Radio Control State Machine configuration.
		0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
		0x1C,   // BSCFG     Bit synchronization Configuration.
		0xC7,   // AGCCTRL2  AGC control.
		0x00,   // AGCCTRL1  AGC control.
		0xB0,   // AGCCTRL0  AGC control.
		0xE9,   // FSCAL3    Frequency synthesizer calibration.
		0x2A,   // FSCAL2    Frequency synthesizer calibration.
		0x00,   // FSCAL1    Frequency synthesizer calibration.
		0x1F,   // FSCAL0    Frequency synthesizer calibration.
		0x59,   // FSTEST    Frequency synthesizer calibration.
		0x81,   // TEST2     Various test settings.
		0x35,   // TEST1     Various test settings.
		0x09,   // TEST0     Various test settings.
		0x47,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
		0x02,   // IOCFG2    GDO2 output pin configuration.
		0x06,   // IOCFG0D   GDO0 output pin configuration. 
		0x04,   // PKTCTRL1  Packet automation control.
		0x02,   // PKTCTRL0  Packet automation control. -- for somfy v1
		//0x32,   // PKTCTRL0  Packet automation control. -- for somfy v2
		0x00,   // ADDR      Device address.
		0xff    // PKTLEN    Packet length.
	};
	cc1101_init.rf_setting = rfSettings;
	cc1101_initialization(&cc1101_init);

	
	while(1)
	{
		if(bsp_board_button_state_get(BSP_BOARD_BUTTON_0))
		{
			uint8_t txBuffer[17+3] = {0x6d, 0xb6, 0x92, 0x49, 0xb4, 0xd3, 0x49, 0xb4, 0xd2, 0x49, 0xb6, 0xd3, 0x6d, 0xb4, 0xd2, 0x49, 0xb7, 0xff, 0xff, 0xff};

			send_poweron_2(&cc1101_init);
			RFStartSendBuffer(&cc1101_init, txBuffer, 17+3);
			while(bsp_board_button_state_get(BSP_BOARD_BUTTON_0))
				RFSendBuffer(&cc1101_init, txBuffer, 17+3);
			
			nrf_delay_ms(300);
			RFSendTerminate(&cc1101_init);
			NRF_LOG_INFO("button 0 is pressed.");
		}
		if(bsp_board_button_state_get(BSP_BOARD_BUTTON_1))
		{
		    nrfx_timer_enable(&timer_0);
			NRF_LOG_INFO("button 1 is pressed.");
		}
		if(bsp_board_button_state_get(BSP_BOARD_BUTTON_2))
		{
			nrfx_timer_disable(&timer_0);
			NRF_LOG_INFO("button 2 is pressed.");
		}
		if(bsp_board_button_state_get(BSP_BOARD_BUTTON_3))
		{
			NRF_LOG_INFO("button 3 is pressed.");
		}
		
		NRF_LOG_INFO("loop is running...");
		NRF_LOG_FLUSH();
		bsp_board_led_invert(BSP_BOARD_LED_0);
		nrf_delay_ms(500);	
			
	}
}
