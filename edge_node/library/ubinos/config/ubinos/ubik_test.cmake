#
# Copyright (c) 2019 Sung Ho Park and CSOS
# 
# SPDX-License-Identifier: Apache-2.0
#

set(INCLUDE__UBINOS__UBIK_TEST                                                  TRUE)


set_cache_default(UBINOS__UBIK_TEST__MSGBUFSIZE                                 1024    STRING "ubik test message buffer size")
set_cache_default(UBINOS__UBIK_TEST__BUSYWAITCOUNTTESTTIMEMS                    5000    STRING "ubik test busy wait count test time (millisecond)")
set_cache_default(UBINOS__UBIK_TEST__TASKWAITTIMEMS                             200     STRING "ubik test task wait time (millisecond)")
set_cache_default(UBINOS__UBIK_TEST__TASKLOOPCOUNT                              6       STRING "ubik test task loop count")
set_cache_default(UBINOS__UBIK_TEST__MEMLEAK_TESTCOUNT                          20      STRING "ubik memory leak test count")
set_cache_default(UBINOS__UBIK_TEST__MEMLEAK_SIGOBJCOUNT                        40      STRING "ubik memory leak test signal object count")
set_cache_default(UBINOS__UBIK_TEST__MEMLEAK_TASKCOUNT                          4       STRING "ubik memory leak test task count")
set_cache_default(UBINOS__UBIK_TEST__MSGQSIZE                                   8       STRING "ubik test message queue size")
set_cache_default(UBINOS__UBIK_TEST__MSGQMAXCOUNT                               10      STRING "ubik test maximum message queue count")

set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_MISCTESTSET                        FALSE   BOOL "Exclude ubik miscellaneous test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET                        FALSE   BOOL "Exclude ubik task test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET2                       FALSE   BOOL "Exclude ubik task test set 2")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_TASKTESTSET3                       FALSE   BOOL "Exclude ubik task test set 3")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_SIGNALTESTSET                      FALSE   BOOL "Exclude ubik signal test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_MUTEXTESTSET                       FALSE   BOOL "Exclude ubik mutex test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_SEMTESTSET                         FALSE   BOOL "Exclude ubik semaphore test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_MSGQTESTSET                        FALSE   BOOL "Exclude ubik message queue test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_CONDVTESTSET                       FALSE   BOOL "Exclude ubik condition variable test set")
set_cache_default(UBINOS__UBIK_TEST__EXCLUDE_STIMERTESTSET                      FALSE   BOOL "Exclude ubik semaphore timer test set")


########


