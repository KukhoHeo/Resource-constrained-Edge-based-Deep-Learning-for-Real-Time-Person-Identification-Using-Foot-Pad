/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA)

#if !(INCLUDE__UBINOS__UBIK == 1)

void HardFault_Handler_C(unsigned int exceptiontype, unsigned int * prev_sp);

void __attribute__((naked)) HardFault_Handler(void) {
    __asm__ __volatile__ (
        "mrs    r0, ipsr                \n\t" // get exception number
        "tst    lr, #4                  \n\t" // test bit 2 of EXC_RETURN
        "ite    eq                      \n\t"
        "mrseq  r1, msp                 \n\t" // if 0, stacking used msp, copy to r0
        "mrsne  r1, psp                 \n\t" // if 1, stacking used psp, copy to r0
        "b      bsp_ubik_irq_handler_c  \n\t"
    );
}

void HardFault_Handler_C(unsigned int exceptiontype, unsigned int * prev_sp) {
#if (UBINOS__BSP__NRF52_IGNORE_HARDFAULT_EXCEPTION_FROM_SOFTDEVICE == 1)
    uint16_t prev_insturction;

    if (exceptiontype == 3) // hard fault
    {
        prev_insturction = *((uint16_t *) &(((char *)prev_sp[6])[-2]));
        if (prev_insturction == 0xDFFF) // svc 0xff
        {
            // ignore
            return;
        }
    }
#endif /* (UBINOS__BSP__NRF52_IGNORE_HARDFAULT_EXCEPTION_FROM_SOFTDEVICE == 1) */

    dtty_puts("\n\nunknown exception (", 80);
    switch (exceptiontype) {
    default:
    	if (exceptiontype >= 100) {
            dtty_putc((int) ((exceptiontype / 100 % 10) + '0'));
    	}
    	if (exceptiontype >= 10) {
            dtty_putc((int) ((exceptiontype / 10 % 10) + '0'));
    	}
    	dtty_putc((int) ((exceptiontype % 10) + '0'));
        break;
    }
    dtty_puts(")\n", 80);
    bsp_abortsystem();
}

#endif /* !(INCLUDE__UBINOS__UBIK == 1) */

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52832XXAA) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__NRF52840XXAA) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */
