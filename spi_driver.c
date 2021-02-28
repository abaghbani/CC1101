#include <stdio.h>
#include <string.h>
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "spi_driver.h"

#define nrfx_spi_0_irq_handler     SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
void nrfx_spi_0_irq_handler(void);

static uint8_t m_tx_buf[64];
static uint8_t m_rx_buf[64];
static volatile int spi_count ;
static volatile int spi_cmd_length ;

int spi_init(NRF_SPI_Type * spi, bool interrupt_enable)
{
	// SPI init:
	nrf_gpio_cfg_output(SPI_SCK_PIN);
	nrf_gpio_cfg_output(SPI_MOSI_PIN);
	nrf_gpio_cfg_output(SPI_SS_PIN);
	nrf_gpio_pin_clear(SPI_SCK_PIN);
	nrf_gpio_pin_clear(SPI_MOSI_PIN);
	nrf_gpio_pin_set(SPI_SS_PIN);
	nrf_gpio_cfg_input(SPI_MISO_PIN, NRF_GPIO_PIN_PULLDOWN);
	
	nrf_spi_pins_set(spi, SPI_SCK_PIN, SPI_MOSI_PIN, SPI_MISO_PIN);
	nrf_spi_frequency_set(spi, NRF_SPI_FREQ_1M);
	nrf_spi_configure(spi, NRF_SPI_MODE_0, NRF_SPI_BIT_ORDER_MSB_FIRST);
	if(interrupt_enable)
	{
		NVIC_SetPriority((uint8_t)((uint32_t)(spi) >> 12), 6);
		NVIC_EnableIRQ((uint8_t)((uint32_t)(spi) >> 12));
		nrf_spi_int_enable(spi, NRF_SPI_INT_READY_MASK);
	}
	else
	{
		nrf_spi_int_disable(spi, NRF_SPI_INT_READY_MASK);
	}
	
	nrf_spi_enable(spi);
	nrf_spi_event_clear(spi, NRF_SPI_EVENT_READY);
	
	return nrf_spi_int_enable_check(spi, NRF_SPI_INT_READY_MASK);;
}

uint8_t spi_xfer(NRF_SPI_Type * spi, uint8_t txByte)
{
	nrf_spi_event_clear(spi, NRF_SPI_EVENT_READY);
	nrf_gpio_pin_clear(SPI_SS_PIN);
	nrf_spi_txd_set(spi, txByte);
	while(!nrf_spi_event_check(spi, NRF_SPI_EVENT_READY));
	nrf_gpio_pin_set(SPI_SS_PIN);
	return nrf_spi_rxd_get(spi);
}

uint8_t spi_xfer_buf(NRF_SPI_Type * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf)
{
	nrf_spi_event_clear(spi, NRF_SPI_EVENT_READY);
	nrf_gpio_pin_clear(SPI_SS_PIN);
	
	nrf_spi_txd_set(spi, tx_buf[0]);
	int i = 0;
	do
	{
		if(++i < cmd_length)
			nrf_spi_txd_set(spi, tx_buf[i]);
		while(!nrf_spi_event_check(spi, NRF_SPI_EVENT_READY));
		rx_buf[i-1] = nrf_spi_rxd_get(spi);
		nrf_spi_event_clear(spi, NRF_SPI_EVENT_READY);
	} while(i < cmd_length);
	
	nrf_gpio_pin_set(SPI_SS_PIN);
	
	return 0;
}

uint8_t spi_int_xfer_buf(NRF_SPI_Type * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf)
{
	nrf_spi_event_clear(spi, NRF_SPI_EVENT_READY);
	nrf_gpio_pin_clear(SPI_SS_PIN);
	memcpy(tx_buf, m_tx_buf, cmd_length);
	nrf_spi_txd_set(spi, m_tx_buf[0]);
	if(cmd_length>1)
		nrf_spi_txd_set(spi, m_tx_buf[1]);
	spi_count = 0;
	spi_cmd_length = cmd_length;
	
	return 0;
}

static void irq_handler(NRF_SPI_Type * spi)
{
	nrf_spi_event_clear(spi, NRF_SPI_EVENT_READY);
	m_rx_buf[spi_count++] = nrf_spi_rxd_get(spi);
	if(spi_count >= spi_cmd_length)
	{
		nrf_gpio_pin_set(SPI_SS_PIN);
	}
	else if((spi_count+1) < spi_cmd_length)
	{
		nrf_spi_txd_set(spi, m_tx_buf[spi_count+1]);
	}
}

void nrfx_spi_0_irq_handler(void)
{
	irq_handler(NRF_SPI0);
}
