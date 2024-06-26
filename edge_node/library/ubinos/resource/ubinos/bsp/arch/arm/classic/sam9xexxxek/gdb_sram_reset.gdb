#
# Copyright (c) 2009 Sung Ho Park
# 
# SPDX-License-Identifier: Apache-2.0
#

target remote localhost:2331

set pagination off

set remote memory-write-packet-size 1024
set remote memory-read-packet-size 1024

monitor reset
monitor sleep 500
monitor speed adaptive
monitor sleep 500
monitor endian little

delete
symbol-file

monitor reg cpsr 0xD3

monitor WriteU32 0xFFFFF124 0xFFFFFFFF
monitor WriteU32 0xFFFFF128 0xFFFFFFFF
monitor WriteU32 0xFFFFFC14 0xFFFFFFFF

monitor WriteU32 0xFFFFFC20 0x00004001
monitor sleep 10
monitor WriteU32 0xFFFFFC28 0x2060bf09
monitor sleep 10
monitor WriteU32 0xFFFFFC2C 0x207c7f0c
monitor sleep 10
monitor WriteU32 0xFFFFFC30 0x00000100
monitor sleep 10
monitor WriteU32 0xFFFFFC30 0x00000102
monitor sleep 10

symbol-file app.elf

monitor reg pc = 0x00300000

#break main

quit



