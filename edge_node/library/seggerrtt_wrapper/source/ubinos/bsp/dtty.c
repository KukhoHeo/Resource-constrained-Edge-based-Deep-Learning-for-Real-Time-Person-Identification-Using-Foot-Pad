/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SEGGER_RTT.h"

#if (INCLUDE__UBINOS__BSP == 1)

#if (UBINOS__BSP__USE_DTTY == 1)

#if (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__EXTERNAL)

#if (SEGGERRTT__DTTY_ENABLE == 1)

#include <ubinos/bsp/arch.h>
#include <ubinos/bsp_ubik.h>

#define SLEEP_TIMEMS	1

extern int _g_bsp_dtty_init;
extern int _g_bsp_dtty_echo;
extern int _g_bsp_dtty_autocr;

static int _dtty_getc_advan(char *ch_p, int blocked);

int dtty_init(void) {
    _g_bsp_dtty_echo = 0;
	_g_bsp_dtty_autocr = 0;

    _g_bsp_dtty_init = 1;

    return 0;
}

int dtty_enable(void) {
    return 0;
}

int dtty_disable(void) {
    return 0;
}

int dtty_geterror(void) {
	return 0;
}

static int _dtty_getc_advan(char *ch_p, int blocked)
{
    unsigned int i;
    int r = 0;

   if (NULL == ch_p)
   {
       return -2;
   }

   if (!_g_bsp_dtty_init)
   {
       dtty_init();
   }

#if (INCLUDE__UBINOS__UBIK == 1)
   if (_bsp_kernel_active)
   {
       for (i = 0;; i++)
       {
           if (SEGGER_RTT_HasKey())
           {
               break;
           }
           if (blocked != 1)
           {
               r = -1;
               break;
           }
           if (255 <= i)
           {
               bsp_task_sleepms(SLEEP_TIMEMS);
               i = 0;
           }
       }
   }
   else {
       for (i=0;; i++)
       {
           if(SEGGER_RTT_HasKey())
           {
               break;
           }
           if (blocked != 1)
           {
               r = -1;
               break;
           }
       }
   }
#else
   for (i=0;; i++)
   {
       if(SEGGER_RTT_HasKey())
       {
           break;
       }
       if (blocked != 1)
       {
           r = -1;
           break;
       }
   }
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

   if (r == 0)
   {
       *ch_p = (char) SEGGER_RTT_GetKey();

       if (0 != _g_bsp_dtty_echo)
       {
           dtty_putc(*ch_p);
       }
   }

   return r;
}

int dtty_getc(char *ch_p)
{
    return _dtty_getc_advan(ch_p, 1);
}

int dtty_getc_unblocked(char *ch_p)
{
    return _dtty_getc_advan(ch_p, 0);
}

int dtty_putc(int ch) {
	unsigned status;

	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

	status = SEGGER_RTT_Write(0, &ch, 1);

	if (status == 1) {
		return 0;
	}
	else {
		return -1;
	}
}

int dtty_putn(const char * str, int len) {
    int i = 0;

    if (NULL == str) {
        return -2;
    }

    if (0 > len) {
        return -3;
    }

    i = (int) SEGGER_RTT_Write(0, str, len);

    return i;
}

int dtty_kbhit(void) {
	if (!_g_bsp_dtty_init) {
		dtty_init();
	}

    if (SEGGER_RTT_HasKey()) {
        return 1;
    } else {
        return 0;
    }
}

int dtty_flush(void)
{
    return 0;
}

#endif /* (SEGGERRTT__DTTY_ENABLE == 1) */

#endif /* (UBINOS__BSP__DTTY_TYPE == UBINOS__BSP__DTTY_TYPE__EXTERNAL) */

#endif /* (UBINOS__BSP__USE_DTTY == 1) */

#endif /* (INCLUDE__UBINOS__BSP == 1) */

