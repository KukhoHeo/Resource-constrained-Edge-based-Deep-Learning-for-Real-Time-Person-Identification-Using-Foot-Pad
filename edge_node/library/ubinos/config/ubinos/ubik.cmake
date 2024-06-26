#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__UBINOS__UBIK                                                       TRUE)


set_cache_default(UBINOS__UBIK__TICK_TYPE                                       "SYSTICK" STRING "Tick Type [SYSTICK | RTC]")
set_cache_default(UBINOS__UBIK__TICK_PER_SEC                                    1000    STRING "Ticks per second")
set_cache_default(UBINOS__UBIK__TICK_COUNT_MAX                                  "UINT_MAX" STRING "Maximum tick count")

set_cache_default(UBINOS__UBIK__TASK_NAME_SIZE_MAX                              15      STRING "Maximum name size of task")
set_cache_default(UBINOS__UBIK__TASK_PRIORITY_MAX                               9       STRING "Maximum priority of task")
set_cache_default(UBINOS__UBIK__TASK_MAXWAITSIGOBJ_MAX                          8       STRING "Maximum waiting signal object of task")
set_cache_default(UBINOS__UBIK__TASK_STACK_DEPTH_DEFAULT                        1024    STRING "Default stack depth of task")
set_cache_default(UBINOS__UBIK__TASK_STACK_DEPTH_MIN                            512     STRING "Minimum stack depth of task")
set_cache_default(UBINOS__UBIK__IDLETASK_STACK_DEPTH                            512     STRING "Stack depth of idle task")

set_cache_default(UBINOS__UBIK__MSGQ_MSGSIZE_MAX                                16      STRING "Maximum message size of message queue")

set_cache_default(UBINOS__UBIK__EXCLUDE_TICK_HOOKFUNC                           FALSE   BOOL "Exclude tick hook functions")

set_cache_default(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING                       FALSE   BOOL "Exclude kernel monitoring feature")
set_cache_default(UBINOS__UBIK__EXCLUDE_TASK_MONITORING                         FALSE   BOOL "Exclude task monitoring feature")
set_cache_default(UBINOS__UBIK__EXCLUDE_TASK_PRIORITY_INHERITANCE               FALSE   BOOL "Exclude task priority inheritance feature")
set_cache_default(UBINOS__UBIK__EXCLUDE_IDLETASK_GARBAGECOLLECT                 FALSE   BOOL "Exclude idle task garbage collect feature")
set_cache_default(UBINOS__UBIK__EXCLUDE_IDLETASK_HOOKFUNC                       FALSE   BOOL "Exclude idle task hook function")
set_cache_default(UBINOS__UBIK__EXCLUDE_CONDV                                   FALSE   BOOL "Exclude condition variable")
set_cache_default(UBINOS__UBIK__EXCLUDE_MSGQ                                    FALSE   BOOL "Exclude message queue")
set_cache_default(UBINOS__UBIK__EXCLUDE_SEM                                     FALSE   BOOL "Exclude semaphore")
set_cache_default(UBINOS__UBIK__EXCLUDE_SIGNAL                                  FALSE   BOOL "Exclude signal")

set_cache_default(UBINOS__UBIK__EXCLUDE_STIMER                                  FALSE   BOOL "Exclude semaphore timer functions")

set_cache_default(UBINOS__UBIK__EXCLUDE_HRTICK                                  FALSE   BOOL "Exclude high resolution tick functions")
set_cache_default(UBINOS__UBIK__EXCLUDE_HRTICK_TICKISR_DELAY_CHECK              FALSE   BOOL "Exclude high resolution tick interrupt service routine delay check feature")

set_cache_default(UBINOS__UBIK__IDLETASK_SPINWAIT_INTERVALTICK                  10      STRING "Spin wait interval tick of idle task")

if(UBINOS__BSP__CPU_ARCH STREQUAL "ARM")

    if(UBINOS__BSP__CPU_TYPE STREQUAL "ARM926EJ_S")

    elseif(UBINOS__BSP__CORTEX_MX)

set_cache_default(UBINOS__UBIK__USE_PENDSV_TASK_YIELD                           TRUE    BOOL "Use PendSV for task yield instead of SVC")

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_TYPE")

    endif()

else()

    message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_ARCH")

endif()


if(UBINOS__UBIK__TICK_TYPE STREQUAL "RTC")

	if((UBINOS__BSP__CPU_MODEL STREQUAL "NRF52832XXAA") OR (UBINOS__BSP__CPU_MODEL STREQUAL "NRF52840XXAA"))

set_cache_default(UBINOS__UBIK__TICK_RTC_NO                                     2           STRING "RTC Number for tick [0 | 1 | 2]")
set_cache_default(UBINOS__UBIK__TICK_RTC_COUNT_MAX                              0xFFFFFFL   STRING "Maximum RTC count for tick")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK                   1           STRING "RTC Tick per kernel tick")

set_cache_default(UBINOS__UBIK__TICK_RTC_CHECK                                  TRUE        BOOL "Enable RTC check")
set_cache_default(UBINOS__UBIK__TICK_RTC_CHECK_TYPE                             "CORRECT"   STRING "RTC check type [CORRECT | ABORT]")

set_cache_default(UBINOS__UBIK__TICK_RTC_SLEEP_IDLE                             TRUE        BOOL "Enable to sleep when idle")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE                          TRUE        BOOL "Enable to ignore tick when idle")

set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MIN    10          STRING "Minimum rtc tick count to ignore while idle")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MAX    0xFFFFF0L   STRING "Maximum rtc tick count to ignore while idle. It must be less than UBINOS__UBIK__TICK_RTC_COUNT_MAX")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MARGIN 0           STRING "Margin of rtc tick count to ignore while idle")

	elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F217IG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32F207ZG"))

set_cache_default(UBINOS__UBIK__TICK_RTC_NO                                     5           STRING "RTC Number for tick [5]")
set_cache_default(UBINOS__UBIK__TICK_RTC_COUNT_MAX                              0xFFFFFFFFL STRING "Maximum RTC count for tick")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK                   2           STRING "RTC Tick per kernel tick")

set_cache_default(UBINOS__UBIK__TICK_RTC_CHECK                                  TRUE        BOOL "Enable RTC check")
set_cache_default(UBINOS__UBIK__TICK_RTC_CHECK_TYPE                             "CORRECT"   STRING "RTC check type [CORRECT | ABORT]")

set_cache_default(UBINOS__UBIK__TICK_RTC_SLEEP_IDLE                             FALSE       BOOL "Enable to sleep when idle")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE                          FALSE       BOOL "Enable to ignore tick when idle")

set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MIN    20          STRING "Minimum rtc tick count to ignore while idle")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MAX    0xFFFFFFF0L STRING "Maximum rtc tick count to ignore while idle. It must be less than UBINOS__UBIK__TICK_RTC_COUNT_MAX")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MARGIN 0           STRING "Margin of rtc tick count to ignore while idle")

	elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476ZG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476RG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L475VG"))

set_cache_default(UBINOS__UBIK__TICK_RTC_NO                                     5           STRING "RTC Number for tick [5]")
set_cache_default(UBINOS__UBIK__TICK_RTC_COUNT_MAX                              0xFFFFFFFFL STRING "Maximum RTC count for tick")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICK_PER_KERNEL_TICK                   2           STRING "RTC Tick per kernel tick")

set_cache_default(UBINOS__UBIK__TICK_RTC_CHECK                                  TRUE        BOOL "Enable RTC check")
set_cache_default(UBINOS__UBIK__TICK_RTC_CHECK_TYPE                             "CORRECT"   STRING "RTC check type [CORRECT | ABORT]")

set_cache_default(UBINOS__UBIK__TICK_RTC_SLEEP_IDLE                             FALSE       BOOL "Enable to sleep when idle")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE                          FALSE       BOOL "Enable to ignore tick when idle")

set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MIN    20          STRING "Minimum rtc tick count to ignore while idle")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MAX    0xFFFFFFF0L STRING "Maximum rtc tick count to ignore while idle. It must be less than UBINOS__UBIK__TICK_RTC_COUNT_MAX")
set_cache_default(UBINOS__UBIK__TICK_RTC_TICKLESS_IDLE_IGNORE_TICK_COUNT_MARGIN 0           STRING "Margin of rtc tick count to ignore while idle")

	else()
		message(FATAL_ERROR "This CPU model does not support RTC tick")
	endif()

endif(UBINOS__UBIK__TICK_TYPE STREQUAL "RTC")

########

set(_tmp_all_flags "")

if(INCLUDE__UBINOS__UBIK)
	set(_tmp_all_flags "${_tmp_all_flags} -DUBINOS_PRESENT")
endif()

if(UBINOS__UBIK__TICK_TYPE STREQUAL "SYSTICK")

elseif(UBINOS__UBIK__TICK_TYPE STREQUAL "RTC")

	if((UBINOS__BSP__CPU_MODEL STREQUAL "NRF52832XXAA") OR (UBINOS__BSP__CPU_MODEL STREQUAL "NRF52840XXAA"))
		set(_tmp_all_flags "${_tmp_all_flags} -u nrf_drv_clock_init")
		set(_tmp_all_flags "${_tmp_all_flags} -u app_error_handler_bare")
	elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F217IG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32F207ZG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476ZG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476RG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L475VG"))
	else()
		message(FATAL_ERROR "This CPU model does not support RTC tick")
	endif()

else()

	message(FATAL_ERROR "Unsupported UBINOS__UBIK__TICK_TYPE")

endif()

set(CMAKE_ASM_FLAGS "${_tmp_all_flags} ${CMAKE_ASM_FLAGS}")
set(CMAKE_C_FLAGS   "${_tmp_all_flags} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${_tmp_all_flags} ${CMAKE_CXX_FLAGS}")

