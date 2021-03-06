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
uint8_t paTable[] = PA_TABLE;

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

	uint8_t CC1101_status;
	writeRfSettings(spi);
	CC1101_status = WriteReg_CC1101(spi, TI_CC1101_PKTLEN, 10);
	WriteBurstReg_CC1101(spi, TI_CC1101_PATABLE, paTable, sizeof(paTable));
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
