/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)

int intr_connectisr(int no, isr_ft isr, int priority, unsigned int option) {
    unsigned int mask;
    unsigned int type;

    mask = 0x1 << no;

	if (0 != (option & INTR_OPT__LEVEL)) {
		if (0 != (option & INTR_OPT__HIGH)) {
			type = (0x2<<5);
		}
		else {
			type = (0x0<<5);
		}
	}
	else {
		if (0 != (option & INTR_OPT__HIGH)) {
			type = (0x3<<5);
		}
		else {
			type = (0x1<<5);
		}
	}

	type |= (AT91C_AIC_PRIOR & priority);

    /* Disable the interrupt on the interrupt controller */
    AT91C_BASE_AIC->AIC_IDCR = mask;

    /* Save the interrupt handler routine pointer and the interrupt priority */
    AT91C_BASE_AIC->AIC_SVR[no] = (unsigned int) isr;

    /* Store the Source Mode Register */
    AT91C_BASE_AIC->AIC_SMR[no] = type;

    /* Clear the interrupt on the interrupt controller */
    AT91C_BASE_AIC->AIC_ICCR = mask;

	return 0;
}

int intr_enable(int no) {
    AT91C_BASE_AIC->AIC_IECR = 1 << no;
	return 0;
}

int intr_disable(int no) {
    AT91C_BASE_AIC->AIC_IDCR = 1 << no;
	return 0;
}

int intr_gethighestpriority(void) {
	return AT91C_AIC_PRIOR_HIGHEST;
}

int intr_getlowestpriority(void) {
	return AT91C_AIC_PRIOR_LOWEST;
}

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

