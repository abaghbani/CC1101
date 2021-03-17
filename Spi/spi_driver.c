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
Spi_t *m_spi;

int spi_init(Spi_t *spi)
{
	// SPI init:
	nrf_gpio_cfg_output(spi->clk_pin);
	nrf_gpio_cfg_output(spi->mosi_pin);
	nrf_gpio_cfg_output(spi->cs_pin);
	nrf_gpio_pin_clear(spi->clk_pin);
	nrf_gpio_pin_clear(spi->mosi_pin);
	nrf_gpio_pin_set(spi->cs_pin);
	nrf_gpio_cfg_input(spi->miso_pin, NRF_GPIO_PIN_PULLDOWN);
	
	nrf_spi_pins_set(spi->spi, spi->clk_pin, spi->mosi_pin, spi->miso_pin);
	nrf_spi_frequency_set(spi->spi, NRF_SPI_FREQ_1M);
	nrf_spi_configure(spi->spi, NRF_SPI_MODE_0, NRF_SPI_BIT_ORDER_MSB_FIRST);
	if(spi->int_enabled)
	{
		NVIC_SetPriority((uint8_t)((uint32_t)(spi->spi) >> 12), 6);
		NVIC_EnableIRQ((uint8_t)((uint32_t)(spi->spi) >> 12));
		nrf_spi_int_enable(spi->spi, NRF_SPI_INT_READY_MASK);
		m_spi = spi;
	}
	else
	{
		nrf_spi_int_disable(spi->spi, NRF_SPI_INT_READY_MASK);
	}
	
	nrf_spi_enable(spi->spi);
	nrf_spi_event_clear(spi->spi, NRF_SPI_EVENT_READY);
	
	return nrf_spi_int_enable_check(spi->spi, NRF_SPI_INT_READY_MASK);;
}

uint8_t spi_xfer(Spi_t * spi, uint8_t txByte)
{
	nrf_spi_event_clear(spi->spi, NRF_SPI_EVENT_READY);
	nrf_gpio_pin_clear(spi->cs_pin);
	nrf_spi_txd_set(spi->spi, txByte);
	while(!nrf_spi_event_check(spi->spi, NRF_SPI_EVENT_READY));
	nrf_gpio_pin_set(spi->cs_pin);
	return nrf_spi_rxd_get(spi->spi);
}

uint8_t spi_xfer_buf(Spi_t * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf)
{
	nrf_spi_event_clear(spi->spi, NRF_SPI_EVENT_READY);
	nrf_gpio_pin_clear(spi->cs_pin);
	
	nrf_spi_txd_set(spi->spi, tx_buf[0]);
	int i = 0;
	do
	{
		if(++i < cmd_length)
			nrf_spi_txd_set(spi->spi, tx_buf[i]);
		while(!nrf_spi_event_check(spi->spi, NRF_SPI_EVENT_READY));
		rx_buf[i-1] = nrf_spi_rxd_get(spi->spi);
		nrf_spi_event_clear(spi->spi, NRF_SPI_EVENT_READY);
	} while(i < cmd_length);
	
	nrf_gpio_pin_set(spi->cs_pin);
	
	return 0;
}

uint8_t spi_int_xfer_buf(Spi_t * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf)
{
	nrf_spi_event_clear(spi->spi, NRF_SPI_EVENT_READY);
	nrf_gpio_pin_clear(spi->cs_pin);
	memcpy(tx_buf, m_tx_buf, cmd_length);
	nrf_spi_txd_set(spi->spi, m_tx_buf[0]);
	if(cmd_length>1)
		nrf_spi_txd_set(spi->spi, m_tx_buf[1]);
	spi_count = 0;
	spi_cmd_length = cmd_length;
	
	return 0;
}

static void irq_handler(Spi_t * spi)
{
	nrf_spi_event_clear(spi->spi, NRF_SPI_EVENT_READY);
	m_rx_buf[spi_count++] = nrf_spi_rxd_get(spi->spi);
	if(spi_count >= spi_cmd_length)
	{
		nrf_gpio_pin_set(spi->cs_pin);
	}
	else if((spi_count+1) < spi_cmd_length)
	{
		nrf_spi_txd_set(spi->spi, m_tx_buf[spi_count+1]);
	}
}

void nrfx_spi_0_irq_handler(void)
{
	irq_handler(m_spi);
}
