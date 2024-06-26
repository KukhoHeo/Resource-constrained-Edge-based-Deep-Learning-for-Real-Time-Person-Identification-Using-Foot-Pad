/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos_config.h>

#include <stdio.h>

static int int_data = 0;
static int int_data2 = 1;
static char string_data[] = "Hello";
static char char_array[] = {'A', 'B', 'C'};
static int int_array[] = {0, 1, 2, 3};
static float float_data = 0.5;
static double double_data = 0.25;

static char format_int[] = "%d\n";
static char format_string[] = "%s\n";
static char format_char[] = "%c\n";
static char format_float[] = "%f\n";

int appmain(int argc, char * argv[])
{
    printf(format_int, int_data);
    printf(format_int, int_data2);
    printf(format_string, string_data);
    printf(format_char, char_array[1]);
    printf(format_int, int_array[2]);
    printf(format_float, float_data);
    printf(format_float, double_data);

    return 0;
}

