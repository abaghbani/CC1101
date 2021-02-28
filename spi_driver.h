#ifndef SPI_DRIVER_H__
#define SPI_DRIVER_H__

int spi_init(NRF_SPI_Type * spi, bool interrupt_enable);
uint8_t spi_xfer(NRF_SPI_Type * spi, uint8_t txByte);
uint8_t spi_xfer_buf(NRF_SPI_Type * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf);
uint8_t spi_int_xfer_buf(NRF_SPI_Type * spi, uint8_t * tx_buf, uint8_t cmd_length, uint8_t * rx_buf);

#endif // SPI_DRIVER_H__
