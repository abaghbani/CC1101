#ifndef SPI_DRIVER_H__
#define SPI_DRIVER_H__

#include "nrf_spi.h"

typedef struct
{
	NRF_SPI_Type			*spi;
	uint8_t					cs_pin;
	uint8_t					clk_pin;
	uint8_t					mosi_pin;
	uint8_t					miso_pin;
	bool					int_enabled;
	
} Spi_t;

int spi_init(Spi_t * spi);
uint8_t spi_xfer(Spi_t * spi, uint8_t txByte);
uint8_t spi_xfer_buf(Spi_t * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf);
uint8_t spi_int_xfer_buf(Spi_t * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf);

#endif // SPI_DRIVER_H__
