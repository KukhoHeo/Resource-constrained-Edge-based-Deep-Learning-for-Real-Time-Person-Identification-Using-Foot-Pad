/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubiclib.h"

#if !(UBINOS__UBICLIB__EXCLUDE_CIRBUF == 1)

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

int cirbuf_create(cirbuf_pt * cirbuf_p, unsigned int maxsize) {
	return cirbuf_create_ext(cirbuf_p, maxsize, 0);
}

int cirbuf_create_ext(cirbuf_pt * cirbuf_p, unsigned int maxsize, unsigned int option) {
	cirbuf_pt cirbuf;

	assert(cirbuf_p != NULL);

	if (NULL == cirbuf_p) {
		logme("parameter 1 is wrong");
		return -2;
	}

	if (0 >= maxsize) {
		logme("parameter 2 is wrong");
		return -3;
	}

	cirbuf = malloc(sizeof(cirbuf_t) + maxsize);
	if (NULL == cirbuf) {
		logme("fail at malloc()");
		return -1;
	}

	cirbuf_init(cirbuf, maxsize);

	if (0 == (option & CIRBUF_OPT__NOOVERWRITE)) {
		cirbuf->overwrite = 1;
	}

	if (0 != (option & CIRBUF_OPT__MTPROTECTION)) {
		cirbuf->mtprotection = 1;
	}

	*cirbuf_p = cirbuf;

	return 0;
}

int cirbuf_delete(cirbuf_pt * cirbuf_p) {
	cirbuf_pt cirbuf;

	assert(cirbuf_p != NULL);
	assert(*cirbuf_p != NULL);

	if (NULL == cirbuf_p) {
		logme("parameter 1 is wrong");
		return -2;
	}

	if (NULL == *cirbuf_p) {
		logme("parameter 1 is wrong");
		return -2;
	}

	cirbuf = *cirbuf_p;

	free(cirbuf);

	*cirbuf_p = NULL;

	return 0;
}


int cirbuf_write(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * written_p) {
	unsigned char *	cirbuf__head;
	unsigned char *	cirbuf__tail;
	unsigned int	cirbuf__size;
	unsigned int 	offset;
	unsigned int 	size1;
	unsigned int 	size2;
	unsigned int 	sizeold;

	assert(cirbuf != NULL);
	assert(buf != NULL);

	if (NULL == cirbuf) {
		logme("parameter 1 is wrong");
		return -2;
	}

	if (NULL == buf) {
		logme("parameter 2 is wrong");
		return -3;
	}

	if (0 == size) {
		if (NULL != written_p) {
			*written_p = size;
		}
		return 0;
	}

	if (cirbuf->maxsize < size)
	{
		logme("parameter 3 is wrong");
		return -4;
	}

	if (0 != cirbuf->mtprotection) {
		bsp_ubik_entercrit();
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
		bsp_ubik_exitcrit();
	}
	else {
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
	}

	size1 = cirbuf->maxsize - cirbuf__size;
	if (size1 < size) {
		if (UINT16_MAX > cirbuf->overflowcount) {
			cirbuf->overflowcount++;
		}
		if (0 == cirbuf->overwrite) {
			size = size1;
		}
	}

	if (0 == size) {
		if (NULL != written_p) {
			*written_p = size;
		}
		return 0;
	}

	offset = cirbuf__tail - cirbuf->buf;

	size1 = cirbuf->maxsize - offset;
	if (size1 >= size) {
		size1 = size;
		size2 = 0;
	}
	else {
		size2 = size - size1;
	}

	if (size1 > 0) {
		memcpy(cirbuf__tail, buf, size1);
	}

	if (size2 > 0) {
		memcpy(cirbuf->buf, buf + size1, size2);
	}

	cirbuf__tail = (offset + size) % cirbuf->maxsize + cirbuf->buf;
	sizeold = cirbuf__size;
	cirbuf__size += size;
	if (cirbuf->maxsize < cirbuf__size || sizeold > cirbuf__size) {
		cirbuf__head = cirbuf__tail;
		cirbuf__size = cirbuf->maxsize;
		if (UINT16_MAX > cirbuf->overflowcount) {
			cirbuf->overflowcount++;
		}
	}

	if (NULL != written_p) {
		*written_p = size;
	}

	if (0 != cirbuf->mtprotection) {
		bsp_ubik_entercrit();
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
		bsp_ubik_exitcrit();
	}
	else {
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
	}

	return 0;
}

int cirbuf_read(cirbuf_pt cirbuf, unsigned char * buf, unsigned int size, unsigned int * read_p) {
	unsigned char *	cirbuf__head;
	unsigned char *	cirbuf__tail;
	unsigned int	cirbuf__size;
	unsigned int 	offset;
	unsigned int 	size1;
	unsigned int 	size2;

	assert(cirbuf != NULL);
	assert(buf != NULL);

	if (NULL == cirbuf) {
		logme("parameter 1 is wrong");
		return -2;
	}

	if (NULL == buf) {
		logme("parameter 2 is wrong");
		return -3;
	}

	if (0 == size) {
		if (NULL != read_p) {
			*read_p = size;
		}
		return 0;
	}

	if (0 != cirbuf->mtprotection) {
		bsp_ubik_entercrit();
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
		bsp_ubik_exitcrit();
	}
	else {
		cirbuf__head = cirbuf->head;
		cirbuf__tail = cirbuf->tail;
		cirbuf__size = cirbuf->size;
	}

	if (cirbuf__size < size) {
		logme("parameter 3 is wrong");
		return -4;
	}

	offset = cirbuf__head - cirbuf->buf;

	size1 = cirbuf->maxsize - offset;
	if (size1 >= size) {
		size1 = size;
		size2 = 0;
	}
	else {
		size2 = size - size1;
	}

	if (size1 > 0) {
		memcpy(buf, cirbuf__head, size1);
	}

	if (size2 > 0) {
		memcpy(buf + size1, cirbuf->buf, size2);
	}

	cirbuf__head = (offset + size) % cirbuf->maxsize + cirbuf->buf;
	cirbuf__size -= size;

	if (NULL != read_p) {
		*read_p = size;
	}

	if (0 != cirbuf->mtprotection) {
		bsp_ubik_entercrit();
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
		bsp_ubik_exitcrit();
	}
	else {
		cirbuf->head = cirbuf__head;
		cirbuf->tail = cirbuf__tail;
		cirbuf->size = cirbuf__size;
	}

	return 0;
}

int cirbuf_clear(cirbuf_pt cirbuf) {
	assert(cirbuf != NULL);

	if (NULL == cirbuf) {
		logme("parameter 1 is wrong");
		return -2;
	}

	if (0 != cirbuf->mtprotection) {
		bsp_ubik_entercrit();
		cirbuf->head			= cirbuf->buf;
		cirbuf->tail			= cirbuf->buf;
		cirbuf->size			= 0;
		cirbuf->overflowcount	= 0;
		bsp_ubik_exitcrit();
	}
	else {
		cirbuf->head			= cirbuf->buf;
		cirbuf->tail			= cirbuf->buf;
		cirbuf->size			= 0;
		cirbuf->overflowcount	= 0;
	}

	return 0;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_CIRBUF == 1) */
