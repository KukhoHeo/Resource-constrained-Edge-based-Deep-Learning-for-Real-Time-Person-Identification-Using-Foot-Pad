/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/ubik_test.h>

#if defined(DOXYGEN_BUILD)

#if (INCLUDE__UBINOS__UBIK_TEST == 1)

#include <stdio.h>
#include <stdlib.h>

#if   (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM7X256)
	#include "../../bsp_sam7x256ek/hdr/board.h"
#elif (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM7X512)
	#include "../../bsp_sam7x256ek/hdr/board__512.h"
#else
	#error "Unsupported UBINOS__BSP__CPU_MODEL"
#endif

#define LOOPCOUNT		10

static sem_pt _g_sem = NULL;

static void htimerisr(void) {
	unsigned int reg;

	sem_give(_g_sem);

	reg = AT91C_BASE_TC0->TC_SR;
}

static void intrtest00_taskfunc(void * arg) {
	unsigned int i;

	for (i=0; i<LOOPCOUNT; i++) {
		sem_take(_g_sem);

		printf("hello world!\n");
	}
}

int ubik_test_intrtest00(void) {
	int r;
	int r2;
	unsigned int reg;
	task_pt task;

	r = sem_create(&_g_sem);
	if (0 != r) {
		logme("fail at sem_create");
		r = -1;
		goto end0;
	}

	r = task_create_noautodel(&task, intrtest00_taskfunc, NULL, task_getpriority(NULL)-1, 0, "intrtest00 task");
	if (0 != r) {
		printf("fail at task_create_noautodel(), err=%d\n", r);
		r = -1;
		goto end1;
	}

	/* Disable hardware timer */
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;
	AT91C_BASE_TC0->TC_IDR = 0xFFFFFFFF;
	reg = AT91C_BASE_TC0->TC_SR;

	/* Connect hardware timer interrupt service routine */
	intr_connectisr(AT91C_ID_TC0, htimerisr, intr_getlowestpriority(), INTR_OPT__LEVEL | INTR_OPT__HIGH);

	/* Enable hardware timer interrupt */
	intr_enable(AT91C_ID_TC0);

	/* Set hardware timer */
	AT91C_BASE_PMC->PMC_PCER	= 1 << AT91C_ID_TC0;				/* Enable timer clock */
	AT91C_BASE_TC0->TC_CMR		= AT91C_TC_CPCTRG |					/* Enable RC compare trigger */
								  AT91C_TC_CLKS_TIMER_DIV5_CLOCK;	/* MCK(48054857) / 1024 */
	AT91C_BASE_TC0->TC_RC		= 46929; 							/* 1 second */
	AT91C_BASE_TC0->TC_CCR		= AT91C_TC_CLKEN; 					/* Enable clock counter */
	AT91C_BASE_TC0->TC_IER		= AT91C_TC_CPCS;					/* Interrupt occurs when RC value matches */

	/* Start hardware timer */
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_SWTRG;

	r = task_join_and_delete(&task, NULL, 1);
	if (0 != r) {
		printf("fail at task_join_and_delete(), err=%d\n", r);
		r = -1;
	}

	/* Disable hardware timer interrupt */
	intr_disable(AT91C_ID_TC0);

	/* Disable hardware timer */
	AT91C_BASE_TC0->TC_CCR = AT91C_TC_CLKDIS;
	AT91C_BASE_TC0->TC_IDR = 0xFFFFFFFF;
	reg = AT91C_BASE_TC0->TC_SR;

	r = 0;

end1:
	r2 = sem_delete(&_g_sem);
	if (0 != r2) {
		printf("fail at sem_delete(), err=%d\n", r2);
		r = -1;
	}

end0:
	return r;
}

#endif /* defined(DOXYGEN_BUILD) */
#endif /* (INCLUDE__UBINOS__UBIK_TEST == 1) */
