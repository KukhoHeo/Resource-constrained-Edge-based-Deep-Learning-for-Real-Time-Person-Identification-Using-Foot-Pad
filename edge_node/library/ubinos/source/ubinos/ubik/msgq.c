/*
 * Copyright (c) 2009 Sung Ho Park
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "_ubik.h"

#if !(UBINOS__UBIK__EXCLUDE_MSGQ == 1)

#if (INCLUDE__UBINOS__UBIK == 1)

#include <assert.h>
#include <string.h>

#undef LOGM_CATEGORY
#define LOGM_CATEGORY LOGM_CATEGORY__MSGQ

int msgq_create(msgq_pt * msgq_p, unsigned int msgsize, unsigned int maxcount) {
	logmfd("msgq_p 0x%x, msgsize %d maxcount %d", msgq_p, msgsize, maxcount);

	return msgq_create_ext(msgq_p, msgsize, maxcount, 0);
}

int msgq_create_ext(msgq_pt * msgq_p, unsigned int msgsize, unsigned int maxcount, unsigned int option) {
	int r;
	int r2;
	_sigobj_pt sigobj;
	cirbuf_pt cirbuf;

	assert(msgq_p != NULL);

	logmfd("msgq_p 0x%x, msgsize %d maxcount %d, option 0x%x", msgq_p, msgsize, maxcount, option);

	if (0 != bsp_isintr()) {
		logme("in interrupt");
		r = -1;
		goto end0;
	}

	if (0 != _bsp_critcount) {
		logme("in critical section");
		r = -1;
		goto end0;
	}

	if (NULL == msgq_p) {
		logme("parameter 1 is wrong");
		r = -2;
		goto end0;
	}

	if (0 >= msgsize || UBINOS__UBIK__MSGQ_MSGSIZE_MAX < msgsize) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	if (0 >= maxcount) {
		logme("parameter 3 is wrong");
		r = -4;
		goto end0;
	}

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r = mutex_lock(_kernel_monitor_mutex);
	if (0 != r) {
		logme("fail at mutex_lock()");
		r = -1;
		goto end0;
	}
	#endif

	r = cirbuf_create_ext(&cirbuf, msgsize * maxcount, CIRBUF_OPT__NOOVERWRITE);
	if (0 != r) {
		logme("fail at cirbuf_create_ext()");
		r = -1;
		goto end1;
	}

	r = _sigobj_create(&sigobj);
	if (0 != r) {
		logme("fail at _sigobj_create()");
		r2 = cirbuf_delete(&cirbuf);
		if (0 != r2) {
			logme("fail at cirbuf_delete()");
		}
		goto end1;
	}

	sigobj->maxcount	= maxcount;
	sigobj->msgsize_1	= msgsize - 1;
	sigobj->msgbuf		= cirbuf;

	if (0 != (option & MSGQ_OPT__IGNOREOVERFLOW)) {
		sigobj->ignoreoverflow = 1;
	}

	sigobj->type 		= OBJTYPE__UBIK_MSGQ;
	sigobj->valid		= 1;

	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	_kernel_monitor_sigobjlist_insertnext(&_kernel_monitor_sigobjlist, NULL, sigobj);
	#endif

	*msgq_p = (msgq_pt) sigobj;

end1:
	#if !(UBINOS__UBIK__EXCLUDE_KERNEL_MONITORING == 1)
	r2 = mutex_unlock(_kernel_monitor_mutex);
	if (0 != r2) {
		logme("fail at mutex_unlock()");
		r = -1;
	}
	#endif

end0:
	if (r == 0) {
		logmfd("msgq 0x%x is created", sigobj);
	}

	return r;
}

int msgq_delete(msgq_pt * msgq_p) {
	assert(msgq_p != NULL);
	assert(*msgq_p != NULL);

	logmfd("msgq_p 0x%x, *msgq_p 0x%x", msgq_p, *msgq_p);

	return _sigobj_delete((_sigobj_pt *) msgq_p);
}

int msgq_receive(msgq_pt _msgq, unsigned char * msg) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;
	_task_pt task = _task_cur;

	assert(_msgq != NULL);
	assert(msg != NULL);

	logmfv("msgq 0x%x, msg 0x%x", _msgq, msg);

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
		goto end0;
	}

	if (0 == task->sysflag01) {
		if (0 != bsp_isintr()) {
			logme("in interrupt");
			r = -1;
			goto end0;
		}

		if (0 != _bsp_critcount) {
			logme("in critical section");
			r = -1;
			goto end0;
		}

		if (0 != _ubik_tasklockcount) {
			logme("task is locked");
			r = -1;
			goto end0;
		}
	}

	if (NULL == msg) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	if (0 < sigobj->count) {
		r = cirbuf_read(sigobj->msgbuf, msg, sigobj->msgsize_1 + 1, NULL);
		if (0 != r) {
			logme("fail at cirbuf_read()");
			r = -1;
			goto end1;
		}
		sigobj->count--;
	}
	else {
		if (0 == task->sysflag01) {
			task->wtask_p->msg = msg;
			r = _sigobj_wait(sigobj, task->wtask_p);
			if (SIGOBJ_SIGTYPE__TIMEOUT != r && 0 != r) {
				logme("fail at _sigobj_wait()");
			}
			task->wtask_p->msg = NULL;
		}
		else {
			r = SIGOBJ_SIGTYPE__TIMEOUT;
		}
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int msgq_receive_timed(msgq_pt _msgq, unsigned char * msg, unsigned int tick) {
	int r;
	unsigned int sysflag01_old = 0;

	assert(_msgq != NULL);
	assert(msg != NULL);

	logmfv("msgq 0x%x, msg 0x%x, tick %d", _msgq, msg, tick);

	if (0 == _bsp_kernel_active) {
		logme("ubik is not active");
		r = -1;
		goto end0;
	}

	if (0 != bsp_isintr()) {
		if (0 == tick) {
			sysflag01_old = _task_cur->sysflag01;
			_task_cur->sysflag01 = 1;
		}
		else {
			logme("in interrupt");
			r = -1;
			goto end0;
		}
	}
	else {
		_task_cur->timed		= 1;
		_task_cur->waittick		= tick;
	}

	r = msgq_receive(_msgq, msg);

	if (0 != bsp_isintr()) {
		 _task_cur->sysflag01 = sysflag01_old;
	}
	else {
		_task_cur->timed		= 0;
	}

end0:
	return r;
}

int msgq_receive_timedms(msgq_pt _msgq, unsigned char * msg, unsigned int timems) {
	assert(_msgq != NULL);
	assert(msg != NULL);

	logmfv("msgq 0x%x, msg 0x%x, timems %d", _msgq, msg, timems);

	return msgq_receive_timed(_msgq, msg, ubik_timemstotick(timems));
}

int msgq_send(msgq_pt _msgq, unsigned char * msg) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;
	_task_pt otask;
	_wtask_pt wtask;

	assert(_msgq != NULL);
	assert(msg != NULL);

	logmfv("msgq 0x%x, msg 0x%x", _msgq, msg);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == msg) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	otask = _task_osigobjlist_owner(sigobj);
	if (	(0 == bsp_isintr() && NULL != otask && _task_cur != otask)	||
			(0 != bsp_isintr() && NULL != otask						)		)
	{
		logme("current task is not an appropriate sender");
		r = -1;
		goto end1;
	}

	if (sigobj->maxcount == sigobj->count) {
		if (0 == sigobj->ignoreoverflow) {
			logme("count is over max count");
			r = UBIK_ERR__OVERFLOWED;
		}
		else {
			r = 0;
		}
	}
	else {
		wtask = _sigobj_wtasklist_gettask(sigobj);
		if (NULL == wtask) {
			r = cirbuf_write(sigobj->msgbuf, msg, sigobj->msgsize_1 + 1, NULL);
			if (0 != r) {
				logme("fail at cirbuf_write()");
				r = -1;
				goto end1;
			}
			sigobj->count++;
		}
		else {
			if (NULL == wtask->msg) {
				logme("message buffer of task is NULL.");
				r = -1;
				goto end1;
			}

			memcpy(wtask->msg, msg, sigobj->msgsize_1 + 1);
			r = _sigobj_send(sigobj, SIGOBJ_SIGTYPE__SUCCESS);
			if (0 != r) {
				logme("fail at _sigobj_send()");
			}
		}
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int msgq_clear(msgq_pt _msgq) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;

	assert(_msgq != NULL);

	logmfd("msgq 0x%x", _msgq);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	r = cirbuf_clear(sigobj->msgbuf);
	if (0 != r) {
		logme("fail at cirbuf_clear()");
	}
	else {
		sigobj->count = 0;
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int msgq_setsender(msgq_pt _msgq, task_pt _task) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;
	_task_pt task = (_task_pt) _task;

	assert(_msgq != NULL);

	logmfd("msgq 0x%x, task 0x%x", _msgq, _task);

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	r = _sigobj_setsender(sigobj, task);
	if (0 != r) {
		logme("fail at _sigobj_setsender()");
	}

end1:
	ubik_exitcrit();

end0:
	return r;
}

int msgq_getcount(msgq_pt _msgq, unsigned int * count_p) {
	int r;
	_sigobj_pt sigobj = (_sigobj_pt) _msgq;

	if (NULL == _task_cur) {
		logme("ubik is not initialized");
		r = -1;
		goto end0;
	}

	if (NULL == count_p) {
		logme("parameter 2 is wrong");
		r = -3;
		goto end0;
	}

	ubik_entercrit();

	if (	(	NULL == sigobj							)	||
			(	0 == sigobj->valid						)	||
			(	OBJTYPE__UBIK_MSGQ != sigobj->type	)		)
	{
		logme("parameter 1 is wrong");
		r = -2;
		goto end1;
	}

	*count_p = sigobj->count;

	r = 0;

end1:
	ubik_exitcrit();

end0:
	return r;
}

#endif /* (INCLUDE__UBINOS__UBIK == 1) */

#endif /* !(UBINOS__UBIK__EXCLUDE_MSGQ == 1) */
