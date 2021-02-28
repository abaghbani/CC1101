/***************************************************************
 *  SmartRF Studio(tm) Export
 *
 *  Radio register settings specifed with C-code
 *  compatible #define statements.
 *
 *  RF device: CC1101
 *
 ***************************************************************/

#ifndef SMARTRF_CC1101_H
#define SMARTRF_CC1101_H

#include "nrf_spi.h"
#include "CC1101.h"

void writeRfSettings(NRF_SPI_Type * spi)
{
    WriteReg_CC1101(spi, TI_CC1101_IOCFG0, 0x06);      //GDO0 Output Pin Configuration
    WriteReg_CC1101(spi, TI_CC1101_FIFOTHR, 0x47);     //RX FIFO and TX FIFO Thresholds
    WriteReg_CC1101(spi, TI_CC1101_PKTCTRL0, 0x05);    //Packet Automation Control
    WriteReg_CC1101(spi, TI_CC1101_FSCTRL1, 0x06);     //Frequency Synthesizer Control
    WriteReg_CC1101(spi, TI_CC1101_FREQ2, 0x21);       //Frequency Control Word, High Byte
    WriteReg_CC1101(spi, TI_CC1101_FREQ1, 0x62);       //Frequency Control Word, Middle Byte
    WriteReg_CC1101(spi, TI_CC1101_FREQ0, 0x76);       //Frequency Control Word, Low Byte
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG4, 0xF5);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG3, 0x83);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG2, 0x13);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_DEVIATN, 0x15);     //Modem Deviation Setting
    WriteReg_CC1101(spi, TI_CC1101_MCSM0, 0x18);       //Main Radio Control State Machine Configuration
    WriteReg_CC1101(spi, TI_CC1101_FOCCFG, 0x16);      //Frequency Offset Compensation Configuration
    WriteReg_CC1101(spi, TI_CC1101_WORCTRL, 0xFB);     //Wake On Radio Control
    WriteReg_CC1101(spi, TI_CC1101_FSCAL3, 0xE9);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL2, 0x2A);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL1, 0x00);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL0, 0x1F);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_TEST2, 0x81);       //Various Test Settings
    WriteReg_CC1101(spi, TI_CC1101_TEST1, 0x35);       //Various Test Settings
    WriteReg_CC1101(spi, TI_CC1101_TEST0, 0x09);       //Various Test Settings
}

#endif
