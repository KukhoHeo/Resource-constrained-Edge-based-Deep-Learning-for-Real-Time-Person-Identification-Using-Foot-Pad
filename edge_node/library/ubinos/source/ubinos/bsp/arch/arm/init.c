/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/bsp.h>

#if (INCLUDE__UBINOS__BSP == 1)
#if (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__ARM)
#if !(UBINOS__BSP__EXCLUDE_ARCH_INIT == 1)

#if (INCLUDE__UBINOS__UBICLIB == 1)
#include <ubinos/ubiclib.h>
#endif
#if (INCLUDE__UBINOS__UBIK == 1)
#include <ubinos/ubik.h>
#endif

extern void SystemInit2(void);

int main(void) {
    int r = 0;

    int argc = 1;
    char * argv[] = {"app"};

    SystemInit2();

#if (INCLUDE__UBINOS__UBICLIB == 1)
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
    r = ubiclib_heap_comp_init();
    if (HEAP_ERR__UNSUPPORTED == r) {
        dtty_puts("heap_comp_init : unsupported\n", 80);
    }
    else if (0 != r) {
        dtty_puts("heap_comp_init : fail\n", 80);
        bsp_abortsystem();
    }
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */
#endif /* (INCLUDE__UBINOS__UBICLIB == 1) */

#if !(UBINOS__UBICLIB__NOSTDLIB == 1)
    r = stdlib_port_comp_init();
    if (0 != r) {
        dtty_puts("stdlib_port_comp_init : fail\n", 80);
        bsp_abortsystem();
    }
#endif

#if (INCLUDE__UBINOS__UBIK == 1)
    r = ubik_comp_init(UBINOS__UBIK__IDLETASK_STACK_DEPTH);
    if (0 != r) {
        dtty_puts("ubik_comp_init : fail\n", 80);
        bsp_abortsystem();
    }
#endif

#if (INCLUDE__UBINOS__UBICLIB == 1)
#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)
    r = ubiclib_heap_comp_init_reent();
    if (HEAP_ERR__UNSUPPORTED == r) {
        dtty_puts("heap_comp_init_reent : unsupported\n", 80);
    }
    else if (0 != r) {
        dtty_puts("heap_comp_init_reent : fail\n", 80);
        bsp_abortsystem();
    }
#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */
#endif /* (INCLUDE__UBINOS__UBICLIB == 1) */

#if !(UBINOS__UBICLIB__NOSTDLIB == 1)
    r = stdlib_port_comp_init_reent();
    if (0 != r) {
        dtty_puts("stdlib_port_init_reent : fail\n", 80);
        bsp_abortsystem();
    }
#endif

    appmain(argc, argv);

    for (;;) {
    }

    return 0;
}

#endif /* !(UBINOS__BSP__EXCLUDE_ARCH_INIT == 1) */
#endif /* (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__ARM) */
#endif /* (INCLUDE__UBINOS__BSP == 1) */

