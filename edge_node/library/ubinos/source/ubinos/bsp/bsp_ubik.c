/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/bsp.h>

#if (INCLUDE__UBINOS__BSP == 1)

#include <ubinos/bsp_ubik.h>
#if (INCLUDE__UBINOS__UBIK == 1)
#include <ubinos/ubik.h>
#endif

void bsp_ubik_entercrit(void) {
#if (INCLUDE__UBINOS__UBIK == 1)
    ubik_entercrit();
#endif
}

void bsp_ubik_exitcrit(void) {
#if (INCLUDE__UBINOS__UBIK == 1)
    ubik_exitcrit();
#endif
}

int bsp_mutex_create(bsp_mutex_pt * bsp_mutex_p) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return mutex_create((mutex_pt *) bsp_mutex_p);
#else
    return 0;
#endif
}

int bsp_mutex_delete(bsp_mutex_pt * bsp_mutex_p) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return mutex_delete((mutex_pt *) bsp_mutex_p);
#else
    return 0;
#endif
}

int bsp_mutex_lock(bsp_mutex_pt bsp_mutex) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return mutex_lock((mutex_pt) bsp_mutex);
#else
    return 0;
#endif
}

int bsp_mutex_lock_timed(bsp_mutex_pt bsp_mutex, unsigned int tick) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return mutex_lock_timed((mutex_pt) bsp_mutex, tick);
#else
    return 0;
#endif
}

int bsp_mutex_unlock(bsp_mutex_pt bsp_mutex) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return mutex_unlock((mutex_pt) bsp_mutex);
#else
    return 0;
#endif
}

void * bsp_task_getcur() {
#if (INCLUDE__UBINOS__UBIK == 1)
    return task_getcur();
#else
    return 0;
#endif
}

int bsp_task_sleepms(unsigned int timems) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return task_sleepms(timems);

#else
    bsp_busywait(bsp_timemstobwc(timems));
    return 0;
#endif
}

int bsp_ubik_isrt(void) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return ubik_isrt();
#else
    return bsp_isintr();
#endif
}

int bsp_ubik_iscrit(void) {
#if (INCLUDE__UBINOS__UBIK == 1)
    return ubik_iscrit();
#else
    return bsp_isintr();
#endif
}

void bsp_ubik_task_schedule_irq(void) {
#if (INCLUDE__UBINOS__UBIK == 1)
    extern void _ubik_task_schedule_irq();
    _ubik_task_schedule_irq();
#else
    dtty_puts("\n\bsp_ubik_task_schedule_irq\n\n", 80);
    bsp_abortsystem();
#endif
}

void bsp_ubik_swisr(unsigned int swino) {
#if (INCLUDE__UBINOS__UBIK == 1)
    extern void _ubik_swisr(unsigned int swino);
    _ubik_swisr(swino);
#else
    dtty_puts("\n\bsp_ubik_swisr\n\n", 80);
    bsp_abortsystem();
#endif
}

#endif /* (INCLUDE__UBINOS__BSP == 1) */

