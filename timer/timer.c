#include "nrf.h"
#include "nrfx_timer.h"
#include "nrf_gpio.h"
#include "nrf_log.h"

static volatile uint8_t timer_tx_data = 0;
static volatile uint8_t timer_event_done = 1;

void timer_test_event_handler(nrf_timer_event_t event_type, void* p_context)
{
	// static uint32_t i;

	switch (event_type)
	{
		case NRF_TIMER_EVENT_COMPARE0:
			
			if(timer_tx_data == 1)
				nrf_gpio_pin_set(CC1101_GDO0_PIN);
			else
				nrf_gpio_pin_clear(CC1101_GDO0_PIN);
			timer_event_done = 1;
			break;
		
		default:
			//Do nothing.
			break;
	}
}

void timer_init(nrfx_timer_t *TIMER, uint32_t time_us)
{
	// uint32_t time_ms = 300; //Time(in miliseconds) between consecutive compare events.
	//uint32_t time_us = 600+3; //Time(in microseconds) between consecutive compare events.
	uint32_t time_ticks;

	// nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
	nrfx_timer_config_t timer_cfg = {
		.frequency			= NRF_TIMER_FREQ_2MHz,
		.mode				= NRF_TIMER_MODE_TIMER,
		.bit_width			= NRF_TIMER_BIT_WIDTH_16,
		.interrupt_priority = 6,
		.p_context			= NULL
	};
	
	if(nrfx_timer_init(TIMER, &timer_cfg, timer_test_event_handler) != NRFX_SUCCESS)
		NRF_LOG_INFO("Error: timer init is failed");

	// time_ticks = nrfx_timer_ms_to_ticks(TIMER, time_ms);
	time_ticks = nrfx_timer_us_to_ticks(TIMER, time_us);
	NRF_LOG_INFO("timer init: tick number : %d", time_ticks);
	nrfx_timer_extended_compare(TIMER, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}

void timer_send_duration(bool state, uint16_t n_tick)
{
	for(uint16_t i=0; i<n_tick;i++)
	{
		while(timer_event_done==0);
		timer_tx_data = state;
		timer_event_done = 0;
	}
	while(timer_event_done==0);
}

void timer_send_data(uint8_t *data, uint16_t len, bool msb_first)
{
	for(uint16_t i=0; i<len;i++)
	{
		for(uint16_t j=0;j<8;j++)
		{
			while(timer_event_done==0);
			if(msb_first)
				timer_tx_data = (data[i]>>(7-j)) & 0x01;
			else
				timer_tx_data = (data[i]>>j) & 0x01;
			timer_event_done = 0;
		}
	}
	while(timer_event_done==0);
}

