/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubiclib.h"

#if !(UBINOS__UBICLIB__EXCLUDE_DLIST == 1)

#include <assert.h>
#include <stdlib.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

dlist_elmt_pt dlist_elmt_create(void * data, void * data2) {
	dlist_elmt_pt elmt;

	elmt = malloc(sizeof(dlist_elmt_t));
	if (NULL != elmt) {
		elmt->prev	= NULL;
		elmt->next	= NULL;
		elmt->list	= NULL;
		elmt->data	= data;
		elmt->data2	= data2;
	}

	return elmt;
}

int dlist_elmt_delete(dlist_elmt_pt elmt) {
	if (NULL == elmt) {
		return -2;
	}

	free(elmt);

	return 0;
}

int dlist_create(dlist_pt * dlist_p) {
	dlist_pt dlist;

	if (NULL == dlist_p) {
		logme("dlist_p is NULL");
		return -2;
	}

	dlist = malloc(sizeof(dlist_t));
	if (NULL == dlist) {
		logme("malloc(...) fail");
		return -1;
	}

	dlist->count	= 0;
	dlist->head		= NULL;
	dlist->tail		= NULL;
	dlist->cur		= NULL;
	dlist->data		= NULL;

	*dlist_p = dlist;

	return 0;
}

int dlist_delete(dlist_pt * dlist_p) {
	dlist_pt dlist;

	if (NULL == dlist_p) {
		logme("dlist_p is NULL");
		return -2;
	}

	if (NULL == *dlist_p) {
		logme("*dlist_p is NULL");
		return -2;
	}

	dlist = *dlist_p;

	if (0 != dlist->count) {
		logme("**dlist_p is not empty");
		return -2;
	}

	dlist->count	= 0;
	dlist->head		= NULL;
	dlist->tail		= NULL;
	dlist->cur		= NULL;
	dlist->data		= NULL;

	free(dlist);

	*dlist_p = NULL;

	return 0;
}

int dlist_insertprev(dlist_pt dlist, dlist_elmt_pt ref, dlist_elmt_pt elmt) {
	if (NULL == dlist) {
		logme("dlist is NULL");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL");
		return -4;
	}

	elmt->next = ref;

	if (NULL == ref) {
		elmt->prev = dlist->tail;
		dlist->tail = elmt;
	}
	else {
		elmt->prev = ref->prev;
		ref->prev = elmt;
	}

	if (NULL == elmt->prev) {
		dlist->head = elmt;
	}
	else {
		elmt->prev->next = elmt;
	}

	dlist->count++;

	elmt->list = dlist;

	return 0;
}

int dlist_insertnext(dlist_pt dlist, dlist_elmt_pt ref, dlist_elmt_pt elmt) {
	if (NULL == dlist) {
		logme("dlist is NULL");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL");
		return -4;
	}

	elmt->prev = ref;

	if (NULL == ref) {
		elmt->next = dlist->head;
		dlist->head = elmt;
	}
	else {
		elmt->next = ref->next;
		ref->next = elmt;
	}

	if (NULL == elmt->next) {
		dlist->tail = elmt;
	}
	else {
		elmt->next->prev = elmt;
	}

	dlist->count++;

	elmt->list = dlist;

	return 0;
}

int dlist_remove(dlist_elmt_pt elmt) {
	dlist_pt dlist;

	if (NULL == elmt) {
		logme("elmt is NULL");
		return -2;
	}

	dlist = (dlist_pt) (elmt->list);

	if (NULL == dlist) {
		logme("dlist is NULL");
		return -2;
	}

	///
	if (elmt == dlist->cur) {
		dlist->cur = elmt->prev;
	}

	if (NULL == elmt->prev) {
		dlist->head = elmt->next;
	}
	else {
		elmt->prev->next = elmt->next;
	}

	if (NULL == elmt->next) {
		dlist->tail = elmt->prev;
	}
	else {
		elmt->next->prev = elmt->prev;
	}

	dlist->count--;

	elmt->prev = NULL;
	elmt->next = NULL;
	elmt->list = NULL;
	///

	return 0;
}

dlist_elmt_pt dlist_getcurnext(dlist_pt dlist) {
	if (NULL == dlist->cur || NULL == dlist->cur->next) {
		dlist->cur = dlist->head;
	}
	else {
		dlist->cur = dlist->cur->next;
	}

	return dlist->cur;
}

int dlist_find(dlist_pt dlist, dlist_elmt_pt * elmt_p, void * data) {
	dlist_elmt_pt elmt;

	if (NULL == dlist) {
		logme("dlist is NULL");
		return -2;
	}

	if (NULL == elmt_p) {
		logme("elmt_p is NULL");
		return -3;
	}

	elmt = dlist->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		elmt = elmt->next;
	}

	*elmt_p = elmt;

	if (NULL == elmt) {
		return DLIST_ERR__NOTEXIST;
	}

	return 0;
}

int dlist_findandremove(dlist_pt dlist, dlist_elmt_pt * elmt_p, void * data) {
	dlist_elmt_pt elmt;

	if (NULL == dlist) {
		logme("dlist is NULL");
		return -2;
	}

	elmt = dlist->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		elmt = elmt->next;
	}

	if (NULL != elmt_p) {
		*elmt_p = elmt;
	}

	if (NULL == elmt) {
		return DLIST_ERR__NOTEXIST;
	}

	///
	if (elmt == dlist->cur) {
		dlist->cur = elmt->prev;
	}

	if (NULL == elmt->prev) {
		dlist->head = elmt->next;
	}
	else {
		elmt->prev->next = elmt->next;
	}

	if (NULL == elmt->next) {
		dlist->tail = elmt->prev;
	}
	else {
		elmt->next->prev = elmt->prev;
	}

	dlist->count--;

	elmt->prev = NULL;
	elmt->next = NULL;
	elmt->list = NULL;
	///

	return 0;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_DLIST == 1) */
