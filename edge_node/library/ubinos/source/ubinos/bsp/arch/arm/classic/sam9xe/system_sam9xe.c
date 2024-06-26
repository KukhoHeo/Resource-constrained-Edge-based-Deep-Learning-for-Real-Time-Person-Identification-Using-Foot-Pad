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

void SystemInit(void) {
	unsigned int reg;

	/* Remap the memory */
    extern unsigned int relocated_isr_vector_start  __asm__ ("__relocated_isr_vector_start__");
    extern unsigned int ram_start            __asm__ ("__ram_start__");
	if (&relocated_isr_vector_start == &ram_start) {
		AT91C_BASE_MATRIX->MATRIX_MRCR = (AT91C_MATRIX_RCA926I | AT91C_MATRIX_RCA926D);
	}
	else {
		AT91C_BASE_MATRIX->MATRIX_MRCR = 0x00000000;
	}

    reg = arm_get_cp15();

#if (UBINOS__BSP__USE_MMU == 1)
    /* Enable MMU */
    reg |= (0x1 <<  0);
#endif

#if (UBINOS__BSP__USE_ICACHE == 1)
    /* Enable i-cache */
    reg |= (0x1 << 12);
#endif

#if (UBINOS__BSP__USE_DCACHE == 1)
    /* Enable d-cache */
    reg |= (0x1 <<  2);
#endif

    arm_set_cp15(reg);
}

#pragma GCC pop_options

__attribute__((weak)) void SystemInit2(void)
{
}

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

