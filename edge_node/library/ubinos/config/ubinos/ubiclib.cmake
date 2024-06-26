#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__UBINOS__UBICLIB                                                    TRUE)


set_cache_default(UBINOS__UBICLIB__NOSTDLIB                                     FALSE   BOOL "Do not use the standard system startup files or libraries when linking")

set_cache_default(UBINOS__UBICLIB__USE_MALLOC_RETARGETING                       FALSE   BOOL "Use malloc retargeting")

set_cache_default(UBINOS__UBICLIB__USE_STDIO_RETARGETING                        TRUE    BOOL "Use stdio retargeting")

set_cache_default(UBINOS__UBICLIB__USE_DUMMY_MKDIR                              FALSE    BOOL "Use dummy mkdir function")

set_cache_default(UBINOS__UBICLIB__EXCLUDE_ARCH_INIT                            FALSE   BOOL "Exclude ubiclib architecture initialization code")

set_cache_default(UBINOS__UBICLIB__EXCLUDE_LOGM                                 FALSE   BOOL "Exclude log message functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_CLI                                  TRUE    BOOL "Exclude command line interface functions")

set_cache_default(UBINOS__UBICLIB__EXCLUDE_LIST                                 FALSE   BOOL "Exclude linked list functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_DLIST                                FALSE   BOOL "Exclude double linked list functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_CIRBUF                               FALSE   BOOL "Exclude circular buffer functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_CBUF                                 FALSE   BOOL "Exclude simple circular buffer functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_BITMAP                               FALSE   BOOL "Exclude multi-level bitmap functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_MATH_EXT                             FALSE   BOOL "Exclude math extention functions")

set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP                                 TRUE    BOOL "Exclude heap functions")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_BOUNDARY_CHECK                  FALSE   BOOL "Exclude heap boundary check feature")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK         FALSE   BOOL "Exclude heap intermediate data check feature")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM                            TRUE    BOOL "Exclude dynamic memory power management feature")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM_MEMORY_READY_CHECK         TRUE    BOOL "Exclude memory ready check of dynamic memory power management feature")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_LOGMD                           FALSE   BOOL "Exclude heap debug log message feature")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_PRINTHEAPINFO                   FALSE   BOOL "Exclude heap print heap information function")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT              FALSE   BOOL "Exclude best fit algorithm")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT             TRUE    BOOL "Exclude first fit algorithm")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT              TRUE    BOOL "Exclude next fit algorithm")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP               TRUE    BOOL "Exclude pure group system algorithm")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP                FALSE   BOOL "Exclude group system algorithm")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY               TRUE    BOOL "Exclude binary buddy system algorithm")
set_cache_default(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY               TRUE    BOOL "Exclude weighted buddy system algorithm")

# maximum size = 256 KiB
set_cache_default(UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM                          "GROUP"     STRING "Algorithm for normal heap [BESTFIT | FIRSTFIT | NEXTFIT | PGROUP | GROUP | BBUDDY | WBUDDY]")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE                           "MUTEX"     STRING "Lock type for normal heap [NONE | MUTEX | CRITICAL]")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR0_M                                  8           STRING "Maximum number of members of group system for normal heap")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT                           226         STRING "Free block list count for normal heap")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE                      36          STRING "Free block list bitmap buffer size for normal heap")

set_cache_default(UBINOS__UBICLIB__HEAP_DIR1_ALGORITHM                          "BESTFIT"   STRING "Algorithm for reverse heap [BESTFIT | FIRSTFIT | NEXTFIT | PGROUP | GROUP | BBUDDY | WBUDDY]")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR1_LOCKTYPE                           "MUTEX"     STRING "Lock type for reverse heap [NONE | MUTEX | CRITICAL]")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR1_M                                  2           STRING "Maximum number of members of group system for reverse heap")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR1_FBLCOUNT                           2           STRING "Free block list count for reverse heap")
set_cache_default(UBINOS__UBICLIB__HEAP_DIR1_FBLBM_BUFSIZE                      4           STRING "Free block list bitmap buffer size for reverse heap")


########


if(NOT UBINOS__UBICLIB__NOSTDLIB)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _sbrk")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _scanf_float")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _printf_float")

else()

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")

endif()


if(UBINOS__UBICLIB__USE_STDIO_RETARGETING)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _write")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _read")

endif(UBINOS__UBICLIB__USE_STDIO_RETARGETING)


if(UBINOS__UBICLIB__USE_MALLOC_RETARGETING)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _malloc_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _free_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _malloc_usable_size_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _realloc_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _calloc_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _cfree_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _memalign_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _valloc_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _pvalloc_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _mallinfo_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _malloc_stats_r")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -u _mallopt_r")

endif()


