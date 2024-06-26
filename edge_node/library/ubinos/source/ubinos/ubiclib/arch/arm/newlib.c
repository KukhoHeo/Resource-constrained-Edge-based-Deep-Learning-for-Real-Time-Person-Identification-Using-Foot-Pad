/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../_ubiclib.h"

#if (INCLUDE__UBINOS__UBICLIB == 1)
#if (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__ARM)
#if !(UBINOS__UBICLIB__NOSTDLIB == 1)
#if !(UBINOS__UBICLIB__EXCLUDE_ARCH_INIT == 1)

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

#define _REENT_SMALL
#undef _REENT_GLOBAL_STDIO_STREAMS

//#define _RETARGETABLE_LOCKING

//#include <_ansi.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/lock.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/reent.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <sys/errno.h>
#include <sys/time.h>

#include "../../_heap.h"

#undef errno
extern int errno;

struct __lock {
    bsp_mutex_pt lock;
};

struct __lock __lock___sinit_recursive_mutex = {0};
struct __lock __lock___sfp_recursive_mutex = {0};
struct __lock __lock___atexit_recursive_mutex = {0};
struct __lock __lock___at_quick_exit_mutex = {0};
struct __lock __lock___malloc_recursive_mutex = {0};
struct __lock __lock___env_recursive_mutex = {0};
struct __lock __lock___tz_mutex = {0};
struct __lock __lock___dd_hash_mutex = {0};
struct __lock __lock___arc4random_mutex = {0};

struct timeval __timeval_offset_plus;
struct timeval __timeval_offset_minus;

int stdlib_port_comp_init(void) {
    struct _reent * ptr = _REENT;

    if (0 == ptr) {
      return -1;
    }

    _REENT_SMALL_CHECK_INIT (ptr);

    ptr->_stdin->_flags  |= __SNBF;
    ptr->_stdout->_flags |= __SNBF;
    ptr->_stderr->_flags |= __SNBF;

    __timeval_offset_plus.tv_sec = 0;
    __timeval_offset_plus.tv_usec = 0;
    __timeval_offset_minus.tv_sec = 0;
    __timeval_offset_minus.tv_usec = 0;

	return 0;
}

int stdlib_port_comp_init_reent(void) {
    bsp_mutex_create(&(__lock___sinit_recursive_mutex.lock));
    bsp_mutex_create(&(__lock___sfp_recursive_mutex.lock));
    bsp_mutex_create(&(__lock___atexit_recursive_mutex.lock));
    bsp_mutex_create(&(__lock___at_quick_exit_mutex.lock));
    bsp_mutex_create(&(__lock___malloc_recursive_mutex.lock));
    bsp_mutex_create(&(__lock___env_recursive_mutex.lock));
    bsp_mutex_create(&(__lock___tz_mutex.lock));
    bsp_mutex_create(&(__lock___dd_hash_mutex.lock));
    bsp_mutex_create(&(__lock___arc4random_mutex.lock));
    return 0;
}

void
__retarget_lock_init (_LOCK_T *lock)
{
    _LOCK_T tmplock;
    int r;
    if (lock) {
        tmplock = malloc(sizeof(struct __lock));
        if (tmplock) {
            r = bsp_mutex_create(&tmplock->lock);
            if (0 == r) {
                *lock = tmplock;
            }
        }
    }
}

void
__retarget_lock_init_recursive(_LOCK_T *lock)
{
    _LOCK_T tmplock;
    int r;
    if (lock) {
        tmplock = malloc(sizeof(struct __lock));
        if (tmplock) {
            r = bsp_mutex_create(&tmplock->lock);
            if (0 == r) {
                *lock = tmplock;
            }
        }
    }
}

void
__retarget_lock_close(_LOCK_T lock)
{
    if (lock) {
        if (lock->lock) {
            bsp_mutex_delete(&lock->lock);
        }
        free(lock);
    }
}

void
__retarget_lock_close_recursive(_LOCK_T lock)
{
    if (lock) {
        if (lock->lock) {
            bsp_mutex_delete(&lock->lock);
        }
        free(lock);
    }
}

void
__retarget_lock_acquire (_LOCK_T lock)
{
    if (lock && lock->lock) {
        bsp_mutex_lock(lock->lock);
    }
}

void
__retarget_lock_acquire_recursive (_LOCK_T lock)
{
    if (lock && lock->lock) {
        bsp_mutex_lock(lock->lock);
    }
}

int
__retarget_lock_try_acquire(_LOCK_T lock)
{
    if (lock && lock->lock) {
        if(0 == bsp_mutex_lock_timed(lock->lock, 0)) {
            return 1;
        }
    }
    return 0;
}

int
__retarget_lock_try_acquire_recursive(_LOCK_T lock)
{
    if (lock && lock->lock) {
        if(0 == bsp_mutex_lock_timed(lock->lock, 0)) {
            return 1;
        }
    }
    return 0;
}

void
__retarget_lock_release (_LOCK_T lock)
{
    if (lock && lock->lock) {
        bsp_mutex_unlock(lock->lock);
    }
}

void
__retarget_lock_release_recursive (_LOCK_T lock)
{
    if (lock && lock->lock) {
        bsp_mutex_unlock(lock->lock);
    }
}

#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)

void * _sbrk (int incr)
{
   return (void *) ENOMEM;
}

void * _malloc_r(struct _reent * reent_ptr, size_t size)
{
    if (_ubiclib_heap == NULL)
    {
        ubiclib_heap_comp_init();
    }
    return _heap_allocate_block(_ubiclib_heap, 0, SIZETOUINT(size));
}

void _free_r(struct _reent * reent_ptr, void * ptr)
{
    if (_ubiclib_heap == NULL)
    {
        ubiclib_heap_comp_init();
    }
    _heap_release_block(_ubiclib_heap, ptr);
}

size_t _malloc_usable_size_r(struct _reent * reent_ptr, void * ptr)
{
    unsigned int size;

    if (_ubiclib_heap == NULL)
    {
        ubiclib_heap_comp_init();
    }
    heap_getblocksize(NULL, ptr, &size);

    return size;
}

void * _realloc_r(struct _reent * reent_ptr, void * ptr, size_t size)
{
    void * ptr_new;

    if (ptr == NULL) {
        return _malloc_r(reent_ptr, size);
    }

    if (_malloc_usable_size_r(reent_ptr, ptr) >= size) {
        return ptr;
    }

    ptr_new = _malloc_r(reent_ptr, size);
    if (ptr_new != NULL) {
        memcpy(ptr_new, ptr, size);
        _free_r(reent_ptr, ptr);
    }
    return ptr_new;
}

void * _calloc_r(struct _reent * reent_ptr, size_t n, size_t elem)
{
    void * ptr;

    ptr = _malloc_r(reent_ptr,  n * elem);
    if (ptr != NULL) {
        memset(ptr, 0, n * elem);
    }
    return ptr;
}

void _cfree_r(struct _reent * reent_ptr, void * ptr) {
    _free_r(reent_ptr, ptr);
}

void * _memalign_r(struct _reent * reent_ptr, size_t align, size_t s) {
    // FIXME: not implemented.;
    dtty_puts(__func__, 80);
    dtty_puts(" : fail (not implemented).\n", 80);
    bsp_abortsystem();
    return NULL;
}

void * _valloc_r(struct _reent * reent_ptr, size_t s)
{
    // FIXME: not implemented.;
    dtty_puts(__func__, 80);
    dtty_puts(" : fail (not implemented).\n", 80);
    bsp_abortsystem();
    return NULL;
}

void * _pvalloc_r(struct _reent * reent_ptr, size_t s)
{
    // FIXME: not implemented.;
    dtty_puts(__func__, 80);
    dtty_puts(" : fail (not implemented).\n", 80);
    bsp_abortsystem();
    return NULL;
}


struct mallinfo _mallinfo_r(struct _reent * reent_ptr)
{
    struct mallinfo mi = {0,};
    // FIXME: not implemented.;
    dtty_puts(__func__, 80);
    dtty_puts(" : fail (not implemented).\n", 80);
    bsp_abortsystem();
    return mi;
}

void _malloc_stats_r(struct _reent * reent_ptr)
{
    // FIXME: not implemented.;
    dtty_puts(__func__, 80);
    dtty_puts(" : fail (not implemented).\n", 80);
    bsp_abortsystem();
}

int _mallopt_r(struct _reent * reent_ptr, int parameter_number, int parameter_value)
{
    // FIXME: not implemented.;
    dtty_puts(__func__, 80);
    dtty_puts(" : fail (not implemented).\n", 80);
    bsp_abortsystem();
    return 0;
}

static void _ubik_tick_to_timeval(tickcount_t tick, struct timeval  *ptimeval) {
    unsigned long long timems;

    assert(ptimeval);

    timems = (unsigned long long) tick.high * UBINOS__UBIK__TICK_COUNT_MAX + tick.low;
    timems = timems * 1000 / UBINOS__UBIK__TICK_PER_SEC;

    ptimeval->tv_sec = (time_t) (timems / 1000);
    ptimeval->tv_usec = (suseconds_t) ((timems % 1000) * 1000);
}

int _gettimeofday (struct timeval  *ptimeval, void *ptimezone)
{
    int ret = -1;
    assert(ptimeval);

#if (INCLUDE__UBINOS__UBIK == 1)
    if (_bsp_kernel_active) {
        _ubik_tick_to_timeval(ubik_gettickcount(), ptimeval);

        ptimeval->tv_sec += __timeval_offset_plus.tv_sec;
        ptimeval->tv_sec -= __timeval_offset_minus.tv_sec;

        ptimeval->tv_usec += __timeval_offset_plus.tv_usec;
        if (ptimeval->tv_usec < __timeval_offset_minus.tv_usec) {
            ptimeval->tv_sec--;
            ptimeval->tv_usec += 1000000;
        }
        ptimeval->tv_usec -= __timeval_offset_minus.tv_usec;

        ret = 0;
    }
    else {
        errno = ENOSYS;
    }
#else
    errno = ENOSYS;
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

    return ret;
}

int settimeofday (const struct timeval *ptimeval, const struct timezone *ptimezone) {
    int ret = -1;
    assert(ptimeval);

#if (INCLUDE__UBINOS__UBIK == 1)
    struct timeval timeval;

    if (_bsp_kernel_active) {
        _ubik_tick_to_timeval(ubik_gettickcount(), &timeval);

        if (ptimeval->tv_sec > timeval.tv_sec) {
            __timeval_offset_plus.tv_sec = ptimeval->tv_sec - timeval.tv_sec;
            __timeval_offset_minus.tv_sec = 0;
        }
        else {
            __timeval_offset_plus.tv_sec = 0;
            __timeval_offset_minus.tv_sec = timeval.tv_sec - ptimeval->tv_sec;
        }

        if (ptimeval->tv_usec > timeval.tv_usec) {
            __timeval_offset_plus.tv_usec = ptimeval->tv_usec - timeval.tv_usec;
            __timeval_offset_minus.tv_usec = 0;
        }
        else {
            __timeval_offset_plus.tv_usec = 0;
            __timeval_offset_minus.tv_usec = timeval.tv_usec - ptimeval->tv_usec;
        }

        ret = 0;
    }
    else {
        errno = ENOSYS;
    }
#else
    errno = ENOSYS;
#endif /* (INCLUDE__UBINOS__UBIK == 1) */

    return ret;
}

#else /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

char * __heap_end = 0;

void * _sbrk (int incr)
{
   extern char   end;           /* Set by linker.  */
   extern char   __stack_limit; /* Set by linker.  */
   char *        prev_heap_end;

   if (__heap_end == 0) {
       __heap_end = &end;
   }

   if ((__heap_end + incr) > &__stack_limit) {
       prev_heap_end = (char *) ENOMEM;
   }
   else {
       prev_heap_end = __heap_end;
       __heap_end += incr;
   }

   return (void *) prev_heap_end;
}

#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

#if (UBINOS__UBICLIB__USE_STDIO_RETARGETING == 1)

int __attribute__((weak)) _write(int file, const char * p_char, int len)
{
    return dtty_putn(p_char, len);
}

int __attribute__((weak)) _read(int file, char * p_char, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        dtty_getc(&((char *)p_char)[i]);
    }

    return i;
}

#endif /* (UBINOS__UBICLIB__USE_STDIO_RETARGETING == 1) */

#endif /* !(UBINOS__UBICLIB__EXCLUDE_ARCH_INIT == 1) */
#endif /* !(UBINOS__UBICLIB__NOSTDLIB == 1) */
#endif /* (UBINOS__BSP__CPU_ARCH == UBINOS__BSP__CPU_ARCH__ARM) */
#endif /* (INCLUDE__UBINOS__UBICLIB == 1) */


