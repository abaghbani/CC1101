
#if TI_CC_RF_FREQ == 315                          // 315 MHz
// Product = CC1100
// Crystal accuracy = 40 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filterbandwidth = 540.000000 kHz
// Deviation = 0.000000
// Return state:  Return to RX state upon leaving either TX or RX
// Datarate = 250.000000 kbps
// Modulation = (7) MSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 315.000000 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = Sensitivity
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (1)  address check
// FIFO autoflush = 0
// Device address = 1
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (11) Serial Clock
void writeRFSettings(void)
{
    // Write register settings
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG2,   0x0B); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG0,   0x06); // GDO0 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTLEN,   0xFF); // Packet length.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL1, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL0, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_ADDR,     0x01); // Device address.
    TI_CC_SPIWriteReg(TI_TI_CC1101_CHANNR,   0x00); // Channel number.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL1,  0x0B); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL0,  0x00); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ2,    0x0C); // Freq control word, high byte
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ1,    0x1D); // Freq control word, mid byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ0,    0x89); // Freq control word, low byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG4,  0x2D); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG3,  0x3B); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG2,  0x73); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG1,  0x22); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG0,  0xF8); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_DEVIATN,  0x00); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM1 ,   0x3F); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_FOCCFG,   0x1D); // Freq Offset Compense. Config
    TI_CC_SPIWriteReg(TI_TI_CC1101_BSCFG,    0x1C); //  Bit synchronization config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL2, 0xC7); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL1, 0x00); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL0, 0xB2); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND1,   0xB6); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND0,   0x10); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL3,   0xEA); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL2,   0x0A); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL1,   0x00); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL0,   0x11); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSTEST,   0x59); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST2,    0x88); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST1,    0x31); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST0,    0x0B); // Various test settings.
}

#endif

#if TI_CC_RF_FREQ == 433                          // 433 MHz
// Product = CC1100
// Crystal accuracy = 40 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filterbandwidth = 540.000000 kHz
// Deviation = 0.000000
// Return state:  Return to RX state upon leaving either TX or RX
// Datarate = 250.000000 kbps
// Modulation = (7) MSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 433.000000 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = Sensitivity
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (11) Serial Clock
void writeRFSettings(void)
{
    // Write register settings
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG2,   0x0B); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG0,   0x06); // GDO0 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTLEN,   0xFF); // Packet length.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL1, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL0, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_ADDR,     0x01); // Device address.
    TI_CC_SPIWriteReg(TI_TI_CC1101_CHANNR,   0x00); // Channel number.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL1,  0x0B); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL0,  0x00); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ2,    0x10); // Freq control word, high byte
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ1,    0xA7); // Freq control word, mid byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ0,    0x62); // Freq control word, low byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG4,  0x2D); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG3,  0x3B); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG2,  0x73); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG1,  0x22); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG0,  0xF8); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_DEVIATN,  0x00); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM1 ,   0x3F); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_FOCCFG,   0x1D); // Freq Offset Compens. Config
    TI_CC_SPIWriteReg(TI_TI_CC1101_BSCFG,    0x1C); //  Bit synchronization config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL2, 0xC7); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL1, 0x00); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL0, 0xB2); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND1,   0xB6); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND0,   0x10); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL3,   0xEA); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL2,   0x0A); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL1,   0x00); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL0,   0x11); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSTEST,   0x59); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST2,    0x88); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST1,    0x31); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST0,    0x0B); // Various test settings.
}

#endif


#if TI_CC_RF_FREQ == 868                          // 868 MHz
// Product = CC1100
// Crystal accuracy = 40 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filterbandwidth = 540.000000 kHz
// Deviation = 0.000000
// Return state:  Return to RX state upon leaving either TX or RX
// Datarate = 250.000000 kbps
// Modulation = (7) MSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 868.000000 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = Sensitivity
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (11) Serial Clock
void writeRFSettings(void)
{
    // Write register settings
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG2,   0x0B); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG0,   0x06); // GDO0 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTLEN,   0xFF); // Packet length.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL1, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL0, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_ADDR,     0x01); // Device address.
    TI_CC_SPIWriteReg(TI_TI_CC1101_CHANNR,   0x00); // Channel number.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL1,  0x0B); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL0,  0x00); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ2,    0x21); // Freq control word, high byte
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ1,    0x62); // Freq control word, mid byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ0,    0x76); // Freq control word, low byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG4,  0x2D); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG3,  0x3B); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG2,  0x73); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG1,  0x22); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG0,  0xF8); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_DEVIATN,  0x00); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM1 ,   0x3F); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_FOCCFG,   0x1D); // Freq Offset Compens. Config
    TI_CC_SPIWriteReg(TI_TI_CC1101_BSCFG,    0x1C); //  Bit synchronization config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL2, 0xC7); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL1, 0x00); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL0, 0xB2); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND1,   0xB6); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND0,   0x10); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL3,   0xEA); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL2,   0x0A); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL1,   0x00); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL0,   0x11); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSTEST,   0x59); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST2,    0x88); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST1,    0x31); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST0,    0x0B); // Various test settings.
}

// PATABLE (0 dBm output power)
extern char paTable[] = {0x60};
extern char paTableLen = 1;

#endif


#if TI_CC_RF_FREQ == 915                          // 915 MHz
// Product = CC1100
// Crystal accuracy = 40 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filterbandwidth = 540.000000 kHz
// Deviation = 0.000000
// Return state:  Return to RX state upon leaving either TX or RX
// Datarate = 250.000000 kbps
// Modulation = (7) MSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 915.000000 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = Sensitivity
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (11) Serial Clock
void writeRFSettings(void)
{
    // Write register settings
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG2,   0x0B); // GDO2 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_IOCFG0,   0x06); // GDO0 output pin config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTLEN,   0xFF); // Packet length.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL1, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_PKTCTRL0, 0x05); // Packet automation control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_ADDR,     0x01); // Device address.
    TI_CC_SPIWriteReg(TI_TI_CC1101_CHANNR,   0x00); // Channel number.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL1,  0x0B); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCTRL0,  0x00); // Freq synthesizer control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ2,    0x23); // Freq control word, high byte
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ1,    0x31); // Freq control word, mid byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREQ0,    0x3B); // Freq control word, low byte.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG4,  0x2D); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG3,  0x3B); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG2,  0x73); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG1,  0x22); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_MDMCFG0,  0xF8); // Modem configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_DEVIATN,  0x00); // Modem dev (when FSK mod en)
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM1 ,   0x3F); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_MCSM0 ,   0x18); //MainRadio Cntrl State Machine
    TI_CC_SPIWriteReg(TI_TI_CC1101_FOCCFG,   0x1D); // Freq Offset Compens. Config
    TI_CC_SPIWriteReg(TI_TI_CC1101_BSCFG,    0x1C); //  Bit synchronization config.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL2, 0xC7); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL1, 0x00); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_AGCCTRL0, 0xB2); // AGC control.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND1,   0xB6); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FREND0,   0x10); // Front end RX configuration.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL3,   0xEA); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL2,   0x0A); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL1,   0x00); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSCAL0,   0x11); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_FSTEST,   0x59); // Frequency synthesizer cal.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST2,    0x88); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST1,    0x31); // Various test settings.
    TI_CC_SPIWriteReg(TI_TI_CC1101_TEST0,    0x0B); // Various test settings.
}

// PATABLE (0 dBm output power)
extern char paTable[] = {0x50};
extern char paTableLen = 1;

#endif

