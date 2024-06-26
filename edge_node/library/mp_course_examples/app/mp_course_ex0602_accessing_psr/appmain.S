/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos_config.h>

    .syntax unified
    .arch   armv7-m


    .text
    .thumb

    .thumb_func
    .align  1
    .globl  appmain
    .type   appmain, %function
appmain:
    @@@@
    mov     r1, #1
    mov     r2, #1
    subs    r0, r1, r2  @ 1 - 1 = 0 (N=0, Z=1, C=1, V=0)
    mov     r0, #0
    mrs     r0, psr
    mov     r0, #0
    mrs     r0, apsr
    mov     r0, #0
    mrs     r0, ipsr

    bx      lr

