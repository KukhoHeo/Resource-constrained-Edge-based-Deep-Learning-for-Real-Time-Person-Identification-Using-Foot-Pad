/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../_bsp.h"

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)

int bsp_getcpuclockfreq(unsigned int * freq_p) {
	if (NULL == freq_p) {
		return -2;
	}

	return -1;
}

int bsp_getcpuclockfreqk(unsigned int * freqk_p) {
    unsigned int mclock_sel = 0;
    unsigned int mclock_div = 0;
    unsigned int pll_mult = 0;
    unsigned int pll_div = 0;

    if (NULL == freqk_p) {
		return -2;
    }

    mclock_sel = (AT91C_BASE_PMC->PMC_MCKR) & 0x03;
    switch (mclock_sel) {
    case 0:	// slow clock
    	*freqk_p = UBINOS__BSP__CLOCK_FREQ_SLOW;
    	return 0;
    case 1:	// main clock
    	*freqk_p =  UBINOS__BSP__CLOCK_FREQ_MAIN;
    	return 0;
    case 2: // pll a clock
        pll_mult   = (AT91C_BASE_CKGR->CKGR_PLLAR & 0x07FF0000) >> 16;
        pll_div    = (AT91C_BASE_CKGR->CKGR_PLLAR & 0x000000FF) >>  0;
        if (pll_div == 0) {
            return -1; // divider output is 0
        }
    	break;
    case 3: // pll b clock
        pll_mult   = (AT91C_BASE_CKGR->CKGR_PLLBR & 0x07FF0000) >> 16;
        pll_div    = (AT91C_BASE_CKGR->CKGR_PLLBR & 0x000000FF) >>  0;
        if (pll_div == 0) {
            return -1; // divider output is 0
        }
    	break;
    default:
    	return -1;
    }

    mclock_div = (AT91C_BASE_PMC->PMC_MCKR >> 2) & 0x07;
    mclock_div = 1 << mclock_div;
    if (128 <= mclock_div) {
        return -1; // reserved
    }

    *freqk_p =  ((UBINOS__BSP__CLOCK_FREQ_MAIN * (pll_mult + 1)) / pll_div) / mclock_div;
    return 0;
}

int bsp_getmckfreq(unsigned int * freq_p) {
	return bsp_getcpuclockfreq(freq_p);
}

int bsp_getmckfreqk(unsigned int * freqk_p) {
	int r;
	unsigned int mdiv;

	r = bsp_getcpuclockfreqk(freqk_p);
	if (0 == r) {
		mdiv = (AT91C_BASE_PMC->PMC_MCKR & AT91C_PMC_MDIV) >> 8;
		switch (mdiv) {
		case 0:
			break;
		case 1:
			(*freqk_p) /= 2;
			break;
		case 2:
			(*freqk_p) /= 4;
			break;
		case 3:
		default:
			r = -1;
			break;
		}
	}

	return r;
}

int bsp_getcpuidsize(void) {
    return 4;
}

int bsp_getcpuid(unsigned char * buf, int max) {
    unsigned char tmp_a[4];

    if (NULL == buf) {
        return -2;
    }
    if (4 > max) {
        return -3;
    }

    *((unsigned int *) tmp_a) = AT91C_BASE_DBGU->DBGU_CIDR;

#if (UBINOS__BSP__CPU_ENDIAN__BIG == UBINOS__BSP__CPU_ENDIAN)
    buf[0] = tmp_a[0];
    buf[1] = tmp_a[1];
    buf[2] = tmp_a[2];
    buf[3] = tmp_a[3];
#else
    buf[0] = tmp_a[3];
    buf[1] = tmp_a[2];
    buf[2] = tmp_a[1];
    buf[3] = tmp_a[0];
#endif

    return 4;
}

#if (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)
	#define MC0_FMR_ADDR				&(AT91C_BASE_EFC->EFC_FMR)
	#define MC0_FCR_ADDR				&(AT91C_BASE_EFC->EFC_FCR)
	#define MC0_FSR_ADDR				&(AT91C_BASE_EFC->EFC_FSR)
	#define MC0_FRR_ADDR				&(AT91C_BASE_EFC->EFC_FRR)

	#define AT91C_MC_FRDY				AT91C_EFC_FRDY_S
	#define AT91C_MC_KEY				AT91C_EFC_FKEY

	#define AT91C_MC_LOCKE				AT91C_EFC_LOCKE
	#define AT91C_MC_PROGE				AT91C_EFC_FCMDE

	#define AT91C_MC_FCMD_START_PROG	AT91C_EFC_FCMD_EWP
	#define AT91C_MC_FCMD_ERASE_ALL		AT91C_EFC_FCMD_EA
	#define AT91C_MC_FCMD_SET_GP_NVM	AT91C_EFC_FCMD_SFB
	#define AT91C_MC_FCMD_CLR_GP_NVM	AT91C_EFC_FCMD_CFB
	#define AT91C_MC_FCMD_GET_GP_NVM	AT91C_EFC_FCMD_GFB
#else
	#error "Unsupported UBINOS__BSP__CPU_MODEL"
#endif

int at91_getnvm(int cmd) {
	int r;
	int bitno;
	unsigned int reg;
	volatile unsigned int * fcr;
	volatile unsigned int * fsr;
	volatile unsigned int * frr;
	unsigned int i;

	if (0 > cmd || cmd > 3) {
		return -2;
	}

	fcr = MC0_FCR_ADDR;
	fsr = MC0_FSR_ADDR;
	frr = MC0_FRR_ADDR;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(18 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	bitno = cmd;

	reg = 0;
	reg |= ((0x0000005A << 24) & AT91C_MC_KEY);
	reg |= (bitno << 8);
	reg |= (AT91C_MC_FCMD_GET_GP_NVM);
	*fcr = reg;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(7 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	if (0 != (reg & AT91C_MC_LOCKE)) {
		return -1;
	}
	if (0 != (reg & AT91C_MC_PROGE)) {
		return -1;
	}

	reg = *frr;
	reg = (reg >> (bitno)) & 0x1;
	r = (int) reg;

	return r;
}

int at91_setnvm(int cmd, int value) {
	int r;
	int bitno;
	unsigned int reg;
	//volatile unsigned int * fmr;
	volatile unsigned int * fcr;
	volatile unsigned int * fsr;
	unsigned int i;

	if (0 > cmd || cmd > 3) {
		return -2;
	}

	if (0 != value && 1 != value) {
		return -3;
	}

	if (AT91_NVM_CMD__SECURITY == cmd && 0 == value) {
		return -3;
	}

	//fmr = MC0_FMR_ADDR;
	fcr = MC0_FCR_ADDR;
	fsr = MC0_FSR_ADDR;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(18 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	bitno = cmd;

	reg = 0;
	reg |= ((0x0000005A << 24) & AT91C_MC_KEY);
	reg |= (bitno << 8);
	if (value == 1) {
		reg |= (AT91C_MC_FCMD_SET_GP_NVM);
	}
	else {
		reg |= (AT91C_MC_FCMD_CLR_GP_NVM);
	}
	*fcr = reg;

	r = -1;
	for (i=0; i<4; i++) {
		reg = *fsr;
		if (1 == (reg & AT91C_MC_FRDY)) {
			r = 0;
			break;
		}
		bsp_busywait(7 * bsp_getbusywaitcountperms());
	}
	if (0 != r) {
		return -1;
	}

	if (0 != (reg & AT91C_MC_LOCKE)) {
		return -1;
	}
	if (0 != (reg & AT91C_MC_PROGE)) {
		return -1;
	}

	return r;
}

#endif /* (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

