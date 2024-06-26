/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_heap.h"

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__HEAP

#define _UBINOS__UBICLIB__HEAP_DIR	0
#define _UBINOS__UBICLIB__HEAP_DIR_r	1

int _heap_n_group_init_region(
		_heap_pt heap, unsigned int addr, unsigned int size, int locktype,
		unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			) {
	int r;
	unsigned int log2m;
	unsigned int k, w, t;
	_heap_region_pt region;
	unsigned int i;
	unsigned int needfblcount;

	region						= &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	region->dir					= _UBINOS__UBICLIB__HEAP_DIR;
	region->algorithm			= UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP;
	region->locktype			= locktype;

	log2m = uilog2_floor(m);

	_asize_to_kwt(size, k, w, t, log2m, m);
	if (k < 1 || HEAP_G_K_MAX < k) {
		r = -4;
		goto end0;
	}
	needfblcount = heap_group_calc_fblcount(size, m);
	if (fblcount < needfblcount) {
		r = -7;
		goto end0;
	}

	region->m					= m;
	region->log2m				= log2m;
	region->maskm				= ~((unsigned int) (-1) << log2m);
	_asize_to_kwt(HEAP_BLOCK_ASIZE_MIN, k, w, t, log2m, m);
	region->min					= _kwt_to_asize(k, w, t, log2m);

	region->size_min			= _kwt_to_asize(k + 2, 1, 1, log2m);
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
	region->fbloffset	 		= _kwt_to_sn(k, w, t, log2m, m) - 1;
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

_heap_block_pt _heap_n_group_expand(_heap_pt heap, unsigned int asize) {
	_heap_region_pt region;
	unsigned int log2m, m, maskm, min;
	unsigned int p1k, p1w_r, p1t_r;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1l, b1r, b1c, b1asize, b1div;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2l, b2r, b2c, b2asize;
	unsigned int bxk, bxw, bxt;
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
	maskm	 = region->maskm;
	min		 = region->min;

	asize = max(asize, min);

	if (0 == region->size) {
		p1k		 = 0;
		p1w_r	 = 0;
		p1t_r	 = 0;

		b1div	 = 0;
		b1asize	 = max(asize, region->size_min);
		_asize_to_kwt(b1asize, bxk, bxw, bxt, log2m, m);
		b1asize	 = _kwt_to_asize(bxk, bxw, bxt, log2m);

		b1k		 = bxk;
		b1l		 = 0;
		b1b		 = bxw;
		b1r		 = bxt;
	}
	else {
		p1k		 = region->k;
		p1w_r	 = region->w;
		p1t_r	 = region->t;
		_kwt_check(p1k, p1w_r, p1t_r, log2m, m);
		p1w_r	 = m - p1w_r;
		if (0 != p1t_r) {
			p1w_r	-= 1;
			p1t_r	 = m - p1t_r;
		}

		if (0 == p1k) {
			b1c		 = asize;
			b2c		 = p1w_r;
		}
		else {
			b1c		 = div_ceil(asize, (unsigned int) 0x1 << (log2m * p1k) >> log2m);
			b2c		 = p1t_r + p1w_r * m;
		}

		if (b1c <= b2c) {
			b1div	= 0;
			if (0 == p1k) {
				b1asize	 = b1c;

				b1k		 = 0;
				b1l		 = 0;
				b1b		 = b1c;
				b1r		 = 0;
			}
			else {
				b1asize = b1c << (log2m * p1k) >> log2m;

				b1k		 = p1k;
				b1b		 = 0;
				if (b1c <= p1t_r) {
					b1l		 = b1c;
					b1r		 = 0;
				}
				else {
					b1l		 = p1t_r;
					b1r		 = b1c - p1t_r;
				}
			}
		}
		else {
			b1c		 = div_ceil(asize, (unsigned int) 0x1 << (log2m * p1k));
			b2c		 = p1w_r + (m - 1) * m;

			if (b1c <= b2c) {
				b1div	 = 1;
				b1asize	 = (p1t_r << (log2m * p1k) >> log2m) + (b1c << (log2m * p1k));

				b1k		 = p1k + 1;
				b1b		 = 0;
				if (b1c <= p1w_r) {
					b1l		= b1c;
					b1r		= 0;
				}
				else {
					b1l		= p1w_r;
					b1r		= b1c - p1w_r;
				}
			}
			else {
				b1div	 = 2;
				b1asize	 = (p1t_r << (log2m * p1k) >> log2m) + (p1w_r << (log2m * p1k));

				b2c		 = (m - 1) + (m - 1) * m;
				for (i = p1k + 1; ; i++) {
					b1c		 = div_ceil(asize, (unsigned int) 0x1 << (log2m *  i));
					if (b1c <= b2c) {
						break;
					}

					b1div	+= 1;
					b1asize	+= ((m - 1) << (log2m *  i));
				}

				b1asize	+= (b1c << (log2m *  i));

				b1k		 = i + 1;
				b1b		 = 0;
				if (b1c <= (m - 1)) {
					b1l		 = b1c;
					b1r		 = 0;
				}
				else {
					b1l		 = (m - 1);
					b1r		 = b1c - (m - 1);
				}
			}
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

	_asize_to_kwt(region->size, region->k, region->w, region->t, log2m, m);

	for (i=1; i<=b1div; ) {
		if (1 == i) {
			if (1 == b1div) {
				i  += 1;
				if (0 < p1t_r) {
					b2k = p1k;
					b2l = p1t_r;
					b2b = 0;
					b2r = 0;
				}
				else {
					continue;
				}
			}
			else {
				i  += 2;
				if (0 < p1t_r || 0 < p1w_r) {
					b2k = p1k;
					b2l = p1t_r;
					b2b = p1w_r;
					b2r = 0;
				}
				else {
					continue;
				}
			}
		}
		else {
			b2k = p1k + i - 2;
			b2l = 0;
			b2b = m - 1;
			b2r = 0;
			i  += 1;
		}

		_kblr_refine(b2k, b2b, b2l, b2r, log2m, m, maskm);
		_kblr_to_kwt(b2k, b2b, b2l, b2r, bxk, bxw, bxt, log2m, m, maskm);
		b2asize	 = _kwt_to_asize(bxk, bxw, bxt, log2m);
		b2		 = (_heap_block_pt) addr;
		addr	+= b2asize;
		b1asize	-= b2asize;
		tag		 = _kblr_to_tag(b2k, b2b, b2l, b2r, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b2, tag, log2m);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);
		_heap_n_group_combine_block(heap, b2, 1);
	}

	_kblr_refine(b1k, b1b, b1l, b1r, log2m, m, maskm);
	b1		 = (_heap_block_pt) addr;
	tag		 = _kblr_to_tag(b1k, b1b, b1l, b1r, 1, _UBINOS__UBICLIB__HEAP_DIR);
	_block_set_tag(b1, tag, log2m);
	heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

	heap_logmfd("0x%08x:         : expanded: heap 0x%08x, dir %d, size 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, region->size);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

int _heap_n_group_reduce(_heap_pt heap) {
	int r;
	_heap_region_pt region;
	unsigned int log2m, m, maskm, offset, size_min;
	unsigned int p1k, p1w, p1t;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1l, b1r, b1asize;
	unsigned int bxk, bxw, bxt, bxi;
	unsigned int tag, addr, end, size;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR);

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 == region->size) {
		r = 0;
		goto end0;
	}

	m		 = region->m;
	log2m	 = region->log2m;
	maskm	 = region->maskm;
	offset	 = region->fbloffset;
	size_min = region->size_min;

	p1k		 = region->k;
	p1w		 = region->w;
	p1t		 = region->t;
	_kwt_check(p1k, p1w, p1t, log2m, m);

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

		_tag_to_kblr(tag, b1k, b1b, b1l, b1r, log2m, m);

		if (
			(                        b1k   == p1k                         ) ||
			(                        b1k+1 == p1k && 0 == b1l && 0 == b1r ) ||
			(1 == p1w && 0 == p1t && b1k+1 == p1k                         ) ||
			(1 == p1w && 0 == p1t && b1k+2 == p1k && 0 == b1l && 0 == b1r )
		) {
			_kblr_to_kwt(b1k, b1b, b1l, b1r, bxk, bxw, bxt, log2m, m, maskm);
			b1asize	 = _kwt_to_asize(bxk, bxw, bxt, log2m);
			bxi		 = _kwt_to_sn(bxk, bxw, bxt, log2m, m);
			bxi		 = _sn_to_index(bxi, offset, log2m, m);
			_region_remove_fb(region, bxi, b1, log2m);
			heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, bxi, log2m);
			heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

			end		-= b1asize;
			size	-= b1asize;
			if (size > 0) {
				_asize_to_kwt(size, p1k, p1w, p1t, log2m, m);
			}
			else {
				p1k = 0;
				p1w = 0;
				p1t = 0;
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
	region->t		 = p1t;

	if (size_min > size) {
		b1 = _heap_n_group_expand(heap, size_min - size);
		_heap_n_group_combine_block(heap, b1, 1);
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

_heap_block_pt _heap_n_group_combine_block(_heap_pt heap, _heap_block_pt block, int endflag) {
	_heap_region_pt region;
	unsigned int log2m, m, maskm, offset;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1l, b1r, b1gid, b1sk, b1sgid;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2l, b2r, b2gid, b2sk, b2sgid;
	unsigned int bxk, bxw, bxt, bxi;
	unsigned int tag;
	unsigned int addr;

	b1 		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, endflag %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, endflag);

	region 	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	m		 = region->m;
	log2m	 = region->log2m;
	maskm	 = region->maskm;
	offset	 = region->fbloffset;

	addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, log2m, m, region->min);

	for (;;) {
		tag		 = b1->tag;
		_tag_to_kblr(tag, b1k, b1b, b1l, b1r, log2m, m);

		if (addr != (unsigned int) b1) {
			b2		 = _block_pt_to_upper_block_pt(b1, log2m);
			tag		 = b2->tag;

			if (1 == _tag_to_a(tag)) {
				_block_get_gid(b1, region, b1k, b1b, b1l, b1r, b1gid, b1sk, b1sgid, log2m);

				_tag_to_kblr(tag, b2k, b2b, b2l, b2r, log2m, m);
				_block_get_gid(b2, region, b2k, b2b, b2l, b2r, b2gid, b2sk, b2sgid, log2m);

				if ((b1k == b2k && b1gid == b2gid) || (b1k == b2sk && b1gid == b2sgid) || (b1sk == b2k && b1sgid == b2gid)) {
					_kblr_to_kwt(b2k, b2b, b2l, b2r, bxk, bxw, bxt, log2m, m, maskm);
					bxi		 = _kwt_to_sn(bxk, bxw, bxt, log2m, m);
					bxi		 = _sn_to_index(bxi, offset, log2m, m);
					_region_remove_fb(region, bxi, b2, log2m);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, log2m);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);

					if (b1k > b2k) {
						b1l += b2b;
					}
					else if (b1k < b2k) {
						b2r += b1b;

						b1k  = b2k;
						b1l  = b2l;
						b1b  = b2b;
						b1r  = b2r;
					}
					else {
						b1l  = b2l;
						b1b += b2b;
						b1b += (0 == b2r ? 0 : 1);
					}
					b1 = b2;

					_kblr_refine(b1k, b1b, b1l, b1r, log2m, m, maskm);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
					tag = _kblr_to_tag(b1k, b1b, b1l, b1r, 1, _UBINOS__UBICLIB__HEAP_DIR);
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
			_block_get_gid(b1, region, b1k, b1b, b1l, b1r, b1gid, b1sk, b1sgid, log2m);

			_tag_to_kblr(tag, b2k, b2b, b2l, b2r, log2m, m);
			_block_get_gid(b2, region, b2k, b2b, b2l, b2r, b2gid, b2sk, b2sgid, log2m);

			if ((b1k == b2k && b1gid == b2gid) || (b1k == b2sk && b1gid == b2sgid) || (b1sk == b2k && b1sgid == b2gid)) {
				_kblr_to_kwt(b2k, b2b, b2l, b2r, bxk, bxw, bxt, log2m, m, maskm);
				bxi		 = _kwt_to_sn(bxk, bxw, bxt, log2m, m);
				bxi		 = _sn_to_index(bxi, offset, log2m, m);
				_region_remove_fb(region, bxi, b2, log2m);
				heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, log2m);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);

				if  (b1k > b2k) {
					b1r += b2b;
				}
				else if (b1k < b2k) {
					b2l += b1b;

					b1k  = b2k;
					b1l  = b2l;
					b1b  = b2b;
					b1r  = b2r;
				}
				else {
					b1b += b2b;
					b1b += (0 == b2l ? 0 : 1);
					b1r  = b2r;
				}

				_kblr_refine(b1k, b1b, b1l, b1r, log2m, m, maskm);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
				tag = _kblr_to_tag(b1k, b1b, b1l, b1r, 1, _UBINOS__UBICLIB__HEAP_DIR);
				_block_set_tag(b1, tag, log2m);
				heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);

				continue;
			}
		}

		break;
	}

	_kblr_to_kwt(b1k, b1b, b1l, b1r, bxk, bxw, bxt, log2m, m, maskm);
	bxi		 = _kwt_to_sn(bxk, bxw, bxt, log2m, m);
	bxi		 = _sn_to_index(bxi, offset, log2m, m);
	if (0 == b1b) {
		_region_insert_fb_tail(region, bxi, b1, log2m);
	}
	else {
		_region_insert_fb_head(region, bxi, b1, log2m);
	}
	heap_logmfd_block_inserted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, bxi, log2m);

	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

_heap_block_pt _heap_n_group_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize) {
	_heap_region_pt region;
	unsigned int log2m, m, maskm/*, offset*/, min;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1l, b1r, b1c;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2l, b2r;
	unsigned int bxk, bxw, bxt, bxc, bxasize;
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
	maskm	 = region->maskm;
	//offset	 = region->fbloffset;
	min		 = region->min;

	asize = max(asize, min);

	//addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, log2m, m, min);

	_asize_to_kwt(asize, bxk, bxw, bxt, log2m, m);

	tag		 = b1->tag;
	_tag_to_kblr(tag, b1k, b1b, b1l, b1r, log2m, m);
	bxasize = _tag_to_asize(tag, log2m);
	if (asize >= bxasize) {
		goto end0;
	}

	for (;;) {
		// bxk <= b1k
		if (0 == bxk && 0 == b1k) {
			b1c  = b1b;
			bxc  = bxw;
			if (bxc == b1c) {
				break;
			}

			b2k  = 0;
			b2l  = 0;
			b2b  = b1b - bxc;
			b2r  = 0;

			//b1k  = 0;
			//b1l  = 0;
			b1b  = bxc;
			//b1r  = 0;
		}
		else {
			b1c  = b1b * m + b1l + b1r;	// b1c != 1
			if (0 == bxk) {
				if (1 < b1k) {
					bxc  = 1;
				}
				else { // 1 == b1k
					bxc = bxw;	// bxt == 0
					if (bxc == b1c) {
						break;
					}
				}
			}
			else {
				if (bxk + 1 < b1k) {
					bxc = 1;
				}
				else if (bxk + 1 == b1k) {
					bxc = bxw + (0 == bxt ? 0 : 1);
					if (bxc == b1c && bxc > b1l) {
						break;
					}
				}
				else {	// bxk == b1k
					bxc = bxw * m + bxt;
					if (bxc == b1c) {
						break;
					}
				}
			}

			if (bxc <= b1l) {
				b2k  = b1k;
				b2l  = b1l - bxc;
				b2b  = b1b;
				b2r  = b1r;

				//b1k  = b1k;
				b1l  = bxc;
				b1b  = 0;
				b1r  = 0;
			}
			else {
				//b1k  = b1k;
				//b1l  = b1l;
				b1b  = (bxc - b1l) >> log2m;
				b1r  = (bxc - b1l) &  maskm;

				b1c -= bxc;

				b2k  = b1k;
				if (0 != b1r) {
					b2l  = min((m - b1r), b1c);
				}
				else {
					b2l  = 0;
				}
				b2b  = (b1c - b2l) >> log2m;
				b2r  = (b1c - b2l) &  maskm;
			}
		}

		_kblr_refine(b2k, b2b, b2l, b2r, log2m, m, maskm);
		bxasize = _kblr_to_asize(b2k, b2b, b2l, b2r, log2m);
		if (min > bxasize) {
			break;
		}
		_kblr_refine(b1k, b1b, b1l, b1r, log2m, m, maskm);
		bxasize = _kblr_to_asize(b1k, b1b, b1l, b1r, log2m);
		if (min > bxasize) {
			break;
		}

		heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
		tag = _kblr_to_tag(b1k, b1b, b1l, b1r, 0, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b1, tag, log2m);

		b2 = (_heap_block_pt) ((unsigned int) b1 + _kblr_to_asize(b1k, b1b, b1l, b1r, log2m));
		tag = _kblr_to_tag(b2k, b2b, b2l, b2r, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b2, tag, log2m);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, log2m);
		_heap_n_group_combine_block(heap, b2, 0);

		tag = _kblr_to_tag(b1k, b1b, b1l, b1r, 1, _UBINOS__UBICLIB__HEAP_DIR);
		_block_set_tag(b1, tag, log2m);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, log2m);
	}

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

void * _heap_n_group_allocate_block(_heap_pt heap, unsigned int size) {
	int r;
	_heap_region_pt region;
	unsigned int log2m, m/*, maskm*/, offset, min;
	_heap_block_pt b1;
	unsigned int b1i, b1asize;
	unsigned int bxk, bxw, bxt, bxi;
	unsigned int tag, tmp;

	b1 = NULL;

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	m		 = region->m;
	log2m	 = region->log2m;
	//maskm	 = region->maskm;
	offset	 = region->fbloffset;
	min		 = region->min;

	b1asize	 = _size_to_asize(size, min);
	_asize_to_kwt(b1asize, bxk, bxw, bxt, log2m, m);
	b1asize	= _kwt_to_asize(bxk, bxw, bxt, log2m);

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

	bxi		 = _kwt_to_sn(bxk, bxw, bxt, log2m, m);
	bxi		 = _sn_to_index(bxi, offset, log2m, m);
#if 1
	b1i 	 = bitmap_getlsb2(region->fblbm, bxi);
#else
	for (b1i = bxi; b1i < region->fblcount; b1i++) {
		if (0 < region->fbl_ap[b1i].count) {
			break;
		}
	}
	if (b1i  == region->fblcount) {
		b1i = 0;
	}
#endif

	if (0 == b1i) {
		b1 = _heap_n_group_expand(heap, b1asize);
	}
	else {
		if (bxi + 2 <= b1i) {
			b1 = _region_get_fb_tail(region, b1i);
		}
		else {
			b1 = _region_get_fb_head(region, b1i);
		}
		_region_remove_fb(region, b1i, b1, log2m);
	}
	if (NULL != b1) {
		b1	= _heap_n_group_split_block(heap, b1, b1asize);
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

int _heap_n_group_release_block(_heap_pt heap, void * ptr) {
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

	_heap_n_group_combine_block(heap, b1, 0);

	region->asize -= asize;
	region->rsize -= rsize;

	_heap_n_group_reduce(heap);

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

unsigned int heap_group_calc_fblcount(unsigned int size, unsigned int m) {
	unsigned int log2m;
	unsigned int k, w, t;
	unsigned int fbln;

	if (HEAP_BLOCK_ASIZE_MIN > size) {
		return 0;
	}

	log2m	= uilog2_floor(m);
	if (m != ((unsigned int) 0x1 << log2m)) {
		return 0;
	}

	_asize_to_kwt(HEAP_BLOCK_ASIZE_MIN, k, w, t, log2m, m);
	fbln	= _kwt_to_sn(k, w, t, log2m, m);
	_asize_to_kwt(size, k, w, t, log2m, m);
	fbln	= _kwt_to_sn(k, w, t, log2m, m) - fbln + 1 + 1; // + dregs list (1) + HEAP_BLOCK_ASIZE_MIN size list (1)

	return fbln;
}

unsigned int heap_group_calc_fblcount_raw(unsigned int size, unsigned int m) {
	unsigned int log2m;
	unsigned int k, w, t;
	unsigned int fbln;

	if (HEAP_BLOCK_ASIZE_MIN > size) {
		return 0;
	}

	log2m	= uilog2_floor(m);
	if (m != ((unsigned int) 0x1 << log2m)) {
		return 0;
	}

	_asize_to_kwt(size, k, w, t, log2m, m);
	fbln	= _kwt_to_sn(k, w, t, log2m, m);

	return fbln;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
