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

load        sys_init.elf
symbol-file sys_init.elf
break usrmain_end
continue
delete
symbol-file

load        flash_writer.elf
symbol-file flash_writer.elf

set $flashaction    = flashaction
print $flashaction
break *$flashaction
continue

set $cmd_none			= 0
set $cmd_write			= 1
set $cmd_setboot1		= 2
set $cmd_setboot0		= 3
set $cmd_getboot		= 4
set $cmd_readmem		= 5
set $cmd_readmembin		= 6
set $cmd_icache_disable	= 10
set $cmd_icache_enable	= 11
set $cmd_dcache_disable	= 12
set $cmd_dcache_enable	= 13
set $cmd_mmu_disable	= 14
set $cmd_mmu_enable		= 15

set $baddr			= 0x00200000
set $srcptr			= srcptr
set $srcstart		= srcptr
set $dstptr			= dstptr
set $size			= size
set $filestart		= 0x00000000
set $fileend		= size
# init filesize
set $filesize		= <size_of_app.bin>

set variable baddr	= $baddr

set variable cmd	= $cmd_write

set $dstptr			= 0x00200000

while $filesize > 0
	if $size > $filesize
		set $size = $filesize
	end
	set variable dstptr	= $dstptr
	set variable size	= $size

#	print $srcptr
#	print $srcstart
	print $dstptr
	print $size
	print $filestart
	print $fileend
	print $filesize

	restore app.bin binary $srcstart $filestart $fileend
	continue

	set $srcstart  	= $srcstart  - $size
	set $dstptr  	= $dstptr    + $size
	set $filestart 	= $filestart + $size
	set $fileend   	= $fileend   + $size
	set $filesize	= $filesize  - $size
end

set variable cmd	= $cmd_mmu_disable
continue

set variable cmd	= $cmd_dcache_disable
continue

set variable cmd	= $cmd_icache_disable
continue

#set variable cmd	= $cmd_getboot
#continue

set variable cmd	= $cmd_setboot1
continue

#set variable cmd	= $cmd_getboot
#continue

#set variable cmd	= $cmd_readmem
#set variable dstptr	= 0x00200000
#set variable size	= 0x00000100
#continue

delete
symbol-file

monitor reg cpsr 0xD3

monitor WriteU32 0xFFFFF124 0xFFFFFFFF
monitor WriteU32 0xFFFFF128 0xFFFFFFFF

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

monitor reg pc = 0x00200000

break main

quit


