/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_heap.h"

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__HEAP

#if HEAP_BLOCK_ASIZE_MIN < 0
	#error "HEAP_BLOCK_ASIZE_MIN should be larger than 0"
#endif

_heap_pt _ubiclib_heap = NULL;

#if (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1)

_heap_t			__ubiclib_defaultheap;
edlist_t		__ubiclib_defaultheap_fbl0_a[UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT];
bitmap_t		__ubiclib_defaultheap_fblbm0;
unsigned char	__ubiclib_defaultheap_fblbm0_buf[UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE];
edlist_t		__ubiclib_defaultheap_fbl1_a[UBINOS__UBICLIB__HEAP_DIR1_FBLCOUNT];
bitmap_t		__ubiclib_defaultheap_fblbm1;
unsigned char	__ubiclib_defaultheap_fblbm1_buf[UBINOS__UBICLIB__HEAP_DIR1_FBLBM_BUFSIZE];

int ubiclib_heap_comp_init(void) {
	int r;
	int enable_dmpm = 0;

    extern char   __heap_base;  /* Set by linker.  */
    extern char   __heap_limit; /* Set by linker.  */
    unsigned int addr = (unsigned int) &__heap_base;
    unsigned int size = ((unsigned int) &__heap_limit) - addr;

    if (_ubiclib_heap != NULL) {
        logmw("already initialized");
        return 0;
    }

	if (HEAP_BLOCK_ASIZE_MIN <= HEAP_BLOCK_OVERHEAD) {
		logme("HEAP_BLOCK_ASIZE_MIN should be larger than HEAP_BLOCK_OVERHEAD");
		return -1;
	}

	if (0 >= size) {
		logme("size is 0");
		return -1;
	}

	r = bitmap_init(&__ubiclib_defaultheap_fblbm0, UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT, __ubiclib_defaultheap_fblbm0_buf, UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE);
	if (0 != r) {
		logme("fail at bitmap_init");
		return -1;
	}
	r = bitmap_init(&__ubiclib_defaultheap_fblbm1, UBINOS__UBICLIB__HEAP_DIR1_FBLCOUNT, __ubiclib_defaultheap_fblbm1_buf, UBINOS__UBICLIB__HEAP_DIR1_FBLBM_BUFSIZE);
	if (0 != r) {
		logme("fail at bitmap_init");
		return -1;
	}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1)
	enable_dmpm = 1;
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1) */

	r = _heap_init(	&__ubiclib_defaultheap, addr, size, enable_dmpm,
			UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM, UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE, UBINOS__UBICLIB__HEAP_DIR0_M,
			UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT, &__ubiclib_defaultheap_fbl0_a[0], &__ubiclib_defaultheap_fblbm0,
			UBINOS__UBICLIB__HEAP_DIR1_ALGORITHM, UBINOS__UBICLIB__HEAP_DIR1_LOCKTYPE, UBINOS__UBICLIB__HEAP_DIR1_M,
			UBINOS__UBICLIB__HEAP_DIR1_FBLCOUNT, &__ubiclib_defaultheap_fbl1_a[0], &__ubiclib_defaultheap_fblbm1	);
	if (0 != r) {
		logme("fail at _heap_init");
		return -1;
	}

	_ubiclib_heap = &__ubiclib_defaultheap;

	return 0;
}

int ubiclib_heap_comp_init_reent(void) {
	int r = 0;

	if (NULL == _ubiclib_heap) {
		logme("_ubiclib_heap is NULL");
		return -1;
	}

	if (OBJTYPE__UBICLIB_HEAP != _ubiclib_heap->type) {
		logme("_ubiclib_heap is not OBJTYPE__UBICLIB_HEAP");
		return -1;
	}

	if (1 != _ubiclib_heap->valid) {
		logme("_ubiclib_heap is not valid");
		return -1;
	}

	r = bsp_mutex_create(&_ubiclib_heap->region[0].mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		return -1;
	}

	r = bsp_mutex_create(&_ubiclib_heap->region[1].mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		r = bsp_mutex_delete(&_ubiclib_heap->region[0].mutex);
		return -1;
	}

	return 0;
}

void * mallocn(size_t size) {
	return _heap_allocate_block(_ubiclib_heap, 0, SIZETOUINT(size));
}

void * mallocr(size_t size) {
	return _heap_allocate_block(_ubiclib_heap, 1, SIZETOUINT(size));
}

#else /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

void * mallocn(size_t size) {
	return malloc(size);
}

void * mallocr(size_t size) {
	return malloc(size);
}

#endif /* (UBINOS__UBICLIB__USE_MALLOC_RETARGETING == 1) */

int _heap_init(	_heap_pt heap, unsigned int addr, unsigned int size, int enable_dmpm,
				int algorithm0, int locktype0, unsigned int m0,
				unsigned int fblcount0, edlist_pt fbl0_p, bitmap_pt fblbm0,
				int algorithm1, int locktype1, unsigned int m1,
				unsigned int fblcount1, edlist_pt fbl1_p, bitmap_pt fblbm1	)	{
	int r;
	//unsigned int i;
	//unsigned int log2m;
	//unsigned int k, w, t;

	if (NULL == heap) {
		r = -2;
		goto end0;
	}
	if (NULL == (char *) addr) {
		r = -3;
		goto end0;
	}
	if (HEAP_BLOCK_ASIZE_MIN > size) {
		r = -4;
		goto end0;
	}
	if (HEAP_G_M_MAX < m0 || uienlp2(m0) != m0) {
		r = -7;
		goto end0;
	}
	if (0 >= fblcount0) {
		r = -8;
		goto end0;
	}
	if (NULL == fbl0_p) {
		r = -9;
		goto end0;
	}
	if (NULL == fblbm0) {
		r = -10;
		goto end0;
	}
	if (HEAP_G_M_MAX < m1 || uienlp2(m1) != m1) {
		r = -13;
		goto end0;
	}
	if (0 >= fblcount1) {
		r = -14;
		goto end0;
	}
	if (NULL == fbl1_p) {
		r = -15;
		goto end0;
	}
	if (NULL == fblbm1) {
		r = -16;
		goto end0;
	}

	heap->type						= OBJTYPE__UBICLIB_HEAP;
	heap->valid						= 0;
	heap->reserved					= 0;
	heap->reserved2					= 0;

	heap->size						= size;
	heap->addr						= addr;
	heap->end						= addr + size;

	heap->acount_max				= 0;
	heap->asize_max					= 0;
	heap->rsize_max					= 0;

	heap->enable_dmpm				= enable_dmpm;

	switch (algorithm0) {

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP:
		r = _heap_n_group_init_region(heap, addr, size, locktype0, m0, fblcount0, fbl0_p, fblbm0);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[0] = *_heap_n_group_allocate_block;
		heap->release_block_afp[0]  = *_heap_n_group_release_block;
		heap->region[0].algorithm 	= algorithm0;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT:
		r = _heap_n_bestfit_init_region(heap, addr, size, locktype0, m0, fblcount0, fbl0_p, fblbm0);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[0] = *_heap_n_bestfit_allocate_block;
		heap->release_block_afp[0]  = *_heap_n_bestfit_release_block;
		heap->region[0].algorithm 	= algorithm0;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY:
		r = _heap_n_pgroup_init_region(heap, addr, size, locktype0, m0, fblcount0, fbl0_p, fblbm0);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[0] = *_heap_n_pgroup_allocate_block;
		heap->release_block_afp[0]  = *_heap_n_pgroup_release_block;
		heap->region[0].algorithm 	= algorithm0;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY:
		r = _heap_n_wbuddy_init_region(heap, addr, size, locktype0, m0, fblcount0, fbl0_p, fblbm0);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[0] = *_heap_n_wbuddy_allocate_block;
		heap->release_block_afp[0]  = *_heap_n_wbuddy_release_block;
		heap->region[0].algorithm 	= algorithm0;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */

	default:
		r = -5;
		goto end0;

	}

	switch (algorithm1) {

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP:
		r = _heap_r_group_init_region(heap, addr, size, locktype1, m1, fblcount1, fbl1_p, fblbm1);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[1] = *_heap_r_group_allocate_block;
		heap->release_block_afp[1]  = *_heap_r_group_release_block;
		heap->region[1].algorithm 	= algorithm1;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT:
		r = _heap_r_bestfit_init_region(heap, addr, size, locktype1, m1, fblcount1, fbl1_p, fblbm1);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[1] = *_heap_r_bestfit_allocate_block;
		heap->release_block_afp[1]  = *_heap_r_bestfit_release_block;
		heap->region[1].algorithm 	= algorithm1;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY:
		r = _heap_r_pgroup_init_region(heap, addr, size, locktype1, m1, fblcount1, fbl1_p, fblbm1);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[1] = *_heap_r_pgroup_allocate_block;
		heap->release_block_afp[1]  = *_heap_r_pgroup_release_block;
		heap->region[1].algorithm 	= algorithm1;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY:
		r = _heap_r_wbuddy_init_region(heap, addr, size, locktype1, m1, fblcount1, fbl1_p, fblbm1);
		if (0 != r) {
			goto end0;
		}
		heap->allocate_block_afp[1] = *_heap_r_wbuddy_allocate_block;
		heap->release_block_afp[1]  = *_heap_r_wbuddy_release_block;
		heap->region[1].algorithm 	= algorithm1;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */

	default:
		r = -11;
		goto end0;

	}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1)
	if (heap->enable_dmpm) {
		_heap_power_off_unused_area(heap->region[0].end, heap->region[1].addr);
	}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1) */

	heap->valid						= 1;

	r = 0;

end0:
	return r;
}


void * _heap_allocate_block(_heap_pt heap, int dir, unsigned int size) {
	register void * ptr = NULL;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, size 0x%08x ", bsp_task_getcur(), heap, dir, size);

	if (NULL == heap) {
		logme("heap is NULL");
		goto end0;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		goto end0;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		goto end0;
	}

	if (0 >= size) {
		logme("size is wrong");
		goto end0;
	}

	ptr = heap->allocate_block_afp[dir](heap, size);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, ptr 0x%08x", bsp_task_getcur(), heap, dir, ptr);

	return ptr;
}

int _heap_release_block(_heap_pt heap, void * ptr) {
	register int r2 = 0;
	register int dir = 0;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	heap_logmfd("0x%08x: called  : heap 0x%08x, ptr 0x%08x ", bsp_task_getcur(), heap, ptr);

	if (NULL == heap) {
		logme("heap is NULL");
		r2 = -2;
		goto end0;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		r2 = -2;
		goto end0;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		r2 = -2;
		goto end0;
	}

	if (NULL == ptr) {
		logmi("ptr is NULL");
		r2 = -3;
		goto end0;
	}

	dir = _tag_to_d(_ptr_to_block_pt(ptr)->tag);
	if (0 != dir && 1 != dir) {
		logme("ptr is wrong");
		r2 = -3;
		goto end0;
	}

	r2 = heap->release_block_afp[dir](heap, ptr);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, result %d", bsp_task_getcur(), heap, dir, r2);

	return r2;
}


int heap_create(heap_pt * heap_p, unsigned int addr, unsigned int size) {
	return heap_create_ext(	heap_p, addr, size,
							UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM, UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE, UBINOS__UBICLIB__HEAP_DIR0_M, UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT,
							UBINOS__UBICLIB__HEAP_DIR1_ALGORITHM, UBINOS__UBICLIB__HEAP_DIR1_LOCKTYPE, UBINOS__UBICLIB__HEAP_DIR1_M, UBINOS__UBICLIB__HEAP_DIR1_FBLCOUNT	);
}

int heap_create_ext(heap_pt * heap_p, unsigned int addr, unsigned int size,
					int algorithm0, int locktype0, unsigned int m0, unsigned int fblcount0,
					int algorithm1, int locktype1, unsigned int m1, unsigned int fblcount1	) {
	int r;
	_heap_pt heap;
	edlist_pt fbl0_p = NULL;
	edlist_pt fbl1_p = NULL;
	bitmap_pt fblbm0 = NULL;
	bitmap_pt fblbm1 = NULL;

	if (NULL == heap_p) {
		logme("heap_p is NULL");
		return -2;
	}

	if (UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY == algorithm0) {
		m0 = 2;
	}

	switch (algorithm0) {

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP:
		fblcount0 = heap_group_calc_fblcount(size, m0);
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT:
		fblcount0 = 2;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY:
		fblcount0 = heap_pgroup_calc_fblcount(size, m0);
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY:
		fblcount0 = heap_wbuddy_calc_fblcount(size, m0);
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */

	default:
		break;
	}

	if (UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY == algorithm1) {
		m1 = 2;
	}

	switch (algorithm1) {

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP:
		fblcount1 = heap_group_calc_fblcount(size, m1);
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT:
		fblcount1 = 2;
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP:
	case UBINOS__UBICLIB__HEAP_ALGORITHM__BBUDDY:
		fblcount1 = heap_pgroup_calc_fblcount(size, m1);
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */

	#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)

	case UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY:
		fblcount1 = heap_wbuddy_calc_fblcount(size, m1);
		break;

	#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */

	default:
		break;
	}

	heap = malloc(sizeof(_heap_t));
	fbl0_p = malloc(sizeof(edlist_t) * fblcount0);
	fbl1_p = malloc(sizeof(edlist_t) * fblcount1);
	if (NULL == heap || NULL == fbl0_p || NULL == fbl1_p) {
		logme("fail at malloc");
		free(fbl0_p);
		free(fbl1_p);
		free(heap);
		return -1;
	}

	bitmap_create(&fblbm0, fblcount0);
	bitmap_create(&fblbm1, fblcount1);
	if (NULL == fblbm0 || NULL == fblbm1) {
		logme("fail at bitmap_create");
		bitmap_delete(&fblbm0);
		bitmap_delete(&fblbm1);
		free(fbl0_p);
		free(fbl1_p);
		free(heap);
		return -1;
	}

	r = _heap_init(	heap, addr, size, 0,
					algorithm0, locktype0, m0, fblcount0, fbl0_p, fblbm0,
					algorithm1, locktype1, m1, fblcount1, fbl1_p, fblbm1	);
	if (0 != r) {
		logme("fail at heap_init");
		bitmap_delete(&fblbm0);
		bitmap_delete(&fblbm1);
		free(fbl0_p);
		free(fbl1_p);
		free(heap);
		return -1;
	}

	r = bsp_mutex_create(&heap->region[0].mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		bitmap_delete(&fblbm0);
		bitmap_delete(&fblbm1);
		free(fbl0_p);
		free(fbl1_p);
		free(heap);
		return -1;
	}

	r = bsp_mutex_create(&heap->region[1].mutex);
	if (0 != r) {
		logme("fail at mutex_create");
		r = bsp_mutex_delete(&heap->region[0].mutex);
		bitmap_delete(&fblbm0);
		bitmap_delete(&fblbm1);
		free(fbl0_p);
		free(fbl1_p);
		free(heap);
		return -1;
	}

	*heap_p = (heap_pt) heap;

	heap_logmfd("heap was created: heap 0x%08x, size 0x%08x, addr %d", heap, heap->size, heap->addr);

	return 0;
}

int heap_delete(heap_pt * heap_p) {
	int r, r2;
	_heap_pt heap;

	if (NULL == heap_p) {
		logme("heap_p is NULL");
		return -2;
	}

	heap = (_heap_pt) *heap_p;

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (_ubiclib_heap == heap) {
		logme("can not delete default heap");
		return -2;
	}

	if (NULL != heap->region[0].mutex) {
		r = bsp_mutex_lock(heap->region[0].mutex);
		if (0 != r) {
			logme("fail at mutex_lock");
			return -1;
		}
	}
	if (NULL != heap->region[1].mutex) {
		r = bsp_mutex_lock(heap->region[1].mutex);
		if (0 != r) {
			logme("fail at mutex_lock");
			r = bsp_mutex_unlock(heap->region[0].mutex);
			return -1;
		}
	}

	r2 = 0;

	heap_checkblockboundaryall((heap_pt) heap);

	heap->valid		= 0;
	heap->type		= 0;

	*heap_p = NULL;

	if (NULL != heap->region[0].mutex) {
		r = bsp_mutex_delete(&heap->region[0].mutex);
		if (0 != r) {
			logme("fail at mutex_delete");
			r2 = -1;
		}
	}

	if (NULL != heap->region[1].mutex) {
		r = bsp_mutex_delete(&heap->region[1].mutex);
		if (0 != r) {
			logme("fail at mutex_delete");
			r2 = -1;
		}
	}

	bitmap_delete(&heap->region[0].fblbm);
	bitmap_delete(&heap->region[1].fblbm);
	free(heap->region[0].fbl_ap);
	free(heap->region[1].fbl_ap);
	free(heap);

	heap_logmfd("heap was deleted: heap 0x%08x, size 0x%08x, addr %d", heap, heap->size, heap->addr);

	return r2;
}

void * heap_malloc(heap_pt heap, unsigned int size) {
	if (0 == bsp_ubik_isrt()) {
		return _heap_allocate_block((_heap_pt) heap, 0, size);
	}
	else {
		return _heap_allocate_block((_heap_pt) heap, 1, size);
	}
}

void * heap_mallocn(heap_pt heap, unsigned int size) {
	return _heap_allocate_block((_heap_pt) heap, 0, size);
}

void * heap_mallocr(heap_pt heap, unsigned int size) {
	return _heap_allocate_block((_heap_pt) heap, 1, size);
}

int heap_free(heap_pt heap, void * ptr) {
	return _heap_release_block((_heap_pt) heap, ptr);
}

int heap_checkblockboundary(heap_pt _heap, void * ptr) {
	_heap_pt heap = (_heap_pt) _heap;

	int r;
	_heap_block_pt block;
	unsigned int log2m;

	(void) log2m;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == ptr) {
		logme("ptr is NULL");
		return -2;
	}

	block = _ptr_to_block_pt(ptr);
	log2m = heap->region[_tag_to_d(block->tag)].log2m;

	if 		(!_check_top_boundary(block)) {
		r = HEAP_ERR__POLLUTED_TOP;
	}
	else if	(!_check_bottom_boundary(block, log2m)) {
		r = HEAP_ERR__POLLUTED_BOTTOM;
	}
	else if	(!_check_header_boundary(block)) {
		r = HEAP_ERR__POLLUTED_HEADER;
	}
	else {
		r = 0;
	}

	return r;
}

int heap_checkblockboundaryall(heap_pt _heap) {
	_heap_pt heap = (_heap_pt) _heap;

	int r, r2;
	unsigned int i, j;
	_heap_region_pt region;
	_heap_block_pt block;
	unsigned int log2m;

	(void) log2m;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		r2 = -2;
		goto end0;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		r2 = -2;
		goto end0;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		r2 = -2;
		goto end0;
	}

	r2 = 0;

	for (i=0; i<2; i++) {
		region = &heap->region[i];
		log2m = region->log2m;

		if (0 == bsp_ubik_iscrit()) {
			switch(region->locktype) {
			case UBINOS__UBICLIB__HEAP_LOCK_TYPE__NONE:
				break;
			case UBINOS__UBICLIB__HEAP_LOCK_TYPE__MUTEX:
				if (NULL != region->mutex) {
					r = bsp_mutex_lock(region->mutex);
					if (0 != r) {
						logme("fail at mutex_lock");
						r2 = -2;
						goto end0;
					}
				}
				break;
			case UBINOS__UBICLIB__HEAP_LOCK_TYPE__CRITICAL:
				bsp_ubik_entercrit();
				break;
			default:
				logme("locktype is wrong");
				r2 = -2;
				goto end0;
			}
		}

		if (1 != heap->valid) {
			logme("heap is not valid");
			r2 = -2;
			goto end1;
		}

		block = _heap_blocklist_head(&region->abl);
		for (;;) {
			if (NULL == block) {
				break;
			}
			_block_check_boundary_and_abort(block, log2m);
			block = _heap_blocklist_next(block);
		}

		for (j=0; j<region->fblcount; j++) {
			block = _heap_blocklist_head(&region->fbl_ap[j]);
			for (;;) {
				if (NULL == block) {
					break;
				}
				_block_check_boundary_and_abort(block, log2m);
				block = _heap_blocklist_next(block);
			}
		}

end1:
		if (0 == bsp_ubik_iscrit()) {
			switch(region->locktype) {
			case UBINOS__UBICLIB__HEAP_LOCK_TYPE__NONE:
				break;
			case UBINOS__UBICLIB__HEAP_LOCK_TYPE__MUTEX:
				if (NULL != region->mutex) {
					r = bsp_mutex_unlock(region->mutex);
					if (0 != r) {
						logme("fail at mutex_unlock");
						r2 = -1;
					}
				}
				break;
			case UBINOS__UBICLIB__HEAP_LOCK_TYPE__CRITICAL:
				bsp_ubik_exitcrit();
				break;
			}
		}

		if (0 != r2) {
			break;
		}
	}

end0:
	return r2;
}

int heap_getblocksize(heap_pt _heap, void * ptr, unsigned int * size_p) {
	_heap_pt heap = (_heap_pt) _heap;

	_heap_block_pt block;
	unsigned int log2m;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == ptr) {
		logme("ptr is NULL");
		return -2;
	}

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	block = _ptr_to_block_pt(ptr);
	log2m = heap->region[_tag_to_d(block->tag)].log2m;

	_block_check_boundary_and_abort(block, log2m);

	*size_p = _tag_to_size(block->tag, log2m);

	return 0;
}

int heap_getsize(heap_pt _heap, unsigned int * size_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = heap->size;

	return 0;
}

int heap_getrequestedsize(heap_pt _heap, unsigned int * size_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = (heap->region[0].rsize + heap->region[1].rsize);

	return 0;
}

int heap_getrequestedsize_ext(heap_pt _heap, unsigned int * size_p, unsigned int * nsize_p, unsigned int * rsize_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL != size_p) {
		*size_p = (heap->region[0].rsize + heap->region[1].rsize);
	}

	if (NULL != nsize_p) {
		*nsize_p = heap->region[0].rsize;
	}

	if (NULL != rsize_p) {
		*rsize_p = heap->region[1].rsize;
	}

	return 0;
}

int heap_getrequestedsizemax(heap_pt _heap, unsigned int * size_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = heap->rsize_max;

	return 0;
}

int heap_getrequestedsizemax_ext(heap_pt _heap, unsigned int * size_p, unsigned int * nsize_p, unsigned int * rsize_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL != size_p) {
		*size_p = heap->rsize_max;
	}

	if (NULL != nsize_p) {
		*nsize_p = heap->region[0].rsize_max;
	}

	if (NULL != rsize_p) {
		*rsize_p = heap->region[1].rsize_max;
	}

	return 0;
}

int heap_getallocatedcount(heap_pt _heap, unsigned int * count_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == count_p) {
		logme("count_p is NULL");
		return -3;
	}

	*count_p = (heap->region[0].abl.count + heap->region[1].abl.count);

	return 0;
}

int heap_getallocatedcount_ext(heap_pt _heap, unsigned int * count_p, unsigned int * ncount_p, unsigned int * rcount_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL != count_p) {
		*count_p = (heap->region[0].abl.count + heap->region[1].abl.count);
	}

	if (NULL != ncount_p) {
		*ncount_p = heap->region[0].abl.count;
	}

	if (NULL != rcount_p) {
		*rcount_p = heap->region[1].abl.count;
	}

	return 0;
}

int heap_getallocatedcountmax(heap_pt _heap, unsigned int * count_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == count_p) {
		logme("count_p is NULL");
		return -3;
	}

	*count_p = heap->acount_max;

	return 0;
}

int heap_getallocatedcountmax_ext(heap_pt _heap, unsigned int * count_p, unsigned int * ncount_p, unsigned int * rcount_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL != count_p) {
		*count_p = heap->acount_max;
	}

	if (NULL != ncount_p) {
		*ncount_p = heap->region[0].acount_max;
	}

	if (NULL != rcount_p) {
		*rcount_p = heap->region[1].acount_max;
	}

	return 0;
}

int heap_getallocatedsize(heap_pt _heap, unsigned int * size_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = (heap->region[0].asize + heap->region[1].asize);

	return 0;
}

int heap_getallocatedsize_ext(heap_pt _heap, unsigned int * size_p, unsigned int * nsize_p, unsigned int * rsize_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL != size_p) {
		*size_p = (heap->region[0].asize + heap->region[1].asize);
	}

	if (NULL != nsize_p) {
		*nsize_p = heap->region[0].asize;
	}

	if (NULL != rsize_p) {
		*rsize_p = heap->region[1].asize;
	}

	return 0;
}

int heap_getallocatedsizemax(heap_pt _heap, unsigned int * size_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == size_p) {
		logme("size_p is NULL");
		return -3;
	}

	*size_p = heap->asize_max;

	return 0;
}

int heap_getallocatedsizemax_ext(heap_pt _heap, unsigned int * size_p, unsigned int * nsize_p, unsigned int * rsize_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL != size_p) {
		*size_p = heap->asize_max;
	}

	if (NULL != nsize_p) {
		*nsize_p = heap->region[0].asize_max;
	}

	if (NULL != rsize_p) {
		*rsize_p = heap->region[1].asize_max;
	}

	return 0;
}

int heap_getfreeblockcount(heap_pt _heap, unsigned int * count_p) {
	_heap_pt heap = (_heap_pt) _heap;

	//edlist_pt fbl;
	unsigned int i;
	unsigned int count;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	count = 0;

	for (i=0; i<heap->region[0].fblcount; i++) {
		count += heap->region[0].fbl_ap[i].count;
	}

	for (i=0; i<heap->region[1].fblcount; i++) {
		count += heap->region[1].fbl_ap[i].count;
	}

	if (NULL != count_p) {
		*count_p = count;
	}

	return 0;
}

int heap_getfreeblockcount_ext(heap_pt _heap, unsigned int * count_p, unsigned int * ncount_p, unsigned int * rcount_p) {
	_heap_pt heap = (_heap_pt) _heap;

	//edlist_pt fbl;
	unsigned int i;
	unsigned int ncount, rcount;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	for (i=0, ncount = 0; i<heap->region[0].fblcount; i++) {
		ncount += heap->region[0].fbl_ap[i].count;
	}

	for (i=0, rcount = 0; i<heap->region[1].fblcount; i++) {
		rcount += heap->region[1].fbl_ap[i].count;
	}

	if (NULL != count_p) {
		*count_p = ncount + rcount;
	}

	if (NULL != ncount_p) {
		*ncount_p = ncount;
	}

	if (NULL != rcount_p) {
		*rcount_p = rcount;
	}

	return 0;
}

int heap_getblockoverhead(heap_pt _heap, unsigned int * overhead_p) {
	_heap_pt heap = (_heap_pt) _heap;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		return -2;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		return -2;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		return -2;
	}

	if (NULL == overhead_p) {
		logme("overhead_p is NULL");
		return -3;
	}

	*overhead_p = HEAP_BLOCK_OVERHEAD;

	return 0;
}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_PRINTHEAPINFO == 1)

int heap_printheapinfo(heap_pt _heap) {
	_heap_pt heap = (_heap_pt) _heap;

	int r;
	unsigned int boh;
	unsigned int size;
	unsigned int count;
	unsigned int nsize;
	unsigned int ncount;
	unsigned int rsize;
	unsigned int rcount;

	if (NULL == heap) {
		heap = _ubiclib_heap;
	}

	if (NULL == heap) {
		logme("heap is NULL");
		r = -2;
		goto end0;
	}

	if (OBJTYPE__UBICLIB_HEAP != heap->type) {
		logme("heap type is not OBJTYPE__UBICLIB_HEAP");
		r = -2;
		goto end0;
	}

	if (1 != heap->valid) {
		logme("heap is not valid");
		r = -2;
		goto end0;
	}

	r = heap_getsize((heap_pt) heap, &size);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("heap size                    : 0x%08x (%8d bytes)\n", size, size);

	r = heap_getblockoverhead((heap_pt) heap, &boh);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("block overhead               : 0x%08x (%8d bytes)\n", boh, boh);

	printf("\n");

	r = heap_getrequestedsize_ext((heap_pt) heap, &size, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("requested size      (total  ): 0x%08x (%8d bytes)\n", size, size);
	printf("                    (normal ): 0x%08x (%8d bytes)\n", nsize, nsize);
	printf("                    (reverse): 0x%08x (%8d bytes)\n", rsize, rsize);

	r = heap_getallocatedcount_ext((heap_pt) heap, &count, &ncount, &rcount);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("allocated count     (total  ): 0x%08x (%8d      )\n", count, count);
	printf("                    (normal ): 0x%08x (%8d      )\n", ncount, ncount);
	printf("                    (reverse): 0x%08x (%8d      )\n", rcount, rcount);

	r = heap_getallocatedsize_ext((heap_pt) heap, &size, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("allocated size      (total  ): 0x%08x (%8d bytes)\n", size, size);
	printf("                    (normal ): 0x%08x (%8d bytes)\n", nsize, nsize);
	printf("                    (reverse): 0x%08x (%8d bytes)\n", rsize, rsize);

	size = size + (count * boh);
	printf("ram usage           (dynamic): 0x%08x (%8d bytes)\n", size, size);

	printf("\n");

	r = heap_getrequestedsizemax_ext((heap_pt) heap, &size, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("requested size max  (total  ): 0x%08x (%8d bytes)\n", size, size);
	printf("                    (normal ): 0x%08x (%8d bytes)\n", nsize, nsize);
	printf("                    (reverse): 0x%08x (%8d bytes)\n", rsize, rsize);

	r = heap_getallocatedcountmax_ext((heap_pt) heap, &count, &ncount, &rcount);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("allocated count max (total  ): 0x%08x (%8d      )\n", count, count);
	printf("                    (normal ): 0x%08x (%8d      )\n", ncount, ncount);
	printf("                    (reverse): 0x%08x (%8d      )\n", rcount, rcount);

	r = heap_getallocatedsizemax_ext((heap_pt) heap, &size, &nsize, &rsize);
	if (0 != r) {
		r = -1;
		goto end0;
	}
	printf("allocated size max  (total  ): 0x%08x (%8d bytes)\n", size, size);
	printf("                    (normal ): 0x%08x (%8d bytes)\n", nsize, nsize);
	printf("                    (reverse): 0x%08x (%8d bytes)\n", rsize, rsize);

	size = size + (count * boh);
	printf("ram usage max       (dynamic): 0x%08x (%8d bytes)\n", size, size);

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1)
	if (heap->enable_dmpm == 1) {
		printf("\n");
		printf("power :");
		_heap_print_power_info();
		printf("\n");
	}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1) */

	///////////////////////
		/////////////////////
	#define _print_block(heap, dir, block, log2m)																										\
		printf("heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u, <%4u>, (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n", 		\
		        (unsigned int) heap, dir, (unsigned int) block, (unsigned int)_block_pt_to_ptr(block), (unsigned int)_block_pt_to_end_prt(block, log2m), (unsigned int)_tag_to_asize(block->tag, log2m),			\
				_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag)&0x3,																	\
				_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag),										\
				(((unsigned int) _block_pt_to_ptr(block) - (unsigned int) heap->region[0].addr) >> _tag_to_g_k(block->tag)) & 0x3, 						\
				 (unsigned int) (block)                            - (unsigned int) heap->region[0].addr, \
				 (unsigned int) _block_pt_to_end_prt(block, log2m) - (unsigned int) heap->region[0].addr, \
				((unsigned int) (block)                            - (unsigned int) heap->region[0].addr) % (0x1 << (_tag_to_g_k(block->tag) + 2)), \
				((unsigned int) _block_pt_to_end_prt(block, log2m) - (unsigned int) heap->region[0].addr) % (0x1 << (_tag_to_g_k(block->tag) + 2))  \
		)

	for (int dir = 0; dir < 2; dir++) {
		unsigned int i;
		_heap_region_pt region;
		_heap_block_pt bx = 0;
		edlist_pt fbl;
		unsigned int log2m/*, m, maskm, min*/;

		printf("\n");
		region 	 = &heap->region[dir];
		log2m	 = region->log2m;

		printf("---------\n");
		printf("dir %d\n", region->dir);
		printf("algorithm %d\n", region->algorithm);
		printf("m %d\n", region->m);
		printf("fbl count %d\n", region->fblcount);
		printf("fbl offset %d\n", region->fbloffset);
		printf("fbl size %d\n", sizeof(edlist_t) * region->fblcount);
		printf("fbl bitmap bit size %d\n", region->fblbm->bitsize);
		printf("fbl bitmap memory size %d bytes\n", bitmap_getmemsize(region->fblbm->bitsize));
		printf("fbl bitmap buffer size %d bytes\n", region->fblbm->map_bytesize);
		printf("fbl count %d\n", region->fblcount);
		printf("---------\n");
		printf("size_min 0x%08x (%10u)\n", region->size_min, region->size_min);
		printf("size     0x%08x (%10u)\n", region->size, region->size);
		printf("addr     0x%08x (%10u)\n", region->addr, region->addr);
		printf("end      0x%08x (%10u)\n", region->end, region->end);
		printf("limit    0x%08x (%10u)\n", region->limit, region->limit);
		printf("---------\n");

		bx = _heap_blocklist_head(&(region->abl));
		while(bx != NULL) {
			_print_block(heap, 0, bx, log2m);
			bx = _heap_blocklist_next(bx);
		}

		printf("---------\n");
		for (i=0; i<region->fblcount; i++) {
			fbl = &region->fbl_ap[i];
			if (fbl->count != 0) {
				printf("region 0 fbl %4d count: %4u\n", i, fbl->count);
				bx = _heap_blocklist_head(fbl);
				while(bx != NULL) {
					_print_block(heap, 0, bx, log2m);
					bx = _heap_blocklist_next(bx);
				}
			}
		}
		printf("---------\n");
	}
		/////////////////////
	///////////////////////

	r = 0;

end0:
	return r;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_PRINTHEAPINFO == 1) */

#else /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */

int ubiclib_heap_comp_init(void) {
	return HEAP_ERR__UNSUPPORTED;
}

int ubiclib_heap_comp_init_reent(void) {
	return HEAP_ERR__UNSUPPORTED;
}

int heap_printheapinfo(heap_pt _heap) {
	return HEAP_ERR__UNSUPPORTED;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
