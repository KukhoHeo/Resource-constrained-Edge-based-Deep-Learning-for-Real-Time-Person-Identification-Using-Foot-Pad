/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_heap.h"

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__HEAP

#define _UBINOS__UBICLIB__HEAP_DIR	0
#define _UBINOS__UBICLIB__HEAP_DIR_r	1

int _heap_n_bestfit_init_region(
		_heap_pt heap, unsigned int addr, unsigned int size, int locktype,
		unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			) {
	int r;
	_heap_region_pt region;
	unsigned int i;

	region						= &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	region->dir					= _UBINOS__UBICLIB__HEAP_DIR;
	region->algorithm			= UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT;
	region->locktype			= locktype;

	if (fblcount < 2) {
		r = -7;
		goto end0;
	}

	region->m					= 0;
	region->log2m				= 0;
	region->maskm				= 0;
	region->min					= HEAP_BLOCK_ASIZE_MIN;

	region->size_min			= region->min;
	region->size				= 0;
	region->k					= 0;
	region->w					= 0;
	region->t					= 0;
	region->addr				= addr;
	region->end					= addr;

	region->limit				= addr + size;

	if (region->size_min > size) {
		r = -4;
		goto end0;
	}

	for (i=0; i<fblcount; i++) {
		edlist_init(&fbl_p[i]);
		fbl_p[i].data = (void *) i;
	}
	region->fblcount			= fblcount;
	region->fbloffset	 		= 0;
	region->fbl_ap				= fbl_p;
	region->fblbm				= fblbm;

	region->dregs_size			= 0;

	edlist_init(&region->abl);

	region->acount_max			= 0;
	region->asize				= 0;
	region->asize_max			= 0;
	region->rsize				= 0;
	region->rsize_max			= 0;

	region->mutex 				= NULL;

	r = 0;

end0:
	return r;
}

_heap_block_pt _heap_n_bestfit_expand(_heap_pt heap, unsigned int asize) {
	_heap_region_pt region;
	unsigned int min;
	_heap_block_pt b1;
	unsigned int tag, addr, end;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, asize 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, asize);

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 >= asize) {
		b1 = NULL;
		goto end0;
	}

	min		 = region->min;

	asize = max(asize, min);

	addr = region->end;
	end  = addr + asize;
	if (end <= region->limit) {
		bsp_ubik_entercrit();
		if (end <= heap->region[_UBINOS__UBICLIB__HEAP_DIR_r].addr) {
			region->end   = end;
			region->size += asize;
		}
		else {
			addr = 0;
		}
		bsp_ubik_exitcrit();
	}
	else {
		addr = 0;
	}

	if (0 == addr) {
		b1 = NULL;
		goto end0;
	}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1)
	if (heap->enable_dmpm) {
		_heap_power_off_unused_area(heap->region[0].end, heap->region[1].addr);
#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM_MEMORY_READY_CHECK == 1)
		unsigned int * last_addr = (unsigned int * ) (heap->region[0].end - INT_SIZE);
		*last_addr = HEAP_BOUNDARY_PATTERN;
		if (*last_addr != HEAP_BOUNDARY_PATTERN) {
			logme("memory is not ready");
			bsp_abortsystem();
		}
		*last_addr = 0x0;
		if (*last_addr != 0x0) {
			logme("memory is not ready");
			bsp_abortsystem();
		}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM_MEMORY_READY_CHECK == 1) */
	}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1) */

	b1		 = (_heap_block_pt) addr;
	tag		 = _asize_to_tag_l(asize, 1, _UBINOS__UBICLIB__HEAP_DIR);
	_block_set_tag(b1, tag, 0);
	heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

	heap_logmfd("0x%08x:         : expanded: heap 0x%08x, dir %d, size 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, region->size);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

int _heap_n_bestfit_reduce(_heap_pt heap) {
	int r;
	_heap_region_pt region;
	unsigned int size_min;
	_heap_block_pt b1;
	unsigned int b1asize;
	unsigned int tag, addr, end, size;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR);

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 == region->size) {
		r = 0;
		goto end0;
	}

	size_min = region->size_min;

	addr 	 = region->addr;
	end  	 = region->end;
	size 	 = region->size;

	for (;;) {
		if (size_min >= size) {
			break;
		}

		b1  = _block_pt_to_upper_block_pt(end, 0);
		tag = b1->tag;
		if (1 != _tag_to_a(tag)) {
			break;
		}

		b1asize = _tag_to_asize(tag, 0);
		_region_remove_fb(region, 1, b1, 0);
		heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1, 0);
		heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

		end		-= b1asize;
		size	-= b1asize;

		break;
	}

	if (size == region->size) {
		r = 0;
		goto end0;
	}

	region->addr	 = addr;
	region->end		 = end;
	region->size	 = size;

	if (size_min > size) {
		b1 = _heap_n_bestfit_expand(heap, size_min - size);
		_heap_n_bestfit_combine_block(heap, b1, 1);
	}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1)
	if (heap->enable_dmpm) {
		_heap_power_off_unused_area(heap->region[0].end, heap->region[1].addr);
	}
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_DMPM == 1) */

	r = 0;

	heap_logmfd("0x%08x:         : reduced : heap 0x%08x, dir %d, size 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, region->size);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, result %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, r);

	return r;
}

_heap_block_pt _heap_n_bestfit_combine_block(_heap_pt heap, _heap_block_pt block, int endflag) {
	_heap_region_pt region;
	_heap_block_pt b1;
	_heap_block_pt b2;
	unsigned int tag, asize;
	unsigned int addr;

	b1 		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, endflag %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, endflag);

	region 	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

	for (;;) {
		tag		 = b1->tag;
		asize	 = _tag_to_asize(tag, 0);

		if (addr != (unsigned int) b1) {
			b2		 = _block_pt_to_upper_block_pt(b1, 0);
			tag		 = b2->tag;

			if (1 == _tag_to_a(tag)) {
				asize	+= _tag_to_asize(tag, 0);

				_region_remove_fb(region, 1, b2, 0);
				heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1, 0);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 0);

				b1 = b2;

				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);
				tag = _asize_to_tag_l(asize, 1, _UBINOS__UBICLIB__HEAP_DIR);
				_block_set_tag(b1, tag, 0);
				heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

				continue;
			}
		}

		if (1 == endflag || region->end == (unsigned int) _block_pt_to_end_prt(b1, 0)) {
			break;
		}

		b2		 = _block_pt_to_lower_block_pt(b1, 0);
		tag		 = b2->tag;

		if (1 == _tag_to_a(tag)) {
			asize	+= _tag_to_asize(tag, 0);

			_region_remove_fb(region, 1, b2, 0);
			heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1, 0);
			heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 0);

			heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);
			tag = _asize_to_tag_l(asize, 1, _UBINOS__UBICLIB__HEAP_DIR);
			_block_set_tag(b1, tag, 0);
			heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

			continue;
		}

		break;
	}

	_region_insert_fb_head(region, 1, b1, 0);

	heap_logmfd_block_inserted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1, 0);

	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

_heap_block_pt _heap_n_bestfit_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize) {
	_heap_region_pt region;
	unsigned int min;
	_heap_block_pt b1;
	_heap_block_pt b2;
	unsigned int bxasize;
	unsigned int tag;
	//unsigned int addr;

	b1		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, asize 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, asize);

	region   = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 >= asize) {
		goto end0;
	}

	min		 = region->min;

	asize = max(asize, min);

	//addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

	tag		 = b1->tag;
	bxasize	 = _tag_to_asize(tag, 0);
	if (asize >= bxasize) {
		goto end0;
	}

	for (;;) {
		bxasize -= asize;
		if (min > bxasize) {
			break;
		}

		heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);
		tag = _asize_to_tag_l(asize,  0, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b1, tag, 0);

		b2 = (_heap_block_pt) ((unsigned int) b1 + asize);
		tag = _asize_to_tag_l(bxasize, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b2, tag, 0);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 0);
		_heap_n_bestfit_combine_block(heap, b2, 0);

		tag = _asize_to_tag_l(asize, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b1, tag, 0);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

		break;
	}

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

void * _heap_n_bestfit_allocate_block(_heap_pt heap, unsigned int size) {
	int r;
	_heap_region_pt region;
	_heap_block_pt b1;
	unsigned int b1asize;
	unsigned int tag, tmp;

	_heap_block_pt bh, bs, bx;
	unsigned int bsasize, bxasize;

	b1 = NULL;

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	b1asize	 = _size_to_asize(size, region->min);

	if (heap->size < b1asize) {
		logmw("memory is not enough");
		goto end0;
	}

	switch(region->locktype) {
	case UBINOS__UBICLIB__HEAP_LOCK_TYPE__NONE:
		break;
	case UBINOS__UBICLIB__HEAP_LOCK_TYPE__MUTEX:
		if (NULL != region->mutex) {
			r = bsp_mutex_lock(region->mutex);
			if (0 != r) {
				logme("fail at mutex_lock");
				goto end0;
			}
		}
		break;
	case UBINOS__UBICLIB__HEAP_LOCK_TYPE__CRITICAL:
		bsp_ubik_entercrit();
		break;
	default:
		logme("locktype is wrong");
		goto end0;
	}

	/////////
	/////////
	switch (region->algorithm) {

	case UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT:
		bs = NULL;
		bsasize = UINT_MAX;

		bh = _region_get_fb_head(region, 1);
		bx = bh;

		for (;;) {
			if (NULL == bx) {
				break;
			}

			_block_check_boundary_and_abort(bx, 0);

			bxasize = _tag_to_asize(bx->tag, 0);
			if (b1asize <= bxasize && bsasize > bxasize) {
				bs = bx;
				bsasize = bxasize;
#if 1
				if (b1asize == bsasize) {
					break;
				}
#endif
			}

			bx = _heap_blocklist_next(bx);
			if (bh == bx) {
				break;
			}
		}

		b1 = bs;
		break;

	case UBINOS__UBICLIB__HEAP_ALGORITHM__FIRSTFIT:
		bs = NULL;

		bh = _region_get_fb_head(region, 1);
		bx = bh;

		for (;;) {
			if (NULL == bx) {
				break;
			}

			_block_check_boundary_and_abort(bx, 0);

			bxasize = _tag_to_asize(bx->tag, 0);
			if (b1asize <= bxasize) {
				bs = bx;
				break;
			}

			bx = _heap_blocklist_next(bx);
			if (bh == bx) {
				break;
			}
		}

		b1 = bs;
		break;

	case UBINOS__UBICLIB__HEAP_ALGORITHM__NEXTFIT:
		bs = NULL;

		bh = _region_get_fb_getcur(region, 1);
		bx = bh;

		for (;;) {
			if (NULL == bx) {
				break;
			}

			_block_check_boundary_and_abort(bx, 0);

			bxasize = _tag_to_asize(bx->tag, 0);
			if (b1asize <= bxasize) {
				bs = bx;
				break;
			}

			bx = _region_get_fb_getcurnext(region, 1);
			if (bh == bx) {
				break;
			}
		}

		b1 = bs;
		break;

	default:
		logme("algorithm is wrong");
		goto end1;

	}
	/////////
	/////////

	if (NULL == b1) {
		b1 = _heap_n_bestfit_expand(heap, b1asize);
	}
	else {
		_region_remove_fb(region, 1, b1, 0);
	}

	if (NULL != b1) {
		b1	= _heap_n_bestfit_split_block(heap, b1, b1asize);
	}

	if (NULL != b1) {
		tag = b1->tag;
		_tag_set_a(tag, 0);
		_block_set_tag(b1, tag, 0);
		_block_set_rsize(b1, size);
		b1asize	= _tag_to_asize(tag, 0);

		if (size > b1asize) {
			logme("b1asize is wrong");
			bsp_abortsystem();
		}

		_heap_blocklist_insertprev(&region->abl, NULL, b1);

		if (region->acount_max < region->abl.count) {
			region->acount_max = region->abl.count;
		}
		region->asize += b1asize;
		if (region->asize_max < region->asize) {
			region->asize_max = region->asize;
		}
		region->rsize += size;
		if (region->rsize_max < region->rsize) {
			region->rsize_max = region->rsize;
		}

		tmp = heap->region[0].abl.count + heap->region[1].abl.count;
		if (heap->acount_max < tmp) {
			heap->acount_max = tmp;
		}
		tmp = heap->region[0].asize + heap->region[1].asize;;
		if (heap->asize_max < tmp) {
			heap->asize_max = tmp;
		}
		tmp = heap->region[0].rsize + heap->region[1].rsize;;
		if (heap->rsize_max < tmp) {
			heap->rsize_max = tmp;
		}
	}

end1:
	switch(region->locktype) {
	case UBINOS__UBICLIB__HEAP_LOCK_TYPE__NONE:
		break;
	case UBINOS__UBICLIB__HEAP_LOCK_TYPE__MUTEX:
		if (NULL != region->mutex) {
			r = bsp_mutex_unlock(region->mutex);
			if (0 != r) {
				logme("fail at mutex_unlock");
			}
		}
		break;
	case UBINOS__UBICLIB__HEAP_LOCK_TYPE__CRITICAL:
		bsp_ubik_exitcrit();
		break;
	}

end0:
	if (NULL == b1) {
		tmp = 0;
	}
	else {
		heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

		tmp = (unsigned int) _block_pt_to_ptr(b1);
	}

	return (void *) tmp;
}

int _heap_n_bestfit_release_block(_heap_pt heap, void * ptr) {
	int r, r2;
	_heap_region_pt region;
	_heap_block_pt b1;
	unsigned int asize, rsize;
	unsigned int tag;

	b1 = _ptr_to_block_pt(ptr);

	region = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	_block_check_freeable_and_abort(b1);
	_block_check_boundary_and_abort(b1, 0);

	tag = b1->tag;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 0);

	asize = _tag_to_asize(tag, 0);
	rsize = b1->rsize;

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

	_heap_blocklist_remove(b1);

	_tag_set_a(tag, 1);
	_block_set_tag(b1, tag, 0);

	_heap_n_bestfit_combine_block(heap, b1, 0);

	region->asize -= asize;
	region->rsize -= rsize;

	_heap_n_bestfit_reduce(heap);

	r2 = 0;

//end1:
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

end0:
	return r2;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
