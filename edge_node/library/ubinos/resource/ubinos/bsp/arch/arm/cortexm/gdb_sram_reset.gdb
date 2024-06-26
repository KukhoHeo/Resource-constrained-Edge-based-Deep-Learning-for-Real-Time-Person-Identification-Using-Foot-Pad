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
monitor speed auto
monitor sleep 500
monitor endian little

delete
symbol-file

load app.elf
symbol-file app.elf

monitor reg pc = Reset_Handler

break main

quit


