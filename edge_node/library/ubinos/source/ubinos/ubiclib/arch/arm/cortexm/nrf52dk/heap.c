/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../../_heap.h"

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1)

#include <ubinos/bsp/arch.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__HEAP

typedef struct _mem_layout_region {
	unsigned int addr;
	uint8_t number;
	uint32_t control_bit;
} mem_layout_region;

#if (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE)

#define MEM_LAYOUT_REGION_COUNT 22

const mem_layout_region _mem_layout[MEM_LAYOUT_REGION_COUNT + 1] = {
	{0x20000000, 0, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, //  0
	{0x20001000, 0, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, //  1
	{0x20002000, 1, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, //  2
	{0x20003000, 1, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, //  3
	{0x20004000, 2, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, //  4
	{0x20005000, 2, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, //  5
	{0x20006000, 3, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, //  6
	{0x20007000, 3, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, //  7
	{0x20008000, 4, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, //  8
	{0x20009000, 4, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, //  9
	{0x2000A000, 5, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, // 10
	{0x2000B000, 5, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, // 11
	{0x2000C000, 6, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, // 12
	{0x2000D000, 6, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, // 13
	{0x2000E000, 7, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, // 14
	{0x2000F000, 7, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, // 15
	{0x20010000, 8, (POWER_RAM_POWER_S0POWER_On << POWER_RAM_POWER_S0POWER_Pos)}, // 16
	{0x20018000, 8, (POWER_RAM_POWER_S1POWER_On << POWER_RAM_POWER_S1POWER_Pos)}, // 17
	{0x20020000, 8, (POWER_RAM_POWER_S2POWER_On << POWER_RAM_POWER_S2POWER_Pos)}, // 18
	{0x20018000, 8, (POWER_RAM_POWER_S3POWER_On << POWER_RAM_POWER_S3POWER_Pos)}, // 19
	{0x20030000, 8, (POWER_RAM_POWER_S4POWER_On << POWER_RAM_POWER_S4POWER_Pos)}, // 20
	{0x20038000, 8, (POWER_RAM_POWER_S5POWER_On << POWER_RAM_POWER_S5POWER_Pos)}, // 21
	{0x20040000, 0, 0}, // end address
};

uint8_t _mem_layout_region_on[MEM_LAYOUT_REGION_COUNT] = {
	0, //  0
	0, //  1
	0, //  2
	0, //  3
	0, //  4
	0, //  5
	0, //  6
	0, //  7
	0, //  8
	0, //  9
	0, // 10
	0, // 11
	0, // 12
	0, // 13
	0, // 14
	0, // 15
	0, // 16
	0, // 17
	0, // 18
	0, // 19
	0, // 20
	0, // 21
};

#else

#error "Unsupported UBINOS__BSP__BOARD_MODEL"

#endif /* (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DK) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NRF52840DONGLE) || (UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__ARDUINONANO33BLE) */

int _heap_power_off_unused_area(unsigned int nomal_resign_end, unsigned int reverse_resign_addr) {
	int r = 0;
	int ni, ri;

	assert(nomal_resign_end <= reverse_resign_addr);

	logmfd("nomal_resign_end = 0x%08x,  reverse_resign_addr = 0x%08x", nomal_resign_end, reverse_resign_addr);

	for (ni = 0; ni < MEM_LAYOUT_REGION_COUNT; ni++) {
		if (nomal_resign_end <= _mem_layout[ni].addr) {
			break;
		}
		_mem_layout_region_on[ni] = 1;
	}

	for (; ni < MEM_LAYOUT_REGION_COUNT; ni++) {
		_mem_layout_region_on[ni] = 0;
	}

	for (ri = MEM_LAYOUT_REGION_COUNT - 1; ri >= ni; ri--) {
		if (_mem_layout[ri + 1].addr <= reverse_resign_addr) {
			break;
		}
		_mem_layout_region_on[ri] = 1;
	}

	for (; ri >= ni; ri--) {
		_mem_layout_region_on[ri] = 0;
	}

	for (int i = 0; i < MEM_LAYOUT_REGION_COUNT; i++) {
		logmfd("index = %d,  on = %d", i, _mem_layout_region_on[i]);

		if (_mem_layout_region_on[i]) {
			NRF_POWER->RAM[_mem_layout[i].number].POWERSET = _mem_layout[i].control_bit;
		}
		else {
			NRF_POWER->RAM[_mem_layout[i].number].POWERCLR = _mem_layout[i].control_bit;
		}
	}

	r = 0;

	return r;
}

int _heap_print_power_info(void) {
	int r = 0;

	for (int i = 0; i < MEM_LAYOUT_REGION_COUNT; i++) {
		printf(" %d", _mem_layout_region_on[i]);
	}

	return r;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1) */
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
