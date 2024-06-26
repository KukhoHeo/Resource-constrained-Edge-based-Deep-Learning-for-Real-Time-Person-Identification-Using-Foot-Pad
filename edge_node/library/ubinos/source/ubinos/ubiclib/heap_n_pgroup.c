/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_heap.h"

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__HEAP

#define _UBINOS__UBICLIB__HEAP_DIR	0
#define _UBINOS__UBICLIB__HEAP_DIR_r	1

#define _kw_to_tag(k, w, a, d)					_kblr_to_tag(k, w, 0, 0, a, d)

#define _kw_check(k, w, log2m, m)				_kwt_check(k, w, 0, log2m, m)

#define _kw_refine(k, w, log2m, m) {																			\
	if ((m) == (w)) {																							\
		(k) += (1);																								\
		(w)  = (1);																								\
	}																											\
	_kw_check(k, w, log2m, m);																					\
}

#define _asize_to_kw(asize, k, w, log2m, m)	{																	\
	(k) = uilog2_floor(asize) / (log2m);																		\
	(w) = div_ceil(asize, (unsigned int) 0x1 << ((log2m) * (k)));												\
	if ((m) == (w)) {																							\
		(k) += (1);																									\
		(w)  = (1);																								\
	}																											\
	_kw_check(k, w, log2m, m);																					\
}

#define _kw_to_asize(k, w, log2m)				_kwt_to_asize(k, w, 0, log2m)

#define _kw_to_asize_r(k, w, log2m, m)			_kwt_to_asize_r(k, w, 0, log2m, m)

#define _kw_to_sn(k, w, log2m, m) 				(((unsigned int) (m)-1) * (k) + (w))

#define _tag_to_kw(tag, k, w, log2m, m)	{																		\
		(k) = _tag_to_g_k(tag);																					\
		(w) = _tag_to_g_b(tag);																					\
		_kw_check(k, w, log2m, m);																				\
}

#define _block_get_gid__pgroup(block, region, k, b, gid, log2m)	{												\
	if (0 == (region)->dir) {                                                                           		\
		gid = ((unsigned int) (block) - (region)->addr   ) >> ((log2m) * ((k) + 1));							\
	}                                                                                                   		\
	else {                                                                                              		\
		gid = ((region)->end - (unsigned int) (block) - 1) >> ((log2m) * ((k) + 1));     						\
	}                                                                                                   		\
}


int _heap_n_pgroup_init_region(
		_heap_pt heap, unsigned int addr, unsigned int size, int locktype,
		unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			) {
	int r;
	unsigned int log2m;
	unsigned int k, w;
	_heap_region_pt region;
	unsigned int i;

	region						= &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	region->dir					= _UBINOS__UBICLIB__HEAP_DIR;
	region->algorithm			= UBINOS__UBICLIB__HEAP_ALGORITHM__PGROUP;
	region->locktype			= locktype;

	log2m = uilog2_floor(m);

	_asize_to_kw(size, k, w, log2m, m);
	if (k < 1 || HEAP_G_K_MAX < k) {
		r = -4;
		goto end0;
	}
	if (fblcount < heap_pgroup_calc_fblcount(size, m)) {
		r = -7;
		goto end0;
	}

	region->m					= m;
	region->log2m				= log2m;
	region->maskm				= ~((unsigned int) (-1) << log2m);
	_asize_to_kw(HEAP_BLOCK_ASIZE_MIN, k, w, log2m, m);
	region->min					= _kw_to_asize(k, w, log2m);

	region->size_min			= _kw_to_asize(k + 2, 1, log2m);
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
	region->fbloffset	 		= _kw_to_sn(k, w, log2m, m) - 1;
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

_heap_block_pt _heap_n_pgroup_expand(_heap_pt heap, unsigned int asize) {
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm*/, min;
	unsigned int p1k, p1w_r;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1c, b1asize, b1div;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2c, b2asize;
	unsigned int bxk, bxw;
	unsigned int tag, addr, end;
	unsigned int i;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, asize 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, asize);

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 >= asize) {
		b1 = NULL;
		goto end0;
	}

	m		 = region->m;
	log2m	 = region->log2m;
	//maskm	 = region->maskm;
	min		 = region->min;

	asize = max(asize, min);

	if (0 == region->size) {
		p1k		 = 0;
		p1w_r	 = 0;

		b1div	 = 0;
		b1asize	 = max(asize, region->size_min);
		_asize_to_kw(b1asize, bxk, bxw, log2m, m);
		b1asize	 = _kw_to_asize(bxk, bxw, log2m);

		b1k		 = bxk;
		b1b		 = bxw;
	}
	else {
		p1k		 = region->k;
		p1w_r	 = region->w;
		_kw_check(p1k, p1w_r, log2m, m);
		p1w_r	 = m - p1w_r;

		b1c		 = div_ceil(asize, (unsigned int) 0x1 << (log2m * p1k));
		b2c		 = p1w_r;

		if (b1c <= b2c) {
			b1div	 = 0;

			b1asize	 = b1c << (log2m * p1k);

			b1k		 = p1k;
			b1b		 = b1c;
		}
		else {
			b1div	 = 1;
			b1asize	 = (p1w_r << (log2m * p1k));

			b2c		 = (m - 1);
			for (i = p1k + 1; ; i++) {
				b1c		 = div_ceil(asize, (unsigned int) 0x1 << (log2m *  i));
				if (b1c <= b2c) {
					break;
				}

				b1div	+= 1;
				b1asize	+= ((m - 1) << (log2m *  i));
			}

			b1asize	+= (b1c << (log2m *  i));

			b1k		 = i;
			b1b		 = b1c;
		}
	}

	addr = region->end;
	end  = addr + b1asize;
	if (end <= region->limit) {
		bsp_ubik_entercrit();
		if (end <= heap->region[_UBINOS__UBICLIB__HEAP_DIR_r].addr) {
			region->end   = end;
			region->size += b1asize;
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

	_asize_to_kw(region->size, region->k, region->w, log2m, m);

	for (i=1; i<=b1div; ) {
		if (1 == i) {
			i  += 1;
			if (0 < p1w_r) {
				b2k = p1k;
				b2b = p1w_r;
			}
			else {
				continue;
			}
		}
		else {
			b2k = p1k + i - 1;
			b2b = m - 1;
			i  += 1;
		}

		_kw_refine(b2k, b2b, log2m, m);
		b2asize	 = _kw_to_asize(b2k, b2b, log2m);
		b2		 = (_heap_block_pt) addr;
		addr	+= b2asize;
		b1asize	-= b2asize;
		tag		 = _kw_to_tag(b2k, b2b, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b2, tag, log2m);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);
		_heap_n_pgroup_combine_block(heap, b2, 1);
	}

	_kw_refine(b1k, b1b, log2m, m);
	b1		 = (_heap_block_pt) addr;
	tag		 = _kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR);
	_block_set_tag(b1, tag, log2m);
	heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

	heap_logmfd("0x%08x:         : expanded: heap 0x%08x, dir %d, size 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, region->size);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

int _heap_n_pgroup_reduce(_heap_pt heap) {
	int r;
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm*/, offset, size_min;
	unsigned int p1k, p1w;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1asize;
	unsigned int bxi;
	unsigned int tag, addr, end, size;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR);

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 == region->size) {
		r = 0;
		goto end0;
	}

	m		 = region->m;
	log2m	 = region->log2m;
	//maskm	 = region->maskm;
	offset	 = region->fbloffset;
	size_min = region->size_min;

	p1k		 = region->k;
	p1w		 = region->w;
	_kw_check(p1k, p1w, log2m, m);

	addr 	 = region->addr;
	end  	 = region->end;
	size 	 = region->size;

	for (;;) {
		if (size_min >= size) {
			break;
		}

		b1  = _block_pt_to_upper_block_pt(end, log2m);
		tag = b1->tag;
		if (1 != _tag_to_a(tag)) {
			break;
		}

		_tag_to_kw(tag, b1k, b1b, log2m, m);

		if (
			(                        b1k   == p1k                         ) ||
			(1 == p1w             && b1k+1 == p1k                         )
		) {
			b1asize	 = _kw_to_asize(b1k, b1b, log2m);
			bxi		 = _kw_to_sn(b1k, b1b, log2m, m);
			bxi		 = _sn_to_index(bxi, offset, log2m, m);
			_region_remove_fb(region, bxi, b1, log2m);
			heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, bxi, log2m);
			heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

			end		-= b1asize;
			size	-= b1asize;
			if (size > 0) {
				_asize_to_kw(size, p1k, p1w, log2m, m);
			}
			else {
				p1k = 0;
				p1w = 0;
			}

			if (addr == (unsigned int) b1) {
				break;
			}
		}
		else {
			break;
		}
	}

	if (size == region->size) {
		r = 0;
		goto end0;
	}

	region->addr	 = addr;
	region->end		 = end;
	region->size	 = size;

	region->k		 = p1k;
	region->w		 = p1w;

	if (size_min > size) {
		b1 = _heap_n_pgroup_expand(heap, size_min - size);
		_heap_n_pgroup_combine_block(heap, b1, 1);
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

_heap_block_pt _heap_n_pgroup_combine_block(_heap_pt heap, _heap_block_pt block, int endflag) {
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm*/, offset;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1gid;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2gid;
	unsigned int bxi;
	unsigned int tag;
	unsigned int addr;

	b1 		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, endflag %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, endflag);

	region 	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	m		 = region->m;
	log2m	 = region->log2m;
	//maskm	 = region->maskm;
	offset	 = region->fbloffset;

	addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, log2m, m, region->min);

	for (;;) {
		tag		 = b1->tag;
		_tag_to_kw(tag, b1k, b1b, log2m, m);

		if (addr != (unsigned int) b1) {
			b2		 = _block_pt_to_upper_block_pt(b1, log2m);
			tag		 = b2->tag;

			if (1 == _tag_to_a(tag)) {
				_block_get_gid__pgroup(b1, region, b1k, b1b, b1gid, log2m);

				_tag_to_kw(tag, b2k, b2b, log2m, m);
				_block_get_gid__pgroup(b2, region, b2k, b2b, b2gid, log2m);

				if (b1k == b2k && b1gid == b2gid) {
					bxi		 = _kw_to_sn(b2k, b2b, log2m, m);
					bxi		 = _sn_to_index(bxi, offset, log2m, m);
					_region_remove_fb(region, bxi, b2, log2m);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, log2m);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);

					b1b += b2b;
					b1 = b2;

					_kw_refine(b1k, b1b, log2m, m);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
					tag = _kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR);
					_block_set_tag(b1, tag, log2m);
					heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

					continue;
				}
			}
		}

		if (1 == endflag || region->end == (unsigned int) _block_pt_to_end_prt(b1, log2m)) {
			break;
		}

		b2		 = _block_pt_to_lower_block_pt(b1, log2m);
		tag		 = b2->tag;

		if (1 == _tag_to_a(tag)) {
			_block_get_gid__pgroup(b1, region, b1k, b1b, b1gid, log2m);

			_tag_to_kw(tag, b2k, b2b, log2m, m);
			_block_get_gid__pgroup(b2, region, b2k, b2b, b2gid, log2m);

			if (b1k == b2k && b1gid == b2gid) {
				bxi		 = _kw_to_sn(b2k, b2b, log2m, m);
				bxi		 = _sn_to_index(bxi, offset, log2m, m);
				_region_remove_fb(region, bxi, b2, log2m);
				heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, log2m);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);

				b1b += b2b;

				_kw_refine(b1k, b1b, log2m, m);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
				tag = _kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR);
				_block_set_tag(b1, tag, log2m);
				heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

				continue;
			}
		}

		break;
	}

	bxi		 = _kw_to_sn(b1k, b1b, log2m, m);
	bxi		 = _sn_to_index(bxi, offset, log2m, m);
	_region_insert_fb_head(region, bxi, b1, log2m);
	heap_logmfd_block_inserted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, bxi, log2m);

	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

_heap_block_pt _heap_n_pgroup_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize) {
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm, offset*/, min;
	_heap_block_pt b1;
	unsigned int b1k, b1b;
	_heap_block_pt b2;
	unsigned int b2k, b2b;
	unsigned int bxk, bxw, bxc, bxasize;
	unsigned int tag;
	//unsigned int addr;

	b1		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, asize 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, asize);

	region   = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 >= asize) {
		goto end0;
	}

	m		 = region->m;
	log2m	 = region->log2m;
	//maskm	 = region->maskm;
	//offset	 = region->fbloffset;
	min		 = region->min;

	asize = max(asize, min);

	//addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, log2m, m, min);

	_asize_to_kw(asize, bxk, bxw, log2m, m);

	tag		 = b1->tag;
	_tag_to_kw(tag, b1k, b1b, log2m, m);
	bxasize = _tag_to_asize(tag, log2m);
	if (asize >= bxasize) {
		goto end0;
	}

	for (;;) {
		if (bxk < b1k && 1 == b1b) {
			b1k -= 1;
			b1b  = m;
		}

		if (bxk < b1k) {
			bxc = 1;
		}
		else {
			bxc = bxw;
			if (bxc == b1b) {
				break;
			}
		}

		b2k  = b1k;
		b2b  = b1b - bxc;

		b1b  = bxc;

		_kw_refine(b2k, b2b, log2m, m);
		bxasize = _kw_to_asize(b2k, b2b, log2m);
		if (min > bxasize) {
			break;
		}
		_kw_refine(b1k, b1b, log2m, m);
		bxasize = _kw_to_asize(b1k, b1b, log2m);
		if (min > bxasize) {
			break;
		}

		heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
		tag = _kw_to_tag(b1k, b1b, 0, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b1, tag, log2m);

		b2 = (_heap_block_pt) ((unsigned int) b1 + _kw_to_asize(b1k, b1b, log2m));
		tag = _kw_to_tag(b2k, b2b, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b2, tag, log2m);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);
		_heap_n_pgroup_combine_block(heap, b2, 0);

		tag = _kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b1, tag, log2m);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	}

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

void * _heap_n_pgroup_allocate_block(_heap_pt heap, unsigned int size) {
	int r;
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm*/, offset, min;
	_heap_block_pt b1;
	unsigned int b1i, b1asize;
	unsigned int bxk, bxw, bxi;
	unsigned int tag, tmp;

	b1 = NULL;

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	m		 = region->m;
	log2m	 = region->log2m;
	//maskm	 = region->maskm;
	offset	 = region->fbloffset;
	min		 = region->min;

	b1asize	 = _size_to_asize(size, min);
	_asize_to_kw(b1asize, bxk, bxw, log2m, m);
	b1asize	= _kw_to_asize(bxk, bxw, log2m);

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

	bxi		 = _kw_to_sn(bxk, bxw, log2m, m);
	bxi		 = _sn_to_index(bxi, offset, log2m, m);
	b1i 	 = bitmap_getlsb2(region->fblbm, bxi);

	if (0 == b1i) {
		b1 = _heap_n_pgroup_expand(heap, b1asize);
	}
	else {
		b1 = _region_get_fb_head(region, b1i);
		_region_remove_fb(region, b1i, b1, log2m);
	}
	if (NULL != b1) {
		b1	= _heap_n_pgroup_split_block(heap, b1, b1asize);
	}

	if (NULL != b1) {
		tag = b1->tag;
		_tag_set_a(tag, 0);
		_block_set_tag(b1, tag, log2m);
		_block_set_rsize(b1, size);
		b1asize	= _tag_to_asize(tag, log2m);

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

//end1:
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
		heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
		_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, log2m, m, min);

		tmp = (unsigned int) _block_pt_to_ptr(b1);
	}

	return (void *) tmp;
}

int _heap_n_pgroup_release_block(_heap_pt heap, void * ptr) {
	int r, r2;
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm, offset*/;
	_heap_block_pt b1;
	unsigned int asize, rsize;
	unsigned int tag;

	(void) m;

	b1 = _ptr_to_block_pt(ptr);

	region = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	m		= region->m;
	log2m	= region->log2m;
	//maskm	= region->maskm;
	//offset	= region->fbloffset;

	_block_check_freeable_and_abort(b1);
	_block_check_boundary_and_abort(b1, log2m);

	tag = b1->tag;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	_tag_check(tag, _UBINOS__UBICLIB__HEAP_DIR, log2m, m, region->min);

	asize = _tag_to_asize(tag, log2m);
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
	_block_set_tag(b1, tag, log2m);

	_heap_n_pgroup_combine_block(heap, b1, 0);

	region->asize -= asize;
	region->rsize -= rsize;

	_heap_n_pgroup_reduce(heap);

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

unsigned int heap_pgroup_calc_fblcount(unsigned int size, unsigned int m) {
	unsigned int log2m;
	unsigned int k, w;
	unsigned int fbln;

	if (HEAP_BLOCK_ASIZE_MIN > size) {
		return 0;
	}

	log2m	= uilog2_floor(m);
	if (m != ((unsigned int) 0x1 << log2m)) {
		return 0;
	}

	_asize_to_kw(HEAP_BLOCK_ASIZE_MIN, k, w, log2m, m);
	fbln	= _kw_to_sn(k, w, log2m, m);
	_asize_to_kw(size, k, w, log2m, m);
	fbln	= _kw_to_sn(k, w, log2m, m) - fbln + 1 + 1;

	return fbln;
}

unsigned int heap_pgroup_calc_fblcount_raw(unsigned int size, unsigned int m) {
	unsigned int log2m;
	unsigned int k, w;
	unsigned int fbln;

	if (HEAP_BLOCK_ASIZE_MIN > size) {
		return 0;
	}

	log2m	= uilog2_floor(m);
	if (m != ((unsigned int) 0x1 << log2m)) {
		return 0;
	}

	_asize_to_kw(size, k, w, log2m, m);
	fbln	= _kw_to_sn(k, w, log2m, m);

	return fbln;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
