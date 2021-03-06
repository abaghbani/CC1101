/* Address Config = No address check */
/* Base Frequency = 433.419952 */
/* CRC Autoflush = false */
/* CRC Enable = true */
/* Carrier Frequency = 433.419952 */
/* Channel Number = 0 */
/* Channel Spacing = 49.987793 */
/* Data Format = Normal mode */
/* Data Rate = 1.6489 */
/* Deviation = 1.586914 */
/* Device Address = 0 */
/* Manchester Enable = true */
/* Modulation Format = ASK/OOK */
/* PA Ramping = false */
/* Packet Length = 10 */
/* Packet Length Mode = Fixed packet length mode. Length configured in PKTLEN register */
/* Preamble Count = 2 */
/* RX Filter BW = 58.035714 */
/* Sync Word Qualifier Mode = No preamble/sync */
/* TX Power = 0 */
/* Whitening = false */
/* PA table */
#define PA_TABLE {0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00}
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
    WriteReg_CC1101(spi, TI_CC1101_PKTLEN, 0x0A);      //Packet Length
    WriteReg_CC1101(spi, TI_CC1101_PKTCTRL0, 0x04);    //Packet Automation Control
    WriteReg_CC1101(spi, TI_CC1101_FSCTRL1, 0x0E);     //Frequency Synthesizer Control
    WriteReg_CC1101(spi, TI_CC1101_FREQ2, 0x10);       //Frequency Control Word, High Byte
    WriteReg_CC1101(spi, TI_CC1101_FREQ1, 0xAB);       //Frequency Control Word, Middle Byte
    WriteReg_CC1101(spi, TI_CC1101_FREQ0, 0x85);       //Frequency Control Word, Low Byte
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG4, 0xF6);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG3, 0x0A);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG2, 0x38);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_MDMCFG1, 0x00);     //Modem Configuration
    WriteReg_CC1101(spi, TI_CC1101_DEVIATN, 0x00);     //Modem Deviation Setting
    WriteReg_CC1101(spi, TI_CC1101_MCSM0, 0x18);       //Main Radio Control State Machine Configuration
    WriteReg_CC1101(spi, TI_CC1101_FOCCFG, 0x1D);      //Frequency Offset Compensation Configuration
    WriteReg_CC1101(spi, TI_CC1101_BSCFG, 0x1C);       //Bit Synchronization Configuration
    WriteReg_CC1101(spi, TI_CC1101_AGCCTRL2, 0xC7);    //AGC Control
    WriteReg_CC1101(spi, TI_CC1101_AGCCTRL1, 0x00);    //AGC Control
    WriteReg_CC1101(spi, TI_CC1101_AGCCTRL0, 0xB0);    //AGC Control
    WriteReg_CC1101(spi, TI_CC1101_FREND1, 0xB6);      //Front End RX Configuration
    WriteReg_CC1101(spi, TI_CC1101_FREND0, 0x11);      //Front End TX Configuration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL3, 0xE9);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL2, 0x2A);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL1, 0x00);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_FSCAL0, 0x1F);      //Frequency Synthesizer Calibration
    WriteReg_CC1101(spi, TI_CC1101_TEST2, 0x81);       //Various Test Settings
    WriteReg_CC1101(spi, TI_CC1101_TEST1, 0x35);       //Various Test Settings
    WriteReg_CC1101(spi, TI_CC1101_TEST0, 0x09);       //Various Test Settings
}

#endif
