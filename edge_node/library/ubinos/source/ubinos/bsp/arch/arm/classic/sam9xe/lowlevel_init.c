/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)

#include "board_memories.h"

#pragma GCC push_options
#pragma GCC target ("arm")
#pragma GCC optimize ("O0")

void lowlevel_init(void) {
	unsigned int i;

	/* Set flash wait states in the EFC
	   Flash wait states : 6 (7 cycle) */
	AT91C_BASE_EFC->EFC_FMR =	(AT91C_EFC_FWS		& (6 << 8));

	/* Disable watchdog */
	AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;

	/* When using the JTAG debugger the hardware is not always initialized to
	   the correct default state.  This line just ensures that this does not
	   cause all interrupts to be masked at the start. */
	for (i = 0; i < 8; i++) {
		AT91C_BASE_AIC->AIC_EOICR = 0;
	}

	/* Init PMC step 1.
	   Enable main sscillator (Y1 =  18.432MHz)
	   Main Oscillator startup time (board specific): slow clock tick * 0x40 */
	AT91C_BASE_PMC->PMC_MOR =	(AT91C_CKGR_OSCOUNT		& (0x40 <<  8)	)	|
								(AT91C_CKGR_MOSCEN);
	/* Wait for main oscillator stabilization */
	while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

	/* Init PMC Step 2.
	   PLLA: 200MHz (18432000 * (0x60 + 1) / 0x09 = 198,656,000)
	   PLLA Startup time (depends on PLL RC filter): slow clock tick * 0x3f (worst case)
	   PLLB: 192MHz (18432000 * (0x7c + 1) / 0x0c = 192,000,000)
	   PLLB Startup time (depends on PLL RC filter): slow clock tick * 0x3f (worst case)
	   USB: 48MHz (PLLB / 4 = 48,000,000)	   */
	AT91C_BASE_PMC->PMC_PLLAR =	(AT91C_CKGR_SRCA)							|
								(AT91C_CKGR_OUTA_2)							|
								(AT91C_CKGR_PLLACOUNT	& (0x3f <<  8)	)	|
								(AT91C_CKGR_MULA 		& (0x60 << 16)	)	|
								(AT91C_CKGR_DIVA 		& (0x09)		);
	/* Wait for PLLA stabilization */
	while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA));

	AT91C_BASE_PMC->PMC_PLLBR = (AT91C_CKGR_USBDIV_2)						|
								(AT91C_CKGR_OUTB_1)							|
								(AT91C_CKGR_PLLBCOUNT	& (0x3f <<  8)	)	|
								(AT91C_CKGR_MULB 		& (0x7c << 16)	)	|
								(AT91C_CKGR_DIVB 		& (0x0c)		);
	/* Wait for PLLB stabilization */
	while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB));

	/* Wait for the master clock if it was already initialized */
	while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

	/* Init PMC Step 3.
	   Processor clock: 200MHz (PLLA)
	   Master clock (MCK): processor clock / 2
	   The PMC_MCKR register must not be programmed in a single write operation
	   (see. Product Errata Sheet) */
	AT91C_BASE_PMC->PMC_MCKR =	(AT91C_PMC_PRES_CLK)						|
								(AT91C_PMC_MDIV_2);
	/* Wait until the master clock is established */
	while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

	AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLLA_CLK;
	/* Wait until the master clock is established */
	while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

	/* Enable debug mode */
	AT91C_BASE_AIC->AIC_DCR = AT91C_AIC_DCR_PROT;

	/* Disable RTT and PIT interrupts (potential problem when program A
	   configures RTT, then program B wants to use PIT only, interrupts
	   from the RTT will still occur since they both use AT91C_ID_SYS) */
	AT91C_BASE_RTTC->RTTC_RTMR &= ~(AT91C_RTTC_ALMIEN | AT91C_RTTC_RTTINCIEN);
	AT91C_BASE_PITC->PITC_PIMR &= ~AT91C_PITC_PITIEN;

	/* Set spurious exception handler */
	AT91C_BASE_AIC->AIC_SPU = (unsigned int) bsp_spurious_exception_handler;

	#if (UBINOS__BSP__USE_EXTSDRAM == 1)
	/* Init SDRAM */
	BOARD_ConfigureSdram(UBINOS__BSP__EXTSDRAM_BUSWIDTH);
	#endif
	#if (UBINOS__BSP__USE_EXTNORFLASH == 1)
	/* Init NorFlash */
	BOARD_ConfigureNorFlash(UBINOS__BSP__EXTNORFLASH_BUSWIDTH);
	#endif
}

#pragma GCC pop_options

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

