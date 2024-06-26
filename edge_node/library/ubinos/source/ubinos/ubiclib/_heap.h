/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <ubinos/bsp.h>
#include <ubinos/bsp_ubik.h>
#include <ubinos/ubiclib.h>

#if !defined(UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM)
	// maximum size = 256 KiB
	#define UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM		UBINOS__UBICLIB__HEAP_ALGORITHM__GROUP
	#define UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE			UBINOS__UBICLIB__HEAP_LOCK_TYPE__MUTEX
	#define UBINOS__UBICLIB__HEAP_DIR0_M				(8)
	#define UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT			(226)
	#define UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE	(36)
#endif

#if !defined(UBINOS__UBICLIB__HEAP_DIR1_ALGORITHM)
	#define UBINOS__UBICLIB__HEAP_DIR0_ALGORITHM		UBINOS__UBICLIB__HEAP_ALGORITHM__BESTFIT
	#define UBINOS__UBICLIB__HEAP_DIR0_LOCKTYPE			UBINOS__UBICLIB__HEAP_LOCK_TYPE__MUTEX
	#define UBINOS__UBICLIB__HEAP_DIR0_M				(2)
	#define UBINOS__UBICLIB__HEAP_DIR0_FBLCOUNT			(2)
	#define UBINOS__UBICLIB__HEAP_DIR0_FBLBM_BUFSIZE	(4)
#endif


#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_LOGMD == 1)
	#define heap_logmd(msg) logmd(msg)
	#define heap_logmfd(format, args...) logmfd(format, ## args)
#else
	#define heap_logmd(msg)
	#define heap_logmfd(format, args...)
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_LOGMD == 1) */


#define div_ceil(x, d)	((0 == (x)) ? 0 : ((x) / (d)) + ((0 == ((x) % (d))) ? 0 : 1))


#if	(2 == INT_SIZE)	// 16 bit machine

	#define HEAP_BOUNDARY_PATTERN		(0xA5B4)
	#define HEAP_BLOCK_ASIZE_MIN		(0x0020)				// 32

	#define HEAP_TAG_SIZE				INT_SIZE				// 2
	#define HEAP_TAG_BITSIZE			(HEAP_TAG_SIZE * 8)		// 16

#if (1 != UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY)
	#error "16 bit system can not support weighted buddy algorithm"
#else
	// definitions for group system and buddy systems
	#define HEAP_G_TAG_A_______BITSIZE	(1)						// available
	#define HEAP_G_TAG__D______BITSIZE	(1)						// direction
	#define HEAP_G_TAG___G_____BITSIZE	(1)						// is group system or buddy system

	#define HEAP_G_TAG____K____BITSIZE	(4)						// level
	#define HEAP_G_TAG_____L___BITSIZE	(3)						// weight of left tail
	#define HEAP_G_TAG______R__BITSIZE	(3)						// weight of right tail
	#define HEAP_G_TAG_______B_BITSIZE	(3)						// weight of body

	#define HEAP_G_TAG_A_______MASK		(0x8000)
	#define HEAP_G_TAG__D______MASK		(0x4000)
	#define HEAP_G_TAG___G_____MASK		(0x2000)
	#define HEAP_G_TAG____K____MASK		(0x1E00)
	#define HEAP_G_TAG_____L___MASK		(0x01C0)
	#define HEAP_G_TAG______R__MASK		(0x0038)
	#define HEAP_G_TAG_______B_MASK		(0x0007)

	#define HEAP_G_K_MAX				(15)					// 2 ^ HEAP_G_TAG____K____BITSIZE = 2 ^ 4 - 1 = 15
	#define HEAP_G_M_MAX				(8)						// 2 ^ HEAP_G_TAG_______B_BITSIZE = 2 ^ 3     = 8

	// definitions for linear search systems (best fit, first fit, last fit)
	#define HEAP_L_BLOCK_ASIZE_OFFSET	(1)						// 2 byte align
	#define HEAP_L_BLOCK_ASIZE_MAX		(0x3FFE)				// (2 ^ (16 - 3) - 1) * 2 = 16,382 = 0x3FFE
																// (2 ^ (bit size of machine - bit size of TAG_ADG) - zero value count) * (2 ^ HEAP_L_BLOCK_ASIZE_OFFSET)
#endif

#elif	(4 == INT_SIZE)	// 32 bit machine

	#define HEAP_BOUNDARY_PATTERN		(0xA5B4C75A)
	#define HEAP_BLOCK_ASIZE_MIN		(0x00000040)			// 64

	#define HEAP_TAG_SIZE				(INT_SIZE)				// 4
	#define HEAP_TAG_BITSIZE			(HEAP_TAG_SIZE * 8)		// 32

#if (1 != UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY)
	// definitions for group system and buddy systems
	#define HEAP_G_TAG_A_______BITSIZE	(1)						// available
	#define HEAP_G_TAG__D______BITSIZE	(1)						// direction
	#define HEAP_G_TAG___G_____BITSIZE	(4)						// is group system or buddy system (if weighted buddy, it also contains block type information)

	#define HEAP_G_TAG____K____BITSIZE	(5)						// level
	#define HEAP_G_TAG_____L___BITSIZE	(7)						// weight of left tail
	#define HEAP_G_TAG______R__BITSIZE	(7)						// weight of right tail
	#define HEAP_G_TAG_______B_BITSIZE	(7)						// weight of body

	#define HEAP_G_TAG_A_______MASK		(0x80000000)
	#define HEAP_G_TAG__D______MASK		(0x40000000)
	#define HEAP_G_TAG___G_____MASK		(0x3C000000)
	#define HEAP_G_TAG____K____MASK		(0x03E00000)
	#define HEAP_G_TAG_____L___MASK		(0x001FC000)
	#define HEAP_G_TAG______R__MASK		(0x00003F80)
	#define HEAP_G_TAG_______B_MASK		(0x0000007F)

	#define HEAP_G_K_MAX				(31)					// 2 ^ HEAP_G_TAG____K____BITSIZE = 2 ^ 5 - 1 = 31
	#define HEAP_G_M_MAX				(128)					// 2 ^ HEAP_G_TAG_______B_BITSIZE = 2 ^ 7     = 128

	// definitions for linear search systems (best fit, first fit, last fit)
	#define HEAP_L_BLOCK_ASIZE_OFFSET	(2)						// 4 byte align
	#define HEAP_L_BLOCK_ASIZE_MAX		(0x0FFFFFFC)			// (2 ^ (32 - 6) - 1) * 4 = 268,435,452 = 0x0FFFFFFC
																// (2 ^ (bit size of machine - bit size of TAG_ADG) - zero value count) * (2 ^ HEAP_L_BLOCK_ASIZE_OFFSET)
#else
	// definitions for group system and buddy systems
	#define HEAP_G_TAG_A_______BITSIZE	(1)						// available
	#define HEAP_G_TAG__D______BITSIZE	(1)						// direction
	#define HEAP_G_TAG___G_____BITSIZE	(1)						// is group system or buddy system

	#define HEAP_G_TAG____K____BITSIZE	(5)						// level
	#define HEAP_G_TAG_____L___BITSIZE	(8)						// weight of left tail
	#define HEAP_G_TAG______R__BITSIZE	(8)						// weight of right tail
	#define HEAP_G_TAG_______B_BITSIZE	(8)						// weight of body

	#define HEAP_G_TAG_A_______MASK		(0x80000000)
	#define HEAP_G_TAG__D______MASK		(0x40000000)
	#define HEAP_G_TAG___G_____MASK		(0x20000000)
	#define HEAP_G_TAG____K____MASK		(0x1F000000)
	#define HEAP_G_TAG_____L___MASK		(0x00FF0000)
	#define HEAP_G_TAG______R__MASK		(0x0000FF00)
	#define HEAP_G_TAG_______B_MASK		(0x000000FF)

	#define HEAP_G_K_MAX				(31)					// 2 ^ HEAP_G_TAG____K____BITSIZE = 2 ^ 5 - 1 = 31
	#define HEAP_G_M_MAX				(256)					// 2 ^ HEAP_G_TAG_______B_BITSIZE = 2 ^ 8     = 256

	// definitions for linear search systems (best fit, first fit, last fit)
	#define HEAP_L_BLOCK_ASIZE_OFFSET	(2)						// 4 byte align
	#define HEAP_L_BLOCK_ASIZE_MAX		(0x7FFFFFFC)			// (2 ^ (32 - 3) - 1) * 4 = 2,147,483,644 = 0x7FFFFFFC
																// (2 ^ (bit size of machine - bit size of TAG_ADG) - zero value count) * (2 ^ HEAP_L_BLOCK_ASIZE_OFFSET)
#endif

#else

	#error "Unsupported int size"

#endif

#define HEAP_G_TAG__DGKLRB_BITSIZE		(	HEAP_G_TAG__D______BITSIZE + 	\
											HEAP_G_TAG___G_____BITSIZE + 	\
											HEAP_G_TAG____K____BITSIZE + 	\
											HEAP_G_TAG_____L___BITSIZE + 	\
											HEAP_G_TAG______R__BITSIZE + 	\
											HEAP_G_TAG_______B_BITSIZE		)
#define HEAP_G_TAG___GKLRB_BITSIZE		(	HEAP_G_TAG___G_____BITSIZE + 	\
											HEAP_G_TAG____K____BITSIZE + 	\
											HEAP_G_TAG_____L___BITSIZE + 	\
											HEAP_G_TAG______R__BITSIZE + 	\
											HEAP_G_TAG_______B_BITSIZE		)
#define HEAP_G_TAG____KLRB_BITSIZE		(	HEAP_G_TAG____K____BITSIZE + 	\
											HEAP_G_TAG_____L___BITSIZE + 	\
											HEAP_G_TAG______R__BITSIZE + 	\
											HEAP_G_TAG_______B_BITSIZE		)
#define HEAP_G_TAG_____LRB_BITSIZE		(	HEAP_G_TAG_____L___BITSIZE + 	\
											HEAP_G_TAG______R__BITSIZE + 	\
											HEAP_G_TAG_______B_BITSIZE		)
#define HEAP_G_TAG______RB_BITSIZE		(	HEAP_G_TAG______R__BITSIZE +	\
											HEAP_G_TAG_______B_BITSIZE		)

#define HEAP_L_TAG_ASIZE_MASK			(	HEAP_G_TAG____K____MASK | 		\
									 	 	HEAP_G_TAG_____L___MASK | 		\
									 	 	HEAP_G_TAG______R__MASK | 		\
									 	 	HEAP_G_TAG_______B_MASK			)


#define _tag_to_a(tag)					(((unsigned int) (tag) & HEAP_G_TAG_A_______MASK) >> HEAP_G_TAG__DGKLRB_BITSIZE)
#define _tag_to_d(tag)					(((unsigned int) (tag) & HEAP_G_TAG__D______MASK) >> HEAP_G_TAG___GKLRB_BITSIZE)
#define _tag_to_g(tag)					(((unsigned int) (tag) & HEAP_G_TAG___G_____MASK) >> HEAP_G_TAG____KLRB_BITSIZE)

#define _tag_to_g_k(tag)				(((unsigned int) (tag) & HEAP_G_TAG____K____MASK) >> HEAP_G_TAG_____LRB_BITSIZE)
#define _tag_to_g_l(tag)				(((unsigned int) (tag) & HEAP_G_TAG_____L___MASK) >> HEAP_G_TAG______RB_BITSIZE)
#define _tag_to_g_r(tag)				(((unsigned int) (tag) & HEAP_G_TAG______R__MASK) >> HEAP_G_TAG_______B_BITSIZE)
#define _tag_to_g_b(tag)				(((unsigned int) (tag) & HEAP_G_TAG_______B_MASK)                              )

#define _tag_to_g_asize_b(tag, log2m)	( _tag_to_g_b(tag)                     << ((log2m) * _tag_to_g_k(tag))           )
#define _tag_to_g_asize_l(tag, log2m)	( _tag_to_g_l(tag)                     << ((log2m) * _tag_to_g_k(tag)) >> (log2m))
#define _tag_to_g_asize_r(tag, log2m)	( _tag_to_g_r(tag)                     << ((log2m) * _tag_to_g_k(tag)) >> (log2m))
#define _tag_to_g_asize_lr(tag, log2m)	((_tag_to_g_l(tag) + _tag_to_g_r(tag)) << ((log2m) * _tag_to_g_k(tag)) >> (log2m))
#define _tag_to_g_asize(tag, log2m)		(_tag_to_g_asize_b(tag, log2m) + _tag_to_g_asize_lr(tag, log2m))
#define _tag_to_l_asize(tag)			(((unsigned int) (tag) & HEAP_L_TAG_ASIZE_MASK) << HEAP_L_BLOCK_ASIZE_OFFSET)

#define _tag_to_asize(tag, log2m)		(_tag_to_g(tag) ? _tag_to_g_asize(tag, log2m) : _tag_to_l_asize(tag))

#define _tag_set_a(tag, a)				{																\
	(tag) &= (~HEAP_G_TAG_A_______MASK);																\
	(tag) |= (((unsigned int) (a    ) << HEAP_G_TAG__DGKLRB_BITSIZE ) & HEAP_G_TAG_A_______MASK);		\
}

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1)
	#define _tag_check(tag, dir, log2m, m, min)	{														\
		if (min > _tag_to_asize(tag, log2m)) {															\
			logme("tag is wrong (error type 1)");														\
			bsp_abortsystem();																			\
		}																								\
		if (dir != _tag_to_d(tag)) {																	\
			logme("tag is wrong (error type 2)");														\
			bsp_abortsystem();																			\
		}																								\
		if (0 != _tag_to_g(tag)) {																		\
			if (HEAP_G_K_MAX < _tag_to_g_k(tag)) {														\
				logme("tag is wrong (error type 3)");													\
				bsp_abortsystem();																		\
			}																							\
			if ((m) <= _tag_to_g_b(tag)) {																\
				logme("tag is wrong (error type 4)");													\
				bsp_abortsystem();																		\
			}																							\
			if ((m) <= _tag_to_g_l(tag)) {																\
				logme("tag is wrong (error type 5)");													\
				bsp_abortsystem();																		\
			}																							\
			if ((m) <= _tag_to_g_r(tag)) {																\
				logme("tag is wrong (error type 6)");													\
				bsp_abortsystem();																		\
			}																							\
		}																								\
	}
#else
    #define _tag_check(tag, dir, log2m, m, min)
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1) */


#define _kblr_to_tag(k, b, l, r, a, d) ( 																\
	(((unsigned int) (a    ) << HEAP_G_TAG__DGKLRB_BITSIZE ) & HEAP_G_TAG_A_______MASK) |				\
	(((unsigned int) (d    ) << HEAP_G_TAG___GKLRB_BITSIZE ) & HEAP_G_TAG__D______MASK) |				\
	(((unsigned int) (1    ) << HEAP_G_TAG____KLRB_BITSIZE ) & HEAP_G_TAG___G_____MASK) |				\
	(((unsigned int) (k    ) << HEAP_G_TAG_____LRB_BITSIZE ) & HEAP_G_TAG____K____MASK) |				\
	(((unsigned int) (l    ) << HEAP_G_TAG______RB_BITSIZE ) & HEAP_G_TAG_____L___MASK) |				\
	(((unsigned int) (r    ) << HEAP_G_TAG_______B_BITSIZE ) & HEAP_G_TAG______R__MASK) |				\
	(((unsigned int) (b    )                               ) & HEAP_G_TAG_______B_MASK)	)

#define _asize_to_tag_l(asize, a, d) ( 																	\
	(((unsigned int) (a    ) << HEAP_G_TAG__DGKLRB_BITSIZE ) & HEAP_G_TAG_A_______MASK) |				\
	(((unsigned int) (d    ) << HEAP_G_TAG___GKLRB_BITSIZE ) & HEAP_G_TAG__D______MASK) |				\
	(((unsigned int) (asize) >> HEAP_L_BLOCK_ASIZE_OFFSET  ) & HEAP_L_TAG_ASIZE_MASK  )	)


#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_BOUNDARY_CHECK == 1)

	#define BOUNDARY_SIZE					INT_SIZE

	typedef	struct __heap_block_t {
		unsigned int	boundary;
		unsigned int	tag;
		struct {
			struct __heap_block_t *	prev;
			struct __heap_block_t *	next;
			edlist_pt				list;
		}				heap_blocklist_link;
		unsigned int	rsize;
	} _heap_block_t;

	typedef _heap_block_t * _heap_block_pt;

	#define HEAP_BLOCK_HEADER_SIZE		(MEM_ALIGN(sizeof(_heap_block_t)))
	#define HEAP_BLOCK_OVERHEAD			(HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE + HEAP_TAG_SIZE + BOUNDARY_SIZE)

	#define _set_header_boundary(block)																												\
		(*((unsigned int *) ((unsigned int) (block)))														= HEAP_BOUNDARY_PATTERN)
	#define _set_top_boundary(block)																												\
		(*((unsigned int *) ((unsigned int) (block) + HEAP_BLOCK_HEADER_SIZE)) 								= HEAP_BOUNDARY_PATTERN)
	#define _set_bottom_boundary(block, log2m)																										\
		(*((unsigned int *) ((unsigned int) (block) + _tag_to_asize((block)->tag, log2m) - BOUNDARY_SIZE))	= HEAP_BOUNDARY_PATTERN)
	#define _set_boundary(block, log2m) { 																											\
		_set_header_boundary(block);																												\
		_set_top_boundary(block);																													\
		_set_bottom_boundary(block, log2m);																											\
	}

	#define _check_header_boundary(block)																											\
		((HEAP_BOUNDARY_PATTERN == *((unsigned int *) ((unsigned int) (block))))														? 1 : 0)
	#define _check_top_boundary(block)																												\
		((HEAP_BOUNDARY_PATTERN == *((unsigned int *) ((unsigned int) (block) + HEAP_BLOCK_HEADER_SIZE)))								? 1 : 0)
	#define _check_bottom_boundary(block, log2m)																									\
		((HEAP_BOUNDARY_PATTERN == *((unsigned int *) ((unsigned int) (block) + _tag_to_asize((block)->tag, log2m) - BOUNDARY_SIZE)))	? 1 : 0)
	#define _check_boundary(block, log2m) (																											\
		_check_header_boundary(block) &&																											\
		_check_top_boundary(block) && 																												\
		_check_bottom_boundary(block, log2m)																										\
	)

	#define _check_boundary_and_abort(block, log2m) {																								\
		if 		(!_check_header_boundary(block)) {																									\
			logmfe("memory(0x%08x) is polluted (head boundary)",		(unsigned int) (block) + (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE));			\
			bsp_abortsystem();																														\
		}																																			\
		else if	(!_check_top_boundary(block)) {																										\
			logmfe("memory(0x%08x) is polluted (top boundary)", 		(unsigned int) (block) + (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE));			\
			bsp_abortsystem();																														\
		}																																			\
		else if	(!_check_bottom_boundary(block, log2m)) {																							\
			logmfe("memory(0x%08x) is polluted (bottom boundary)", 	(unsigned int) (block) + (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE));				\
			bsp_abortsystem();																														\
		}																																			\
	}

	#define _check_freeable_and_abort(block) {																										\
		if (_tag_to_a((block)->tag) != 0) {																											\
			logmfe("memory(0x%08x) is not freeable (not allocated)",	(unsigned int) (block) + (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE));			\
			bsp_abortsystem();																														\
		}																																			\
	}

#else

	#define BOUNDARY_SIZE					0

	typedef	struct __heap_block_t {
		unsigned int	tag;
		struct {
			struct __heap_block_t *	prev;
			struct __heap_block_t *	next;
			edlist_pt				list;
		}				heap_blocklist_link;
		unsigned int	rsize;
	} _heap_block_t;


	typedef _heap_block_t * _heap_block_pt;

	#define HEAP_BLOCK_HEADER_SIZE		(MEM_ALIGN(sizeof(_heap_block_t)))
	#define HEAP_BLOCK_OVERHEAD			(HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE + HEAP_TAG_SIZE + BOUNDARY_SIZE)

	#define _set_header_boundary(block)
	#define _set_top_boundary(block)
	#define _set_bottom_boundary(block, log2m)
	#define _set_boundary(block, log2m)

	#define _check_header_boundary(block)				1
	#define _check_top_boundary(block)					1
	#define _check_bottom_boundary(block, log2m)		1
	#define _check_boundary(block, log2m)				1

	#define _check_boundary_and_abort(block, log2m)
	#define _check_freeable_and_abort(block)

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_BOUNDARY_CHECK == 1) */


#define _tag_to_size(tag, log2m)					(_tag_to_asize(tag, log2m) - HEAP_BLOCK_OVERHEAD)

#define	_ptr_to_block_pt(ptr)						((_heap_block_pt)   ((unsigned int) (ptr  ) - (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE)))
#define	_block_pt_to_ptr(block)						((void *)           ((unsigned int) (block) + (HEAP_BLOCK_HEADER_SIZE + BOUNDARY_SIZE)))
#define	_block_pt_to_end_prt(block, log2m)			((void *)           ((unsigned int) (block) + _tag_to_asize((block)->tag, log2m)))
#define	_block_pt_to_endtag_ptr(block, log2m)		((unsigned int *)   ((unsigned int) (block) + _tag_to_asize((block)->tag, log2m) - (HEAP_TAG_SIZE + BOUNDARY_SIZE)))
#define _block_pt_to_upper_endtag(block)			(*((unsigned int *) ((unsigned int) (block) - (HEAP_TAG_SIZE + BOUNDARY_SIZE))))
#define _block_pt_to_upper_block_pt(block, log2m)	((_heap_block_pt)   ((unsigned int) (block) - _tag_to_asize(_block_pt_to_upper_endtag(block), log2m)))
#define _block_pt_to_lower_block_pt(block, log2m)	((_heap_block_pt) _block_pt_to_end_prt(block, log2m))

#define _heap_blocklist_insertprev(blocklist, ref, block)	edlist_insertprev(	_heap_block_pt, heap_blocklist_link, blocklist, ref, block)
#define _heap_blocklist_insertnext(blocklist, ref, block)	edlist_insertnext(	_heap_block_pt, heap_blocklist_link, blocklist, ref, block)
#define _heap_blocklist_remove(block)						edlist_remove(		_heap_block_pt, heap_blocklist_link, block)
#define _heap_blocklist_getcur(blocklist)					edlist_getcur(		_heap_block_pt, heap_blocklist_link, blocklist)
#define _heap_blocklist_getcurnext(blocklist)				edlist_getcurnext(	_heap_block_pt, heap_blocklist_link, blocklist)
#define _heap_blocklist_head(blocklist)						edlist_head(		_heap_block_pt, heap_blocklist_link, blocklist)
#define _heap_blocklist_tail(blocklist)						edlist_tail(		_heap_block_pt, heap_blocklist_link, blocklist)
#define _heap_blocklist_next(block)							edlist_next(		_heap_block_pt, heap_blocklist_link, block)
#define _heap_blocklist_prev(block)							edlist_prev(		_heap_block_pt, heap_blocklist_link, block)
#define _heap_blocklist_list(block)							edlist_list(		_heap_block_pt, heap_blocklist_link, block)


typedef struct {
	unsigned int		dir;
	unsigned int		algorithm;
	unsigned int		locktype;

	unsigned int		m;
	unsigned int		log2m;
	unsigned int		maskm;
	unsigned int		min;

	unsigned int		size_min;
	unsigned int		size;
	unsigned int		k;
	unsigned int		w;
	unsigned int		t;
	unsigned int		addr;
	unsigned int		end;

	unsigned int		limit;

	unsigned int		fblcount; // free block list count
	unsigned int		fbloffset; // free block list index offset (calculated from HEAP_BLOCK_ASIZE_MIN)
	edlist_pt			fbl_ap; // free block list pointer array
	bitmap_pt			fblbm; // free block list index empty bitmap pointer

	unsigned int		dregs_size; //total size of blocks that cannot be used because they are smaller than the minimum allocation size

	edlist_t			abl;

	unsigned int		acount_max;
	unsigned int		asize;
	unsigned int		asize_max;
	unsigned int		rsize;
	unsigned int 		rsize_max;

	bsp_mutex_pt		mutex;
} _heap_region_t;

typedef _heap_region_t * _heap_region_pt;

typedef struct __heap_t {
	unsigned int		type			:  8;
	unsigned int		valid			:  1;
	unsigned int		reserved		:  7;
	unsigned int		reserved2		: 16;

	unsigned int		size;
	unsigned int 		addr;
	unsigned int		end;

	unsigned int		acount_max;
	unsigned int		asize_max;
	unsigned int		rsize_max;

	unsigned char		enable_dmpm; // enable dynamic memory power management
	_heap_region_t		region[2]; // heap region array, 0: normal direction, 1: reverse direction

	void * (* allocate_block_afp[2]) (struct __heap_t *, unsigned int); // allocate block function pointer array, 0: normal direction, 1: reverse direction
	int    (*  release_block_afp[2]) (struct __heap_t *, void *); // release block function pointer array, 0: normal direction, 1: reverse direction
} _heap_t;

typedef _heap_t * _heap_pt;


#define	_size_to_asize(size, min)		(MEM_ALIGN(max(((size) + HEAP_BLOCK_OVERHEAD), min)))

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1)
	#define _kwt_check(k, w, t, log2m, m) {																		\
		if (HEAP_G_K_MAX < (k)) {																				\
			logme("kwt is wrong (error type 1)");																\
			bsp_abortsystem();																					\
		}																										\
		if (m <= (w)) {																							\
			logme("kwt is wrong (error type 2)");																\
			bsp_abortsystem();																					\
		}																										\
		if (m <= (t)) {																							\
			logme("kwt is wrong (error type 3)");																\
			bsp_abortsystem();																					\
		}																										\
		if (0 == (w)) {																							\
			logme("kwt is wrong (error type 4)");																\
			bsp_abortsystem();																					\
		}																										\
		if (0 == (k) && 0 != (t)) {																				\
			logme("kwt is wrong (error type 5)");																\
			bsp_abortsystem();																					\
		}																										\
	}
#else
    #define _kwt_check(k, w, t, log2m, m)
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1) */

#define _kwt_refine(k, w, t, log2m, m, maskm) {																	\
	(w) += ((t) >> (log2m));																					\
	(t)  = (t) & (maskm);                                                 										\
	if (0 == (w) && 0 < (k)) {																					\
		(k) -= (1);																								\
		(w)  = (t);																								\
		(t)  = (0);																								\
	}																											\
	if ((m) == (w) && 0 == (t)) {																				\
		(k) += (1);																								\
		(w)  = (1);																								\
	}																											\
	_kwt_check(k, w, t, log2m, m);																				\
}

#define _asize_to_kwt(asize, k, w, t, log2m, m)	{																\
	(k) = uilog2_floor(asize) / (log2m);																		\
	(w) = (unsigned int) (asize) >> ((log2m) * (k));															\
	(t) = div_ceil(	(asize) - ((unsigned int) (w) << ((log2m) * (k))            ), 								\
						      ((unsigned int) 0x1 << ((log2m) * (k)) >> (log2m) )	);							\
	if ((m) == (t)) {																							\
		(w) += (1);																								\
		(t)  = (0);																								\
	}																											\
	if ((m) == (w)) {																							\
		(k) += (1);																								\
		(w)  = (1);																								\
	}																											\
	_kwt_check(k, w, t, log2m, m);																				\
}

#define _kwt_to_asize(k, w, t, log2m)		(	((unsigned int) (w) << ((log2m) * (k))           ) + 			\
												((unsigned int) (t) << ((log2m) * (k)) >> (log2m))		)

#define _kwt_to_asize_r(k, w, t, log2m, m)	( 																	\
	(0 == w && 0 == t) ? (0) : 																					\
		(	(0 == w) ? ((unsigned int) ((m) - (t)) << ((log2m) * (k)) >> (log2m)) : 							\
				(	   ((unsigned int) ((m) - (t)) << ((log2m) * (k)) >> (log2m)) +								\
					   ((unsigned int) ((m) - (w)) << ((log2m) * (k))           )	)	)	)

#define _kwt_to_sn(k, w, t, log2m, m) 		(	(0 == k) ? 														\
												(w) : 															\
												((((((unsigned int) (m)-1) * ((k)-1)) + (w)) * (m)) + (t))	)

#define _sn_to_index(sn, offset, log2m, m)  (	(offset) < (sn) ? (sn) - (offset) : 0		)

#define _kblr_to_kwt(k, b, l, r, k2, w2, t2, log2m, m, maskm) {													\
	(k2) = (k);																									\
	(w2) = (b);																									\
	(t2) = (l) + (r);																							\
	_kwt_refine(k2, w2, t2, log2m, m, maskm);																	\
}

#define _kblr_to_asize(k, b, l, r, log2m) 	_kwt_to_asize(k, b, (l) + (r), log2m)

#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1)
	#define _kblr_check(k, b, l, r, log2m, m) {																	\
		if (HEAP_G_K_MAX < (k)) {																				\
			logme("kblr is wrong (error type 1)");																\
			bsp_abortsystem();																					\
		}																										\
		if (m <= (b)) {																							\
			logme("kblr is wrong (error type 2)");																\
			bsp_abortsystem();																					\
		}																										\
		if (m <= (l)) {																							\
			logme("kblr is wrong (error type 3)");																\
			bsp_abortsystem();																					\
		}																										\
		if (m <= (r)) {																							\
			logme("kblr is wrong (error type 4)");																\
			bsp_abortsystem();																					\
		}																										\
		if (0 == (b) &&  0 == (l) && 0 == (r) ) {																\
			logme("kblr is wrong (error type 5)");																\
			bsp_abortsystem();																					\
		}																										\
		if (0 == (b) && (0 == (l) || 0 == (r))) {																\
			logme("kblr is wrong (error type 6)");																\
			bsp_abortsystem();																					\
		}																										\
		if (0 == (k) && (0 != (l) || 0 != (r))) {																\
			logme("kblr is wrong (error type 7)");																\
			bsp_abortsystem();																					\
		}																										\
	}
#else
    #define _kblr_check(k, b, l, r, log2m, m)
#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_INTERMEDIATE_DATA_CHECK == 1) */

#define _kblr_refine(k, b, l, r, log2m, m, maskm) {																\
	(b) += (((l) >> (log2m)) + ((r) >> (log2m)));																\
	(l)  = (l) & (maskm);                                                 										\
	(r)  = (r) & (maskm);                                                 										\
	if (0 == (b) && (0 == (l) || 0 == (r)) && 0 < (k)) {														\
		(k) -= 1;																								\
		(b)  = (l) + (r);																						\
		(l)  = 0;																								\
		(r)  = 0;																								\
	}																											\
	if ((m) == (b) && 0 == (l) && 0 == (r)) {																	\
		(k) += 1;																								\
		(b)  = 1;																								\
	}																											\
	_kblr_check(k, b, l, r, log2m, m)																			\
}

#define _tag_to_kblr(tag, k, b, l, r, log2m, m)	{																\
		(k) = _tag_to_g_k(tag);																					\
		(b) = _tag_to_g_b(tag);																					\
		(l) = _tag_to_g_l(tag);																					\
		(r) = _tag_to_g_r(tag);																					\
		_kblr_check(k, b, l, r, log2m, m);																		\
}

#define _block_check_freeable_and_abort(block)				_check_freeable_and_abort(block)
#define _block_check_boundary(block, log2m)					_check_boundary(block, log2m)
#define _block_check_boundary_and_abort(block, log2m)		_check_boundary_and_abort(block, log2m)
#define _block_set_boundary(block, log2m)					_set_boundary(block, log2m)

#define _block_set_tag(block, value, log2m) {																	\
	(block)->tag 							= value;															\
	*_block_pt_to_endtag_ptr(block, log2m)	= value;															\
	_block_set_boundary(block, log2m);																			\
}

#define _block_set_rsize(block, value)		{	block->rsize = value;	}

#define _block_get_gid(block, region, k, b, l, r, gid, sk, sgid, log2m)	{										\
	if (0 == (region)->dir) {                                                                           		\
		gid = ((unsigned int) (block) - (region)->addr   ) >> ((log2m) * ((k) + 1));							\
	}                                                                                                   		\
	else {                                                                                              		\
		gid = ((region)->end - (unsigned int) (block) - 1) >> ((log2m) * ((k) + 1));     						\
	}                                                                                                   		\
	if (0 == (l) && 0 == (r)) {																					\
		(sk)	= (k) + 1;																						\
		(sgid)	= (gid) >> (log2m);																				\
	}																											\
	else {																										\
		(sk)	= (k);																							\
		(sgid)	= (gid);																						\
	}																											\
}

#define _region_insert_fb_head(region, index, block, log2m) {													\
	_heap_blocklist_insertnext(&((region)->fbl_ap[(index)]), NULL, (block));									\
	if (0 < (index)) {																							\
		if (1 == (region)->fbl_ap[(index)].count) {																\
			bitmap_setbit((region)->fblbm, (index), 1);															\
		}																										\
	}																											\
	else {																										\
		(region)->dregs_size += _tag_to_asize((block)->tag, (log2m));											\
	}																											\
}

#define _region_insert_fb_tail(region, index, block, log2m) {													\
	_heap_blocklist_insertprev(&((region)->fbl_ap[(index)]), NULL, (block));									\
	if (0 < (index)) {																							\
		if (1 == (region)->fbl_ap[(index)].count) {																\
			bitmap_setbit((region)->fblbm, (index), 1);															\
		}																										\
	}																											\
	else {																										\
		(region)->dregs_size += _tag_to_asize((block)->tag, (log2m));											\
	}																											\
}

#define _region_remove_fb(region, index, block, log2m) {														\
	if (0 < (index)) {																							\
		if (1 == (region)->fbl_ap[(index)].count) {																\
			bitmap_setbit((region)->fblbm, (index), 0);															\
		}																										\
	}																											\
	else {																										\
		(region)->dregs_size -= _tag_to_asize((block)->tag, (log2m));											\
	}																											\
	_heap_blocklist_remove(block)																				\
}

#define _region_get_fb_head(region, index)			_heap_blocklist_head(&((region)->fbl_ap[(index)]))

#define _region_get_fb_tail(region, index)			_heap_blocklist_tail(&((region)->fbl_ap[(index)]))

#define _region_get_fb_getcur(region, index)		_heap_blocklist_getcur(&((region)->fbl_ap[(index)]))

#define _region_get_fb_getcurnext(region, index)	_heap_blocklist_getcurnext(&((region)->fbl_ap[(index)]))


int _heap_init( _heap_pt heap, unsigned int addr, unsigned int size, int enable_dmpm,
				   int algorithm0, int locktype0, unsigned int m0,
				   unsigned int fblcount0, edlist_pt fbl0_p, bitmap_pt fblbm0,
				   int algorithm1, int locktype1, unsigned int m1,
				   unsigned int fblcount1, edlist_pt fbl1_p, bitmap_pt fblbm1	);

void * _heap_allocate_block(_heap_pt heap, int dir, unsigned int size);
int _heap_release_block(_heap_pt heap, void * ptr);

int _heap_power_off_unused_area(unsigned int nomal_resign_end, unsigned int reverse_resign_addr);
int _heap_print_power_info(void);

extern _heap_pt _ubiclib_heap;


#if 0
#define heap_logmfd_block_created(heap, dir, block, log2m)																								\
	heap_logmfd("0x%08x: created : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u", 			\
			bsp_task_getcur(), heap, dir, block, _block_pt_to_ptr(block), _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag)	)

#define heap_logmfd_block_deleted(heap, dir, block, log2m)																								\
	heap_logmfd("0x%08x: deleted : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u", 			\
			bsp_task_getcur(), heap, dir, block, _block_pt_to_ptr(block), _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag)	)

#define heap_logmfd_block_inserted(heap, dir, block, ix, log2m)																							\
	heap_logmfd("0x%08x: inserted: heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u, i %4u", 	\
			bsp_task_getcur(), heap, dir, block, _block_pt_to_ptr(block), _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag),										\
			ix																										)
#define heap_logmfd_block_removed(heap, dir, block, ix, log2m)																							\
	heap_logmfd("0x%08x: removed : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u, i %4u", 	\
			bsp_task_getcur(), heap, dir, block, _block_pt_to_ptr(block), _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag),										\
			ix																										)

#define heap_logmfd_block(heap, dir, block, log2m)																										\
	heap_logmfd("0x%08x:         : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u", 			\
			bsp_task_getcur(), heap, dir, block, _block_pt_to_ptr(block), _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag)	)

#else

#define heap_logmfd_block_created(heap, dir, block, log2m)																								\
	heap_logmfd("0x%08x: created : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u", 			\
			bsp_task_getcur(), heap, dir, block, (((unsigned int) block - (unsigned int) heap->region[dir].addr)) , _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag)	)

#define heap_logmfd_block_deleted(heap, dir, block, log2m)																								\
	heap_logmfd("0x%08x: deleted : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u", 			\
			bsp_task_getcur(), heap, dir, block, (((unsigned int) block - (unsigned int) heap->region[dir].addr)) , _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag)	)

#define heap_logmfd_block_inserted(heap, dir, block, ix, log2m)																							\
	heap_logmfd("0x%08x: inserted: heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u, i %4u", 	\
			bsp_task_getcur(), heap, dir, block, (((unsigned int) block - (unsigned int) heap->region[dir].addr)) , _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag),										\
			ix																										)
#define heap_logmfd_block_removed(heap, dir, block, ix, log2m)																							\
	heap_logmfd("0x%08x: removed : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u, i %4u", 	\
			bsp_task_getcur(), heap, dir, block, (((unsigned int) block - (unsigned int) heap->region[dir].addr)) , _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag),										\
			ix																										)

#define heap_logmfd_block(heap, dir, block, log2m)																										\
	heap_logmfd("0x%08x:         : heap 0x%08x, dir %d, block 0x%08x (0x%08x, 0x%08x), asize 0x%08x, %d %d %d, k %4u, b %4u, l %4u, r %4u", 			\
			bsp_task_getcur(), heap, dir, block, (((unsigned int) block - (unsigned int) heap->region[dir].addr)) , _block_pt_to_end_prt(block, log2m), _tag_to_asize(block->tag, log2m),			\
			_tag_to_a(block->tag), _tag_to_d(block->tag), _tag_to_g(block->tag) & 0x3,																	\
			_tag_to_g_k(block->tag), _tag_to_g_b(block->tag), _tag_to_g_l(block->tag), _tag_to_g_r(block->tag)	)

#endif


#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1)

           int _heap_n_group_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_n_group_expand(_heap_pt heap, unsigned int asize);
          int  _heap_n_group_reduce(_heap_pt heap);

_heap_block_pt _heap_n_group_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_n_group_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
        void * _heap_n_group_allocate_block(_heap_pt heap, unsigned int size);
           int _heap_n_group_release_block(_heap_pt heap, void * ptr);

           int _heap_r_group_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_r_group_expand(_heap_pt heap, unsigned int asize);
		  int  _heap_r_group_reduce(_heap_pt heap);

_heap_block_pt _heap_r_group_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_r_group_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
	    void * _heap_r_group_allocate_block(_heap_pt heap, unsigned int size);
		   int _heap_r_group_release_block(_heap_pt heap, void * ptr);

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__GROUP == 1) */


#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1)

		   int _heap_n_bestfit_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_n_bestfit_expand(_heap_pt heap, unsigned int asize);
          int  _heap_n_bestfit_reduce(_heap_pt heap);

_heap_block_pt _heap_n_bestfit_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_n_bestfit_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
        void * _heap_n_bestfit_allocate_block(_heap_pt heap, unsigned int size);
           int _heap_n_bestfit_release_block(_heap_pt heap, void * ptr);

           int _heap_r_bestfit_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_r_bestfit_expand(_heap_pt heap, unsigned int asize);
		  int  _heap_r_bestfit_reduce(_heap_pt heap);

_heap_block_pt _heap_r_bestfit_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_r_bestfit_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
	    void * _heap_r_bestfit_allocate_block(_heap_pt heap, unsigned int size);
		   int _heap_r_bestfit_release_block(_heap_pt heap, void * ptr);

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BESTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__FIRSTFIT == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__NEXTFIT == 1) */


#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1)

           int _heap_n_pgroup_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_n_pgroup_expand(_heap_pt heap, unsigned int asize);
          int  _heap_n_pgroup_reduce(_heap_pt heap);

_heap_block_pt _heap_n_pgroup_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_n_pgroup_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
        void * _heap_n_pgroup_allocate_block(_heap_pt heap, unsigned int size);
           int _heap_n_pgroup_release_block(_heap_pt heap, void * ptr);

           int _heap_r_pgroup_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_r_pgroup_expand(_heap_pt heap, unsigned int asize);
		  int  _heap_r_pgroup_reduce(_heap_pt heap);

_heap_block_pt _heap_r_pgroup_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_r_pgroup_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
	    void * _heap_r_pgroup_allocate_block(_heap_pt heap, unsigned int size);
		   int _heap_r_pgroup_release_block(_heap_pt heap, void * ptr);

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__PGROUP == 1) || !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__BBUDDY == 1) */


#if !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1)

           int _heap_n_wbuddy_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_n_wbuddy_expand(_heap_pt heap, unsigned int asize);
          int  _heap_n_wbuddy_reduce(_heap_pt heap);

_heap_block_pt _heap_n_wbuddy_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_n_wbuddy_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
        void * _heap_n_wbuddy_allocate_block(_heap_pt heap, unsigned int size);
           int _heap_n_wbuddy_release_block(_heap_pt heap, void * ptr);

           int _heap_r_wbuddy_init_region(
        	     _heap_pt heap, unsigned int addr, unsigned int size, int locktype,
           	     unsigned int m, unsigned int fblcount, edlist_pt fbl_p, bitmap_pt fblbm			);

_heap_block_pt _heap_r_wbuddy_expand(_heap_pt heap, unsigned int asize);
		  int  _heap_r_wbuddy_reduce(_heap_pt heap);

_heap_block_pt _heap_r_wbuddy_combine_block(_heap_pt heap, _heap_block_pt block, int endflag);
_heap_block_pt _heap_r_wbuddy_split_block(_heap_pt heap, _heap_block_pt block, unsigned int asize);
	    void * _heap_r_wbuddy_allocate_block(_heap_pt heap, unsigned int size);
		   int _heap_r_wbuddy_release_block(_heap_pt heap, void * ptr);

#endif /* !(UBINOS__UBICLIB__EXCLUDE_HEAP_ALGORITHM__WBUDDY == 1) */

#ifdef	__cplusplus
}
#endif

#endif /* _HEAP_H_ */
