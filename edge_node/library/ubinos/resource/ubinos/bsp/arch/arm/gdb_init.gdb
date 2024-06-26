#
# Copyright (c) 2022 Sung Ho Park
# 
# SPDX-License-Identifier: Apache-2.0
#

set pagination off

set remote memory-write-packet-size 1024
set remote memory-read-packet-size 1024

define ushowmemx
    set $addr = $arg0
    set $len = $arg1
    set $cur = $addr
    set $end = $addr + $len
    while ($cur < $end)
        x/1wx $cur
        set $cur = $cur + 4
    end
end

define ushowmems
    set $addr = $arg0
    x/1s $addr
end

define ushowstack
    ushowmemx $sp $arg0
end

define ushowcore
    x/5i $pc - 6
    info reg
    ushowstack 64
end

define ushowcoreall
    x/5i $pc - 6
    info all-reg
    ushowstack 64
end
