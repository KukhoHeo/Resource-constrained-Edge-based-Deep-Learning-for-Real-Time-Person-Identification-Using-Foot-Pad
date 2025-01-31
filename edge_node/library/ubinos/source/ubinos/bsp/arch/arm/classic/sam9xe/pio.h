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
/// \unit
///
/// !!!Purpose
/// 
/// This file provides a basic API for PIO configuration and usage of
/// user-controlled pins. Please refer to the board.h file for a list of
/// available pin definitions.
/// 
/// !!!Usage
/// 
/// -# Define a constant pin description array such as the following one, using
///    the existing definitions provided by the board.h file if possible:
///    \code
///       const Pin pPins[] = {PIN_USART0_TXD, PIN_USART0_RXD};
///    \endcode
///    Alternatively, it is possible to add new pins by provided the full Pin
///    structure:
///    \code
///    // Pin instance to configure PA10 & PA11 as inputs with the internal
///    // pull-up enabled.
///    const Pin pPins = {
///         (1 << 10) | (1 << 11),
///         AT91C_BASE_PIOA,
///         AT91C_ID_PIOA,
///         PIO_INPUT,
///         PIO_PULLUP
///    };
///    \endcode
/// -# Configure a pin array by calling PIO_Configure() with a pointer to the
///    array and its size (which is computed using the PIO_LISTSIZE macro).
/// -# Change and get the value of a user-controlled pin using the PIO_Set,
///    PIO_Clear and PIO_Get methods.
/// -# Get the level being currently output by a user-controlled pin configured
///    as an output using PIO_GetOutputDataStatus().
//------------------------------------------------------------------------------
 
#ifndef _BOARD_PIO_H_
#define _BOARD_PIO_H_

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "board.h"

//------------------------------------------------------------------------------
//         Global Definitions
//------------------------------------------------------------------------------

/// The pin is controlled by the associated signal of peripheral A.
#define PIO_PERIPH_A                0
/// The pin is controlled by the associated signal of peripheral B.
#define PIO_PERIPH_B                1
/// The pin is an input.
#define PIO_INPUT                   2
/// The pin is an output and has a default level of 0.
#define PIO_OUTPUT_0                3
/// The pin is an output and has a default level of 1.
#define PIO_OUTPUT_1                4

/// Default pin configuration (no attribute).
#define PIO_DEFAULT                 (0 << 0)
/// The internal pin pull-up is active.
#define PIO_PULLUP                  (1 << 0)
/// The internal glitch filter is active.
#define PIO_DEGLITCH                (1 << 1)
/// The pin is open-drain.
#define PIO_OPENDRAIN               (1 << 2)

//------------------------------------------------------------------------------
//         Global Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Calculates the size of an array of Pin instances. The array must be defined
/// locally (i.e. not a pointer), otherwise the computation will not be correct.
/// \param pPins  Local array of Pin instances.
/// \return Number of elements in array.
//------------------------------------------------------------------------------
#define PIO_LISTSIZE(pPins)    (sizeof(pPins) / sizeof(Pin))

//------------------------------------------------------------------------------
//         Global Types
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Describes the type and attribute of one PIO pin or a group of similar pins.
/// The #type# field can have the following values:
///    - PIO_PERIPH_A
///    - PIO_PERIPH_B
///    - PIO_OUTPUT_0
///    - PIO_OUTPUT_1
///    - PIO_INPUT
///
/// The #attribute# field is a bitmask that can either be set to PIO_DEFAULt,
/// or combine (using bitwise OR '|') any number of the following constants:
///    - PIO_PULLUP
///    - PIO_DEGLITCH
///    - PIO_OPENDRAIN
//------------------------------------------------------------------------------
typedef struct {

    /// Bitmask indicating which pin(s) to configure.
    unsigned int mask; 
    /// Pointer to the PIO controller which has the pin(s).
    AT91S_PIO    *pio;
    /// Peripheral ID of the PIO controller which has the pin(s).
    unsigned char id;
    /// Pin type.
    unsigned char type;
    /// Pin attribute.
    unsigned char attribute;

} Pin;

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

extern unsigned char PIO_Configure(const Pin *list, unsigned int size);

extern void PIO_Set(const Pin *pin);

extern void PIO_Clear(const Pin *pin);

extern unsigned char PIO_Get(const Pin *pin);

extern unsigned int PIO_GetISR(const Pin *pin);

extern unsigned char PIO_GetOutputDataStatus(const Pin *pin);

#endif //#ifndef _BOARD_PIO_H_

