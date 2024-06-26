/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos/bsp.h>
#include <ubinos/bsp_ubik.h>

#if (INCLUDE__UBINOS__BSP == 1)

#if (UBINOS__BSP__USE_DTTY == 1)

int _g_bsp_dtty_init = 0;
int _g_bsp_dtty_in_init = 0;
int _g_bsp_dtty_echo = 0;
int _g_bsp_dtty_autocr = 0; // auto carriage return

#if (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__ARM926EJ_S)
#pragma GCC push_options
#pragma GCC target ("arm")
#endif
int dtty_puts(const char *str, int max)
{
    int i = 0;

    if (NULL == str)
    {
        return -2;
    }

    if (0 > max)
    {
        return -3;
    }

    for (i = 0; i < max; i++)
    {
        if ('\0' == *str)
        {
            break;
        }
        dtty_putc(*str);
        str++;
    }

    return i;
}
#if (UBINOS__BSP__CPU_TYPE == UBINOS__BSP__CPU_TYPE__ARM926EJ_S)
#pragma GCC pop_options
#endif

int dtty_gets(char *str, int max)
{
    int i;
    int r;

    if (NULL == str)
    {
        return -2;
    }

    if (0 > max)
    {
        return -3;
    }

    for (i = 0; i < max; i++)
    {
        r = dtty_getc(&str[i]);
        if (0 != r || '\0' == str[i] || '\n' == str[i] || '\r' == str[i])
        {
            break;
        }
    }
    if (0 != i && max == i)
    {
        i--;
    }
    str[i] = '\0';

    return i;
}

int dtty_setecho(int echo)
{
    _g_bsp_dtty_echo = echo;

    return 0;
}

int dtty_getecho()
{
    return _g_bsp_dtty_echo;
}

int dtty_setautocr(int autocr)
{
    _g_bsp_dtty_autocr = autocr;

    return 0;
}

int dtty_getautocr()
{
    return _g_bsp_dtty_autocr;
}

#else /* (UBINOS__BSP__USE_DTTY == ...) */

int dtty_puts(const char *str, int max)
{
    return 0;
}

int dtty_putn(const char *str, int len)
{
    return 0;
}

int dtty_gets(char *str, int max)
{
    return 0;
}

int dtty_setecho(int echo)
{
    return 0;
}

int dtty_setautocr(int autocr)
{
    return 0;
}

int dtty_init(void)
{
    return 0;
}

int dtty_enable(void)
{
    return 0;
}

int dtty_disable(void)
{
    return 0;
}

int dtty_geterror(void)
{
    return 0;
}

int dtty_getc(char *ch_p)
{
    return 0;
}

int dtty_putc(int ch)
{
    return 0;
}

int dtty_kbhit(void)
{
    return 0;
}

int dtty_flush(void)
{
    return 0;
}

#endif /* (UBINOS__BSP__USE_DTTY == ...) */

#endif /* (INCLUDE__UBINOS__BSP == 1) */

