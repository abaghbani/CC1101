#include "CC1101_regs.h"

uint8_t PowerupReset_CC1101(NRF_SPI_Type * spi);
uint8_t WriteReg_CC1101(NRF_SPI_Type * spi, uint8_t addr, uint8_t value);
uint8_t WriteBurstReg_CC1101(NRF_SPI_Type * spi, uint8_t addr, uint8_t *buffer, uint8_t count);
uint8_t ReadReg_CC1101(NRF_SPI_Type * spi, uint8_t addr);
uint8_t ReadBurstReg_CC1101(NRF_SPI_Type * spi, uint8_t addr, uint8_t *buffer, uint8_t count);
uint8_t ReadStatus_CC1101(NRF_SPI_Type * spi, uint8_t addr);
uint8_t WriteStrobe_CC1101(NRF_SPI_Type * spi, uint8_t strobe);
// uint8_t writeRfSettings(NRF_SPI_Type * spi, RF_SETTINGS * pRfSettings);
uint8_t RFSendPacket(NRF_SPI_Type * spi, uint8_t *txBuffer, uint8_t size, uint8_t GD0_PIN);
uint8_t RFReceivePacket(NRF_SPI_Type * spi, uint8_t *rxBuffer, uint8_t length);
