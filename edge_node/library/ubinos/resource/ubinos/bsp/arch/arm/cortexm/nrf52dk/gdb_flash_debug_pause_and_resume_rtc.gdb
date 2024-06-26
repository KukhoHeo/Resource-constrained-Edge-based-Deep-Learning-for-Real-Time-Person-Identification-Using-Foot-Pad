#
# Copyright (c) 2009 Sung Ho Park
# 
# SPDX-License-Identifier: Apache-2.0
#

target remote localhost:2331

set pagination off

set remote memory-write-packet-size 1024
set remote memory-read-packet-size 1024

#    NRF_RTC0_BASE 0x4000B000
#    NRF_RTC1_BASE 0x40011000
#    NRF_RTC2_BASE 0x40024000
#    uint32_t  TASKS_START; /*!< (@ 0x00000000) Start RTC COUNTER
#    uint32_t  TASKS_STOP; /*!< (@ 0x00000004) Stop RTC COUNTER
#    uint32_t  COUNTER; /*!< (@ 0x00000504) Current COUNTER value

set $rtc0_tick  = 0
set $rtc1_tick  = 0
set $rtc2_tick  = 0

define pause_handler
    if *0x4000B504 > 0
        set *0x4000B004 = 0x00000001
        set $rtc0_tick  = *0x4000B504
    end
    if *0x40011504 > 0
        set *0x40011004 = 0x00000001
        set $rtc1_tick  = *0x40011504
    end
    if *0x40024504 > 0
        set *0x40024004 = 0x00000001
        set $rtc2_tick  = *0x40024504
    end

    printf "    pause\n"
    printf "        rtc0_tick = %d, %d\n", $rtc0_tick, *0x4000B504
    printf "        rtc1_tick = %d, %d\n", $rtc1_tick, *0x40011504
    printf "        rtc2_tick = %d, %d\n", $rtc2_tick, *0x40024504
end

define resume_handler
    if $rtc0_tick > 0
        set *0x4000B504 = $rtc0_tick
        set *0x4000B000 = 0x00000001
    end
    if $rtc1_tick > 0
        set *0x4000B504 = $rtc1_tick
        set *0x40011000 = 0x00000001
    end
    if $rtc2_tick > 0
        set *0x4000B504 = $rtc2_tick
        set *0x40024000 = 0x00000001
    end

    printf "    resume\n"
    printf "        rtc0_tick = %d, %d\n", $rtc0_tick, *0x4000B504
    printf "        rtc1_tick = %d, %d\n", $rtc1_tick, *0x40011504
    printf "        rtc2_tick = %d, %d\n", $rtc2_tick, *0x40024504
end

define hook-stop
    printf "\nstop\n"
    pause_handler
end

define hook-run
    printf "\nrun\n"
    resume_handler
end

define hook-continue
    printf "\ncontinue\n"
    resume_handler
end

define hook-step
    printf "\nstep\n"
    resume_handler
end

define hook-next
    printf "\nnext\n"
    resume_handler
end

define hook-finish
    printf "\nfinish\n"
    resume_handler
end

define hook-until
    printf "\nuntil\n"
    resume_handler
end

define hook-stepi
    printf "\nstepi\n"
    resume_handler
end

define hook-nexti
    printf "\nnexti\n"
    resume_handler
end

define hook-return
    printf "\nreturn\n"
    resume_handler
end

delete
symbol-file

symbol-file app.elf

break main

continue
