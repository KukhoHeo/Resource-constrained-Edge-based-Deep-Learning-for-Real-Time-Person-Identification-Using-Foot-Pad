#
# Copyright (c) 2019 Sung Ho Park and CSOS
#
# SPDX-License-Identifier: Apache-2.0
#

file(READ "${CMAKE_CURRENT_LIST_DIR}/../../VERSION" _ver)

string(REGEX MATCH "VERSION_MAJOR([ \t\r\n]*)([0-9]*)" _ ${_ver})
set(UBINOS__VERSION_MAJOR ${CMAKE_MATCH_2})

string(REGEX MATCH "VERSION_MINOR([ \t\r\n]*)([0-9]*)" _ ${_ver})
set(UBINOS__VERSION_MINOR ${CMAKE_MATCH_2})

string(REGEX MATCH "VERSION_PATCH([ \t\r\n]*)([0-9]*)" _ ${_ver})
set(UBINOS__VERSION_PATCH ${CMAKE_MATCH_2})

string(REGEX MATCH "VERSION_TWEAK([ \t\r\n]*)([0-9]*)" _ ${_ver})
set(UBINOS__VERSION_TWEAK ${CMAKE_MATCH_2})

if(UBINOS__VERSION_MAJOR GREATER_EQUAL 10)
    set(_ver_major "${UBINOS__VERSION_MAJOR}")
else()
    set(_ver_major "0${UBINOS__VERSION_MAJOR}")
endif()
if(UBINOS__VERSION_MINOR GREATER_EQUAL 10)
    set(_ver_minor "${UBINOS__VERSION_MINOR}")
else()
    set(_ver_minor "0${UBINOS__VERSION_MINOR}")
endif()
if(UBINOS__VERSION_PATCH GREATER_EQUAL 10)
    set(_ver_patch "${UBINOS__VERSION_PATCH}")
else()
    set(_ver_patch "0${UBINOS__VERSION_PATCH}")
endif()
if(UBINOS__VERSION_TWEAK GREATER_EQUAL 10)
    set(_ver_tweak "${UBINOS__VERSION_TWEAK}")
else()
    set(_ver_tweak "0${UBINOS__VERSION_TWEAK}")
endif()
set(UBINOS__VERSION "${_ver_major}.${_ver_minor}.${_ver_patch}-${_ver_tweak}")


########


set(INCLUDE__UBINOS__BSP                                                        TRUE)
set(PROJECT_UBINOS_LIBRARIES ${PROJECT_UBINOS_LIBRARIES} ubinos)

set_cache_default(UBINOS__BSP__CPU_ARCH                                         ""      STRING "CPU Architecture [ARM | LOCAL]")
set_cache_default(UBINOS__BSP__CPU_TYPE                                         ""      STRING "CPU Type [ARM7TDMI | ARM926EJ_S | CORTEX_M7 | CORTEX_M4 | CORTEX_M3 | LOCAL]")
set_cache_default(UBINOS__BSP__CPU_ENDIAN                                       ""      STRING "CPU endian [LITTLE | BIG]")
set_cache_default(UBINOS__BSP__CPU_MODEL                                        ""      STRING "CPU model [SAM7X256 | SAM7X512 | SAM9XE512 | NRF52832XXAA | NRF52840XXAA | STM32F217IG | STM32F207ZG  | STM32L476ZG | STM32L476RG | STM32L475VG | STM32F769NI | STM32H747XI | STM32F429NI | STM32F429ZI | STM32F100XB | STM32F405RG | LOCAL]")

set_cache_default(UBINOS__BSP__BOARD_MODEL                                      ""      STRING "Board model [SAM7X256EK | SAM7X512EK | SAM9XE512EK | NRF52DK | NRF52840DK | NRF52840DONGLE | ARDUINONANO33BLE | STM3221GEVAL | NUCLEOF207ZG | STM32L476GEVAL | NUCLEOL476RG | STM32L475EIOT01 | STM32F769IEVAL | STM32F769IDISCO | STM32H747IDISCO | STM32429IEVAL | STM32F429IDISCO | STM32VLDISCO | NETDUINOPLUS2 | LOCAL]")
set_cache_default(UBINOS__BSP__BOARD_REVISION_NO                                0       STRING "Board revision number")
set_cache_default(UBINOS__BSP__BOARD_VARIATION_NAME  "${UBINOS__BSP__BOARD_MODEL}"      STRING "Board variation name")

set_cache_default(UBINOS__BSP__LINK_MEMMAP_TYPE                                 ""      STRING "Link memory map type [FLASH | SRAM | FLASH_EXTSRAM | FLASH_EXTSDRAM | EXTFLASH_EXTSDRAM]")

set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH_ORIGIN                         0       STRING "FLASH origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH_LENGTH                         0       STRING "FLASH length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH2_ORIGIN                        0       STRING "FLASH2 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH2_LENGTH                        0       STRING "FLASH2 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH3_ORIGIN                        0       STRING "FLASH3 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH3_LENGTH                        0       STRING "FLASH3 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH4_ORIGIN                        0       STRING "FLASH4 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH4_LENGTH                        0       STRING "FLASH4 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH5_ORIGIN                        0       STRING "FLASH5 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH5_LENGTH                        0       STRING "FLASH5 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH6_ORIGIN                        0       STRING "FLASH6 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH6_LENGTH                        0       STRING "FLASH6 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH7_ORIGIN                        0       STRING "FLASH7 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH7_LENGTH                        0       STRING "FLASH7 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH8_ORIGIN                        0       STRING "FLASH8 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_FLASH8_LENGTH                        0       STRING "FLASH8 length of link memory map")

set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM_ORIGIN                           0       STRING "RAM origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM_LENGTH                           0       STRING "RAM length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM2_ORIGIN                          0       STRING "RAM2 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM2_LENGTH                          0       STRING "RAM2 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM3_ORIGIN                          0       STRING "RAM3 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM3_LENGTH                          0       STRING "RAM3 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM4_ORIGIN                          0       STRING "RAM4 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM4_LENGTH                          0       STRING "RAM4 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM5_ORIGIN                          0       STRING "RAM5 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM5_LENGTH                          0       STRING "RAM5 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM6_ORIGIN                          0       STRING "RAM6 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM6_LENGTH                          0       STRING "RAM6 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM7_ORIGIN                          0       STRING "RAM7 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM7_LENGTH                          0       STRING "RAM7 length of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM8_ORIGIN                          0       STRING "RAM8 origin address of link memory map")
set_cache_default(UBINOS__BSP__LINK_MEMMAP_RAM8_LENGTH                          0       STRING "RAM8 length of link memory map")


set_cache_default(UBINOS__BSP__USE_MMU                                          FALSE   BOOL "Use MMU")
set_cache_default(UBINOS__BSP__USE_ICACHE                                       FALSE   BOOL "Use i-cache")
set_cache_default(UBINOS__BSP__USE_DCACHE                                       FALSE   BOOL "Use d-cache")

set_cache_default(UBINOS__BSP__CLOCK_FREQ_MAIN                                  0       STRING "Main clock frequency (KHz)")
set_cache_default(UBINOS__BSP__CLOCK_FREQ_SLOW                                  0       STRING "Slow clock frequency (KHz)")
set_cache_default(UBINOS__BSP__BUSYWAITCOUNT_PER_MS                             0       STRING "Busy wait count per millisecond")

set_cache_default(UBINOS__BSP__USE_DTTY                                         TRUE    BOOL "Use dtty (debug tty)")
set_cache_default(UBINOS__BSP__DTTY_TYPE                                        "UART"  STRING "dtty type [UART | UART_ASYNC | EXTERNAL]")

set_cache_default(UBINOS__BSP__USE_EXTSRAM                                      FALSE   BOOL "Use external SRAM")
set_cache_default(UBINOS__BSP__USE_EXTSDRAM                                     FALSE   BOOL "Use external SDRAM")
set_cache_default(UBINOS__BSP__USE_EXTNORFLASH                                  FALSE   BOOL "Use external NOR Flash")

set_cache_default(UBINOS__BSP__EXTSDRAM_BUSWIDTH                                0       STRING "External SDRAM bus width")
set_cache_default(UBINOS__BSP__EXTNORFLASH_BUSWIDTH                             0       STRING "External NORFLASH bus width")
set_cache_default(UBINOS__BSP__EXTNORFLASH_ADDR                                 0       STRING "External NORFLASH address")

set_cache_default(UBINOS__BSP__STACK_SIZE                                       0       STRING "Stack size (ARM cortexm: Master stack size, ARM classic: total stack size of ISR handlers)")
set_cache_default(UBINOS__BSP__STACK_OVERFLOW_CHECK_MARGIN                      0       STRING "Stack overflow check margin")

set_cache_default(UBINOS__BSP__EXCLUDE_ARCH_INIT                                FALSE   BOOL "Exclude bsp architecture initialization code")

set_cache_default(UBINOS__BSP__ABORTSYSTEM_TYPE                                 "RESET"  STRING "Abort system type [STOP | RESET]")

set_cache_default(UBINOS__BSP__USE_QEMU                                         FALSE   BOOL "Use QEMU")

set_cache_default(UBINOS__BSP__USE_CRITCOUNT_IN_ISR_CHECK                       TRUE    BOOL "Use critical section entry count in interrupt service routine check")

if(UBINOS__BSP__CPU_ARCH STREQUAL "ARM")

set_cache_default(UBINOS__BSP__USE_RELOCATED_ISR_VECTOR                         TRUE    BOOL "Use relocated ISR vector")

    if(UBINOS__BSP__CPU_TYPE STREQUAL "ARM926EJ_S")

set_cache_default(UBINOS__BSP__CPU_ARMTHUMBSTATE                                "ARM"   STRING "CPU default ARM/THUMB state [ARM | THUMB]")
set_cache_default(UBINOS__BSP__INCLUDE_INTERRUPT_DISABLE_ENABLE_RETRY           TRUE    BOOL "Include interrupt disable enable retry")

    elseif((UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M7") OR (UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M4") OR (UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M3"))

set_cache_default(UBINOS__BSP__CORTEX_MX                                        TRUE    BOOL "ARM Cortex-M family CPU Type")
set_cache_default(UBINOS__BSP__CMSIS_INCLUDE_DIR                                "${PROJECT_UBINOS_DIR}/include/ubinos/bsp/arch/arm/cortexm/cmsis_v5_7_0" PATH "CMSIS include directory")

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_TYPE")

    endif()


    if(UBINOS__BSP__CPU_MODEL STREQUAL "SAM9XE512")

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "NRF52832XXAA") OR (UBINOS__BSP__CPU_MODEL STREQUAL "NRF52840XXAA"))

        if(UBINOS__BSP__CPU_MODEL STREQUAL "NRF52832XXAA")

set_cache_default(UBINOS__BSP__NRF52_NRF52XXX                                   TRUE    BOOL "")

set_cache_default(UBINOS__BSP__NRF52_NRF52                                      TRUE    BOOL "")
set_cache_default(UBINOS__BSP__NRF52_NRF52832_XXAA                              TRUE    BOOL "")
set_cache_default(UBINOS__BSP__NRF52_NRF52_PAN_74                               TRUE    BOOL "")

        elseif(UBINOS__BSP__CPU_MODEL STREQUAL "NRF52840XXAA")

set_cache_default(UBINOS__BSP__NRF52_NRF52XXX                                   TRUE    BOOL "")

set_cache_default(UBINOS__BSP__NRF52_NRF52840_XXAA                              TRUE    BOOL "")

        else()

            message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_MODEL")

        endif()

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")
set_cache_default(UBINOS__BSP__NRF52_SOFTDEVICE_PRESENT                         FALSE   BOOL "")
set_cache_default(UBINOS__BSP__NRF52_SOFTDEVICE_NAME                            ""      STRING "[S132 | S140]")
set_cache_default(UBINOS__BSP__NRF52_SOFTDEVICE_BLE_API_VERSION                 ""      STRING "[7]")
set_cache_default(UBINOS__BSP__NRF52_SOFTDEVICE_FILE                            ""      PATH "nRF52 softdevice file")
set_cache_default(UBINOS__BSP__NRF52_SOFTDEVICE_INCLUDE_DIR                     ""      PATH "nRF52 softdevice include directory")

set_cache_default(UBINOS__BSP__NRF52_MBR_PRESENT                                FALSE   BOOL "")

set_cache_default(UBINOS__BSP__NRF52_IGNORE_HARDFAULT_EXCEPTION_FROM_SOFTDEVICE TRUE    BOOL "Ignore hard fault exception from nrf softdevice for debugging")

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F217IG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32F207ZG"))

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32F2XX                                  TRUE    BOOL "STM32F2XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32F2X7                                  TRUE    BOOL "STM32F2X7 family CPU Model")

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476ZG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476RG"))

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32L4XX                                  TRUE    BOOL "STM32F4XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32L476                                  TRUE    BOOL "STM32L476 family CPU Model")

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32L475VG")

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32L4XX                                  TRUE    BOOL "STM32F4XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32L475                                  TRUE    BOOL "STM32L475 family CPU Model")

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32F769NI")

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32F7XX                                  TRUE    BOOL "STM32F7XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32F769                                  TRUE    BOOL "STM32F769 family CPU Model")

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32H747XI")

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32H7XX                                  TRUE    BOOL "STM32H7XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32H747                                  TRUE    BOOL "STM32H747 family CPU Model")

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F429NI") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32F429ZI"))

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32F4XX                                  TRUE    BOOL "STM32F4XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32F429                                  TRUE    BOOL "STM32F429 family CPU Model")

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F405RG"))

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32F4XX                                  TRUE    BOOL "STM32F4XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32F405                                  TRUE    BOOL "STM32F405 family CPU Model")

set_cache_default(UBINOS__BSP__USE_SOFTFLOAT                                    FALSE   BOOL "Use soft float")

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32F100XB")

set_cache_default(UBINOS__BSP__STM32_STM32XXXX                                  TRUE    BOOL "STM32XXXX family CPU Model")

set_cache_default(UBINOS__BSP__STM32_STM32F1XX                                  TRUE    BOOL "STM32F1XX family CPU Model")
set_cache_default(UBINOS__BSP__STM32_STM32F100                                  TRUE    BOOL "STM32F100 family CPU Model")

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_MODEL")

    endif()


    if(UBINOS__BSP__BOARD_MODEL STREQUAL "SAM9XE512EK")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "NRF52DK") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "NRF52840DK") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "NRF52840DONGLE") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "ARDUINONANO33BLE"))

set_cache_default(UBINOS__BSP__NRF52_CONFIG_GPIO_AS_PINRESET                    TRUE    BOOL "")
set_cache_default(UBINOS__BSP__NRF52_ENABLE_SWO                                 FALSE   BOOL "")
set_cache_default(UBINOS__BSP__NRF52_ENABLE_TRACE                               FALSE   BOOL "")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "STM3221GEVAL") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "NUCLEOF207ZG"))

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_RCC_HSE_CONFIG           "ON"             STRING "[ON | OFF | BYPASS]")
set_cache_default(UBINOS__BSP__STM32_HSE_VALUE                "25000000U"      STRING "Value of the External oscillator in Hz [25000000U | 8000000U]")

set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "3"            STRING "[3 | 6 | 2]")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "STM32L476GEVAL") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "NUCLEOL476RG") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "STM32L475EIOT01"))

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "1"            STRING "[1 | 2]")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "STM32F769IEVAL") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "STM32F769IDISCO"))

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_HSE_VALUE                "25000000U"      STRING "Value of the External oscillator in Hz [25000000U | 8000000U]")

set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "1"            STRING "[1]")

set_cache_default(UBINOS__BSP__STM32_ENABLE_ETHERNET                            FALSE    BOOL "")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "STM32H747IDISCO"))

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_HSE_VALUE                "25000000U"      STRING "Value of the External oscillator in Hz [25000000U]")

set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "1"            STRING "[1]")

set_cache_default(UBINOS__BSP__STM32_ENABLE_ETHERNET                            FALSE    BOOL "")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "STM32429IEVAL") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "STM32F429IDISCO"))

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_HSE_VALUE                "25000000U"      STRING "Value of the External oscillator in Hz [25000000U | 8000000U]")

set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "1"            STRING "[1]")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "NETDUINOPLUS2"))

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_HSE_VALUE                "25000000U"      STRING "Value of the External oscillator in Hz [25000000U]")

        if (UBINOS__BSP__USE_QEMU)
set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "1"            STRING "[1]")
        else()
set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "2"            STRING "[2]")
        endif()

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32VLDISCO")

set_cache_default(UBINOS__BSP__STM32_ENABLE_TRACE                               FALSE   BOOL "")

set_cache_default(UBINOS__BSP__STM32_DTTY_USARTx_INSTANCE_NUMBER "1"            STRING "[1 | 2]")

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__BOARD_MODEL")

    endif()

elseif(UBINOS__BSP__CPU_ARCH STREQUAL "LOCAL")

else()

    message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_ARCH")

endif()


set_cache_default(UBINOS__BSP__LINKSCRIPT_FILE                                  ""      PATH "Link script file")

set_cache_default(UBINOS__BSP__FLASH_WRITER_FILE                                ""      PATH "Flash writer file")
set_cache_default(UBINOS__BSP__SYS_INIT_FILE                                    ""      PATH "System initiation file")

set_cache_default(UBINOS__BSP__OPENOCD_CONFIG_FILE                              ""      PATH "OpenOCD config file")

set_cache_default(UBINOS__BSP__GDBSCRIPT_FILE_INIT                              ""      PATH "GDB script file to init")

set_cache_default(UBINOS__BSP__GDBSCRIPT_FILE_ATTACH                            ""      PATH "Gdb script file to attach")
set_cache_default(UBINOS__BSP__GDBSCRIPT_FILE_DEBUG                             ""      PATH "Gdb script file to debug")
set_cache_default(UBINOS__BSP__GDBSCRIPT_FILE_RUN                               ""      PATH "Gdb script file to run")

set_cache_default(UBINOS__BSP__GDBSCRIPT_FILE_LOAD                              ""      PATH "GDB script file to load")
set_cache_default(UBINOS__BSP__GDBSCRIPT_FILE_RESET                             ""      PATH "GDB script file to reset")

set_cache_default(UBINOS__BSP__T32SCRIPT_FILE_LOAD                              ""      PATH "Trace32 script file to load")
set_cache_default(UBINOS__BSP__T32SCRIPT_FILE_RESET                             ""      PATH "Trace32 script file to reset")

set_cache_default(UBINOS__BSP__DOXYGEN_FILE                                     "${PROJECT_UBINOS_DIR}/resource/ubinos/doc/Doxyfile" PATH "Doxygen file")

set_cache_default(UBINOS__BSP__DEBUG_SERVER_HOST "" STRING "Debug server host name or IP")
set_cache_default(UBINOS__BSP__DEBUG_SERVER_PORT "2331" STRING "Debug server port number")
set_cache_default(UBINOS__BSP__DEBUG_SERVER_TYPE "NONE" STRING "Debug server type [NONE | GDBSERVER | JLINK | OPENOCD | QEMU]")
set_cache_default(UBINOS__BSP__DEBUG_SERVER_COMMAND "" STRING "Debug server command")
set_cache_default(UBINOS__BSP__DEBUG_SERVER_SERIAL "" STRING "Debug server serial number")
set_cache_default(UBINOS__BSP__DEBUG_SERVER_TARGET_INTERFACE "SWD" STRING "Debug server target interface [SWE | JTAG]")

########


set(_tmp_all_flags "")

if(UBINOS__BSP__CPU_ARCH STREQUAL "ARM")

    if(UBINOS__BSP__CPU_TYPE STREQUAL "ARM926EJ_S")

        set(_tmp_all_flags "${_tmp_all_flags} -mcpu=arm926ej-s")

    elseif( UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M7")

        set(_tmp_all_flags "${_tmp_all_flags} -mcpu=cortex-m7")
        set(_tmp_all_flags "${_tmp_all_flags} -mthumb")

        include_directories(${UBINOS__BSP__CMSIS_INCLUDE_DIR})

    elseif( UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M4")

        set(_tmp_all_flags "${_tmp_all_flags} -mcpu=cortex-m4")
        set(_tmp_all_flags "${_tmp_all_flags} -mthumb")

        include_directories(${UBINOS__BSP__CMSIS_INCLUDE_DIR})

    elseif( UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M3")

        set(_tmp_all_flags "${_tmp_all_flags} -mcpu=cortex-m3")
        set(_tmp_all_flags "${_tmp_all_flags} -mthumb")
        set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=soft")

        include_directories(${UBINOS__BSP__CMSIS_INCLUDE_DIR})

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_TYPE")

    endif()


    if(UBINOS__BSP__CPU_ENDIAN STREQUAL "LITTLE")

        set(_tmp_all_flags "${_tmp_all_flags} -mlittle-endian")

    elseif(UBINOS__BSP__CPU_ENDIAN STREQUAL "BIG")

        set(_tmp_all_flags "${_tmp_all_flags} -mbig-endian")

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_ENDIAN")

    endif()

    if(UBINOS__BSP__CPU_MODEL STREQUAL "SAM9XE512")

        if(UBINOS__BSP__CPU_ARMTHUMBSTATE STREQUAL "ARM")

            set(_tmp_all_flags "${_tmp_all_flags} -marm")

        elseif(UBINOS__BSP__CPU_ARMTHUMBSTATE STREQUAL "THUMB")

            set(_tmp_all_flags "${_tmp_all_flags} -mthumb -mthumb-interwork")

        else()

            message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_ARMTHUMBSTATE")

        endif()

        set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")
        set(_tmp_all_flags "${_tmp_all_flags} -fomit-frame-pointer")

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "NRF52832XXAA") OR (UBINOS__BSP__CPU_MODEL STREQUAL "NRF52840XXAA"))

        if(UBINOS__BSP__USE_SOFTFLOAT)

            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")

        else()

            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv4-sp-d16")
            set(_tmp_all_flags "${_tmp_all_flags} -DFLOAT_ABI_HARD")

        endif()

        if(UBINOS__BSP__NRF52_NRF52)

            set(_tmp_all_flags "${_tmp_all_flags} -DNRF52")

        endif()

        if(UBINOS__BSP__NRF52_NRF52832_XXAA)

            set(_tmp_all_flags "${_tmp_all_flags} -DNRF52832_XXAA")

        endif()

        if(UBINOS__BSP__NRF52_NRF52_PAN_74)

            set(_tmp_all_flags "${_tmp_all_flags} -DNRF52_PAN_74")

        endif()

        if(UBINOS__BSP__NRF52_NRF52840_XXAA)

            set(_tmp_all_flags "${_tmp_all_flags} -DNRF52840_XXAA")

        endif()

        if(UBINOS__BSP__NRF52_SOFTDEVICE_PRESENT)

            set(_tmp_all_flags "${_tmp_all_flags} -DSOFTDEVICE_PRESENT")

        endif()

        if(UBINOS__BSP__NRF52_MBR_PRESENT)

            set(_tmp_all_flags "${_tmp_all_flags} -DMBR_PRESENT")

        endif()

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32F217IG")

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32F217xx")

            if(NOT UBINOS__BSP__STM32_HSE_VALUE STREQUAL "")
                set(_tmp_all_flags "${_tmp_all_flags} -DHSE_VALUE=${UBINOS__BSP__STM32_HSE_VALUE}")
            endif()

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32F207ZG")

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32F207xx")

            if(NOT UBINOS__BSP__STM32_HSE_VALUE STREQUAL "")
                set(_tmp_all_flags "${_tmp_all_flags} -DHSE_VALUE=${UBINOS__BSP__STM32_HSE_VALUE}")
            endif()

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32L475VG")

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32L475xx")

        if(UBINOS__BSP__USE_SOFTFLOAT)

            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")

        else()

            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv4-sp-d16")

        endif()

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476ZG") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32L476RG"))

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32L476xx")

        if(UBINOS__BSP__USE_SOFTFLOAT)

            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")

        else()

            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv4-sp-d16")

        endif()

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32F769NI")

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32F769xx")

            if(NOT UBINOS__BSP__STM32_HSE_VALUE STREQUAL "")
                set(_tmp_all_flags "${_tmp_all_flags} -DHSE_VALUE=${UBINOS__BSP__STM32_HSE_VALUE}")
            endif()

        if(UBINOS__BSP__USE_SOFTFLOAT)

            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")

        else()

            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv5-d16")

        endif()

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32H747XI")
    
        if(UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M7")
            set(_tmp_all_flags "${_tmp_all_flags} -DCORE_CM7")
        elseif(UBINOS__BSP__CPU_TYPE STREQUAL "CORTEX_M4")
            set(_tmp_all_flags "${_tmp_all_flags} -DCORE_CM4")
        else()
            message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_TYPE")
        endif()

        set(_tmp_all_flags "${_tmp_all_flags} -DSTM32H747xx")

        if(NOT UBINOS__BSP__STM32_HSE_VALUE STREQUAL "")
            set(_tmp_all_flags "${_tmp_all_flags} -DHSE_VALUE=${UBINOS__BSP__STM32_HSE_VALUE}")
        endif()

        if(UBINOS__BSP__USE_SOFTFLOAT)
            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")
        else()
            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv5-d16")
        endif()

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F429NI") OR (UBINOS__BSP__CPU_MODEL STREQUAL "STM32F429ZI"))

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32F429xx")

            if(NOT UBINOS__BSP__STM32_HSE_VALUE STREQUAL "")
                set(_tmp_all_flags "${_tmp_all_flags} -DHSE_VALUE=${UBINOS__BSP__STM32_HSE_VALUE}")
            endif()

        if(UBINOS__BSP__USE_SOFTFLOAT)

            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")

        else()

            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv4-sp-d16")

        endif()

    elseif((UBINOS__BSP__CPU_MODEL STREQUAL "STM32F405RG"))

            set(_tmp_all_flags "${_tmp_all_flags} -DSTM32F405xx")

            if(NOT UBINOS__BSP__STM32_HSE_VALUE STREQUAL "")
                set(_tmp_all_flags "${_tmp_all_flags} -DHSE_VALUE=${UBINOS__BSP__STM32_HSE_VALUE}")
            endif()

        if(UBINOS__BSP__USE_SOFTFLOAT)

            set(_tmp_all_flags "${_tmp_all_flags} -msoft-float")

        else()

            set(_tmp_all_flags "${_tmp_all_flags} -mfloat-abi=hard")
            set(_tmp_all_flags "${_tmp_all_flags} -mfpu=fpv4-sp-d16")

        endif()

    elseif(UBINOS__BSP__CPU_MODEL STREQUAL "STM32F100XB")

        set(_tmp_all_flags "${_tmp_all_flags} -DSTM32F100xB")

    else()

       message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_MODEL")

    endif()


    if(UBINOS__BSP__BOARD_MODEL STREQUAL "SAM9XE512EK")

    elseif((UBINOS__BSP__BOARD_MODEL STREQUAL "NRF52DK") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "NRF52840DK") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "NRF52840DONGLE") OR (UBINOS__BSP__BOARD_MODEL STREQUAL "ARDUINONANO33BLE"))

        if(UBINOS__BSP__NRF52_CONFIG_GPIO_AS_PINRESET)

            set(_tmp_all_flags "${_tmp_all_flags} -DCONFIG_GPIO_AS_PINRESET")

        endif()

        if(UBINOS__BSP__NRF52_ENABLE_SWO)

            set(_tmp_all_flags "${_tmp_all_flags} -DENABLE_SWO")

        endif()

        if(UBINOS__BSP__NRF52_ENABLE_TRACE)

            set(_tmp_all_flags "${_tmp_all_flags} -DENABLE_TRACE")

        endif()

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM3221GEVAL")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM322xG_EVAL")

        if(UBINOS__BSP__USE_EXTSRAM)

        	set(_tmp_all_flags "${_tmp_all_flags} -DDATA_IN_ExtSRAM")

        endif(UBINOS__BSP__USE_EXTSRAM)

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "NUCLEOF207ZG")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32F2XX_NUCLEO_144")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32L476GEVAL")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32L476G_EVAL_REVB")
        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_IOEXPANDER")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "NUCLEOL476RG")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32L4XX_NUCLEO")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "NETDUINOPLUS2")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32L475EIOT01")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32L475E_IOT01")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32F769IEVAL")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32F769I_EVAL")
        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_IOEXPANDER")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32F769IDISCO")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32F769I_DISCO")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32H747IDISCO")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32H747I_DISCO")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32429IEVAL")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM324x9I_EVAL")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32F429IDISCO")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32F429I_DISCO")

    elseif(UBINOS__BSP__BOARD_MODEL STREQUAL "STM32VLDISCO")

        set(_tmp_all_flags "${_tmp_all_flags} -DUSE_STM32VL_DISCO")

    else()

        message(FATAL_ERROR "Unsupported UBINOS__BSP__BOARD_MODEL")

    endif()

	if(UBINOS__BSP__STM32_ENABLE_TRACE)

		set(_tmp_all_flags "${_tmp_all_flags} -DSTM32_ENABLE_TRACE")

	endif()

    set(_tmp_all_flags "${_tmp_all_flags} -D__STACK_SIZE=${UBINOS__BSP__STACK_SIZE}")

    set(_tmp_all_flags "${_tmp_all_flags} -mabi=aapcs")
    set(_tmp_all_flags "${_tmp_all_flags} -ffunction-sections")
    set(_tmp_all_flags "${_tmp_all_flags} -fdata-sections")
    set(_tmp_all_flags "${_tmp_all_flags} -fno-strict-aliasing")
    set(_tmp_all_flags "${_tmp_all_flags} -fno-builtin")
    set(_tmp_all_flags "${_tmp_all_flags} -fshort-enums")
    set(_tmp_all_flags "${_tmp_all_flags} -falign-functions=16")

    set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} m)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs --specs=nosys.specs")

    set(_tmp_all_flags "${_tmp_all_flags} -g3")
    set(_tmp_all_flags "${_tmp_all_flags} -Wall -Werror -fmessage-length=0")
    
    set(_tmp_all_flags "${_tmp_all_flags} -D_GNU_SOURCE")

elseif(UBINOS__BSP__CPU_ARCH STREQUAL "LOCAL")

else() # if(UBINOS__BSP__CPU_ARCH STREQUAL "...")

    message(FATAL_ERROR "Unsupported UBINOS__BSP__CPU_ARCH")

endif() # if(UBINOS__BSP__CPU_ARCH STREQUAL "...")

if(INCLUDE__UBINOS__BSP)
	set(_tmp_all_flags "${_tmp_all_flags} -DUBINOS_BSP_PRESENT")
endif()

set(CMAKE_ASM_FLAGS "${_tmp_all_flags} ${CMAKE_ASM_FLAGS}")
set(CMAKE_C_FLAGS   "${_tmp_all_flags} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${_tmp_all_flags} ${CMAKE_CXX_FLAGS}")

