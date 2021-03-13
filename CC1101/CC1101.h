#ifndef CC1101_CONFIG_H
#define CC1101_CONFIG_H

#include "nrf_spi.h"

typedef struct CC1101_struct
{
    uint8_t             gd0_pin;
    uint8_t             gd1_pin;
    uint8_t             gd2_pin;
    uint8_t             spi_ss_pin;
    NRF_SPI_Type        *spi;
} CC1101_t;

typedef struct S_RF_SETTINGS
{
    uint8_t FSCTRL1;   // Frequency synthesizer control.
    uint8_t FSCTRL0;   // Frequency synthesizer control.
    uint8_t FREQ2;     // Frequency control word, high byte.
    uint8_t FREQ1;     // Frequency control word, middle byte.
    uint8_t FREQ0;     // Frequency control word, low byte.
    uint8_t MDMCFG4;   // Modem configuration.
    uint8_t MDMCFG3;   // Modem configuration.
    uint8_t MDMCFG2;   // Modem configuration.
    uint8_t MDMCFG1;   // Modem configuration.
    uint8_t MDMCFG0;   // Modem configuration.
    uint8_t CHANNR;    // Channel number.
    uint8_t DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    uint8_t FREND1;    // Front end RX configuration.
    uint8_t FREND0;    // Front end RX configuration.
    uint8_t MCSM0;     // Main Radio Control State Machine configuration.
    uint8_t FOCCFG;    // Frequency Offset Compensation Configuration.
    uint8_t BSCFG;     // Bit synchronization Configuration.
    uint8_t AGCCTRL2;  // AGC control.
	uint8_t AGCCTRL1;  // AGC control.
    uint8_t AGCCTRL0;  // AGC control.
    uint8_t FSCAL3;    // Frequency synthesizer calibration.
    uint8_t FSCAL2;    // Frequency synthesizer calibration.
	uint8_t FSCAL1;    // Frequency synthesizer calibration.
    uint8_t FSCAL0;    // Frequency synthesizer calibration.
    uint8_t FSTEST;    // Frequency synthesizer calibration control
    uint8_t TEST2;     // Various test settings.
    uint8_t TEST1;     // Various test settings.
    uint8_t TEST0;     // Various test settings.
    uint8_t FIFOTHR;   // RXFIFO and TXFIFO thresholds.
    uint8_t IOCFG2;    // GDO2 output pin configuration
    uint8_t IOCFG0;    // GDO0 output pin configuration
    uint8_t PKTCTRL1;  // Packet automation control.
    uint8_t PKTCTRL0;  // Packet automation control.
    uint8_t ADDR;      // Device address.
    uint8_t PKTLEN;    // Packet length.
} RF_SETTINGS;


uint8_t Init_CC1101(CC1101_t * cc1101_init);
uint8_t PowerupReset_CC1101();
uint8_t WriteReg_CC1101(uint8_t addr, uint8_t value);
uint8_t WriteBurstReg_CC1101(uint8_t addr, uint8_t *buffer, uint8_t count);
uint8_t ReadReg_CC1101(uint8_t addr);
uint8_t ReadBurstReg_CC1101(uint8_t addr, uint8_t *buffer, uint8_t count);
uint8_t ReadStatus_CC1101(uint8_t addr);
uint8_t WriteStrobe_CC1101(uint8_t strobe);
uint8_t writeRfSettings(RF_SETTINGS * pRfSettings);
uint8_t RFSendPacket(uint8_t *txBuffer, uint8_t size);
uint8_t RFReceivePacket(uint8_t *rxBuffer, uint8_t length);

#endif //CC1101_CONFIG_H
