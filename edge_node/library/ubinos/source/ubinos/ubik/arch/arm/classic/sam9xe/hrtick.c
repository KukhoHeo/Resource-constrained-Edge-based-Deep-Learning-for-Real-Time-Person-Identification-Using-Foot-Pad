/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__SAM9XE512EK)


#if !(UBINOS__UBIK__EXCLUDE_HRTICK == 1)


#if   (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM7X256) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM7X512) || (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM7L128)
	// MCK = 48000000, TIMER_CLOCK3: Freq = MCK/32  = 1500000, Interval = 1/Freq =  666.666667 ns, 16 bit (65536): Max = Interval * 65536 = 43,690,666 ns
	#define HRTICK_DIV_TYPE		AT91C_TC_CLKS_TIMER_DIV3_CLOCK
	#define HRTICK_DIV			32
#elif (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)
	// MCK = 99328000, TIMER_CLOCK4: Freq = MCK/128 =  776000, Interval = 1/Freq = 1288.659794 ns, 16 bit (65536): Max = Interval * 65536 = 84,453,608 ns
	#define HRTICK_DIV_TYPE		AT91C_TC_CLKS_TIMER_DIV4_CLOCK
	#define HRTICK_DIV			128
#else
	#error "Unsupported UBINOS__BSP__CPU_MODEL"
#endif

AT91S_TC * 	_ubik_hrtick_htimer_p 				= NULL;
int 			_ubik_hrtick_htimer_id 				= -1;

int ubik_hrtick_enable(int htimerno) {
	unsigned int mckfreqk;

	if (0 != _ubik_hrtick_active) {
		return -1;
	}

	switch (htimerno) {
	case 0:
		_ubik_hrtick_htimer_p 	= AT91C_BASE_TC0;
		_ubik_hrtick_htimer_id 	= AT91C_ID_TC0;
		break;
	case 1:
		_ubik_hrtick_htimer_p 	= AT91C_BASE_TC1;
		_ubik_hrtick_htimer_id 	= AT91C_ID_TC1;
		break;
	case 2:
		_ubik_hrtick_htimer_p 	= AT91C_BASE_TC2;
		_ubik_hrtick_htimer_id 	= AT91C_ID_TC2;
		break;
	default:
		return -1;
	}

	bsp_getmckfreqk(&mckfreqk);
#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *    1 / (UBINOS__UBIK__TICK_PER_SEC / 1000);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *   10 / (UBINOS__UBIK__TICK_PER_SEC /  100);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV *  100 / (UBINOS__UBIK__TICK_PER_SEC /   10);
#else
	_ubik_hrtick_hrtickpertick = mckfreqk / HRTICK_DIV * 1000 / (UBINOS__UBIK__TICK_PER_SEC /    1);
#endif

	// Enable peripheral clock
	AT91C_BASE_PMC->PMC_PCER = 1 << _ubik_hrtick_htimer_id;
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;
	// Disable interrupts
	_ubik_hrtick_htimer_p->TC_IDR = 0xFFFFFFFF;
	// Clear status register
	_ubik_hrtick_htimer_p->TC_SR;
	// Set mode
	_ubik_hrtick_htimer_p->TC_CMR = (HRTICK_DIV_TYPE | AT91C_TC_WAVESEL_UP | AT91C_TC_WAVE);
	// Reset and enable TC
	_ubik_hrtick_htimer_p->TC_CCR = (AT91C_TC_CLKEN | AT91C_TC_SWTRG);
	// Disable TC
	//_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;

	_ubik_hrtick_active 		= 1;

	return 0;
}

int ubik_hrtick_disable(void) {
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	// Disable TC
	_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;

	_ubik_hrtick_active = 0;

	return 0;
}

int ubik_hrtick_reset(void) {
	// Disable TC
	_ubik_hrtick_htimer_p->TC_CCR = AT91C_TC_CLKDIS;
	// Reset and enable TC
	_ubik_hrtick_htimer_p->TC_CCR = (AT91C_TC_CLKEN | AT91C_TC_SWTRG);

	return 0;
}

unsigned int ubik_hrtick_gettickpersec(void) {
	return UBINOS__UBIK__TICK_PER_SEC;
}

unsigned int ubik_hrtick_gethrtickpermsec(void) {
	unsigned int value;
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	value = _ubik_hrtick_hrtickpertick * (UBINOS__UBIK__TICK_PER_SEC / 1000);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	value = _ubik_hrtick_hrtickpertick * (UBINOS__UBIK__TICK_PER_SEC  / 100) /   10;
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	value = _ubik_hrtick_hrtickpertick * (UBINOS__UBIK__TICK_PER_SEC   / 10) /  100;
#else
	value = _ubik_hrtick_hrtickpertick * (UBINOS__UBIK__TICK_PER_SEC       ) / 1000;
#endif

	return value;
}

int ubik_hrtick_gettick(hrtick_t * tick_p) {
	if (NULL == tick_p) {
		return -2;
	}
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	ubik_entercrit();

	tick_p->high	= _ubik_tickcounth;
	tick_p->low		= _ubik_tickcount;
	tick_p->hrtick	= _ubik_hrtick_htimer_p->TC_CV;

	ubik_exitcrit();

	return 0;
}

int ubik_hrtick_gettickdiff(hrtick_t * tick1_p, hrtick_t * tick2_p, hrtick_t * tickdiff_p) {
	if (NULL == tick1_p) {
		return -2;
	}
	if (NULL == tick2_p) {
		return -3;
	}
	if (NULL == tickdiff_p) {
		return -4;
	}
	if (1 != _ubik_hrtick_active) {
		return -1;
	}

	if (tick1_p->high > tick2_p->high) {
		tickdiff_p->high = UBINOS__UBIK__TICK_COUNT_MAX - tick1_p->high + 1 + tick2_p->high;
	}
	else {
		tickdiff_p->high = tick2_p->high - tick1_p->high;
	}

	if (tick1_p->low > tick2_p->low) {
		tickdiff_p->high--;
		tickdiff_p->low = UBINOS__UBIK__TICK_COUNT_MAX - tick1_p->low + 1 + tick2_p->low;
	}
	else {
		tickdiff_p->low = tick2_p->low - tick1_p->low;
	}

	if (tick1_p->hrtick > tick2_p->hrtick) {
		if (0 >= tickdiff_p->low) {
			tickdiff_p->high--;
			tickdiff_p->low = UBINOS__UBIK__TICK_COUNT_MAX;
		}
		else {
			tickdiff_p->low--;
		}
		tickdiff_p->hrtick = _ubik_hrtick_hrtickpertick - tick1_p->hrtick + 1 + tick2_p->hrtick;
	}
	else {
		tickdiff_p->hrtick = tick2_p->hrtick - tick1_p->hrtick;
	}

	return 0;
}

int ubik_hrtick_hrticktotime(hrtick_t * tick_p, unsigned int * nsec_p, unsigned int * usec_p, unsigned int * msec_p) {
	unsigned int timems;
	unsigned int timeus;
	unsigned int timens;
	unsigned int temp;
	unsigned int tickperxx;

	if (NULL == tick_p) {
		return -2;
	}

#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /   1000);
	temp		 = (tick_p->low *    1);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /    100);
	temp		 = (tick_p->low *   10);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /     10);
	temp		 = (tick_p->low *  100);
#else
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /      1);
	temp		 = (tick_p->low * 1000);
#endif
	timems		 = (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timeus		 = (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timens		 = (temp / tickperxx);

	tickperxx	 = ubik_hrtick_gethrtickpermsec();
	temp		 = (tick_p->hrtick);
	timems		+= (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timeus		+= (temp / tickperxx);
	temp		 = (temp % tickperxx) * 1000;
	timens		+= (temp / tickperxx);

	if (NULL != nsec_p) {
		*nsec_p = timens;
	}
	if (NULL != usec_p) {
		*usec_p = timeus;
	}
	if (NULL != msec_p) {
		*msec_p = timems;
	}

	return 0;
}

int ubik_hrtick_hrticktotimens(hrtick_t * tick_p, unsigned int * nsec_p) {
	unsigned int time;
	unsigned int temp;
	unsigned int tickperxx;

	if (NULL == tick_p) {
		return -2;
	}

#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /   1000);
	temp		 = (tick_p->low *    1);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /    100);
	temp		 = (tick_p->low *   10);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /     10);
	temp		 = (tick_p->low *  100);
#else
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /      1);
	temp		 = (tick_p->low * 1000);
#endif
	time		 = (temp * 1000 * 10 / tickperxx * 100);

	tickperxx	 = ubik_hrtick_gethrtickpermsec();
	temp		 = (tick_p->hrtick);

	time		+= (temp * 1000 * 10 / tickperxx * 100);

	if (NULL != nsec_p) {
		*nsec_p = time;
	}

	return 0;
}

int ubik_hrtick_hrticktotimeus(hrtick_t * tick_p, unsigned int * usec_p) {
	unsigned int time;
	unsigned int temp;
	unsigned int tickperxx;

	if (NULL == tick_p) {
		return -2;
	}

#if		(0 == UBINOS__UBIK__TICK_PER_SEC%1000)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /   1000);
	temp		 = (tick_p->low *    1);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%100)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /    100);
	temp		 = (tick_p->low *   10);
#elif	(0 == UBINOS__UBIK__TICK_PER_SEC%10)
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /     10);
	temp		 = (tick_p->low *  100);
#else
	tickperxx	 = (UBINOS__UBIK__TICK_PER_SEC /      1);
	temp		 = (tick_p->low * 1000);
#endif
	time		 = (temp * 1000 / tickperxx);

	tickperxx	 = ubik_hrtick_gethrtickpermsec();
	temp		 = (tick_p->hrtick);

	time		+= (temp * 1000 / tickperxx);

	if (NULL != usec_p) {
		*usec_p = time;
	}

	return 0;
}

#endif /* !(UBINOS__UBIK__EXCLUDE_HRTICK == 1) */


#endif /* (UBINOS__BSP__BOARD_MODEL == ...) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


