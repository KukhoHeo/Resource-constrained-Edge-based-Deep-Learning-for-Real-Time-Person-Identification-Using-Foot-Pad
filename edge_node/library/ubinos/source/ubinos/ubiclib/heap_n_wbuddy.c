/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_heap.h"

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)

#include <assert.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__HEAP

#define _UBINOS__UBICLIB__HEAP_DIR	0
#define _UBINOS__UBICLIB__HEAP_DIR_r	1

#define HEAP_GTYPE_W1					(0x8 + 0x4 + 0x1)
#define HEAP_GTYPE_W3					(0x8 + 0x4 + 0x3)

#define	_wbuddy_tag_to_asize(tag)		_tag_to_g_asize(tag, 1)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1)
	#define _wbuddy_tag_check(tag, dir, min)	{														\
		if (min > _wbuddy_tag_to_asize(tag)) {															\
			logme("tag is wrong (error type 1)");														\
			bsp_abortsystem();																			\
		}																								\
		if (dir != _tag_to_d(tag)) {																	\
			logme("tag is wrong (error type 2)");														\
			bsp_abortsystem();																			\
		}																								\
		if (HEAP_G_K_MAX < _tag_to_g_k(tag)) {															\
			logme("tag is wrong (error type 3)");														\
			bsp_abortsystem();																			\
		}																								\
		if ((4) <= _tag_to_g_b(tag)) {																	\
			logme("tag is wrong (error type 4)");														\
			bsp_abortsystem();																			\
		}																								\
		if ((0) != _tag_to_g_l(tag)) {																	\
			logme("tag is wrong (error type 5)");														\
			bsp_abortsystem();																			\
		}																								\
		if ((0) != _tag_to_g_r(tag)) {																	\
			logme("tag is wrong (error type 6)");														\
			bsp_abortsystem();																			\
		}																								\
		if (	  HEAP_GTYPE_W1 != _tag_to_g(tag)														\
		   	   && HEAP_GTYPE_W3 != _tag_to_g(tag)) {													\
			logme("tag is wrong (error type 7)");														\
			bsp_abortsystem();																			\
		}																								\
	}
#else
    #define _wbuddy_tag_check(tag, dir, min)
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1) */


#define  _wbuddy_kw_to_tag(k, w, a, d, g) ( 															\
	(((unsigned int) (a    ) << HEAP_G_TAG__DGKLRB_BITSIZE ) & HEAP_G_TAG_A_______MASK) |				\
	(((unsigned int) (d    ) << HEAP_G_TAG___GKLRB_BITSIZE ) & HEAP_G_TAG__D______MASK) |				\
	(((unsigned int) (g    ) << HEAP_G_TAG____KLRB_BITSIZE ) & HEAP_G_TAG___G_____MASK) |				\
	(((unsigned int) (k    ) << HEAP_G_TAG_____LRB_BITSIZE ) & HEAP_G_TAG____K____MASK) |				\
	(((unsigned int) (w    )                               ) & HEAP_G_TAG_______B_MASK)	)


#define	_wbuddy_tag_to_size(tag)			(_wbuddy_tag_to_asize(tag) - HEAP_BLOCK_OVERHEAD)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1)
	#define _wbuddy_kw_check(k, w) {																			\
		if (HEAP_G_K_MAX < (k)) {																				\
			logme("kw is wrong (error type 1)");																\
			bsp_abortsystem();																					\
		}																										\
		if (3 != w && 1 != w) {																					\
			logme("kw is wrong (error type 2)");																\
			bsp_abortsystem();																					\
		}																										\
	}
#else
    #define _wbuddy_kw_check(k, w)
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1) */

#define _wbuddy_kw_refine(k, w) {																				\
	if ((4) == (w)) {																							\
		(k) += (2);																								\
		(w)  = (1);																								\
	}																											\
	else if ((2) == (w)) {																						\
		(k) += (1);																								\
		(w)  = (1);																								\
	}																											\
	_wbuddy_kw_check(k, w);																						\
}

#define _wbuddy_asize_to_kw(asize, k, w)	{																	\
	(k) = uilog2_floor(asize) / (1);																			\
	(w) = (unsigned int) (asize) >> ((1) * (k));																\
	switch ( div_ceil(	(asize) - ((unsigned int) (w) << ((1) * (k))        ), 									\
						      	  ((unsigned int) 0x1 << ((1) * (k)) >> (1) )	)	) {							\
	case 0:																										\
		break;																									\
	case 1:																										\
		(k) -= (1);																								\
		(w)  = (3);																								\
		break;																									\
	case 2:																										\
		(k) += (1);																								\
		break;																									\
	default:																									\
		bsp_abortsystem();																						\
	}																											\
	_wbuddy_kw_refine(k, w);																					\
}

#define _wbuddy_kw_to_asize(k, w)		_kwt_to_asize(k, w, 0, 1)

#define _wbuddy_kw_to_asize_r(k, w)		(3 == w ? (unsigned int) (1) << (k) : (unsigned int) (1) << (k) >> (1))

#define _wbuddy_kw_to_sn(k, w) 			(3 == w ? _kwt_to_sn(k + 1, 1, 1, 1, 2) : _kwt_to_sn(k, 1, 0, 1, 2))

#define _wbuddy_sn_to_index(sn, offset)  _sn_to_index(sn, offset, 1, 2)

#define _wbuddy_tag_to_kw(tag, k, w)	{																		\
		(k) = _tag_to_g_k(tag);																					\
		(w) = _tag_to_g_b(tag);																					\
		_wbuddy_kw_check(k, w);																					\
}


int _heap_n_wbuddy_init_region(
		_heap_pt heap, unsigned int addr, unsigned int size, int locktype,
		unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			) {
	int r;
	unsigned int k, w;
	_heap_region_pt region;
	unsigned int i;

	region						= &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	region->dir					= _UBINOS__UBICLIB__HEAP_DIR;
	region->algorithm			= UBINOS__UBICLIB__HEAP_ALGORITHM__WBUDDY;
	region->locktype			= locktype;

	_wbuddy_asize_to_kw(size, k, w);
	if (k < 1 || HEAP_G_K_MAX < k) {
		r = -4;
		goto end0;
	}
	if (fblcount < heap_wbuddy_calc_fblcount(size, 2)) {
		r = -7;
		goto end0;
	}

	region->m					= 2;
	region->log2m				= 1;
	region->maskm				= ~((unsigned int) (-1) << 1);
	_wbuddy_asize_to_kw(HEAP_BLOCK_ASIZE_MIN, k, w);
	region->min					= _wbuddy_kw_to_asize(k, w);

	region->size_min			= _wbuddy_kw_to_asize(k + 2, 1);
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
	region->fbloffset	 		= _wbuddy_kw_to_sn(k, w) - 1;
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

_heap_block_pt _heap_n_wbuddy_expand(_heap_pt heap, unsigned int asize) {
	_heap_region_pt region;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1g, b1asize;
	unsigned int bxk, bxw;
	unsigned int tag, addr, end;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, asize 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, asize);

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 >= asize) {
		b1 = NULL;
		goto end0;
	}

	if (0 == region->size) {
		b1asize = region->limit - region->addr;
		_wbuddy_asize_to_kw(b1asize, bxk, bxw);
		b1asize	 = _wbuddy_kw_to_asize(bxk, bxw);

		b1k		 = bxk;
		b1b		 = bxw;
		
		if (3 == b1b) {
			b1g = HEAP_GTYPE_W3;
		}
		else {
			b1g = HEAP_GTYPE_W1;
		}
	}
	else {
		b1 = NULL;
		goto end0;
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

	_wbuddy_asize_to_kw(region->size, region->k, region->w);

	_wbuddy_kw_refine(b1k, b1b);
	b1		 = (_heap_block_pt) addr;
	tag		 = _wbuddy_kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR, b1g);
	_block_set_tag(b1, tag, 1);
	heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

	heap_logmfd("0x%08x:         : expanded: heap 0x%08x, dir %d, size 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, region->size);

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

int _heap_n_wbuddy_reduce(_heap_pt heap) {
	return 0;
}

_heap_block_pt _heap_n_wbuddy_combine_block(_heap_pt heap, _heap_block_pt block, int endflag) {
	_heap_region_pt region;
	unsigned int offset;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1g;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2g;
	_heap_block_pt b3;
	unsigned int b3k, b3b, b3g;
	unsigned int bxi;
	unsigned int tag;
	unsigned int addr;
	unsigned int end;
	unsigned int flag;

	b1 		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, endflag %d", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, endflag);

	region 	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	offset	 = region->fbloffset;

	addr	 = region->addr;
	end		 = region->end;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);
	_wbuddy_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, region->min);

	for (;;) {

		flag = 0;

		tag		 = b1->tag;
		_wbuddy_tag_to_kw(tag, b1k, b1b);
		b1g = _tag_to_g(tag);

		if (HEAP_GTYPE_W3 == b1g && 1 == b1b && addr != (unsigned int) b1) {
			b2		 = _block_pt_to_upper_block_pt(b1, 1);
			tag		 = b2->tag;
			_wbuddy_tag_to_kw(tag, b2k, b2b);
			b2g = _tag_to_g(tag);

			if (0 == _tag_to_a(tag)) {
			}
			else if (HEAP_GTYPE_W1 == b2g && 1 == b2b && b1k == b2k && addr != (unsigned int) b2) {
				b3		 = _block_pt_to_upper_block_pt(b2, 1);
				tag		 = b3->tag;
				_wbuddy_tag_to_kw(tag, b3k, b3b);
				b3g = _tag_to_g(tag);

				if (0 == _tag_to_a(tag)) {
				}
				else if (HEAP_GTYPE_W1 == b3g && 1 == b3b && b1k + 1 == b3k) {
					bxi		 = _wbuddy_kw_to_sn(b3k, b3b);
					bxi		 = _wbuddy_sn_to_index(bxi, offset);
					_region_remove_fb(region, bxi, b3, 1);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b3, bxi, 1);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b3, 1);

					bxi		 = _wbuddy_kw_to_sn(b2k, b2b);
					bxi		 = _wbuddy_sn_to_index(bxi, offset);
					_region_remove_fb(region, bxi, b2, 1);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, 1);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1);

					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

					b1k += 2;
					b1b  = 1;
					b1   = b3;
					flag = 1;
				}
			}
			else if (HEAP_GTYPE_W3 == b2g && 3 == b2b && b1k == b2k) {
				bxi		 = _wbuddy_kw_to_sn(b2k, b2b);
				bxi		 = _wbuddy_sn_to_index(bxi, offset);
				_region_remove_fb(region, bxi, b2, 1);
				heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, 1);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1);

				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

				b1k += 2;
				b1b  = 1;
				b1   = b2;
				flag = 1;
			}
		}
		else if (HEAP_GTYPE_W1 == b1g && 1 == b1b && end != (unsigned int) _block_pt_to_end_prt(b1, 1)) {
			b2		 = _block_pt_to_lower_block_pt(b1, 1);
			tag		 = b2->tag;
			_wbuddy_tag_to_kw(tag, b2k, b2b);
			b2g = _tag_to_g(tag);

			if (0 == _tag_to_a(tag)) {
			}
			else if (HEAP_GTYPE_W3 == b2g && 1 == b2b && b1k == b2k && addr != (unsigned int) b1) {
				b3		 = _block_pt_to_upper_block_pt(b1, 1);
				tag		 = b3->tag;
				_wbuddy_tag_to_kw(tag, b3k, b3b);
				b3g = _tag_to_g(tag);

				if (0 == _tag_to_a(tag)) {
				}
				else if (HEAP_GTYPE_W1 == b3g && 1 == b3b && b1k + 1 == b3k) {
					bxi		 = _wbuddy_kw_to_sn(b3k, b3b);
					bxi		 = _wbuddy_sn_to_index(bxi, offset);
					_region_remove_fb(region, bxi, b3, 1);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b3, bxi, 1);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b3, 1);

					bxi		 = _wbuddy_kw_to_sn(b2k, b2b);
					bxi		 = _wbuddy_sn_to_index(bxi, offset);
					_region_remove_fb(region, bxi, b2, 1);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, 1);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1);

					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

					b1k += 2;
					b1b  = 1;
					b1   = b3;
					flag = 1;
				}
			}
			else if (HEAP_GTYPE_W1 == b2g && 1 == b2b && b1k == b2k + 1 && end != (unsigned int) _block_pt_to_end_prt(b2, 1)) {
				b3		 = _block_pt_to_lower_block_pt(b2, 1);
				tag		 = b3->tag;
				_wbuddy_tag_to_kw(tag, b3k, b3b);
				b3g = _tag_to_g(tag);

				if (0 == _tag_to_a(tag)) {
				}
				else if (HEAP_GTYPE_W3 == b3g && 1 == b3b && b1k == b3k + 1) {
					bxi		 = _wbuddy_kw_to_sn(b3k, b3b);
					bxi		 = _wbuddy_sn_to_index(bxi, offset);
					_region_remove_fb(region, bxi, b3, 1);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b3, bxi, 1);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b3, 1);

					bxi		 = _wbuddy_kw_to_sn(b2k, b2b);
					bxi		 = _wbuddy_sn_to_index(bxi, offset);
					_region_remove_fb(region, bxi, b2, 1);
					heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, 1);
					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1);

					heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

					b1k += 1;
					b1b  = 1;
					flag = 1;
				}
			}
		}
		else if (HEAP_GTYPE_W3 == b1g && 3 == b1b && end != (unsigned int) _block_pt_to_end_prt(b1, 1)) {
			b2		 = _block_pt_to_lower_block_pt(b1, 1);
			tag		 = b2->tag;
			_wbuddy_tag_to_kw(tag, b2k, b2b);
			b2g = _tag_to_g(tag);

			if (0 == _tag_to_a(tag)) {
			}
			else if (HEAP_GTYPE_W3 == b2g && 1 == b2b && b1k == b2k) {
				bxi		 = _wbuddy_kw_to_sn(b2k, b2b);
				bxi		 = _wbuddy_sn_to_index(bxi, offset);
				_region_remove_fb(region, bxi, b2, 1);
				heap_logmfd_block_removed(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, bxi, 1);
				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1);

				heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

				b1k += 2;
				b1b  = 1;
				flag = 1;
			}
		}

		if (flag == 1) {
			_wbuddy_kw_refine(b1k, b1b);

			if (		0 != (((unsigned int) b1                      - addr) % (0x1 << (b1k + 2)))	&&
						0 == (((unsigned int) b1 + (b1b << b1k)       - addr) % (0x1 << (b1k + 2)))		) {
				b1g = HEAP_GTYPE_W3;
			}
			else if (	0 != (((unsigned int) b1 + (b1b << b1k)       - addr) % (0x1 << (b1k + 2)))		) {
				b1g = HEAP_GTYPE_W1;
			}
			else {
				bsp_abortsystem();
			}

			tag = _wbuddy_kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR, b1g);
			_block_set_tag(b1, tag, 1);
			heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

			continue;
		}

		break;
	}

	bxi		 = _wbuddy_kw_to_sn(b1k, b1b);
	bxi		 = _wbuddy_sn_to_index(bxi, offset);
	_region_insert_fb_head(region, bxi, b1, 1);
	heap_logmfd_block_inserted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, bxi, 1);

	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

_heap_block_pt _heap_n_wbuddy_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize) {
	_heap_region_pt region;
	unsigned int /*offset, */min;
	_heap_block_pt b1;
	unsigned int b1k, b1b, b1g, b1asize;
	_heap_block_pt b2;
	unsigned int b2k, b2b, b2g, b2asize;
	unsigned int bxk, bxw, bxc;
	unsigned int tag;
	unsigned int addr;

	b1		 = block;

	heap_logmfd("0x%08x: called  : heap 0x%08x, dir %d, block 0x%08x, asize 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, block, asize);

	region   = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	if (0 >= asize) {
		goto end0;
	}

	//offset	 = region->fbloffset;
	min		 = region->min;

	asize = max(asize, min);

	addr	 = region->addr;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);
	_wbuddy_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, min);

	_wbuddy_asize_to_kw(asize, bxk, bxw);

	tag		 = b1->tag;
	_wbuddy_tag_to_kw(tag, b1k, b1b);
	b1asize = _wbuddy_tag_to_asize(tag);
	if (asize >= b1asize) {
		goto end0;
	}

	for (;;) {
		if (1 == bxw) {
			if      (bxk + 2 <= b1k) {
				if (1 == b1b) {
					b1k -= 2;
					b1b  = 4;
					bxc  = 1;
				}
				else {
					bxc  = 1;
				}
			}
			else if (bxk + 1 == b1k) {
				if (1 == b1b) {
					b1k -= 2;
					b1b  = 4;
					bxc  = 3;
				}
				else {
					bxc  = 1;
				}
			}
			else if (bxk     >  b1k) {
				if (1 == b1b) {
					bxc = 0;
					bsp_abortsystem();
				}
				else {
					bxc  = 2;
				}
			}
			else { // if (bxk     == b1k) {
				if (1 == b1b) {
					bxc = 0;
					break;
				}
				else {
					bxc  = 1;
				}
			}
		}
		else { // if (3 == bxw) {
			if      (bxk + 4 <= b1k) {
				if (1 == b1b) {
					b1k -= 2;
					b1b  = 4;
					bxc  = 1;
				}
				else {
					bxc  = 1;
				}
			}
			else if (bxk + 3 == b1k || bxk + 2 == b1k) {
				if (1 == b1b) {
					b1k -= 2;
					b1b  = 4;
					bxc  = 3;
				}
				else {
					bxc  = 1;
				}
			}
			else if (bxk + 1 == b1k) {
				if (1 == b1b) {
					bxc = 0;
					bsp_abortsystem();
				}
				else {
					bxc  = 2;
				}
			}
			else { // if (bxk     == b1k) {
				if (1 == b1b) {
					bxc = 0;
					bsp_abortsystem();
				}
				else {
					bxc = 0;
					break;
				}
			}

		}

		b2k  = b1k;
		b2b  = b1b - bxc;

		b1b  = bxc;

		_wbuddy_kw_refine(b2k, b2b);
		b2asize = _wbuddy_kw_to_asize(b2k, b2b);
		if (min > b2asize) {
			break;
		}
		_wbuddy_kw_refine(b1k, b1b);
		b1asize = _wbuddy_kw_to_asize(b1k, b1b);
		if (min > b1asize) {
			break;
		}

		heap_logmfd_block_deleted(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);

		if (b1asize > b2asize) {
			b2 = (_heap_block_pt) ((unsigned int) b1 + _wbuddy_kw_to_asize(b1k, b1b));
			if (3 == b1b) {
				b1g = HEAP_GTYPE_W3;
				b2g = HEAP_GTYPE_W3;

				if (	0 == (((unsigned int) b1                - addr) % (0x1 << (b1k + 2)))	&&
						0 != (((unsigned int) b1 + (b1b << b1k) - addr) % (0x1 << (b1k + 2)))		) {
				}
				else {
					bsp_abortsystem();
				}
				if (	0 != (((unsigned int) b2                - addr) % (0x1 << (b2k + 2)))	&&
						0 == (((unsigned int) b2 + (b2b << b2k) - addr) % (0x1 << (b2k + 2)))		) {
				}
				else {
					bsp_abortsystem();
				}

				if (b1k != b2k) {
					bsp_abortsystem();
				}
				if (b1b != b2b * 3) {
					bsp_abortsystem();
				}
			}
			else {
				b1g = HEAP_GTYPE_W1;
				b2g = HEAP_GTYPE_W1;

				if (	0 != (((unsigned int) b1 + (b1b << b1k) - addr) % (0x1 << (b1k + 2)))	) {
				}
				else {
					bsp_abortsystem();
				}
				if (	0 != (((unsigned int) b2                - addr) % (0x1 << (b2k + 2)))	&&
						0 != (((unsigned int) b2 + (b2b << b2k) - addr) % (0x1 << (b2k + 2)))		) {
				}
				else {
					bsp_abortsystem();
				}

				if (b1k != b2k + 1) {
					bsp_abortsystem();
				}
				if (b1b != 1 || b2b != 1) {
					bsp_abortsystem();
				}
			}
		}
		else {
			b2 = b1;
			b1 = (_heap_block_pt) ((unsigned int) b1 + _wbuddy_kw_to_asize(b2k, b2b));
			if (3 == b2b) {
				b2g = HEAP_GTYPE_W3;
				b1g = HEAP_GTYPE_W3;

				if (	0 != (((unsigned int) b1                - addr) % (0x1 << (b1k + 2)))	&&
						0 == (((unsigned int) b1 + (b1b << b1k) - addr) % (0x1 << (b1k + 2)))		) {
				}
				else {
					bsp_abortsystem();
				}
				if (	0 == (((unsigned int) b2                - addr) % (0x1 << (b2k + 2)))	&&
						0 != (((unsigned int) b2 + (b2b << b2k) - addr) % (0x1 << (b2k + 2)))		) {
				}
				else {
					bsp_abortsystem();
				}

				if (b1k != b2k) {
					bsp_abortsystem();
				}
				if (b1b * 3 != b2b) {
					bsp_abortsystem();
				}
			}
			else {
				b2g = HEAP_GTYPE_W1;
				b1g = HEAP_GTYPE_W1;

				if (	0 != (((unsigned int) b1                - addr) % (0x1 << (b1k + 2)))	&&
						0 != (((unsigned int) b1 + (b1b << b1k) - addr) % (0x1 << (b1k + 2)))		) {
				}
				else {
					bsp_abortsystem();
				}
				if (	0 != (((unsigned int) b2 + (b2b << b2k) - addr) % (0x1 << (b2k + 2)))	) {
				}
				else {
					bsp_abortsystem();
				}

				if (b1k + 1 != b2k) {
					bsp_abortsystem();
				}
				if (b1b != 1 || b2b != 1) {
					bsp_abortsystem();
				}
			}
		}
		
		tag = _wbuddy_kw_to_tag(b1k, b1b, 0, _UBINOS__UBICLIB__HEAP_DIR, b1g);
		_block_set_tag(b1, tag, 1);

		tag = _wbuddy_kw_to_tag(b2k, b2b, 1, _UBINOS__UBICLIB__HEAP_DIR, b2g);
		_block_set_tag(b2, tag, 1);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b2, 1);
		_heap_n_wbuddy_combine_block(heap, b2, 0);

		tag = _wbuddy_kw_to_tag(b1k, b1b, 1, _UBINOS__UBICLIB__HEAP_DIR, b1g);
		_block_set_tag(b1, tag, 1);
		heap_logmfd_block_created(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);
	}

end0:
	heap_logmfd("0x%08x: return  : heap 0x%08x, dir %d, block 0x%08x", bsp_task_getcur(), heap, _UBINOS__UBICLIB__HEAP_DIR, b1);

	return b1;
}

void * _heap_n_wbuddy_allocate_block(_heap_pt heap, unsigned int size) {
	int r;
	_heap_region_pt region;
	unsigned int offset, min;
	_heap_block_pt b1;
	unsigned int b1i, b1asize;
	unsigned int bxk, bxw, bxi;
	unsigned int tag, temp;

	b1 = NULL;

	region	 = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	offset	 = region->fbloffset;
	min		 = region->min;

	b1asize	 = _size_to_asize(size, min);
	_wbuddy_asize_to_kw(b1asize, bxk, bxw);
	b1asize	= _wbuddy_kw_to_asize(bxk, bxw);

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

	bxi		 = _wbuddy_kw_to_sn(bxk, bxw);
	bxi		 = _wbuddy_sn_to_index(bxi, offset);
	b1i 	 = bitmap_getlsb2(region->fblbm, bxi);

	if (0 == b1i) {
		b1 = _heap_n_wbuddy_expand(heap, b1asize);
	}
	else {
		b1 = _region_get_fb_head(region, b1i);
		_region_remove_fb(region, b1i, b1, 1);
	}
	if (NULL != b1) {
		b1	= _heap_n_wbuddy_split_block(heap, b1, b1asize);
	}

	if (NULL != b1) {
		tag = b1->tag;
		_tag_set_a(tag, 0);
		_block_set_tag(b1, tag, 1);
		_block_set_rsize(b1, size);
		b1asize	= _wbuddy_tag_to_asize(tag);

		if (size > b1asize) {
			logme("b1asize is wrong");
			heap_printheapinfo((heap_pt) heap);
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

		temp = heap->region[0].abl.count + heap->region[1].abl.count;
		if (heap->acount_max < temp) {
			heap->acount_max = temp;
		}
		temp = heap->region[0].asize + heap->region[1].asize;;
		if (heap->asize_max < temp) {
			heap->asize_max = temp;
		}
		temp = heap->region[0].rsize + heap->region[1].rsize;;
		if (heap->rsize_max < temp) {
			heap->rsize_max = temp;
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
		temp = 0;
	}
	else {
		heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);
		_wbuddy_tag_check(b1->tag, _UBINOS__UBICLIB__HEAP_DIR, min);

		temp = (unsigned int) _block_pt_to_ptr(b1);
	}

	return (void *) temp;
}

int _heap_n_wbuddy_release_block(_heap_pt heap, void * ptr) {
	int r, r2;
	_heap_region_pt region;
	//unsigned int offset;
	_heap_block_pt b1;
	unsigned int asize, rsize;
	unsigned int tag;

	b1 = _ptr_to_block_pt(ptr);

	region = &heap->region[_UBINOS__UBICLIB__HEAP_DIR];

	//offset	= region->fbloffset;

	_block_check_freeable_and_abort(b1);
	_block_check_boundary_and_abort(b1, 1);

	tag = b1->tag;

	heap_logmfd_block(heap, _UBINOS__UBICLIB__HEAP_DIR, b1, 1);
	_wbuddy_tag_check(tag, _UBINOS__UBICLIB__HEAP_DIR, region->min);

	asize = _wbuddy_tag_to_asize(tag);
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
	_block_set_tag(b1, tag, 1);

	_heap_n_wbuddy_combine_block(heap, b1, 0);

	region->asize -= asize;
	region->rsize -= rsize;

	_heap_n_wbuddy_reduce(heap);

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

unsigned int heap_wbuddy_calc_fblcount(unsigned int size, unsigned int m) {
	unsigned int k, w;
	unsigned int fbln;

	if (HEAP_BLOCK_ASIZE_MIN > size) {
		return 0;
	}

	_wbuddy_asize_to_kw(HEAP_BLOCK_ASIZE_MIN, k, w);
	fbln	= _wbuddy_kw_to_sn(k, w);
	_wbuddy_asize_to_kw(size, k, w);
	fbln	= _wbuddy_kw_to_sn(k, w) - fbln + 1 + 1;

	return fbln;
}

unsigned int heap_wbuddy_calc_fblcount_raw(unsigned int size, unsigned int m) {
	unsigned int k, w;
	unsigned int fbln;

	if (HEAP_BLOCK_ASIZE_MIN > size) {
		return 0;
	}

	_wbuddy_asize_to_kw(size, k, w);
	fbln	= _wbuddy_kw_to_sn(k, w);

	return fbln;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP == 1) */
