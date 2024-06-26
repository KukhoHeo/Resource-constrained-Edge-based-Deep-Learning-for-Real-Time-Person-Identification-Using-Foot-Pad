/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__ARM926EJ_S)

#include <stdio.h>

#if     (0x50 > UBINOS__BSP__STACK_OVERFLOW_CHECK_MARGIN)
    #error "UBINOS__BSP__STACK_OVERFLOW_CHECK_MARGIN should be equal or larger than 0x50."
#endif

volatile unsigned int _bsp_intrcount = 0;

int bsp_isintr(void) {
    if (0 == _bsp_intrcount) {
        return 0;
    } else {
        return 1;
    }
}

#pragma GCC push_options
#pragma GCC target ("arm")

void bsp_busywait(unsigned int count) {
    __asm__ __volatile__ (                                                  \
        "cmp    r0, #0                                              \n\t"   \
        "beq    2f                                                  \n\t"   \
        "1:                                                         \n\t"   \
        "subs   r0, r0, #1                                          \n\t"   \
        "bne    1b                                                  \n\t"   \
        "2:                                                         \n\t"   \
    );
}

void bsp_abortsystem(void) {
    if (!_bsp_aborted)
    {
        _bsp_aborted = 1;

        ARM_INTERRUPT_DISABLE();
        dtty_puts("\n\nsystem is aborted\n\n", 80);
    }

#if (UBINOS__BSP__ABORTSYSTEM_TYPE == UBINOS__BSP__ABORTSYSTEM_TYPE__STOP)
    __asm__ __volatile__ (
            "1:                                                 \n\t"
            "b  1b                                              \n\t"
    );
#elif (UBINOS__BSP__ABORTSYSTEM_TYPE == UBINOS__BSP__ABORTSYSTEM_TYPE__RESET)
    AT91C_BASE_RSTC->RSTC_RCR = AT91C_RSTC_KEY |
                                AT91C_RSTC_PERRST |
                                AT91C_RSTC_EXTRST |
                                AT91C_RSTC_PROCRST;

    while (AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_SRCMP);
#else
    #error "Unsupported UBINOS__BSP__ABORTSYSTEM_TYPE"
#endif
}

void bsp_enableintr(void) {
    ARM_INTERRUPT_ENABLE();
}

void bsp_disableintr(void) {
    ARM_INTERRUPT_DISABLE();
}

#define _exception_check_bsp_critcount() {                              \
    if (0 != _bsp_critcount) {                                          \
        dtty_puts("\ninterrupt disable fail\n", 80);        \
        bsp_abortsystem();                                              \
    }                                                                   \
}

void bsp_exception_handler(unsigned int exceptiontype, unsigned int swino) {
    isr_ft isr = 0;

    _bsp_intrcount++;

    switch (exceptiontype) {

    case EXCEPTION_TYPE_IRQ:

        isr = (isr_ft) AT91C_BASE_AIC->AIC_IVR;
        AT91C_BASE_AIC->AIC_IVR = 0;

        _exception_check_bsp_critcount();

        if (isr != (isr_ft) 0x0) {
            (*isr)();
        }

        bsp_ubik_task_schedule_irq();

        AT91C_BASE_AIC->AIC_EOICR = 0;

        break;

    case EXCEPTION_TYPE_SWI:

        bsp_ubik_swisr(swino);

        break;

    case EXCEPTION_TYPE_UND:
    case EXCEPTION_TYPE_PABT:
    case EXCEPTION_TYPE_DABT:
    case EXCEPTION_TYPE_FIQ:
    default:
        dtty_puts("\n\nunknown exception (", 80);
        switch (exceptiontype) {
        case EXCEPTION_TYPE_UND:
            dtty_puts("UND", 80);
            break;
        case EXCEPTION_TYPE_SWI:
            dtty_puts("SWI", 80);
            break;
        case EXCEPTION_TYPE_PABT:
            dtty_puts("PABT", 80);
            break;
        case EXCEPTION_TYPE_DABT:
            dtty_puts("DABT", 80);
            break;
        case EXCEPTION_TYPE_IRQ:
            dtty_puts("FIQ", 80);
            break;
        case EXCEPTION_TYPE_FIQ:
            dtty_puts("FIQ", 80);
            break;
        default:
            dtty_putc((int) (exceptiontype + '0'));
            break;
        }
        dtty_puts(")\n", 80);
        bsp_abortsystem();
        break;
    }

    _bsp_intrcount--;
}

void bsp_spurious_exception_handler(void) {
    dtty_puts("\n\nspurious interrupt\n", 80);
    bsp_abortsystem();
}

int bsp_getstacksize(int type, unsigned int * stacksize_p) {
    if (NULL == stacksize_p) {
        return -3;
    }

    *stacksize_p = __STACK_SIZE;

    return 0;
}

int bsp_getmaxstackusage(int type, unsigned int * maxstackusage_p) {
    if (NULL != maxstackusage_p) {
        *maxstackusage_p = 0;
    }

    return -1;
}

unsigned int bsp_getbusywaitcountperms(void) {
    return UBINOS__BSP__BUSYWAITCOUNT_PER_MS;
}

unsigned int bsp_timemstobwc(unsigned int timems) {
    return (timems * UBINOS__BSP__BUSYWAITCOUNT_PER_MS);
}

unsigned int bsp_bwctotimems(unsigned int count) {
    return (count / UBINOS__BSP__BUSYWAITCOUNT_PER_MS);
}


void bsp_busywaitms(unsigned int timems) {
    unsigned int count;

    count = bsp_timemstobwc(timems);
    if (count < timems) {
        count = UINT_MAX;
    }

    bsp_busywait(count);
}

void bsp_busywaitus(unsigned int timems) {
    unsigned int count;

    count = bsp_timemstobwc(timems);
    if (count < timems) {
        count = UINT_MAX;
    }

    count /= 1000;
    if (count == 0) {
        count = 1;
    }

    bsp_busywait(count);
}


unsigned int arm_get_cp15(void) {
    unsigned int value;
    __asm__ __volatile__ ("mrc p15, 0, %0, c1, c0, 0" : "=r" (value));
    return value;
}

void arm_set_cp15(unsigned int value) {
    __asm__ __volatile__ ("mcr p15, 0, %0, c1, c0, 0" : : "r" (value));
}

unsigned int arm_get_cpsr(void) {
    unsigned int value;
    __asm__ __volatile__ ("mrs %0, cpsr" : "=r" (value));
    return value;
}

#pragma GCC pop_options

#endif /* (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__ARM926EJ_S) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

