/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubiclib.h"

#if !(UBINOS__UBICLIB__EXCLUDE_LIST == 1)

#include <assert.h>
#include <stdlib.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__UBICLIB

list_elmt_pt list_elmt_create(void * data) {
	list_elmt_pt elmt;

	elmt = malloc(sizeof(list_elmt_t));
	if (NULL != elmt) {
		elmt->next = NULL;
		elmt->data = data;
	}

	return elmt;
}

int list_elmt_delete(list_elmt_pt elmt) {
	if (NULL == elmt) {
		return -2;
	}

	free(elmt);

	return 0;
}

int list_create(list_pt * list_p) {
	list_pt list;

	if (NULL == list_p) {
		logme("list_p is NULL");
		return -2;
	}

	list = malloc(sizeof(list_t));
	if (NULL == list) {
		logme("malloc(...) fail");
		return -1;
	}

	list->count	= 0;
	list->head	= NULL;
	list->tail	= NULL;

	*list_p = list;

	return 0;
}

int list_delete(list_pt * list_p) {
	list_pt list;

	if (NULL == list_p) {
		logme("list_p is NULL");
		return -2;
	}

	if (NULL == *list_p) {
		logme("*list_p is NULL");
		return -2;
	}

	list = *list_p;

	if (0 != list->count) {
		logme("**list_p is not empty");
		return -2;
	}

	list->count	= 0;
	list->head	= NULL;
	list->tail	= NULL;

	free(list);

	*list_p = NULL;

	return 0;
}

int list_insertnext(list_pt list, list_elmt_pt ref, list_elmt_pt elmt) {
	if (NULL == list) {
		logme("list is NULL");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL");
		return -4;
	}

	if (NULL == ref) {
		elmt->next = list->head;
		list->head = elmt;
	}
	else {
		elmt->next = ref->next;
		ref->next = elmt;
	}

	if (NULL == elmt->next) {
		list->tail = elmt;
	}

	list->count++;

	return 0;
}

int list_inserttail(list_pt list, list_elmt_pt elmt) {
	if (NULL == list) {
		logme("list is NULL");
		return -2;
	}

	if (NULL == elmt) {
		logme("elmt is NULL");
		return -4;
	}

	elmt->next = NULL;

	if (NULL == list->tail) {
		list->head = elmt;
	}
	else {
		list->tail->next = elmt;
	}

	list->tail = elmt;

	list->count++;

	return 0;

}

int list_removenext(list_pt list, list_elmt_pt ref, list_elmt_pt * elmt_p) {
	list_elmt_pt elmt;

	if (NULL == list) {
		logme("list is NULL");
		return -2;
	}

	if (NULL == list->head) {
		logme("list is empty");
		return -1;
	}

	if (NULL != ref && NULL == ref->next) {
		logme("next is NULL");
		return -1;
	}

	///
	if (NULL == ref) {
		elmt = list->head;
		list->head = elmt->next;

		if (NULL == elmt->next) {
			list->tail = NULL;
		}
	}
	else {
		elmt = ref->next;
		ref->next = elmt->next;

		if (NULL == ref->next) {
			list->tail = ref;
		}
	}

	list->count--;

	elmt->next = NULL;
	///

	if (NULL != elmt_p) {
		*elmt_p = elmt;
	}

	return 0;
}

int list_find(list_pt list, list_elmt_pt * elmt_p, void * data) {
	list_elmt_pt elmt;

	if (NULL == list) {
		logme("list is NULL");
		return -2;
	}
	if (NULL == elmt_p) {
		logme("elmt_p is NULL");
		return -3;
	}

	elmt = list->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		elmt = elmt->next;
	}

	*elmt_p = elmt;

	if (NULL == elmt) {
		return LIST_ERR__NOTEXIST;
	}

	return 0;
}

int list_findandremove(list_pt list, list_elmt_pt * elmt_p, void * data) {
	list_elmt_pt ref;
	list_elmt_pt elmt;

	if (NULL == list) {
		logme("list is NULL");
		return -2;
	}

	ref = NULL;
	elmt = list->head;
	for (;;) {
		if (NULL == elmt || data == elmt->data) {
			break;
		}
		ref = elmt;
		elmt = elmt->next;
	}

	if (NULL != elmt_p) {
		*elmt_p = elmt;
	}

	if (NULL == elmt) {
		return LIST_ERR__NOTEXIST;
	}

	///
	if (NULL == ref) {
		elmt = list->head;
		list->head = elmt->next;

		if (NULL == elmt->next) {
			list->tail = NULL;
		}
	}
	else {
		elmt = ref->next;
		ref->next = elmt->next;

		if (NULL == ref->next) {
			list->tail = ref;
		}
	}

	list->count--;

	elmt->next = NULL;
	///

	return 0;
}

#endif /* !(UBINOS__UBICLIB__EXCLUDE_LIST == 1) */
