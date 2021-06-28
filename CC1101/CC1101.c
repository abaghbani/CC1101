#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "spi_driver.h"
#include "CC1101_regs.h"
#include "CC1101.h"
#include "nrf_log.h"

uint8_t Init_CC1101(CC1101_t *cc1101, bool serial_mode_en)
{
	if(!serial_mode_en)
	{
		nrf_gpio_cfg_input(cc1101->gd0_pin, NRF_GPIO_PIN_PULLDOWN);
		nrf_gpio_cfg_input(cc1101->gd2_pin, NRF_GPIO_PIN_PULLDOWN);
		cc1101->rf_setting.PKTCTRL0 &= 0xcf; 
	}
	else
	{
		nrf_gpio_cfg_output(cc1101->gd0_pin);
		nrf_gpio_pin_clear(cc1101->gd0_pin);
		nrf_gpio_cfg_input(cc1101->gd2_pin, NRF_GPIO_PIN_PULLDOWN);
		cc1101->rf_setting.PKTCTRL0 |= 0x30;
	}

	PowerupReset_CC1101(cc1101);
	WriteStrobe_CC1101(cc1101, TI_CC1101_SRES);
	
	uint8_t CC1101_status = writeRfSettings(cc1101);
	WriteBurstReg_CC1101(cc1101, TI_CC1101_PATABLE, cc1101->pa_table, 8);

	return CC1101_status;
}

uint8_t PowerupReset_CC1101(CC1101_t *cc1101)
{
	nrf_gpio_pin_set(cc1101->spi_cs_pin);
	nrf_delay_us(30);
	nrf_gpio_pin_clear(cc1101->spi_cs_pin);
	nrf_delay_us(30);
	nrf_gpio_pin_set(cc1101->spi_cs_pin);
	nrf_delay_us(45);

	if(spi_xfer(cc1101->spi, 0x00) != 0xff)
		return 1;
	else
		return 0;
}

uint8_t WriteReg_CC1101(CC1101_t *cc1101, uint8_t addr, uint8_t value)
{
	uint8_t tx_buf[2] = {addr, value};
	uint8_t rx_buf[2];
	spi_xfer_buf(cc1101->spi, tx_buf, 2, rx_buf);
	return rx_buf[0];
}

uint8_t WriteBurstReg_CC1101(CC1101_t *cc1101, uint8_t addr, uint8_t *buffer, uint8_t count)
{
	uint8_t tx_buf[count+1];
	uint8_t rx_buf[count+1];
	tx_buf[0] = addr | TI_CC1101_WRITE_BURST;
	memcpy(tx_buf+1*sizeof(uint8_t), buffer, count);
	spi_xfer_buf(cc1101->spi, tx_buf, count+1, rx_buf);
	return rx_buf[0];
}

uint8_t ReadReg_CC1101(CC1101_t *cc1101, uint8_t addr)
{
	uint8_t tx_buf[2] = {addr | TI_CC1101_READ_SINGLE, 0x00};
	uint8_t rx_buf[2];
	spi_xfer_buf(cc1101->spi, tx_buf, 2, rx_buf);
	return rx_buf[1];
}

uint8_t ReadBurstReg_CC1101(CC1101_t *cc1101, uint8_t addr, uint8_t *buffer, uint8_t count)
{
	uint8_t tx_buf[count+1];
	tx_buf[0] = addr | TI_CC1101_READ_BURST;
	spi_xfer_buf(cc1101->spi, tx_buf, count+1, buffer);
	return buffer[0];
}

uint8_t ReadStatus_CC1101(CC1101_t *cc1101, uint8_t addr)
{
	uint8_t tx_buf[2];
	uint8_t rx_buf[2];
	tx_buf[0] = addr | TI_CC1101_READ_BURST;
	spi_xfer_buf(cc1101->spi, tx_buf, 2, rx_buf);
	return rx_buf[1];
}

uint8_t WriteStrobe_CC1101(CC1101_t *cc1101, uint8_t strobe)
{
	return spi_xfer(cc1101->spi, strobe);
}

uint8_t writeRfSettings(CC1101_t *cc1101)
{
	WriteReg_CC1101(cc1101, TI_CC1101_FSCTRL1,  cc1101->rf_setting.FSCTRL1);
	WriteReg_CC1101(cc1101, TI_CC1101_FSCTRL0,  cc1101->rf_setting.FSCTRL0);
	WriteReg_CC1101(cc1101, TI_CC1101_FREQ2,    cc1101->rf_setting.FREQ2);
	WriteReg_CC1101(cc1101, TI_CC1101_FREQ1,    cc1101->rf_setting.FREQ1);
	WriteReg_CC1101(cc1101, TI_CC1101_FREQ0,    cc1101->rf_setting.FREQ0);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG4,  cc1101->rf_setting.MDMCFG4);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG3,  cc1101->rf_setting.MDMCFG3);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG2,  cc1101->rf_setting.MDMCFG2);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG1,  cc1101->rf_setting.MDMCFG1);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG0,  cc1101->rf_setting.MDMCFG0);
	WriteReg_CC1101(cc1101, TI_CC1101_CHANNR,   cc1101->rf_setting.CHANNR);
	WriteReg_CC1101(cc1101, TI_CC1101_DEVIATN,  cc1101->rf_setting.DEVIATN);
	WriteReg_CC1101(cc1101, TI_CC1101_FREND1,   cc1101->rf_setting.FREND1);
	WriteReg_CC1101(cc1101, TI_CC1101_FREND0,   cc1101->rf_setting.FREND0);
	WriteReg_CC1101(cc1101, TI_CC1101_MCSM2 ,   cc1101->rf_setting.MCSM2 );
	WriteReg_CC1101(cc1101, TI_CC1101_MCSM1 ,   cc1101->rf_setting.MCSM1 );
	WriteReg_CC1101(cc1101, TI_CC1101_MCSM0 ,   cc1101->rf_setting.MCSM0 );
	WriteReg_CC1101(cc1101, TI_CC1101_FOCCFG,   cc1101->rf_setting.FOCCFG);
	WriteReg_CC1101(cc1101, TI_CC1101_BSCFG,    cc1101->rf_setting.BSCFG);
	WriteReg_CC1101(cc1101, TI_CC1101_AGCCTRL2, cc1101->rf_setting.AGCCTRL2);
	WriteReg_CC1101(cc1101, TI_CC1101_AGCCTRL1, cc1101->rf_setting.AGCCTRL1);
	WriteReg_CC1101(cc1101, TI_CC1101_AGCCTRL0, cc1101->rf_setting.AGCCTRL0);
	WriteReg_CC1101(cc1101, TI_CC1101_FSCAL3,   cc1101->rf_setting.FSCAL3);
	WriteReg_CC1101(cc1101, TI_CC1101_FSCAL2,   cc1101->rf_setting.FSCAL2);
	WriteReg_CC1101(cc1101, TI_CC1101_FSCAL1,   cc1101->rf_setting.FSCAL1);
	WriteReg_CC1101(cc1101, TI_CC1101_FSCAL0,   cc1101->rf_setting.FSCAL0);
	WriteReg_CC1101(cc1101, TI_CC1101_FSTEST,   cc1101->rf_setting.FSTEST);
	WriteReg_CC1101(cc1101, TI_CC1101_TEST2,    cc1101->rf_setting.TEST2);
	WriteReg_CC1101(cc1101, TI_CC1101_TEST1,    cc1101->rf_setting.TEST1);
	WriteReg_CC1101(cc1101, TI_CC1101_TEST0,    cc1101->rf_setting.TEST0);
	WriteReg_CC1101(cc1101, TI_CC1101_FIFOTHR,  cc1101->rf_setting.FIFOTHR);
	WriteReg_CC1101(cc1101, TI_CC1101_IOCFG2,   cc1101->rf_setting.IOCFG2);
	WriteReg_CC1101(cc1101, TI_CC1101_IOCFG0,   cc1101->rf_setting.IOCFG0);    
	WriteReg_CC1101(cc1101, TI_CC1101_PKTCTRL1, cc1101->rf_setting.PKTCTRL1);
	WriteReg_CC1101(cc1101, TI_CC1101_PKTCTRL0, cc1101->rf_setting.PKTCTRL0);
	WriteReg_CC1101(cc1101, TI_CC1101_ADDR,     cc1101->rf_setting.ADDR);
	uint8_t retVal = WriteReg_CC1101(cc1101, TI_CC1101_PKTLEN,   cc1101->rf_setting.PKTLEN);
	return retVal;
}

void updateModemSettings(CC1101_t *cc1101)
{
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG4,  cc1101->rf_setting.MDMCFG4);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG3,  cc1101->rf_setting.MDMCFG3);
	WriteReg_CC1101(cc1101, TI_CC1101_MDMCFG2,  cc1101->rf_setting.MDMCFG2);
}

void modemSetting(CC1101_t *cc1101, double baudrate, bool manchester_enabled)
{
	// rate = (256+DRATE_M).2**DRATE_E.f_OSC/2**28	
	cc1101->rf_setting.MDMCFG4 = (cc1101->rf_setting.MDMCFG4 & 0xf0) + (uint8_t)(log2(baudrate*pow(2,28)/f_osc)-8);
	cc1101->rf_setting.MDMCFG3 = (uint8_t)((baudrate*pow(2,28-(cc1101->rf_setting.MDMCFG4 & 0x0f))/f_osc)-256);
	cc1101->rf_setting.MDMCFG2 = manchester_enabled ? cc1101->rf_setting.MDMCFG2 | 0x08 : cc1101->rf_setting.MDMCFG2 & 0xf7; 
}

void updateFrequencySettings(CC1101_t *cc1101, double freq)
{
	// frequency = f_OSC/2**16 * FREQ[23:0]
	double correction_ratio = 1.0000599; // on cc1101 board, OSC is not exactly 26.000MHz, so need correction
	uint32_t freq_value = (uint32_t)(freq*pow(2, 16)*correction_ratio/f_osc);
	NRF_LOG_INFO("CC1101: frequency int = %d ", freq_value);
	cc1101->rf_setting.FREQ0 = (uint8_t)(freq_value);
	cc1101->rf_setting.FREQ1 = (uint8_t)(freq_value>>8);
	cc1101->rf_setting.FREQ2 = (uint8_t)(freq_value>>16);
	NRF_LOG_INFO("CC1101: frequency is update to = %d so setting bytes = %x, %x, %x ", freq, cc1101->rf_setting.FREQ0, cc1101->rf_setting.FREQ1, cc1101->rf_setting.FREQ2);
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
uint8_t RFSendPacket(CC1101_t *cc1101, uint8_t *txBuffer, uint8_t size)
{
	uint8_t retValue;
	retValue = WriteBurstReg_CC1101(cc1101, TI_CC1101_TXFIFO, txBuffer, size);
	WriteReg_CC1101(cc1101, TI_CC1101_PKTLEN, size);
	WriteStrobe_CC1101(cc1101, TI_CC1101_STX);				// Change state to TX, initiating, data transfer
	while(!nrf_gpio_pin_read(cc1101->gd0_pin));		// Wait GDO0 to go hi -> sync TX'ed
	while(nrf_gpio_pin_read(cc1101->gd0_pin));		// Wait GDO0 to clear -> end of pkt
	return retValue;
}

void RFStartSendBuffer(CC1101_t *cc1101, uint8_t *txBuffer, uint8_t size)
{
	WriteBurstReg_CC1101(cc1101, TI_CC1101_TXFIFO, txBuffer, size);
	WriteStrobe_CC1101(cc1101, TI_CC1101_STX);				// Change state to TX, initiating, data transfer
	while(!nrf_gpio_pin_read(cc1101->gd0_pin));		// Wait GDO0 to go hi -> sync TX'ed
}

void RFSendBuffer(CC1101_t *cc1101, uint8_t *txBuffer, uint8_t size)
{
	while(nrf_gpio_pin_read(cc1101->gd2_pin));		// Wait GDO2 to go low -> TxFifo is half empty
	WriteBurstReg_CC1101(cc1101, TI_CC1101_TXFIFO, txBuffer, size);
}

void RFSendTerminate(CC1101_t *cc1101)
{
	WriteStrobe_CC1101(cc1101, TI_CC1101_SIDLE);				// Change state to TX, initiating, data transfer
}

void RFSend_async(CC1101_t *cc1101)
{
	WriteStrobe_CC1101(cc1101, TI_CC1101_STX);
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
uint8_t RFReceivePacket(CC1101_t *cc1101, uint8_t *rxBuffer, uint8_t length)
{
	uint8_t status[2] = {0, 0};
	uint8_t pktLen;

	if ((ReadStatus_CC1101(cc1101, TI_CC1101_RXBYTES) & TI_CC1101_NUM_RXBYTES))
	{
		pktLen = ReadReg_CC1101(cc1101, TI_CC1101_RXFIFO); // Read length byte

		if (pktLen <= length)                  // If pktLen size <= rxBuffer
		{
			ReadBurstReg_CC1101(cc1101, TI_CC1101_RXFIFO, rxBuffer, pktLen); // Pull data
			length = pktLen;                     // Return the actual size
			ReadBurstReg_CC1101(cc1101, TI_CC1101_RXFIFO, status, 2); 	// Read appended status bytes
			return (char)(status[TI_CC1101_LQI_RX] & TI_CC1101_CRC_OK); // Return CRC_OK bit
		}
		else
		{
			length = pktLen;                     // Return the large size
			WriteStrobe_CC1101(cc1101, TI_CC1101_SFRX);      // Flush RXFIFO
			return 0;                             // Error
		}
	}
	else
		return 0;                             // Error
}
