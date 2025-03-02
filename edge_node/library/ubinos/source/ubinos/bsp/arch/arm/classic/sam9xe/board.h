/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \dir
/// !Purpose
/// 
/// Definition and functions for using AT91SAM9XE-related features, such
/// has PIO pins, memories, etc.
/// 
/// !Usage
/// -# The code for booting the board is provided by board_cstartup.S and
///    board_lowlevel.c.
/// -# For using board PIOs, board characteristics (clock, etc.) and external
///    components, see board.h.
/// -# For manipulating memories (remapping, SDRAM, etc.), see board_memories.h.
//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------
/// \unit
/// !Purpose
/// 
/// Definition of AT91SAM9XE-EK characteristics, AT91SAM9XE-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM9XE-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM9XE-EK - PIO definitions".
/// -# Several USB definitions are included here (see "SAM9XE-EK - USB device").
/// -# For external components definitions, see "SAM79260-EK - External components".
/// -# For memory-related definitions, see "SAM79260-EK - Memories".
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - Board Description"
/// This page lists several definition related to the board description.
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME              "AT91SAM9XE-EK"
/// Board definition.
#define at91sam9xeek
/// Family definition.
#define at91sam9xe
/// CP15 available
#define CP15_PRESENT
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
/// 
/// !Definitions
/// - BOARD_MAINOSC
/// - BOARD_MCK

/// Frequency of the board main oscillator.
#define BOARD_MAINOSC           18432000

/// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               ((18432000 * 97 / 9) / 2)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ADC
//------------------------------------------------------------------------------
/// ADC clock frequency, at 10-bit resolution (in Hz)
#define ADC_MAX_CK_10BIT         5000000
/// Startup time max, return from Idle mode (in �s)
#define ADC_STARTUP_TIME_MAX       15
/// Track and hold Acquisition Time min (in ns)
#define ADC_TRACK_HOLD_TIME_MIN  1200


//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - USB device"
/// This page lists constants describing several characteristics (controller
/// type, D+ pull-up type, etc.) of the USB device controller of the chip/board.
/// 
/// !Constants
/// - BOARD_USB_UDP
/// - BOARD_USB_PULLUP_INTERNAL
/// - BOARD_USB_NUMENDPOINTS
/// - BOARD_USB_ENDPOINTS_MAXPACKETSIZE
/// - BOARD_USB_ENDPOINTS_BANKS
/// - BOARD_USB_BMATTRIBUTES

/// Chip has a UDP controller.
#define BOARD_USB_UDP

/// Indicates the D+ pull-up is internal to the USB controller.
#define BOARD_USB_PULLUP_INTERNAL

/// Number of endpoints in the USB controller.
#define BOARD_USB_NUMENDPOINTS                  6

/// Returns the maximum packet size of the given endpoint.
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE(i)    ((i >= 4) ? 512 : 64)
#define BOARD_USB_ENDPOINTS_MAXPACKETSIZE_FS    64

/// Returns the number of FIFO banks for the given endpoint.
#define BOARD_USB_ENDPOINTS_BANKS(i)            (((i == 0) || (i == 3)) ? 1 : 2)

/// USB attributes configuration descriptor (bus or self powered, remote wakeup)
#define BOARD_USB_BMATTRIBUTES                  USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - PIO definitions"
/// This pages lists all the pio definitions contained in board.h. The constants
/// are named using the following convention: PIN_* for a constant which defines
/// a single Pin instance (but may include several PIOs sharing the same
/// controller), and PINS_* for a list of Pin instances.
///
/// !DBGU
/// - PINS_DBGU
/// 
/// !LEDs
/// - PIN_LED_0
/// - PIN_LED_1
/// - PINS_LEDS
/// - LED_POWER
/// - LED_DS1
/// 
/// !Push buttons
/// - PIN_PUSHBUTTON_1
/// - PIN_PUSHBUTTON_2
/// - PINS_PUSHBUTTONS
/// - PUSHBUTTON_BP1
/// - PUSHBUTTON_BP2
/// - JOYSTICK_LEFT
/// - JOYSTICK_RIGHT
/// 
/// !USART0
/// - PIN_USART0_RXD
/// - PIN_USART0_TXD
/// - PIN_USART0_SCK
/// 
/// !SPI0
/// - PIN_SPI0_MISO
/// - PIN_SPI0_MOSI
/// - PIN_SPI0_SPCK
/// - PINS_SPI0
/// - PIN_SPI0_NPCS0
/// - PIN_SPI0_NPCS1
/// 
/// !SSC
/// - PINS_SSC_TX
/// 
/// !USB
/// - PIN_USB_VBUS
/// 
/// !MCI
/// - PINS_MCI
/// 
/// !ADC
/// - PIN_ADC_ADC0
/// - PIN_ADC_ADC1
/// - PIN_ADC_ADC2
/// - PIN_ADC_ADC3
/// - PIN_ADC_ADC4
/// - PIN_ADC_ADC5
/// - PIN_ADC_ADC6
/// - PIN_ADC_ADC7
/// - PINS_ADC
///
/// !TWI0
/// - PINS_TWI0

/// List of all DBGU pin definitions.
#define PINS_DBGU  {(1<<14) | (1<<15), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_0   {1 << 9, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1   {1 << 6, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
/// List of all LED definitions.
#define PINS_LEDS   PIN_LED_0, PIN_LED_1
/// Power LED index.
#define LED_POWER       0
/// DS1 LED index.
#define LED_DS1         1

/// Push button #1 pin definition.
#define PIN_PUSHBUTTON_1  {1 << 30, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
/// Pusb button #2 pin definition.
#define PIN_PUSHBUTTON_2  {1 << 31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_PULLUP}
/// List of all pushbutton pin definitions.
#define PINS_PUSHBUTTONS  PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2
/// Push button #1 index.
#define PUSHBUTTON_BP1   0
/// Push button #2 index.
#define PUSHBUTTON_BP2   1
/// Simulated joystick LEFT index.
#define JOYSTICK_LEFT    0
/// Simulated joystick RIGHT index.
#define JOYSTICK_RIGHT   1

/// USART0 TXD pin definition.
#define PIN_USART0_TXD  {1 << 4, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RXD pin definition.
#define PIN_USART0_RXD  {1 << 5, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 RTS pin definition.
#define PIN_USART0_RTS  {1 << 26, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 CTS pin definition.
#define PIN_USART0_CTS  {1 << 27, AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 SCK pin definition.
#define PIN_USART0_SCK  {1 << 31, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// SPI0 MISO pin definition.
#define PIN_SPI0_MISO  {1 << 0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
/// SPI0 MOSI pin definition.
#define PIN_SPI0_MOSI  {1 << 1, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI0 SPCK pin definition.
#define PIN_SPI0_SPCK  {1 << 2, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// List of SPI0 pin definitions (MISO, MOSI & SPCK).
#define PINS_SPI0      PIN_SPI0_MISO, PIN_SPI0_MOSI, PIN_SPI0_SPCK
/// SPI0 chip select 0 pin definition.
#define PIN_SPI0_NPCS0 {AT91C_PA3_SPI0_NPCS0, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI0 chip select 1 pin definition.
#define PIN_SPI0_NPCS1 {AT91C_PC11_SPI0_NPCS1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT}

/// SSC transmitter pins definition.
#define PINS_SSC_TX  {0x00038000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// USB VBus monitoring pin definition.
#define PIN_USB_VBUS    {1 << 5, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}

/// List of MCI pins definitions.
#define PINS_MCI  {0x0000003B, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}, \
                  {1 << 8, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}

/// ADC_AD0 pin definition.
#define PIN_ADC_ADC0 {1 << 0, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/// ADC_AD1 pin definition.
#define PIN_ADC_ADC1 {1 << 1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/// ADC_AD2 pin definition.
#define PIN_ADC_ADC2 {1 << 2, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/// ADC_AD3 pin definition.
#define PIN_ADC_ADC3 {1 << 3, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/// Pins ADC
#define PINS_ADC PIN_ADC_ADC0, PIN_ADC_ADC1, PIN_ADC_ADC2, PIN_ADC_ADC3

/// TWI0 pins definition.
#define PINS_TWI0  {0x01800000, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the AT91SAM9XE-EK.
/// 
/// !AT45 Dataflash Card (A)
/// - BOARD_AT45_A_SPI_BASE
/// - BOARD_AT45_A_SPI_ID
/// - BOARD_AT45_A_SPI_PINS
/// - BOARD_AT45_A_SPI
/// - BOARD_AT45_A_NPCS
/// - BOARD_AT45_A_NPCS_PIN
/// 
/// !AT45 Dataflash (B)
/// - BOARD_AT45_B_SPI_BASE
/// - BOARD_AT45_B_SPI_ID
/// - BOARD_AT45_B_SPI_PINS
/// - BOARD_AT45_B_SPI
/// - BOARD_AT45_B_NPCS
/// - BOARD_AT45_B_NPCS_PIN
/// 
/// !SD Card
/// - BOARD_SD_MCI_BASE
/// - BOARD_SD_MCI_ID
/// - BOARD_SD_PINS
/// - BOARD_SD_SLOT
///
///
/// !EMAC
/// - AT91C_BASE_EMAC
/// - BOARD_EMAC_POWER_ALWAYS_ON
/// - BOARD_EMAC_MODE_RMII
/// - BOARD_EMAC_PINS
/// - BOARD_EMAC_PIN_TEST
/// - BOARD_EMAC_PIN_RPTR
/// - BOARD_EMAC_RST_PINS
/// - BOARD_EMAC_RUN_PINS
///
/// !ISI
/// - BOARD_ISI_PIO_CNTRL1
/// - BOARD_ISI_PIO_CNTRL2
/// - BOARD_ISI_TWCK
/// - BOARD_ISI_TWD
/// - BOARD_ISI_MCK
/// - BOARD_ISI_VSYNC
/// - BOARD_ISI_HSYNC
/// - BOARD_ISI_PCK
/// - BOARD_ISI_PINS_DATA


/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT45_A_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_A_NPCS             0
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_A_NPCS_PIN         PIN_SPI0_NPCS0

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_B_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT45_B_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_B_NPCS             1
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_B_NPCS_PIN         PIN_SPI0_NPCS1

/// Base address of SPI peripheral connected to the serialflash.
#define BOARD_AT26_A_SPI_BASE         AT91C_BASE_SPI0
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT26_A_SPI_ID           AT91C_ID_SPI0
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT26_A_SPI_PINS         PINS_SPI0
/// Dataflahs SPI number.
#define BOARD_AT26_A_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT26_A_NPCS             0
/// Chip select pin connected to the dataflash.
#define BOARD_AT26_A_NPCS_PIN         PIN_SPI0_NPCS0

/// Base address of the MCI peripheral connected to the SD card.
#define BOARD_SD_MCI_BASE           AT91C_BASE_MCI
/// Peripheral identifier of the MCI connected to the SD card.
#define BOARD_SD_MCI_ID             AT91C_ID_MCI
/// MCI pins that shall be configured to access the SD card.
#define BOARD_SD_PINS               PINS_MCI
/// SD card detection pin definition.
//#define BOARD_SD_PIN_CD     {1 << 9, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}
/// MCI slot to which the SD card is connected to.
#define BOARD_SD_SLOT               MCI_SD_SLOTB

/// Base address of the SPI peripheral connected to the SD card.
#define BOARD_SD_SPI_BASE   AT91C_BASE_SPI0
/// Identifier of the SPI peripheral connected to the SD card.
#define BOARD_SD_SPI_ID     AT91C_ID_SPI0
/// List of pins to configure to access the SD card
#define BOARD_SD_SPI_PINS   PINS_SPI0, PIN_SPI0_NPCS0
/// NPCS number
#define BOARD_SD_NPCS       0

/// Board EMAC base address
#if !defined(AT91C_BASE_EMAC) && defined(AT91C_BASE_EMACB)
#define AT91C_BASE_EMAC             AT91C_BASE_EMACB
#endif
/// Board EMAC power control - ALWAYS ON
#define BOARD_EMAC_POWER_ALWAYS_ON
/// Board EMAC work mode - RMII/MII ( 1 / 0 )
#define BOARD_EMAC_MODE_RMII        1
/// The PIN list of PIO for EMAC
#define BOARD_EMAC_PINS     { ((1<<19)|(1<<13)|(1<<12)|(1<<16)|(1<<15)|(1<<14)\
                              |(1<<17)|(1<<18)|(1<<20)|(1<<21)|(1<<7)),\
                              AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},\
                            { ((1<<11)|(1<<10)|(1<<26)|(1<<25)|(1<<27)|(1<<22)\
                              |(1<<29)|(1<<28)),\
                              AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_B, PIO_DEFAULT}
/// The power up reset latch PIO for PHY
#define BOARD_EMAC_PIN_TEST   {(1<<17), AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
//#define BOARD_EMAC_PIN_RMII : connected to 3v3 (RMII)
// We force the address
// (1<<14) PHY address 0, (1<<15) PHY address 1 (PIO A, perih A)
// (1<<25) PHY address 2, (1<<26) PHY address 3 (PIO A, perih B)
#define BOARD_EMAC_PINS_PHYAD { ((1<<14)|(1<<15)),\
                                AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT},\
                              { ((1<<25)|(1<<26)),\
                                AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT}
//#define BOARD_EMAC_PIN_10BT : not connected
#define BOARD_EMAC_PIN_RPTR   {(1<<27), AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// The PIN Configure list for EMAC on power up reset
#define BOARD_EMAC_RST_PINS BOARD_EMAC_PINS_PHYAD,\
                            BOARD_EMAC_PIN_TEST,\
                            BOARD_EMAC_PIN_RPTR
/// The runtime pin configure list for EMAC
#define BOARD_EMAC_RUN_PINS BOARD_EMAC_PINS

/// ISI
#define BOARD_ISI_PIO_CTRL1 {(1<<4),  AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}
#define BOARD_ISI_PIO_CTRL2 {(1<<19), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
#define BOARD_ISI_TWCK      {(1<<24), AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define BOARD_ISI_TWD       {(1<<23), AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
// PCK1 use instead of ISI_MCK
#define BOARD_ISI_MCK       {(1<<31), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}
#define BOARD_ISI_VSYNC     {(1<<29), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT}
#define BOARD_ISI_HSYNC     {(1<<30), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT}
#define BOARD_ISI_PCK       {(1<<28), AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT}
#define BOARD_ISI_PINS_DATA {(1<<20)|(1<<21)|(1<<22)|(1<<23)|(1<<24)|(1<<25)|(1<<26)|(1<<27)|\
                             (1<<10)|(1<<11)|(1<<12)|(1<<13),\
                             AT91C_BASE_PIOB, AT91C_ID_PIOB, PIO_PERIPH_B, PIO_DEFAULT}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - Memories"
/// This page lists definitions related to external on-board memories.
/// 
/// !Embedded Flash
/// - BOARD_FLASH_EEFC
///
/// !SDRAM
/// - BOARD_SDRAM_SIZE
/// - PINS_SDRAM
/// - BOARD_SDRAM_BUSWIDTH
///
/// !Nandflash
/// - PINS_NANDFLASH
/// - BOARD_NF_COMMAND_ADDR
/// - BOARD_NF_ADDRESS_ADDR
/// - BOARD_NF_DATA_ADDR
/// - BOARD_NF_CE_PIN
/// - BOARD_NF_RB_PIN

/// Indicates chip has an Enhanced EFC.
#define BOARD_FLASH_EEFC
/// Address of the IAP function in ROM.
#define BOARD_FLASH_IAP_ADDRESS         0x100008

/// Board SDRAM address
#define BOARD_SDRAM_ADDRESS     0x20000000
/// Board SDRAM size
#define BOARD_SDRAM_SIZE        (64*1024*1024)  // 64 MB
/// List of all SDRAM pins definitions.
#define PINS_SDRAM              {0xFFFF0000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// SDRAM bus width.
#define BOARD_SDRAM_BUSWIDTH    32


/// Nandflash controller peripheral pins definition.
#define PINS_NANDFLASH         BOARD_NF_CE_PIN, BOARD_NF_RB_PIN
/// Nandflash chip enable pin definition.
#define BOARD_NF_CE_PIN        {1 << 14, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// Nandflash ready/busy pin definition.
#define BOARD_NF_RB_PIN        {1 << 13, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
/// Address for transferring command bytes to the nandflash.
#define BOARD_NF_COMMAND_ADDR  0x40400000
/// Address for transferring address bytes to the nandflash.
#define BOARD_NF_ADDRESS_ADDR  0x40200000
/// Address for transferring data bytes to the nandflash.
#define BOARD_NF_DATA_ADDR     0x40000000

/// Address for transferring command bytes to the norflash.
#define BOARD_NORFLASH_ADDR  0x10000000
/// NorFlash bus width.
#define BOARD_NORFLASH_BUSWIDTH    16

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM9XE-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the SAM9XE-EK.
/// 
/// !ISO7816
/// - PIN_SMARTCARD_CONNECT
/// - PIN_ISO7816_RSTMC
/// - PINS_ISO7816

/// Smartcard detection pin
#define PIN_SMARTCARD_CONNECT   {1 << 5, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_INPUT, PIO_DEFAULT}
/// PIN used for reset the smartcard
#define PIN_ISO7816_RSTMC       {1 << 7, AT91C_BASE_PIOA, AT91C_ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}
/// Pins used for connect the smartcard
#define PINS_ISO7816            PIN_USART0_TXD, PIN_USART0_SCK, PIN_ISO7816_RSTMC
//------------------------------------------------------------------------------

#endif //#ifndef BOARD_H

