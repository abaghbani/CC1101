#include <stdio.h>
#include <string.h>
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "spi_driver.h"
#include "CC1101_regs.h"
#include "CC1101.h"

CC1101_t cc1101;

uint8_t Init_CC1101(CC1101_t *cc1101_init)
{
	cc1101.gd0_pin = cc1101_init->gd0_pin;
	cc1101.gd2_pin = cc1101_init->gd2_pin;
	cc1101.spi_ss_pin = cc1101_init->spi_ss_pin;
	cc1101.spi = cc1101_init->spi;
	nrf_gpio_cfg_input(cc1101.gd0_pin, NRF_GPIO_PIN_PULLDOWN);

	return 0;
}

uint8_t PowerupReset_CC1101()
{
	nrf_gpio_pin_set(cc1101.spi_ss_pin);
	nrf_delay_us(30);
	nrf_gpio_pin_clear(cc1101.spi_ss_pin);
	nrf_delay_us(30);
	nrf_gpio_pin_set(cc1101.spi_ss_pin);
	nrf_delay_us(45);

	if(spi_xfer(cc1101.spi, 0x00) != 0xff)
		return 1;
	else
		return 0;
}

uint8_t WriteReg_CC1101(uint8_t addr, uint8_t value)
{
	uint8_t tx_buf[2] = {addr, value};
	uint8_t rx_buf[2];
	spi_xfer_buf(cc1101.spi, tx_buf, 2, rx_buf);
	return rx_buf[0];
}

uint8_t WriteBurstReg_CC1101(uint8_t addr, uint8_t *buffer, uint8_t count)
{
	uint8_t tx_buf[count+1];
	uint8_t rx_buf[count+1];
	tx_buf[0] = addr | TI_CC1101_WRITE_BURST;
	memcpy(tx_buf+1*sizeof(uint8_t), buffer, count);
	spi_xfer_buf(cc1101.spi, tx_buf, count+1, rx_buf);
	return rx_buf[0];
}

uint8_t ReadReg_CC1101(uint8_t addr)
{
	uint8_t tx_buf[2] = {addr | TI_CC1101_READ_SINGLE, 0x00};
	uint8_t rx_buf[2];
	spi_xfer_buf(cc1101.spi, tx_buf, 2, rx_buf);
	return rx_buf[1];
}

uint8_t ReadBurstReg_CC1101(uint8_t addr, uint8_t *buffer, uint8_t count)
{
	uint8_t tx_buf[count+1];
	tx_buf[0] = addr | TI_CC1101_READ_BURST;
	spi_xfer_buf(cc1101.spi, tx_buf, count+1, buffer);
	return buffer[0];
}

uint8_t ReadStatus_CC1101(uint8_t addr)
{
	uint8_t tx_buf[2];
	uint8_t rx_buf[2];
	tx_buf[0] = addr | TI_CC1101_READ_BURST;
	spi_xfer_buf(cc1101.spi, tx_buf, 2, rx_buf);
	return rx_buf[1];
}

uint8_t WriteStrobe_CC1101(uint8_t strobe)
{
	return spi_xfer(cc1101.spi, strobe);
}

uint8_t writeRfSettings(RF_SETTINGS * pRfSettings)
{
	WriteReg_CC1101(TI_CC1101_FSCTRL1,  pRfSettings->FSCTRL1);
	WriteReg_CC1101(TI_CC1101_FSCTRL0,  pRfSettings->FSCTRL0);
	WriteReg_CC1101(TI_CC1101_FREQ2,    pRfSettings->FREQ2);
	WriteReg_CC1101(TI_CC1101_FREQ1,    pRfSettings->FREQ1);
	WriteReg_CC1101(TI_CC1101_FREQ0,    pRfSettings->FREQ0);
	WriteReg_CC1101(TI_CC1101_MDMCFG4,  pRfSettings->MDMCFG4);
	WriteReg_CC1101(TI_CC1101_MDMCFG3,  pRfSettings->MDMCFG3);
	WriteReg_CC1101(TI_CC1101_MDMCFG2,  pRfSettings->MDMCFG2);
	WriteReg_CC1101(TI_CC1101_MDMCFG1,  pRfSettings->MDMCFG1);
	WriteReg_CC1101(TI_CC1101_MDMCFG0,  pRfSettings->MDMCFG0);
	WriteReg_CC1101(TI_CC1101_CHANNR,   pRfSettings->CHANNR);
	WriteReg_CC1101(TI_CC1101_DEVIATN,  pRfSettings->DEVIATN);
	WriteReg_CC1101(TI_CC1101_FREND1,   pRfSettings->FREND1);
	WriteReg_CC1101(TI_CC1101_FREND0,   pRfSettings->FREND0);
	WriteReg_CC1101(TI_CC1101_MCSM0 ,   pRfSettings->MCSM0 );
	WriteReg_CC1101(TI_CC1101_FOCCFG,   pRfSettings->FOCCFG);
	WriteReg_CC1101(TI_CC1101_BSCFG,    pRfSettings->BSCFG);
	WriteReg_CC1101(TI_CC1101_AGCCTRL2, pRfSettings->AGCCTRL2);
	WriteReg_CC1101(TI_CC1101_AGCCTRL1, pRfSettings->AGCCTRL1);
	WriteReg_CC1101(TI_CC1101_AGCCTRL0, pRfSettings->AGCCTRL0);
	WriteReg_CC1101(TI_CC1101_FSCAL3,   pRfSettings->FSCAL3);
	WriteReg_CC1101(TI_CC1101_FSCAL2,   pRfSettings->FSCAL2);
	WriteReg_CC1101(TI_CC1101_FSCAL1,   pRfSettings->FSCAL1);
	WriteReg_CC1101(TI_CC1101_FSCAL0,   pRfSettings->FSCAL0);
	WriteReg_CC1101(TI_CC1101_FSTEST,   pRfSettings->FSTEST);
	WriteReg_CC1101(TI_CC1101_TEST2,    pRfSettings->TEST2);
	WriteReg_CC1101(TI_CC1101_TEST1,    pRfSettings->TEST1);
	WriteReg_CC1101(TI_CC1101_TEST0,    pRfSettings->TEST0);
	WriteReg_CC1101(TI_CC1101_FIFOTHR,  pRfSettings->FIFOTHR);
	WriteReg_CC1101(TI_CC1101_IOCFG2,   pRfSettings->IOCFG2);
	WriteReg_CC1101(TI_CC1101_IOCFG0,   pRfSettings->IOCFG0);    
	WriteReg_CC1101(TI_CC1101_PKTCTRL1, pRfSettings->PKTCTRL1);
	WriteReg_CC1101(TI_CC1101_PKTCTRL0, pRfSettings->PKTCTRL0);
	WriteReg_CC1101(TI_CC1101_ADDR,     pRfSettings->ADDR);
	uint8_t retVal = WriteReg_CC1101(TI_CC1101_PKTLEN,   pRfSettings->PKTLEN);
	return retVal;
}

//-----------------------------------------------------------------------------
//  void RFSendPacket(char *txBuffer, char size)
//
//  DESCRIPTION:
//  This function transmits a packet with length up to 63 bytes.  To use this
//  function, GD00 must be configured to be asserted when sync word is sent and
//  de-asserted at the end of the packet, which is accomplished by setting the
//  IOCFG0 register to 0x06, per the CCxxxx datasheet.  GDO0 goes high at
//  packet start and returns low when complete.  The function polls GDO0 to
//  ensure packet completion before returning.
//
//  ARGUMENTS:
//      char *txBuffer
//          Pointer to a buffer containing the data to be transmitted
//
//      char size
//          The size of the txBuffer
//-----------------------------------------------------------------------------
uint8_t RFSendPacket(uint8_t *txBuffer, uint8_t size)
{
	uint8_t retValue;
	retValue = WriteBurstReg_CC1101(TI_CC1101_TXFIFO, txBuffer, size);
	WriteStrobe_CC1101(TI_CC1101_STX);				// Change state to TX, initiating, data transfer
	while(!nrf_gpio_pin_read(cc1101.gd0_pin));		// Wait GDO0 to go hi -> sync TX'ed
	while(nrf_gpio_pin_read(cc1101.gd0_pin));		// Wait GDO0 to clear -> end of pkt
	return retValue;
}

//-----------------------------------------------------------------------------
//  char RFReceivePacket(char *rxBuffer, char *length)
//
//  DESCRIPTION:
//  Receives a packet of variable length (first byte in the packet must be the
//  length byte).  The packet length should not exceed the RXFIFO size.  To use
//  this function, APPEND_STATUS in the PKTCTRL1 register must be enabled.  It
//  is assumed that the function is called after it is known that a packet has
//  been received; for example, in response to GDO0 going low when it is
//  configured to output packet reception status.
//
//  The RXBYTES register is first read to ensure there are bytes in the FIFO.
//  This is done because the GDO signal will go high even if the FIFO is flushed
//  due to address filtering, CRC filtering, or packet length filtering.
//
//  ARGUMENTS:
//      char *rxBuffer
//          Pointer to the buffer where the incoming data should be stored
//      char *length
//          Pointer to a variable containing the size of the buffer where the
//          incoming data should be stored. After this function returns, that
//          variable holds the packet length.
//
//  RETURN VALUE:
//      char
//          0x80:  CRC OK
//          0x00:  CRC NOT OK (or no pkt was put in the RXFIFO due to filtering)
//-----------------------------------------------------------------------------
uint8_t RFReceivePacket(uint8_t *rxBuffer, uint8_t length)
{
	uint8_t status[2] = {0, 0};
	uint8_t pktLen;

	if ((ReadStatus_CC1101(TI_CC1101_RXBYTES) & TI_CC1101_NUM_RXBYTES))
	{
		pktLen = ReadReg_CC1101(TI_CC1101_RXFIFO); // Read length byte

		if (pktLen <= length)                  // If pktLen size <= rxBuffer
		{
			ReadBurstReg_CC1101(TI_CC1101_RXFIFO, rxBuffer, pktLen); // Pull data
			length = pktLen;                     // Return the actual size
			ReadBurstReg_CC1101(TI_CC1101_RXFIFO, status, 2); 	// Read appended status bytes
			return (char)(status[TI_CC1101_LQI_RX] & TI_CC1101_CRC_OK); // Return CRC_OK bit
		}
		else
		{
			length = pktLen;                     // Return the large size
			WriteStrobe_CC1101(TI_CC1101_SFRX);      // Flush RXFIFO
			return 0;                             // Error
		}
	}
	else
		return 0;                             // Error
}
