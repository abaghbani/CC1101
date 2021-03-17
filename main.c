#include <stdio.h>
#include <string.h>
#include "nrf_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "spi/spi_driver.h"
#include "cc1101/CC1101_regs.h"
#include "cc1101/CC1101.h"

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

int main(void)
{
	
	bsp_board_init(BSP_INIT_LEDS);
	
	APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
	NRF_LOG_DEFAULT_BACKENDS_INIT();

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
		// Somfy - OOK @ 433.42MHz baud_rate=1.6KHz
	uint8_t paTable[] = {0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00};
	cc1101_init.pa_table = paTable;
	RF_SETTINGS rfSettings =
	{
		0x06,   // FSCTRL1   Frequency synthesizer control.
		0x00,   // FSCTRL0   Frequency synthesizer control.
		0x10,   // FREQ2     Frequency control word, high byte.
		0xAB,   // FREQ1     Frequency control word, middle byte.
		0x85,   // FREQ0     Frequency control word, low byte.
		0xf6,   // MDMCFG4   Modem configuration.
		0x02,   // MDMCFG3   Modem configuration.
		0x38,   // MDMCFG2   Modem configuration.
		0x02,   // MDMCFG1   Modem configuration.
		0xF8,   // MDMCFG0   Modem configuration.
		0x00,   // CHANNR    Channel number.
		0x05,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
		0x56,   // FREND1    Front end RX configuration.
		0x10,   // FREND0    Front end TX configuration.
		0x18,   // MCSM0     Main Radio Control State Machine configuration.
		0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
		0x6C,   // BSCFG     Bit synchronization Configuration.
		0x03,   // AGCCTRL2  AGC control.
		0x40,   // AGCCTRL1  AGC control.
		0x91,   // AGCCTRL0  AGC control.
		0xE9,   // FSCAL3    Frequency synthesizer calibration.
		0x2A,   // FSCAL2    Frequency synthesizer calibration.
		0x00,   // FSCAL1    Frequency synthesizer calibration.
		0x1F,   // FSCAL0    Frequency synthesizer calibration.
		0x59,   // FSTEST    Frequency synthesizer calibration.
		0x81,   // TEST2     Various test settings.
		0x35,   // TEST1     Various test settings.
		0x09,   // TEST0     Various test settings.
		0x07,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
		0x29,   // IOCFG2    GDO2 output pin configuration.
		0x06,   // IOCFG0D   GDO0 output pin configuration. 
		0x04,   // PKTCTRL1  Packet automation control.
		0x04,   // PKTCTRL0  Packet automation control.
		0x00,   // ADDR      Device address.
		0x0a    // PKTLEN    Packet length.
	};
	cc1101_init.rf_setting = rfSettings;
	cc1101_initialization(&cc1101_init);

	uint8_t txBuffer[] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x50, 0x51, 0x52, 0x53, 0x54};
	// NRF_LOG_INFO("Transfer completed.");
	
	while(1)
	{
		//spi_xfer_buf(spi, m_tx_buf, 5, m_rx_buf);
		//spi_int_xfer_buf(spi, m_tx_buf, 5, m_rx_buf);
		RFSendPacket(&cc1101_init, txBuffer, 10);
		NRF_LOG_INFO("Transfer completed.");
		WriteStrobe_CC1101(&cc1101_init, TI_CC1101_SIDLE);

		NRF_LOG_INFO("loop is running...");
		NRF_LOG_FLUSH();
		bsp_board_led_invert(BSP_BOARD_LED_0);
		nrf_delay_ms(3000);
		
	}
}
