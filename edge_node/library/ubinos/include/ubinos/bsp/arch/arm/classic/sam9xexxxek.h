/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UBINOS_BSP_ARM_CLASSIC_SAM9XEXXXEK_H_
#define UBINOS_BSP_ARM_CLASSIC_SAM9XEXXXEK_H_

/**
 * @file board.h
 *
 * @brief BOARD definition
 *
 * Board 관련 정의
 */

#include <ubinos_config.h>

#if   (UBINOS__BSP__CPU_MODEL == UBINOS__BSP__CPU_MODEL__SAM9XE512)
#include "sam/AT91SAM9XE512.h"
#else
#error "Unsupported UBINOS__BSP__CPU_MODEL"
#endif

/* Common definition for ARM */

#define ARM_MODE_USR				0x10
#define ARM_MODE_FIQ				0x11
#define ARM_MODE_IRQ				0x12
#define ARM_MODE_SVC				0x13
#define ARM_MODE_ABT				0x17
#define ARM_MODE_UND				0x1B
#define ARM_MODE_SYS				0x1F

#define I_BIT						0x80
#define F_BIT						0x40
#define T_BIT						0x20

#define EXCEPTION_TYPE_UND			0x01
#define EXCEPTION_TYPE_SWI			0x02
#define EXCEPTION_TYPE_PABT			0x03
#define EXCEPTION_TYPE_DABT			0x04
#define EXCEPTION_TYPE_IRQ			0x05
#define EXCEPTION_TYPE_FIQ			0x06

#define SWINO__TASK_YIELD			0x01

#if (UBINOS__BSP__INCLUDE_INTERRUPT_DISABLE_ENABLE_RETRY == 1)
#define ARM_INTERRUPT_ENABLE() {												\
		__asm__ __volatile__ (														\
			"stmfd	sp!, {r0}												\n\t"	\
			"1:																\n\t"	\
			"mrs	r0, cpsr												\n\t"	\
			"bic	r0, r0, #0x80											\n\t"	\
			"msr	cpsr, r0												\n\t"	\
			"nop															\n\t"	\
			"mrs	r0, cpsr												\n\t"	\
			"ands	r0, r0, #0x80											\n\t"	\
			"bne	1b														\n\t"	\
			"ldmfd	sp!, {r0}												\n\t"	\
		);																			\
	}
#else
#define ARM_INTERRUPT_ENABLE() {												\
		__asm__ __volatile__ (														\
			"stmfd	sp!, {r0}												\n\t"	\
			"mrs	r0, cpsr												\n\t"	\
			"bic	r0, r0, #0x80											\n\t"	\
			"msr	cpsr, r0												\n\t"	\
			"ldmfd	sp!, {r0}												\n\t"	\
		);																			\
	}
#endif

#if (UBINOS__BSP__INCLUDE_INTERRUPT_DISABLE_ENABLE_RETRY == 1)
#define ARM_INTERRUPT_DISABLE() {												\
		__asm__ __volatile__ (														\
			"stmfd	sp!, {r0}												\n\t"	\
			"1:																\n\t"	\
			"mrs	r0, cpsr												\n\t"	\
			"orr	r0, r0, #0x80											\n\t"	\
			"msr	cpsr, r0												\n\t"	\
			"nop															\n\t"	\
			"mrs	r0, cpsr												\n\t"	\
			"ands	r0, r0, #0x80											\n\t"	\
			"beq	1b														\n\t"	\
			"ldmfd	sp!, {r0}												\n\t"	\
		);																			\
	}
#else
#define ARM_INTERRUPT_DISABLE() {												\
		__asm__ __volatile__ (														\
			"stmfd	sp!, {r0}												\n\t"	\
			"mrs	r0, cpsr												\n\t"	\
			"orr	r0, r0, #0x80											\n\t"	\
			"msr	cpsr, r0												\n\t"	\
			"ldmfd	sp!, {r0}												\n\t"	\
		);																			\
	}
#endif

#define ARM_CONTEXT_SIZE				(18 * 4)

#ifndef __ASSEMBLY__

unsigned int arm_get_cp15(void);
void arm_set_cp15(unsigned int value);
unsigned int arm_get_cpsr(void);

#endif /* __ASSEMBLY__ */

/* Platform specific definition */

#define AT91_CHIP_ID__SAM7X256				0x275B0940
#define AT91_CHIP_ID__SAM7X512				0x275C0A40
#define AT91_CHIP_ID__SAM9XE512				0x329AA3A0
#define AT91_CHIP_ID__SAM9XE512_SAMPLE		0x329A73A0

#define AT91_NVM_CMD__SECURITY							0
#define AT91_NVM_CMD__BROWNOUT_DETECTOR_ENABLE			1
#define AT91_NVM_CMD__BROWNOUT_DETECTOR_RESET_ENABLE	2
#define AT91_NVM_CMD__BOOT_MODE_SELECT					3

#ifndef __ASSEMBLY__

int at91_getnvm(int cmd);
int at91_setnvm(int cmd, int value);

#endif

#endif /* UBINOS_BSP_ARM_CLASSIC_SAM9XEXXXEK_H_ */
