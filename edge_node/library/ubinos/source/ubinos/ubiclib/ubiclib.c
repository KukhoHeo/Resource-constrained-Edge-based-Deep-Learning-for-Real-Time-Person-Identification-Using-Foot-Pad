/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubiclib.h"

#if (INCLUDE__UBINOS__UBICLIB == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

#if (UBINOS__UBICLIB__USE_DUMMY_MKDIR == 1)

#include <sys/stat.h>

int	mkdir (const char *_path, mode_t __mode )
{
    return 0;
}

#endif /* (UBINOS__UBICLIB__USE_DUMMY_MKDIR == 1) */

#endif /* (INCLUDE__UBINOS__UBICLIB == 1) */

