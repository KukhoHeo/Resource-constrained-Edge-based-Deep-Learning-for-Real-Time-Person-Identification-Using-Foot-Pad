/*
 * Copyright (c) 2019 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CORTEX_MX == 1)

#if (UBINOS__BSP__USE_RELOCATED_ISR_VECTOR == 1)

int intr_connectisr(int no, isr_ft isr, int priority, unsigned int option) {
    extern char __relocated_isr_vector_start__;
    unsigned int isr_vector_addr = (unsigned int) &__relocated_isr_vector_start__;

    if (NVIC_IRQN_START > no || NVIC_IRQN_END < no) {
        return -2;
    }
    if (NVIC_PRIO_LOWEST < priority || NVIC_PRIO_HIGHEST > priority) {
        return -3;
    }

    /* Disable the interrupt on the interrupt controller */
    NVIC_DisableIRQ(no);

    /* Clear the Pending Register on the interrupt controller */
    NVIC_ClearPendingIRQ(no);

    /* Set the interrupt priority */
    NVIC_SetPriority(no, priority);

    /* Register the interrupt handler on the interrupt handler table */
    isr_vector_addr += ((16 + no) * sizeof(isr_ft));
    *((isr_ft *) isr_vector_addr) = isr;

    return 0;
}

#endif /* (UBINOS__BSP__USE_RELOCATED_ISR_VECTOR == 1) */

int intr_enable(int no) {
    NVIC_EnableIRQ(no);
    return 0;
}

int intr_disable(int no) {
    NVIC_DisableIRQ(no);
    return 0;
}

int intr_clearpending(int no) {
    NVIC_ClearPendingIRQ(no);
    return 0;
}

int intr_gethighestpriority(void) {
    return NVIC_PRIO_HIGHEST;
}

int intr_getlowestpriority(void) {
    return NVIC_PRIO_LOWEST;
}

#endif /* (UBINOS__BSP__CORTEX_MX == 1) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

