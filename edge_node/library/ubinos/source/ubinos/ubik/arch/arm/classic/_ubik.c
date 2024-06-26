/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "../../../_ubik.h"

#if (INCLUDE__UBINOS__UBIK == 1)
#if (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__ARM926EJ_S)

#include <assert.h>

#pragma GCC push_options
#pragma GCC target ("arm")

void ubik_entercrit(void) {
	if (0 == bsp_isintr()) {
		ARM_INTERRUPT_DISABLE();
		_bsp_critcount++;
	}
}

void ubik_exitcrit(void) {
	if (0 == bsp_isintr()) {
		if (0 == _bsp_critcount) {
			dtty_puts("\nubik_exitcrit fail (_bsp_critcount is already 0)\n", 80);
			bsp_abortsystem();
		}
		_bsp_critcount--;
		if (0 == _bsp_critcount && 0 != _bsp_kernel_active) {
			ARM_INTERRUPT_ENABLE();
		}
	}
}

#define save_context() {													\
	__asm__ __volatile__ (													\
		/* Push r0 into current stack */									\
		"stmfd		sp!, {r0}										\n\t"	\
																			\
		/* Set r0 to task stack pointer */									\
		"mov		r0, sp											\n\t"	\
		"sub		sp, sp, #4										\n\t"	\
		"stmfd		r0, {sp}^										\n\t"	\
		"nop														\n\t"	\
		"ldmfd		sp!, {r0}										\n\t"	\
																			\
		/* Save return address into task stack */							\
		"stmfd		r0!, {lr}										\n\t"	\
																			\
		/* Set lr to task stack pointer and Pop r0 from current stack */	\
		"mov		lr, r0											\n\t"	\
		"ldmfd		sp!, {r0}										\n\t"	\
																			\
		/* Save all system mode registers into task stack */				\
		"stmfd		lr, {r0-r14}^									\n\t"	\
		"nop														\n\t"	\
		"sub		lr, lr, #60										\n\t"	\
																			\
		/* Save spsr into task stack */										\
		"mrs		r0, spsr										\n\t"	\
		"stmfd		lr!, {r0}										\n\t"	\
																			\
		/* Save _bsp_critcount into task stack */							\
		"ldr		r0, =_bsp_critcount								\n\t"	\
		"ldr		r0, [r0]										\n\t"	\
		"stmfd		lr!, {r0}										\n\t"	\
																			\
		/* Set _task_cur->stacktop to new task's */							\
		"ldr		r0, =_task_cur									\n\t"	\
		"ldr		r0, [r0]										\n\t"	\
		"add		r0, r0, #4										\n\t"	\
		"str		lr, [r0]										\n\t"	\
	);																		\
}

#define restore_context() {													\
	__asm__ __volatile__ (													\
		/* Set lr to _task_cur->stacktop */									\
		"ldr		r0, =_task_cur									\n\t"	\
		"ldr		r0, [r0]										\n\t"	\
		"add		r0, r0, #4										\n\t"	\
		"ldr		lr, [r0]										\n\t"	\
																			\
		/* Restore _bsp_critcount from task stack */						\
		"ldmfd		lr!, {r1}										\n\t"	\
		"ldr		r0, =_bsp_critcount								\n\t"	\
		"str		r1, [r0]										\n\t"	\
																			\
		/* Restore spsr from task stack */									\
		"ldmfd		lr!, {r0}										\n\t"	\
		/* TODO: (Ubinos) 불필요한 코드임, 시험 후 제거할 것 */						\
		"cmp		r1, #0											\n\t"	\
		"biceq		r0, r0, #0x80									\n\t"	\
		"orrne		r0, r0, #0x80									\n\t"	\
																			\
		"msr		spsr, r0										\n\t"	\
																			\
		/* Restore all system mode registers from task stack */				\
		"ldmfd		lr, {r0-r14}^									\n\t"	\
		"nop														\n\t"	\
		"add		lr, lr, #60										\n\t"	\
																			\
		/* Return to return address. */										\
		"ldmfd		lr, {pc}^										\n\t"	\
	);																		\
}

void __attribute__((naked)) ubik_irq_handler(void);

void __attribute__((naked)) ubik_irq_handler(void) {
	__asm__ __volatile__ (													\
		"sub		lr, lr, #4										\n\t"	\
	);

	save_context();

	__asm__ __volatile__ (													\
		"mrs		r0, cpsr										\n\t"	\
		/* Set to cpsr_c to (#ARM_MODE_SVC | I_BIT) (#0x93) */				\
		"msr		cpsr_c, #0x93									\n\t"	\
		/* TODO: (Ubinos) lr은 save_context, restore_context 매크로에서 백업되고 복원 됨 , 시험 후 lr 제거할 것 */	\
		"stmfd		sp!, {r0, lr}									\n\t"	\
		\
		/* Set r0 to (#EXCEPTION_TYPE_IRQ)(#0x05) */						\
		"mov		r0, #0x05										\n\t"	\
		"bl			bsp_exception_handler							\n\t"	\
		/* TODO: (Ubinos) lr은 save_context, restore_context 매크로에서 백업되고 복원 됨 , 시험 후 lr 제거할 것 */	\
		"ldmfd		sp!, {r0, lr}									\n\t"	\
		\
		"msr		cpsr, r0										\n\t"	\
	);

	restore_context();
}

void __attribute__((naked)) ubik_swi_handler(void);

void __attribute__((naked)) ubik_swi_handler(void) {
	__asm__ __volatile__ (													\
		/* Get swino */														\
		"sub		sp, sp, #4										\n\t"	\
		"stmfd		sp!, {r0}										\n\t"	\
		"mrs		r0, spsr										\n\t"	\
		"ands		r0, r0, #0x20									\n\t"	\
		"ldrh		r0, [lr, #-2]									\n\t"	\
		"bicne		r0, r0, #0xffffff00								\n\t"	\
		"ldreq		r0, [lr, #-4]									\n\t"	\
		"biceq		r0, r0, #0xff000000								\n\t"	\
		"str		r0, [sp, #4]									\n\t"	\
		"ldmfd		sp!, {r0}										\n\t"	\
	);

	save_context();

	__asm__ __volatile__ (													\
		/* Set r1 to swino */												\
		"ldmfd		sp!, {r1}										\n\t"	\
		/* Set r0 to (#EXCEPTION_TYPE_SWI)(#0x02) */						\
		"mov		r0, #0x02										\n\t"	\
		"bl			bsp_exception_handler							\n\t"	\
	);

	restore_context();
}

void task_comp_start(void) {
	_bsp_kernel_active = 1;

	_task_cur->state = TASK_STATE__RUNNING;

	restore_context();
}

#pragma GCC pop_options


#endif /* (UBINOS__BSP__CPU_TYPE == ...) */
#endif /* (INCLUDE__UBINOS__UBIK == 1) */


