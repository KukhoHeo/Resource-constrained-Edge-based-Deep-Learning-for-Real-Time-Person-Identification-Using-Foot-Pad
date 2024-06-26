/*
 * Copyright (c) 2022 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos_config.h>

    .syntax unified
    .arch   armv7e-m


    .text
    .thumb

    .thumb_func
    .align  1
    .globl  appmain
    .type   appmain, %function
appmain:
    @@@@
    mov     r0, #0
    vmov.f32 s0, #2.25000000

    @ Copy value from s0 to r0
    vmov    r0, s0

    @ Move single-precision value into floating point register S0
    vmov.f32 s0, #1.0

    @ Copy value from r0 to s0
    vmov    s0, r0

    @@@@
    bx      lr

