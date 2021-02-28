#include "CC1101_regs.h"



void main(void)
{
    // spi initialization:
    SPISetup();

    // CC1101 reset:
    PowerupResetCC1101();

    // CC1101 initialization:

    // Chip version = F   (VERSION = 0x03)
    // Crystal accuracy = 10 ppm
    // X-tal frequency = 26 MHz
    // RF output power = 0 dBm
    // RX filterbandwidth = 325.000000 kHz
    // Deviation = 48 kHz
    // Datarate = 99.975586 kBaud
    // Modulation = (0) 2-FSK
    // Manchester enable = (0) Manchester disabled
    // RF Frequency = 432.999817 MHz
    // Channel spacing = 199.951172 kHz
    // Channel number = 0
    // Optimization = -
    // Sync mode = (0) No preamble/sync
    // Format of RX/TX data = (1) Serial Synchronous mode, used for backwards compatibility
    // CRC operation = (0) CRC disabled for TX and RX
    // Forward Error Correction = (0) FEC disabled
    // Length configuration = (2) Enable infinite length packets.
    // Packetlength = 255
    // Preamble count = (2)  4 bytes
    // Append status = 0
    // Address check = (0) No address check
    // FIFO autoflush = 0
    // Device address = 0
    // GDO0 signal selection = (12) Serial Synchronous Data Output
    // GDO2 signal selection = (11) Serial Clock
    RF_SETTINGS code rfSettings = {
        0x08,   // FSCTRL1   Frequency synthesizer control.
        0x00,   // FSCTRL0   Frequency synthesizer control.
        0x10,   // FREQ2     Frequency control word, high byte.
        0xA7,   // FREQ1     Frequency control word, middle byte.
        0x62,   // FREQ0     Frequency control word, low byte.
        0x5B,   // MDMCFG4   Modem configuration.
        0xF8,   // MDMCFG3   Modem configuration.
        0x00,   // MDMCFG2   Modem configuration.
        0x22,   // MDMCFG1   Modem configuration.
        0xF8,   // MDMCFG0   Modem configuration.
        0x00,   // CHANNR    Channel number.
        0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
        0xB6,   // FREND1    Front end RX configuration.
        0x10,   // FREND0    Front end TX configuration.
        0x18,   // MCSM0     Main Radio Control State Machine configuration.
        0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
        0x1C,   // BSCFG     Bit synchronization Configuration.
        0xC7,   // AGCCTRL2  AGC control.
        0x00,   // AGCCTRL1  AGC control.
        0xB2,   // AGCCTRL0  AGC control.
        0xEA,   // FSCAL3    Frequency synthesizer calibration.
        0x2A,   // FSCAL2    Frequency synthesizer calibration.
        0x00,   // FSCAL1    Frequency synthesizer calibration.
        0x1F,   // FSCAL0    Frequency synthesizer calibration.
        0x59,   // FSTEST    Frequency synthesizer calibration.
        0x81,   // TEST2     Various test settings.
        0x35,   // TEST1     Various test settings.
        0x09,   // TEST0     Various test settings.
        0x07,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
        0x0B,   // IOCFG2    GDO2 output pin configuration.
        0x0C,   // IOCFG0D   GDO0 output pin configuration. 
        0x00,   // PKTCTRL1  Packet automation control.
        0x12,   // PKTCTRL0  Packet automation control.
        0x00,   // ADDR      Device address.
        0xFF    // PKTLEN    Packet length.
    };



    writeRFSettings(&rfSettings);
    
    // don't know: BUGBUG: fixme!!!
    SpiWriteReg(CCxxx0_IOCFG1, 0x0B);    // Serial clock
    
    // PA setting 
    // PATABLE (0 dBm output power)
    BYTE code paTable = 0x60;
    SpiWriteReg(CCxxx0_PATABLE, paTable);
	
    while (TRUE)
    {
        switch (mode) 
        {
            case RX:
                // Infinite loop    
                while (TRUE) {
                    if (halRfReceivePacketSerial(rxBuffer, SYNC3, SYNC2, SYNC1, SYNC0, VARIABLE_LENGTH, CRC_ENABLE)) {
                        intToAscii(++packetsReceived);
                        ebLcdUpdate("Received:", asciiString);
                    }
                }
                break;
                
            case TX:

                P0MDOUT |= GDO0_;   // In TX, GDO0 is serial data input, and must be configured as a push/pull output
                                    // on the MCU
                while (TRUE) 
                {
                    while (!ebButtonPushed());    
                    halRfSendPacketSerial(txBuffer, sizeof(txBuffer), START_OF_PAYLOAD, CRC_ENABLE);
                    intToAscii(++packetsSent);
                    ebLcdUpdate("Sent:", asciiString);
                }
                break;         
        }
    }
}
