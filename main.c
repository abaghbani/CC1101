#include <stdio.h>
#include <string.h>
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "spi_driver.h"
#include "CC1101_regs.h"
#include "CC1101.h"
#include "smartrf_setting_CC1101.h"

#include "app_util_platform.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


NRF_SPI_Type * spi = NRF_SPI0;
// static uint8_t m_tx_buf[] = {0x50, 0x51, 0x52, 0x53, 0x54};
// static uint8_t m_rx_buf[5];

int main(void)
{
	
	bsp_board_init(BSP_INIT_LEDS);
	
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	
	nrf_gpio_cfg_input(CC1101_GDO0_PIN, NRF_GPIO_PIN_PULLDOWN);

	
	if(spi_init(spi, false))
	{
		NRF_LOG_INFO("SPI initialization is done, SPI started...");
	}
	
	// reset CC1101
	PowerupReset_CC1101(spi);
	WriteStrobe_CC1101(spi, TI_CC1101_SRES);
	// RF_SETTINGS rfSettings =
	// {
		// 0x06,   // FSCTRL1   Frequency synthesizer control.
		// 0x00,   // FSCTRL0   Frequency synthesizer control.
		// 0x21,   // FREQ2     Frequency control word, high byte.
		// 0x62,   // FREQ1     Frequency control word, middle byte.
		// 0x76,   // FREQ0     Frequency control word, low byte.
		// 0xf5,   // MDMCFG4   Modem configuration.
		// 0x83,   // MDMCFG3   Modem configuration.
		// 0x13,   // MDMCFG2   Modem configuration.
		// 0x22,   // MDMCFG1   Modem configuration.
		// 0xF8,   // MDMCFG0   Modem configuration.
		// 0x00,   // CHANNR    Channel number.
		// 0x15,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
		// 0x56,   // FREND1    Front end RX configuration.
		// 0x10,   // FREND0    Front end TX configuration.
		// 0x18,   // MCSM0     Main Radio Control State Machine configuration.
		// 0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
		// 0x6C,   // BSCFG     Bit synchronization Configuration.
		// 0x03,   // AGCCTRL2  AGC control.
		// 0x40,   // AGCCTRL1  AGC control.
		// 0x91,   // AGCCTRL0  AGC control.
		// 0xE9,   // FSCAL3    Frequency synthesizer calibration.
		// 0x2A,   // FSCAL2    Frequency synthesizer calibration.
		// 0x00,   // FSCAL1    Frequency synthesizer calibration.
		// 0x1F,   // FSCAL0    Frequency synthesizer calibration.
		// 0x59,   // FSTEST    Frequency synthesizer calibration.
		// 0x81,   // TEST2     Various test settings.
		// 0x35,   // TEST1     Various test settings.
		// 0x09,   // TEST0     Various test settings.
		// 0x07,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
		// 0x29,   // IOCFG2    GDO2 output pin configuration.
		// 0x06,   // IOCFG0D   GDO0 output pin configuration. 
		// 0x04,   // PKTCTRL1  Packet automation control.
		// 0x05,   // PKTCTRL0  Packet automation control.
		// 0x00,   // ADDR      Device address.
		// 0xFF    // PKTLEN    Packet length.
	// };

	uint8_t CC1101_status;
	// CC1101_status = writeRfSettings(spi, &rfSettings);
	writeRfSettings(spi);
	CC1101_status = WriteReg_CC1101(spi, TI_CC1101_PKTLEN, 5);
	NRF_LOG_INFO("CC1101 initialization is done, Chip stsus = %x", CC1101_status);
	bsp_board_led_invert(BSP_BOARD_LED_0);
	
	NRF_LOG_INFO("chip part number = %x", ReadStatus_CC1101(spi, TI_CC1101_PARTNUM));
	NRF_LOG_INFO("chip version = %x", ReadStatus_CC1101(spi, TI_CC1101_VERSION));
	
	uint8_t txBuffer[] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x50, 0x51, 0x52, 0x53, 0x54};
	// RFSendPacket(spi, txBuffer, 10, CC1101_GDO0_PIN);
	// NRF_LOG_INFO("Transfer completed.");
	
	while(1)
	{
		//spi_xfer_buf(spi, m_tx_buf, 5, m_rx_buf);
		//spi_int_xfer_buf(spi, m_tx_buf, 5, m_rx_buf);
		RFSendPacket(spi, txBuffer, 10, CC1101_GDO0_PIN);
		NRF_LOG_INFO("Transfer completed.");
		WriteStrobe_CC1101(spi, TI_CC1101_SIDLE);

		NRF_LOG_INFO("loop is running...");
		NRF_LOG_FLUSH();
		bsp_board_led_invert(BSP_BOARD_LED_0);
		nrf_delay_ms(3000);
		
	}
}
