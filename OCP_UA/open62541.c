/* THIS IS A SINGLE-FILE DISTRIBUTION CONCATENATED FROM THE OPEN62541 SOURCES 
 * visit http://open62541.org/ for information about this software
 * Git-Revision: unknown--no-git-found
 */
 
 /*
 * Copyright (C) 2015 the contributors as stated in the AUTHORS file
 *
 * This file is part of open62541. open62541 is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License, version 3 (as published by the Free Software Foundation) with
 * a static linking exception as stated in the LICENSE file provided with
 * open62541.
 *
 * open62541 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */


 #ifndef UA_DYNAMIC_LINKING
# define UA_DYNAMIC_LINKING
#endif

#ifndef UA_INTERNAL
#define UA_INTERNAL
#endif

#include "open62541.h"

/*********************************** amalgamated original file "C:/open62541/src/ua_util.h" ***********************************/



/* Subtract from nodeids to get from the encoding to the content */
#define UA_ENCODINGOFFSET_XML 1
#define UA_ENCODINGOFFSET_BINARY 2

#include <assert.h> // assert
#define UA_assert(ignore) assert(ignore)

/*********************/
/* Memory Management */
/*********************/

/* Replace the macros with functions for custom allocators if necessary */
#include <stdlib.h> // malloc, free
#ifdef _WIN32
# include <malloc.h>
#endif

#ifndef UA_free
# define UA_free(ptr) free(ptr)
#endif
#ifndef UA_malloc
# define UA_malloc(size) malloc(size)
#endif
#ifndef UA_calloc
# define UA_calloc(num, size) calloc(num, size)
#endif
#ifndef UA_realloc
# define UA_realloc(ptr, size) realloc(ptr, size)
#endif

#ifndef NO_ALLOCA
# ifdef __GNUC__
#  define UA_alloca(size) __builtin_alloca (size)
# elif defined(_WIN32)
#  define UA_alloca(SIZE) _alloca(SIZE)
# else
#  include <alloca.h>
#  define UA_alloca(SIZE) alloca(SIZE)
# endif
#endif

/************************/
/* Thread Local Storage */
/************************/

#ifdef UA_MULTITHREADING
# ifdef __GNUC__
#  define UA_THREAD_LOCAL __thread
# elif defined(_MSC_VER)
#  define UA_THREAD_LOCAL __declspec(thread)
# else
#  error No thread local storage keyword defined for this compiler
# endif
#else
# define UA_THREAD_LOCAL
#endif

/********************/
/* System Libraries */
/********************/

#include <time.h>
#ifdef _WIN32
# include <winsock2.h> //needed for amalgation
# include <windows.h>
# undef SLIST_ENTRY
#else
# include <sys/time.h>
#endif

/*************************/
/* External Dependencies */
/*************************/


#ifdef UA_MULTITHREADING
# define _LGPL_SOURCE
# include <urcu.h>
# include <urcu/wfcqueue.h>
# include <urcu/uatomic.h>
# include <urcu/rculfhash.h>
#include <urcu/lfstack.h>
#endif


/*********************************** amalgamated original file "C:/open62541/deps/queue.h" ***********************************/

/*	$OpenBSD: queue.h,v 1.38 2013/07/03 15:05:21 fgsch Exp $	*/
/*	$NetBSD: queue.h,v 1.11 1996/05/16 05:17:14 mycroft Exp $	*/

/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)queue.h	8.5 (Berkeley) 8/20/94
 */

#ifndef	_SYS_QUEUE_H_
#define	_SYS_QUEUE_H_

/*
 * This file defines five types of data structures: singly-linked lists, 
 * lists, simple queues, tail queues, and circular queues.
 *
 *
 * A singly-linked list is headed by a single forward pointer. The elements
 * are singly linked for minimum space and pointer manipulation overhead at
 * the expense of O(n) removal for arbitrary elements. New elements can be
 * added to the list after an existing element or at the head of the list.
 * Elements being removed from the head of the list should use the explicit
 * macro for this purpose for optimum efficiency. A singly-linked list may
 * only be traversed in the forward direction.  Singly-linked lists are ideal
 * for applications with large datasets and few or no removals or for
 * implementing a LIFO queue.
 *
 * A list is headed by a single forward pointer (or an array of forward
 * pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before
 * or after an existing element or at the head of the list. A list
 * may only be traversed in the forward direction.
 *
 * A simple queue is headed by a pair of pointers, one the head of the
 * list and the other to the tail of the list. The elements are singly
 * linked to save space, so elements can only be removed from the
 * head of the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the
 * list. A simple queue may only be traversed in the forward direction.
 *
 * A tail queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or
 * after an existing element, at the head of the list, or at the end of
 * the list. A tail queue may be traversed in either direction.
 *
 * A circle queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A circle queue may be traversed in either direction, but has a more
 * complex end of list detection.
 *
 * For details on the use of these macros, see the queue(3) manual page.
 */

#if defined(QUEUE_MACRO_DEBUG) || (defined(_KERNEL) && defined(DIAGNOSTIC))
#define _Q_INVALIDATE(a) (a) = ((void *)-1)
#else
#define _Q_INVALIDATE(a)
#endif

/*
 * Singly-linked List definitions.
 */
#define SLIST_HEAD(name, type)						\
struct name {								\
	struct type *slh_first;	/* first element */			\
}
 
#define	SLIST_HEAD_INITIALIZER(head)					\
	{ NULL }
 
#define SLIST_ENTRY(type)						\
struct {								\
	struct type *sle_next;	/* next element */			\
}
 
/*
 * Singly-linked List access methods.
 */
#define	SLIST_FIRST(head)	((head)->slh_first)
#define	SLIST_END(head)		NULL
#define	SLIST_EMPTY(head)	(SLIST_FIRST(head) == SLIST_END(head))
#define	SLIST_NEXT(elm, field)	((elm)->field.sle_next)

#define	SLIST_FOREACH(var, head, field)					\
	for((var) = SLIST_FIRST(head);					\
	    (var) != SLIST_END(head);					\
	    (var) = SLIST_NEXT(var, field))

#define	SLIST_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = SLIST_FIRST(head);				\
	    (var) && ((tvar) = SLIST_NEXT(var, field), 1);		\
	    (var) = (tvar))

/*
 * Singly-linked List functions.
 */
#define	SLIST_INIT(head) {						\
	SLIST_FIRST(head) = SLIST_END(head);				\
}

#define	SLIST_INSERT_AFTER(slistelm, elm, field) do {			\
	(elm)->field.sle_next = (slistelm)->field.sle_next;		\
	(slistelm)->field.sle_next = (elm);				\
} while (0)

#define	SLIST_INSERT_HEAD(head, elm, field) do {			\
	(elm)->field.sle_next = (head)->slh_first;			\
	(head)->slh_first = (elm);					\
} while (0)

#define	SLIST_REMOVE_AFTER(elm, field) do {				\
	(elm)->field.sle_next = (elm)->field.sle_next->field.sle_next;	\
} while (0)

#define	SLIST_REMOVE_HEAD(head, field) do {				\
	(head)->slh_first = (head)->slh_first->field.sle_next;		\
} while (0)

#define SLIST_REMOVE(head, elm, type, field) do {			\
	if ((head)->slh_first == (elm)) {				\
		SLIST_REMOVE_HEAD((head), field);			\
	} else {							\
		struct type *curelm = (head)->slh_first;		\
									\
		while (curelm->field.sle_next != (elm))			\
			curelm = curelm->field.sle_next;		\
		curelm->field.sle_next =				\
		    curelm->field.sle_next->field.sle_next;		\
		_Q_INVALIDATE((elm)->field.sle_next);			\
	}								\
} while (0)

/*
 * List definitions.
 */
#define LIST_HEAD(name, type)						\
struct name {								\
	struct type *lh_first;	/* first element */			\
}

#define LIST_HEAD_INITIALIZER(head)					\
	{ NULL }

#define LIST_ENTRY(type)						\
struct {								\
	struct type *le_next;	/* next element */			\
	struct type **le_prev;	/* address of previous next element */	\
}

/*
 * List access methods
 */
#define	LIST_FIRST(head)		((head)->lh_first)
#define	LIST_END(head)			NULL
#define	LIST_EMPTY(head)		(LIST_FIRST(head) == LIST_END(head))
#define	LIST_NEXT(elm, field)		((elm)->field.le_next)

#define LIST_FOREACH(var, head, field)					\
	for((var) = LIST_FIRST(head);					\
	    (var)!= LIST_END(head);					\
	    (var) = LIST_NEXT(var, field))

#define	LIST_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = LIST_FIRST(head);				\
	    (var) && ((tvar) = LIST_NEXT(var, field), 1);		\
	    (var) = (tvar))

/*
 * List functions.
 */
#define	LIST_INIT(head) do {						\
	LIST_FIRST(head) = LIST_END(head);				\
} while (0)

#define LIST_INSERT_AFTER(listelm, elm, field) do {			\
	if (((elm)->field.le_next = (listelm)->field.le_next) != NULL)	\
		(listelm)->field.le_next->field.le_prev =		\
		    &(elm)->field.le_next;				\
	(listelm)->field.le_next = (elm);				\
	(elm)->field.le_prev = &(listelm)->field.le_next;		\
} while (0)

#define	LIST_INSERT_BEFORE(listelm, elm, field) do {			\
	(elm)->field.le_prev = (listelm)->field.le_prev;		\
	(elm)->field.le_next = (listelm);				\
	*(listelm)->field.le_prev = (elm);				\
	(listelm)->field.le_prev = &(elm)->field.le_next;		\
} while (0)

#define LIST_INSERT_HEAD(head, elm, field) do {				\
	if (((elm)->field.le_next = (head)->lh_first) != NULL)		\
		(head)->lh_first->field.le_prev = &(elm)->field.le_next;\
	(head)->lh_first = (elm);					\
	(elm)->field.le_prev = &(head)->lh_first;			\
} while (0)

#define LIST_REMOVE(elm, field) do {					\
	if ((elm)->field.le_next != NULL)				\
		(elm)->field.le_next->field.le_prev =			\
		    (elm)->field.le_prev;				\
	*(elm)->field.le_prev = (elm)->field.le_next;			\
	_Q_INVALIDATE((elm)->field.le_prev);				\
	_Q_INVALIDATE((elm)->field.le_next);				\
} while (0)

#define LIST_REPLACE(elm, elm2, field) do {				\
	if (((elm2)->field.le_next = (elm)->field.le_next) != NULL)	\
		(elm2)->field.le_next->field.le_prev =			\
		    &(elm2)->field.le_next;				\
	(elm2)->field.le_prev = (elm)->field.le_prev;			\
	*(elm2)->field.le_prev = (elm2);				\
	_Q_INVALIDATE((elm)->field.le_prev);				\
	_Q_INVALIDATE((elm)->field.le_next);				\
} while (0)

/*
 * Simple queue definitions.
 */
#define SIMPLEQ_HEAD(name, type)					\
struct name {								\
	struct type *sqh_first;	/* first element */			\
	struct type **sqh_last;	/* addr of last next element */		\
}

#define SIMPLEQ_HEAD_INITIALIZER(head)					\
	{ NULL, &(head).sqh_first }

#define SIMPLEQ_ENTRY(type)						\
struct {								\
	struct type *sqe_next;	/* next element */			\
}

/*
 * Simple queue access methods.
 */
#define	SIMPLEQ_FIRST(head)	    ((head)->sqh_first)
#define	SIMPLEQ_END(head)	    NULL
#define	SIMPLEQ_EMPTY(head)	    (SIMPLEQ_FIRST(head) == SIMPLEQ_END(head))
#define	SIMPLEQ_NEXT(elm, field)    ((elm)->field.sqe_next)

#define SIMPLEQ_FOREACH(var, head, field)				\
	for((var) = SIMPLEQ_FIRST(head);				\
	    (var) != SIMPLEQ_END(head);					\
	    (var) = SIMPLEQ_NEXT(var, field))

#define	SIMPLEQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = SIMPLEQ_FIRST(head);				\
	    (var) && ((tvar) = SIMPLEQ_NEXT(var, field), 1);		\
	    (var) = (tvar))

/*
 * Simple queue functions.
 */
#define	SIMPLEQ_INIT(head) do {						\
	(head)->sqh_first = NULL;					\
	(head)->sqh_last = &(head)->sqh_first;				\
} while (0)

#define SIMPLEQ_INSERT_HEAD(head, elm, field) do {			\
	if (((elm)->field.sqe_next = (head)->sqh_first) == NULL)	\
		(head)->sqh_last = &(elm)->field.sqe_next;		\
	(head)->sqh_first = (elm);					\
} while (0)

#define SIMPLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.sqe_next = NULL;					\
	*(head)->sqh_last = (elm);					\
	(head)->sqh_last = &(elm)->field.sqe_next;			\
} while (0)

#define SIMPLEQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	if (((elm)->field.sqe_next = (listelm)->field.sqe_next) == NULL)\
		(head)->sqh_last = &(elm)->field.sqe_next;		\
	(listelm)->field.sqe_next = (elm);				\
} while (0)

#define SIMPLEQ_REMOVE_HEAD(head, field) do {			\
	if (((head)->sqh_first = (head)->sqh_first->field.sqe_next) == NULL) \
		(head)->sqh_last = &(head)->sqh_first;			\
} while (0)

#define SIMPLEQ_REMOVE_AFTER(head, elm, field) do {			\
	if (((elm)->field.sqe_next = (elm)->field.sqe_next->field.sqe_next) \
	    == NULL)							\
		(head)->sqh_last = &(elm)->field.sqe_next;		\
} while (0)

/*
 * XOR Simple queue definitions.
 */
#define XSIMPLEQ_HEAD(name, type)					\
struct name {								\
	struct type *sqx_first;	/* first element */			\
	struct type **sqx_last;	/* addr of last next element */		\
	unsigned long sqx_cookie;					\
}

#define XSIMPLEQ_ENTRY(type)						\
struct {								\
	struct type *sqx_next;	/* next element */			\
}

/*
 * XOR Simple queue access methods.
 */
#define XSIMPLEQ_XOR(head, ptr)	    ((__typeof(ptr))((head)->sqx_cookie ^ \
					(unsigned long)(ptr)))
#define	XSIMPLEQ_FIRST(head)	    XSIMPLEQ_XOR(head, ((head)->sqx_first))
#define	XSIMPLEQ_END(head)	    NULL
#define	XSIMPLEQ_EMPTY(head)	    (XSIMPLEQ_FIRST(head) == XSIMPLEQ_END(head))
#define	XSIMPLEQ_NEXT(head, elm, field)    XSIMPLEQ_XOR(head, ((elm)->field.sqx_next))


#define XSIMPLEQ_FOREACH(var, head, field)				\
	for ((var) = XSIMPLEQ_FIRST(head);				\
	    (var) != XSIMPLEQ_END(head);				\
	    (var) = XSIMPLEQ_NEXT(head, var, field))

#define	XSIMPLEQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = XSIMPLEQ_FIRST(head);				\
	    (var) && ((tvar) = XSIMPLEQ_NEXT(head, var, field), 1);	\
	    (var) = (tvar))

/*
 * XOR Simple queue functions.
 */
#define	XSIMPLEQ_INIT(head) do {					\
	arc4random_buf(&(head)->sqx_cookie, sizeof((head)->sqx_cookie)); \
	(head)->sqx_first = XSIMPLEQ_XOR(head, NULL);			\
	(head)->sqx_last = XSIMPLEQ_XOR(head, &(head)->sqx_first);	\
} while (0)

#define XSIMPLEQ_INSERT_HEAD(head, elm, field) do {			\
	if (((elm)->field.sqx_next = (head)->sqx_first) ==		\
	    XSIMPLEQ_XOR(head, NULL))					\
		(head)->sqx_last = XSIMPLEQ_XOR(head, &(elm)->field.sqx_next); \
	(head)->sqx_first = XSIMPLEQ_XOR(head, (elm));			\
} while (0)

#define XSIMPLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.sqx_next = XSIMPLEQ_XOR(head, NULL);		\
	*(XSIMPLEQ_XOR(head, (head)->sqx_last)) = XSIMPLEQ_XOR(head, (elm)); \
	(head)->sqx_last = XSIMPLEQ_XOR(head, &(elm)->field.sqx_next);	\
} while (0)

#define XSIMPLEQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	if (((elm)->field.sqx_next = (listelm)->field.sqx_next) ==	\
	    XSIMPLEQ_XOR(head, NULL))					\
		(head)->sqx_last = XSIMPLEQ_XOR(head, &(elm)->field.sqx_next); \
	(listelm)->field.sqx_next = XSIMPLEQ_XOR(head, (elm));		\
} while (0)

#define XSIMPLEQ_REMOVE_HEAD(head, field) do {				\
	if (((head)->sqx_first = XSIMPLEQ_XOR(head,			\
	    (head)->sqx_first)->field.sqx_next) == XSIMPLEQ_XOR(head, NULL)) \
		(head)->sqx_last = XSIMPLEQ_XOR(head, &(head)->sqx_first); \
} while (0)

#define XSIMPLEQ_REMOVE_AFTER(head, elm, field) do {			\
	if (((elm)->field.sqx_next = XSIMPLEQ_XOR(head,			\
	    (elm)->field.sqx_next)->field.sqx_next)			\
	    == XSIMPLEQ_XOR(head, NULL))				\
		(head)->sqx_last = 					\
		    XSIMPLEQ_XOR(head, &(elm)->field.sqx_next);		\
} while (0)

		    
/*
 * Tail queue definitions.
 */
#define TAILQ_HEAD(name, type)						\
struct name {								\
	struct type *tqh_first;	/* first element */			\
	struct type **tqh_last;	/* addr of last next element */		\
}

#define TAILQ_HEAD_INITIALIZER(head)					\
	{ NULL, &(head).tqh_first }

#define TAILQ_ENTRY(type)						\
struct {								\
	struct type *tqe_next;	/* next element */			\
	struct type **tqe_prev;	/* address of previous next element */	\
}

/* 
 * tail queue access methods 
 */
#define	TAILQ_FIRST(head)		((head)->tqh_first)
#define	TAILQ_END(head)			NULL
#define	TAILQ_NEXT(elm, field)		((elm)->field.tqe_next)
#define TAILQ_LAST(head, headname)					\
	(*(((struct headname *)((head)->tqh_last))->tqh_last))
/* XXX */
#define TAILQ_PREV(elm, headname, field)				\
	(*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))
#define	TAILQ_EMPTY(head)						\
	(TAILQ_FIRST(head) == TAILQ_END(head))

#define TAILQ_FOREACH(var, head, field)					\
	for((var) = TAILQ_FIRST(head);					\
	    (var) != TAILQ_END(head);					\
	    (var) = TAILQ_NEXT(var, field))

#define	TAILQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = TAILQ_FIRST(head);					\
	    (var) != TAILQ_END(head) &&					\
	    ((tvar) = TAILQ_NEXT(var, field), 1);			\
	    (var) = (tvar))


#define TAILQ_FOREACH_REVERSE(var, head, headname, field)		\
	for((var) = TAILQ_LAST(head, headname);				\
	    (var) != TAILQ_END(head);					\
	    (var) = TAILQ_PREV(var, headname, field))

#define	TAILQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)	\
	for ((var) = TAILQ_LAST(head, headname);			\
	    (var) != TAILQ_END(head) &&					\
	    ((tvar) = TAILQ_PREV(var, headname, field), 1);		\
	    (var) = (tvar))

/*
 * Tail queue functions.
 */
#define	TAILQ_INIT(head) do {						\
	(head)->tqh_first = NULL;					\
	(head)->tqh_last = &(head)->tqh_first;				\
} while (0)

#define TAILQ_INSERT_HEAD(head, elm, field) do {			\
	if (((elm)->field.tqe_next = (head)->tqh_first) != NULL)	\
		(head)->tqh_first->field.tqe_prev =			\
		    &(elm)->field.tqe_next;				\
	else								\
		(head)->tqh_last = &(elm)->field.tqe_next;		\
	(head)->tqh_first = (elm);					\
	(elm)->field.tqe_prev = &(head)->tqh_first;			\
} while (0)

#define TAILQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.tqe_next = NULL;					\
	(elm)->field.tqe_prev = (head)->tqh_last;			\
	*(head)->tqh_last = (elm);					\
	(head)->tqh_last = &(elm)->field.tqe_next;			\
} while (0)

#define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	if (((elm)->field.tqe_next = (listelm)->field.tqe_next) != NULL)\
		(elm)->field.tqe_next->field.tqe_prev =			\
		    &(elm)->field.tqe_next;				\
	else								\
		(head)->tqh_last = &(elm)->field.tqe_next;		\
	(listelm)->field.tqe_next = (elm);				\
	(elm)->field.tqe_prev = &(listelm)->field.tqe_next;		\
} while (0)

#define	TAILQ_INSERT_BEFORE(listelm, elm, field) do {			\
	(elm)->field.tqe_prev = (listelm)->field.tqe_prev;		\
	(elm)->field.tqe_next = (listelm);				\
	*(listelm)->field.tqe_prev = (elm);				\
	(listelm)->field.tqe_prev = &(elm)->field.tqe_next;		\
} while (0)

#define TAILQ_REMOVE(head, elm, field) do {				\
	if (((elm)->field.tqe_next) != NULL)				\
		(elm)->field.tqe_next->field.tqe_prev =			\
		    (elm)->field.tqe_prev;				\
	else								\
		(head)->tqh_last = (elm)->field.tqe_prev;		\
	*(elm)->field.tqe_prev = (elm)->field.tqe_next;			\
	_Q_INVALIDATE((elm)->field.tqe_prev);				\
	_Q_INVALIDATE((elm)->field.tqe_next);				\
} while (0)

#define TAILQ_REPLACE(head, elm, elm2, field) do {			\
	if (((elm2)->field.tqe_next = (elm)->field.tqe_next) != NULL)	\
		(elm2)->field.tqe_next->field.tqe_prev =		\
		    &(elm2)->field.tqe_next;				\
	else								\
		(head)->tqh_last = &(elm2)->field.tqe_next;		\
	(elm2)->field.tqe_prev = (elm)->field.tqe_prev;			\
	*(elm2)->field.tqe_prev = (elm2);				\
	_Q_INVALIDATE((elm)->field.tqe_prev);				\
	_Q_INVALIDATE((elm)->field.tqe_next);				\
} while (0)

/*
 * Circular queue definitions.
 */
#define CIRCLEQ_HEAD(name, type)					\
struct name {								\
	struct type *cqh_first;		/* first element */		\
	struct type *cqh_last;		/* last element */		\
}

#define CIRCLEQ_HEAD_INITIALIZER(head)					\
	{ CIRCLEQ_END(&head), CIRCLEQ_END(&head) }

#define CIRCLEQ_ENTRY(type)						\
struct {								\
	struct type *cqe_next;		/* next element */		\
	struct type *cqe_prev;		/* previous element */		\
}

/*
 * Circular queue access methods 
 */
#define	CIRCLEQ_FIRST(head)		((head)->cqh_first)
#define	CIRCLEQ_LAST(head)		((head)->cqh_last)
#define	CIRCLEQ_END(head)		((void *)(head))
#define	CIRCLEQ_NEXT(elm, field)	((elm)->field.cqe_next)
#define	CIRCLEQ_PREV(elm, field)	((elm)->field.cqe_prev)
#define	CIRCLEQ_EMPTY(head)						\
	(CIRCLEQ_FIRST(head) == CIRCLEQ_END(head))

#define CIRCLEQ_FOREACH(var, head, field)				\
	for((var) = CIRCLEQ_FIRST(head);				\
	    (var) != CIRCLEQ_END(head);					\
	    (var) = CIRCLEQ_NEXT(var, field))

#define	CIRCLEQ_FOREACH_SAFE(var, head, field, tvar)			\
	for ((var) = CIRCLEQ_FIRST(head);				\
	    (var) != CIRCLEQ_END(head) &&				\
	    ((tvar) = CIRCLEQ_NEXT(var, field), 1);			\
	    (var) = (tvar))

#define CIRCLEQ_FOREACH_REVERSE(var, head, field)			\
	for((var) = CIRCLEQ_LAST(head);					\
	    (var) != CIRCLEQ_END(head);					\
	    (var) = CIRCLEQ_PREV(var, field))

#define	CIRCLEQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)	\
	for ((var) = CIRCLEQ_LAST(head, headname);			\
	    (var) != CIRCLEQ_END(head) && 				\
	    ((tvar) = CIRCLEQ_PREV(var, headname, field), 1);		\
	    (var) = (tvar))

/*
 * Circular queue functions.
 */
#define	CIRCLEQ_INIT(head) do {						\
	(head)->cqh_first = CIRCLEQ_END(head);				\
	(head)->cqh_last = CIRCLEQ_END(head);				\
} while (0)

#define CIRCLEQ_INSERT_AFTER(head, listelm, elm, field) do {		\
	(elm)->field.cqe_next = (listelm)->field.cqe_next;		\
	(elm)->field.cqe_prev = (listelm);				\
	if ((listelm)->field.cqe_next == CIRCLEQ_END(head))		\
		(head)->cqh_last = (elm);				\
	else								\
		(listelm)->field.cqe_next->field.cqe_prev = (elm);	\
	(listelm)->field.cqe_next = (elm);				\
} while (0)

#define CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field) do {		\
	(elm)->field.cqe_next = (listelm);				\
	(elm)->field.cqe_prev = (listelm)->field.cqe_prev;		\
	if ((listelm)->field.cqe_prev == CIRCLEQ_END(head))		\
		(head)->cqh_first = (elm);				\
	else								\
		(listelm)->field.cqe_prev->field.cqe_next = (elm);	\
	(listelm)->field.cqe_prev = (elm);				\
} while (0)

#define CIRCLEQ_INSERT_HEAD(head, elm, field) do {			\
	(elm)->field.cqe_next = (head)->cqh_first;			\
	(elm)->field.cqe_prev = CIRCLEQ_END(head);			\
	if ((head)->cqh_last == CIRCLEQ_END(head))			\
		(head)->cqh_last = (elm);				\
	else								\
		(head)->cqh_first->field.cqe_prev = (elm);		\
	(head)->cqh_first = (elm);					\
} while (0)

#define CIRCLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.cqe_next = CIRCLEQ_END(head);			\
	(elm)->field.cqe_prev = (head)->cqh_last;			\
	if ((head)->cqh_first == CIRCLEQ_END(head))			\
		(head)->cqh_first = (elm);				\
	else								\
		(head)->cqh_last->field.cqe_next = (elm);		\
	(head)->cqh_last = (elm);					\
} while (0)

#define	CIRCLEQ_REMOVE(head, elm, field) do {				\
	if ((elm)->field.cqe_next == CIRCLEQ_END(head))			\
		(head)->cqh_last = (elm)->field.cqe_prev;		\
	else								\
		(elm)->field.cqe_next->field.cqe_prev =			\
		    (elm)->field.cqe_prev;				\
	if ((elm)->field.cqe_prev == CIRCLEQ_END(head))			\
		(head)->cqh_first = (elm)->field.cqe_next;		\
	else								\
		(elm)->field.cqe_prev->field.cqe_next =			\
		    (elm)->field.cqe_next;				\
	_Q_INVALIDATE((elm)->field.cqe_prev);				\
	_Q_INVALIDATE((elm)->field.cqe_next);				\
} while (0)

#define CIRCLEQ_REPLACE(head, elm, elm2, field) do {			\
	if (((elm2)->field.cqe_next = (elm)->field.cqe_next) ==		\
	    CIRCLEQ_END(head))						\
		(head)->cqh_last = (elm2);				\
	else								\
		(elm2)->field.cqe_next->field.cqe_prev = (elm2);	\
	if (((elm2)->field.cqe_prev = (elm)->field.cqe_prev) ==		\
	    CIRCLEQ_END(head))						\
		(head)->cqh_first = (elm2);				\
	else								\
		(elm2)->field.cqe_prev->field.cqe_next = (elm2);	\
	_Q_INVALIDATE((elm)->field.cqe_prev);				\
	_Q_INVALIDATE((elm)->field.cqe_next);				\
} while (0)

#endif	/* !_SYS_QUEUE_H_ */

/*********************************** amalgamated original file "C:/open62541/deps/pcg_basic.h" ***********************************/

/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *     http://www.pcg-random.org
 */

/*
 * This code is derived from the full C implementation, which is in turn
 * derived from the canonical C++ PCG implementation. The C++ version
 * has many additional features and is preferable if you can use C++ in
 * your project.
 */


#include <inttypes.h>

#if __cplusplus
extern "C" {
#endif

struct pcg_state_setseq_64 {    // Internals are *Private*.
    uint64_t state;             // RNG state.  All values are possible.
    uint64_t inc;               // Controls which RNG sequence (stream) is
                                // selected. Must *always* be odd.
};
typedef struct pcg_state_setseq_64 pcg32_random_t;

// If you *must* statically initialize it, here's one.

#define PCG32_INITIALIZER   { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

// pcg32_srandom(initstate, initseq)
// pcg32_srandom_r(rng, initstate, initseq):
//     Seed the rng.  Specified in two parts, state initializer and a
//     sequence selection constant (a.k.a. stream id)

void pcg32_srandom(uint64_t initstate, uint64_t initseq);
void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate,
                     uint64_t initseq);

// pcg32_random()
// pcg32_random_r(rng)
//     Generate a uniformly distributed 32-bit random number

uint32_t pcg32_random(void);
uint32_t pcg32_random_r(pcg32_random_t* rng);

// pcg32_boundedrand(bound):
// pcg32_boundedrand_r(rng, bound):
//     Generate a uniformly distributed number, r, where 0 <= r < bound

uint32_t pcg32_boundedrand(uint32_t bound);
uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound);

#if __cplusplus
}
#endif


/*********************************** amalgamated original file "C:/open62541/src/ua_types_encoding_binary.h" ***********************************/



/**
 * @ingroup types
 *
 * @defgroup encoding_binary Binary Encoding
 *
 * @brief Functions for binary en- and decoding of built-in datatypes as defined
 * in the standard. The encoding of the remaining datatypes is autogenerated
 * from XML descriptions.
 *
 * All datatypes have similar functions with a common postfix. DO NOT CALL THESE
 * FUNCTIONS WITH NULL-POINTERS IF IT IS NOT EXPLICITLY PERMITTED.
 *
 * - _encode: Encodes a variable into a bytestring. If an error occurs
 *   (indicated by the return value), the bytestring may be left in an
 *   inconsistent state.
 *
 * - _decode: Decodes a variable stored in a bytestring. The destination is
 *   cleaned up (init) before decoding into it. If an error occurs (indicated by
 *   the return value), the destination is cleaned up (deleteMembers, but no
 *   init) before returning.
 *
 * @{
 */

#define UA_TYPE_BINARY_ENCODING(TYPE)                                   \
    UA_StatusCode TYPE##_encodeBinary(TYPE const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset); \
    UA_StatusCode TYPE##_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, TYPE *dst);

UA_TYPE_BINARY_ENCODING(UA_Boolean)
UA_TYPE_BINARY_ENCODING(UA_Byte)
#define UA_SByte_encodeBinary(src, dst, offset) UA_Byte_encodeBinary((const UA_Byte *)src, dst, offset)
#define UA_SByte_decodeBinary(src, offset, dst) UA_Byte_decodeBinary(src, offset, (UA_Byte *)dst)
UA_TYPE_BINARY_ENCODING(UA_UInt16)
#define UA_Int16_encodeBinary(src, dst, offset) UA_UInt16_encodeBinary((const UA_UInt16 *)src, dst, offset)
#define UA_Int16_decodeBinary(src, offset, dst) UA_UInt16_decodeBinary(src, offset, (UA_UInt16 *)dst)
UA_TYPE_BINARY_ENCODING(UA_UInt32)
#define UA_Int32_encodeBinary(src, dst, offset) UA_UInt32_encodeBinary((const UA_UInt32 *)src, dst, offset)
#define UA_Int32_decodeBinary(src, offset, dst) UA_UInt32_decodeBinary(src, offset, (UA_UInt32 *)dst)
UA_TYPE_BINARY_ENCODING(UA_UInt64)
#define UA_Int64_encodeBinary(src, dst, offset) UA_UInt64_encodeBinary((const UA_UInt64 *)src, dst, offset)
#define UA_Int64_decodeBinary(src, offset, dst) UA_UInt64_decodeBinary(src, offset, (UA_UInt64 *)dst)
#ifdef UA_MIXED_ENDIAN
 UA_TYPE_BINARY_ENCODING(UA_Float)
 UA_TYPE_BINARY_ENCODING(UA_Double)
#else
 #define UA_Float_encodeBinary(src, dst, offset) UA_UInt32_encodeBinary((const UA_UInt32 *)src, dst, offset)
 #define UA_Float_decodeBinary(src, offset, dst) UA_UInt32_decodeBinary(src, offset, (UA_UInt32 *)dst)
 #define UA_Double_encodeBinary(src, dst, offset) UA_UInt64_encodeBinary((const UA_UInt64 *)src, dst, offset)
 #define UA_Double_decodeBinary(src, offset, dst) UA_UInt64_decodeBinary(src, offset, (UA_UInt64 *)dst)
#endif
UA_TYPE_BINARY_ENCODING(UA_String)
#define UA_DateTime_encodeBinary(src, dst, offset) UA_UInt64_encodeBinary((const UA_UInt64 *)src, dst, offset)
#define UA_DateTime_decodeBinary(src, offset, dst) UA_UInt64_decodeBinary(src, offset, (UA_UInt64 *)dst)
UA_TYPE_BINARY_ENCODING(UA_Guid)
#define UA_ByteString_encodeBinary(src, dst, offset) UA_String_encodeBinary((const UA_String *)src, dst, offset)
#define UA_ByteString_decodeBinary(src, offset, dst) UA_String_decodeBinary(src, offset, (UA_String *)dst)
#define UA_XmlElement_encodeBinary(src, dst, offset) UA_String_encodeBinary((const UA_String *)src, dst, offset)
#define UA_XmlElement_decodeBinary(src, offset, dst) UA_String_decodeBinary(src, offset, (UA_String *)dst)
UA_TYPE_BINARY_ENCODING(UA_NodeId)
UA_TYPE_BINARY_ENCODING(UA_ExpandedNodeId)
#define UA_StatusCode_encodeBinary(src, dst, offset) UA_UInt32_encodeBinary((const UA_UInt32 *)src, dst, offset)
#define UA_StatusCode_decodeBinary(src, offset, dst) UA_UInt32_decodeBinary(src, offset, (UA_UInt32 *)dst)
UA_TYPE_BINARY_ENCODING(UA_QualifiedName)
UA_TYPE_BINARY_ENCODING(UA_LocalizedText)
UA_TYPE_BINARY_ENCODING(UA_ExtensionObject)
UA_TYPE_BINARY_ENCODING(UA_DataValue)
UA_TYPE_BINARY_ENCODING(UA_Variant)
UA_TYPE_BINARY_ENCODING(UA_DiagnosticInfo)

UA_StatusCode UA_encodeBinary(const void *src, const UA_DataType *dataType, UA_ByteString *dst,
                              size_t *UA_RESTRICT offset) UA_FUNC_ATTR_WARN_UNUSED_RESULT;
UA_StatusCode UA_decodeBinary(const UA_ByteString *src, size_t * UA_RESTRICT offset, void *dst,
                              const UA_DataType *dataType) UA_FUNC_ATTR_WARN_UNUSED_RESULT;
size_t UA_calcSizeBinary(const void *p, const UA_DataType *dataType) UA_FUNC_ATTR_PURE;

/// @}


/*********************************** amalgamated original file "C:/open62541/build/src_generated/ua_types_generated_encoding_binary.h" ***********************************/

/**
* @file ua_types_generated_encoding_binary.h
*
* @brief Binary encoding for autogenerated data types
*
* Generated from Opc.Ua.Types.bsd with script C:/open62541/tools/generate_datatypes.py
* on host opcua-PC by user opcua at 2015-11-18 11:21:45
*/


/* UA_IdType */
static UA_INLINE UA_StatusCode UA_IdType_encodeBinary(const UA_IdType *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_IdType_decodeBinary(const UA_ByteString *src, size_t *offset, UA_IdType *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_NodeClass */
static UA_INLINE UA_StatusCode UA_NodeClass_encodeBinary(const UA_NodeClass *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_NodeClass_decodeBinary(const UA_ByteString *src, size_t *offset, UA_NodeClass *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ReferenceNode */
static UA_INLINE UA_StatusCode UA_ReferenceNode_encodeBinary(const UA_ReferenceNode *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REFERENCENODE], dst, offset); }
static UA_INLINE UA_StatusCode UA_ReferenceNode_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ReferenceNode *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REFERENCENODE]); }

/* UA_Argument */
static UA_INLINE UA_StatusCode UA_Argument_encodeBinary(const UA_Argument *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ARGUMENT], dst, offset); }
static UA_INLINE UA_StatusCode UA_Argument_decodeBinary(const UA_ByteString *src, size_t *offset, UA_Argument *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ARGUMENT]); }

/* UA_ApplicationType */
static UA_INLINE UA_StatusCode UA_ApplicationType_encodeBinary(const UA_ApplicationType *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_ApplicationType_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ApplicationType *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ApplicationDescription */
static UA_INLINE UA_StatusCode UA_ApplicationDescription_encodeBinary(const UA_ApplicationDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_ApplicationDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ApplicationDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_APPLICATIONDESCRIPTION]); }

/* UA_RequestHeader */
static UA_INLINE UA_StatusCode UA_RequestHeader_encodeBinary(const UA_RequestHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REQUESTHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_RequestHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_RequestHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REQUESTHEADER]); }

/* UA_ResponseHeader */
static UA_INLINE UA_StatusCode UA_ResponseHeader_encodeBinary(const UA_ResponseHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_RESPONSEHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_ResponseHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ResponseHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_RESPONSEHEADER]); }

/* UA_ServiceFault */
static UA_INLINE UA_StatusCode UA_ServiceFault_encodeBinary(const UA_ServiceFault *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SERVICEFAULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_ServiceFault_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ServiceFault *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SERVICEFAULT]); }

/* UA_FindServersRequest */
static UA_INLINE UA_StatusCode UA_FindServersRequest_encodeBinary(const UA_FindServersRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_FindServersRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_FindServersRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_FINDSERVERSREQUEST]); }

/* UA_FindServersResponse */
static UA_INLINE UA_StatusCode UA_FindServersResponse_encodeBinary(const UA_FindServersResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_FindServersResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_FindServersResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE]); }

/* UA_MessageSecurityMode */
static UA_INLINE UA_StatusCode UA_MessageSecurityMode_encodeBinary(const UA_MessageSecurityMode *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_MessageSecurityMode_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MessageSecurityMode *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_UserTokenType */
static UA_INLINE UA_StatusCode UA_UserTokenType_encodeBinary(const UA_UserTokenType *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_UserTokenType_decodeBinary(const UA_ByteString *src, size_t *offset, UA_UserTokenType *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_UserTokenPolicy */
static UA_INLINE UA_StatusCode UA_UserTokenPolicy_encodeBinary(const UA_UserTokenPolicy *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_USERTOKENPOLICY], dst, offset); }
static UA_INLINE UA_StatusCode UA_UserTokenPolicy_decodeBinary(const UA_ByteString *src, size_t *offset, UA_UserTokenPolicy *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_USERTOKENPOLICY]); }

/* UA_EndpointDescription */
static UA_INLINE UA_StatusCode UA_EndpointDescription_encodeBinary(const UA_EndpointDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_EndpointDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_EndpointDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]); }

/* UA_GetEndpointsRequest */
static UA_INLINE UA_StatusCode UA_GetEndpointsRequest_encodeBinary(const UA_GetEndpointsRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_GetEndpointsRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_GetEndpointsRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST]); }

/* UA_GetEndpointsResponse */
static UA_INLINE UA_StatusCode UA_GetEndpointsResponse_encodeBinary(const UA_GetEndpointsResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_GetEndpointsResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_GetEndpointsResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE]); }

/* UA_SecurityTokenRequestType */
static UA_INLINE UA_StatusCode UA_SecurityTokenRequestType_encodeBinary(const UA_SecurityTokenRequestType *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_SecurityTokenRequestType_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SecurityTokenRequestType *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ChannelSecurityToken */
static UA_INLINE UA_StatusCode UA_ChannelSecurityToken_encodeBinary(const UA_ChannelSecurityToken *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN], dst, offset); }
static UA_INLINE UA_StatusCode UA_ChannelSecurityToken_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ChannelSecurityToken *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CHANNELSECURITYTOKEN]); }

/* UA_OpenSecureChannelRequest */
static UA_INLINE UA_StatusCode UA_OpenSecureChannelRequest_encodeBinary(const UA_OpenSecureChannelRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_OpenSecureChannelRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_OpenSecureChannelRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OPENSECURECHANNELREQUEST]); }

/* UA_OpenSecureChannelResponse */
static UA_INLINE UA_StatusCode UA_OpenSecureChannelResponse_encodeBinary(const UA_OpenSecureChannelResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_OpenSecureChannelResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_OpenSecureChannelResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OPENSECURECHANNELRESPONSE]); }

/* UA_CloseSecureChannelRequest */
static UA_INLINE UA_StatusCode UA_CloseSecureChannelRequest_encodeBinary(const UA_CloseSecureChannelRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CloseSecureChannelRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CloseSecureChannelRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST]); }

/* UA_CloseSecureChannelResponse */
static UA_INLINE UA_StatusCode UA_CloseSecureChannelResponse_encodeBinary(const UA_CloseSecureChannelResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_CloseSecureChannelResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CloseSecureChannelResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELRESPONSE]); }

/* UA_SignedSoftwareCertificate */
static UA_INLINE UA_StatusCode UA_SignedSoftwareCertificate_encodeBinary(const UA_SignedSoftwareCertificate *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE], dst, offset); }
static UA_INLINE UA_StatusCode UA_SignedSoftwareCertificate_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SignedSoftwareCertificate *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SIGNEDSOFTWARECERTIFICATE]); }

/* UA_SignatureData */
static UA_INLINE UA_StatusCode UA_SignatureData_encodeBinary(const UA_SignatureData *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SIGNATUREDATA], dst, offset); }
static UA_INLINE UA_StatusCode UA_SignatureData_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SignatureData *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SIGNATUREDATA]); }

/* UA_CreateSessionRequest */
static UA_INLINE UA_StatusCode UA_CreateSessionRequest_encodeBinary(const UA_CreateSessionRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CreateSessionRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CreateSessionRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST]); }

/* UA_CreateSessionResponse */
static UA_INLINE UA_StatusCode UA_CreateSessionResponse_encodeBinary(const UA_CreateSessionResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_CreateSessionResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CreateSessionResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE]); }

/* UA_UserIdentityToken */
static UA_INLINE UA_StatusCode UA_UserIdentityToken_encodeBinary(const UA_UserIdentityToken *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN], dst, offset); }
static UA_INLINE UA_StatusCode UA_UserIdentityToken_decodeBinary(const UA_ByteString *src, size_t *offset, UA_UserIdentityToken *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_USERIDENTITYTOKEN]); }

/* UA_AnonymousIdentityToken */
static UA_INLINE UA_StatusCode UA_AnonymousIdentityToken_encodeBinary(const UA_AnonymousIdentityToken *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN], dst, offset); }
static UA_INLINE UA_StatusCode UA_AnonymousIdentityToken_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AnonymousIdentityToken *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN]); }

/* UA_UserNameIdentityToken */
static UA_INLINE UA_StatusCode UA_UserNameIdentityToken_encodeBinary(const UA_UserNameIdentityToken *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN], dst, offset); }
static UA_INLINE UA_StatusCode UA_UserNameIdentityToken_decodeBinary(const UA_ByteString *src, size_t *offset, UA_UserNameIdentityToken *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_USERNAMEIDENTITYTOKEN]); }

/* UA_ActivateSessionRequest */
static UA_INLINE UA_StatusCode UA_ActivateSessionRequest_encodeBinary(const UA_ActivateSessionRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_ActivateSessionRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ActivateSessionRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST]); }

/* UA_ActivateSessionResponse */
static UA_INLINE UA_StatusCode UA_ActivateSessionResponse_encodeBinary(const UA_ActivateSessionResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_ActivateSessionResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ActivateSessionResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE]); }

/* UA_CloseSessionRequest */
static UA_INLINE UA_StatusCode UA_CloseSessionRequest_encodeBinary(const UA_CloseSessionRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CloseSessionRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CloseSessionRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST]); }

/* UA_CloseSessionResponse */
static UA_INLINE UA_StatusCode UA_CloseSessionResponse_encodeBinary(const UA_CloseSessionResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_CloseSessionResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CloseSessionResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE]); }

/* UA_NodeAttributesMask */
static UA_INLINE UA_StatusCode UA_NodeAttributesMask_encodeBinary(const UA_NodeAttributesMask *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_NodeAttributesMask_decodeBinary(const UA_ByteString *src, size_t *offset, UA_NodeAttributesMask *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_NodeAttributes */
static UA_INLINE UA_StatusCode UA_NodeAttributes_encodeBinary(const UA_NodeAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_NODEATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_NodeAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_NodeAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_NODEATTRIBUTES]); }

/* UA_ObjectAttributes */
static UA_INLINE UA_StatusCode UA_ObjectAttributes_encodeBinary(const UA_ObjectAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_ObjectAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ObjectAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES]); }

/* UA_VariableAttributes */
static UA_INLINE UA_StatusCode UA_VariableAttributes_encodeBinary(const UA_VariableAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_VariableAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_VariableAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES]); }

/* UA_MethodAttributes */
static UA_INLINE UA_StatusCode UA_MethodAttributes_encodeBinary(const UA_MethodAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_METHODATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_MethodAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MethodAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_METHODATTRIBUTES]); }

/* UA_ObjectTypeAttributes */
static UA_INLINE UA_StatusCode UA_ObjectTypeAttributes_encodeBinary(const UA_ObjectTypeAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_ObjectTypeAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ObjectTypeAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES]); }

/* UA_VariableTypeAttributes */
static UA_INLINE UA_StatusCode UA_VariableTypeAttributes_encodeBinary(const UA_VariableTypeAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VARIABLETYPEATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_VariableTypeAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_VariableTypeAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VARIABLETYPEATTRIBUTES]); }

/* UA_ReferenceTypeAttributes */
static UA_INLINE UA_StatusCode UA_ReferenceTypeAttributes_encodeBinary(const UA_ReferenceTypeAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_ReferenceTypeAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ReferenceTypeAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES]); }

/* UA_DataTypeAttributes */
static UA_INLINE UA_StatusCode UA_DataTypeAttributes_encodeBinary(const UA_DataTypeAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_DataTypeAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DataTypeAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES]); }

/* UA_ViewAttributes */
static UA_INLINE UA_StatusCode UA_ViewAttributes_encodeBinary(const UA_ViewAttributes *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES], dst, offset); }
static UA_INLINE UA_StatusCode UA_ViewAttributes_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewAttributes *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VIEWATTRIBUTES]); }

/* UA_AddNodesItem */
static UA_INLINE UA_StatusCode UA_AddNodesItem_encodeBinary(const UA_AddNodesItem *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESITEM], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddNodesItem_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddNodesItem *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESITEM]); }

/* UA_AddNodesResult */
static UA_INLINE UA_StatusCode UA_AddNodesResult_encodeBinary(const UA_AddNodesResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESRESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddNodesResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddNodesResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESRESULT]); }

/* UA_AddNodesRequest */
static UA_INLINE UA_StatusCode UA_AddNodesRequest_encodeBinary(const UA_AddNodesRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddNodesRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddNodesRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESREQUEST]); }

/* UA_AddNodesResponse */
static UA_INLINE UA_StatusCode UA_AddNodesResponse_encodeBinary(const UA_AddNodesResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddNodesResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddNodesResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDNODESRESPONSE]); }

/* UA_AddReferencesItem */
static UA_INLINE UA_StatusCode UA_AddReferencesItem_encodeBinary(const UA_AddReferencesItem *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddReferencesItem_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddReferencesItem *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESITEM]); }

/* UA_AddReferencesRequest */
static UA_INLINE UA_StatusCode UA_AddReferencesRequest_encodeBinary(const UA_AddReferencesRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddReferencesRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddReferencesRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESREQUEST]); }

/* UA_AddReferencesResponse */
static UA_INLINE UA_StatusCode UA_AddReferencesResponse_encodeBinary(const UA_AddReferencesResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_AddReferencesResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AddReferencesResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_ADDREFERENCESRESPONSE]); }

/* UA_DeleteNodesItem */
static UA_INLINE UA_StatusCode UA_DeleteNodesItem_encodeBinary(const UA_DeleteNodesItem *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETENODESITEM], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteNodesItem_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteNodesItem *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETENODESITEM]); }

/* UA_DeleteNodesRequest */
static UA_INLINE UA_StatusCode UA_DeleteNodesRequest_encodeBinary(const UA_DeleteNodesRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETENODESREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteNodesRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteNodesRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETENODESREQUEST]); }

/* UA_DeleteNodesResponse */
static UA_INLINE UA_StatusCode UA_DeleteNodesResponse_encodeBinary(const UA_DeleteNodesResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteNodesResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteNodesResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETENODESRESPONSE]); }

/* UA_DeleteReferencesItem */
static UA_INLINE UA_StatusCode UA_DeleteReferencesItem_encodeBinary(const UA_DeleteReferencesItem *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteReferencesItem_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteReferencesItem *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESITEM]); }

/* UA_DeleteReferencesRequest */
static UA_INLINE UA_StatusCode UA_DeleteReferencesRequest_encodeBinary(const UA_DeleteReferencesRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteReferencesRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteReferencesRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESREQUEST]); }

/* UA_DeleteReferencesResponse */
static UA_INLINE UA_StatusCode UA_DeleteReferencesResponse_encodeBinary(const UA_DeleteReferencesResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteReferencesResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteReferencesResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETEREFERENCESRESPONSE]); }

/* UA_BrowseDirection */
static UA_INLINE UA_StatusCode UA_BrowseDirection_encodeBinary(const UA_BrowseDirection *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseDirection_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseDirection *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ViewDescription */
static UA_INLINE UA_StatusCode UA_ViewDescription_encodeBinary(const UA_ViewDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_ViewDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ViewDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_VIEWDESCRIPTION]); }

/* UA_BrowseDescription */
static UA_INLINE UA_StatusCode UA_BrowseDescription_encodeBinary(const UA_BrowseDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEDESCRIPTION]); }

/* UA_BrowseResultMask */
static UA_INLINE UA_StatusCode UA_BrowseResultMask_encodeBinary(const UA_BrowseResultMask *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseResultMask_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseResultMask *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ReferenceDescription */
static UA_INLINE UA_StatusCode UA_ReferenceDescription_encodeBinary(const UA_ReferenceDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_ReferenceDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ReferenceDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION]); }

/* UA_BrowseResult */
static UA_INLINE UA_StatusCode UA_BrowseResult_encodeBinary(const UA_BrowseResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSERESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSERESULT]); }

/* UA_BrowseRequest */
static UA_INLINE UA_StatusCode UA_BrowseRequest_encodeBinary(const UA_BrowseRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEREQUEST]); }

/* UA_BrowseResponse */
static UA_INLINE UA_StatusCode UA_BrowseResponse_encodeBinary(const UA_BrowseResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSERESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSERESPONSE]); }

/* UA_BrowseNextRequest */
static UA_INLINE UA_StatusCode UA_BrowseNextRequest_encodeBinary(const UA_BrowseNextRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseNextRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseNextRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSENEXTREQUEST]); }

/* UA_BrowseNextResponse */
static UA_INLINE UA_StatusCode UA_BrowseNextResponse_encodeBinary(const UA_BrowseNextResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowseNextResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowseNextResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSENEXTRESPONSE]); }

/* UA_RelativePathElement */
static UA_INLINE UA_StatusCode UA_RelativePathElement_encodeBinary(const UA_RelativePathElement *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT], dst, offset); }
static UA_INLINE UA_StatusCode UA_RelativePathElement_decodeBinary(const UA_ByteString *src, size_t *offset, UA_RelativePathElement *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]); }

/* UA_RelativePath */
static UA_INLINE UA_StatusCode UA_RelativePath_encodeBinary(const UA_RelativePath *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_RELATIVEPATH], dst, offset); }
static UA_INLINE UA_StatusCode UA_RelativePath_decodeBinary(const UA_ByteString *src, size_t *offset, UA_RelativePath *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_RELATIVEPATH]); }

/* UA_BrowsePath */
static UA_INLINE UA_StatusCode UA_BrowsePath_encodeBinary(const UA_BrowsePath *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEPATH], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowsePath_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowsePath *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEPATH]); }

/* UA_BrowsePathTarget */
static UA_INLINE UA_StatusCode UA_BrowsePathTarget_encodeBinary(const UA_BrowsePathTarget *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowsePathTarget_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowsePathTarget *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET]); }

/* UA_BrowsePathResult */
static UA_INLINE UA_StatusCode UA_BrowsePathResult_encodeBinary(const UA_BrowsePathResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_BrowsePathResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BrowsePathResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT]); }

/* UA_TranslateBrowsePathsToNodeIdsRequest */
static UA_INLINE UA_StatusCode UA_TranslateBrowsePathsToNodeIdsRequest_encodeBinary(const UA_TranslateBrowsePathsToNodeIdsRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_TranslateBrowsePathsToNodeIdsRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_TranslateBrowsePathsToNodeIdsRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST]); }

/* UA_TranslateBrowsePathsToNodeIdsResponse */
static UA_INLINE UA_StatusCode UA_TranslateBrowsePathsToNodeIdsResponse_encodeBinary(const UA_TranslateBrowsePathsToNodeIdsResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_TranslateBrowsePathsToNodeIdsResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_TranslateBrowsePathsToNodeIdsResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE]); }

/* UA_RegisterNodesRequest */
static UA_INLINE UA_StatusCode UA_RegisterNodesRequest_encodeBinary(const UA_RegisterNodesRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_RegisterNodesRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_RegisterNodesRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REGISTERNODESREQUEST]); }

/* UA_RegisterNodesResponse */
static UA_INLINE UA_StatusCode UA_RegisterNodesResponse_encodeBinary(const UA_RegisterNodesResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_RegisterNodesResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_RegisterNodesResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_REGISTERNODESRESPONSE]); }

/* UA_UnregisterNodesRequest */
static UA_INLINE UA_StatusCode UA_UnregisterNodesRequest_encodeBinary(const UA_UnregisterNodesRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_UnregisterNodesRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_UnregisterNodesRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_UNREGISTERNODESREQUEST]); }

/* UA_UnregisterNodesResponse */
static UA_INLINE UA_StatusCode UA_UnregisterNodesResponse_encodeBinary(const UA_UnregisterNodesResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_UnregisterNodesResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_UnregisterNodesResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_UNREGISTERNODESRESPONSE]); }

/* UA_QueryDataDescription */
static UA_INLINE UA_StatusCode UA_QueryDataDescription_encodeBinary(const UA_QueryDataDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_QUERYDATADESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_QueryDataDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_QueryDataDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_QUERYDATADESCRIPTION]); }

/* UA_NodeTypeDescription */
static UA_INLINE UA_StatusCode UA_NodeTypeDescription_encodeBinary(const UA_NodeTypeDescription *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_NODETYPEDESCRIPTION], dst, offset); }
static UA_INLINE UA_StatusCode UA_NodeTypeDescription_decodeBinary(const UA_ByteString *src, size_t *offset, UA_NodeTypeDescription *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_NODETYPEDESCRIPTION]); }

/* UA_FilterOperator */
static UA_INLINE UA_StatusCode UA_FilterOperator_encodeBinary(const UA_FilterOperator *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_FilterOperator_decodeBinary(const UA_ByteString *src, size_t *offset, UA_FilterOperator *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_QueryDataSet */
static UA_INLINE UA_StatusCode UA_QueryDataSet_encodeBinary(const UA_QueryDataSet *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_QUERYDATASET], dst, offset); }
static UA_INLINE UA_StatusCode UA_QueryDataSet_decodeBinary(const UA_ByteString *src, size_t *offset, UA_QueryDataSet *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_QUERYDATASET]); }

/* UA_ContentFilterElement */
static UA_INLINE UA_StatusCode UA_ContentFilterElement_encodeBinary(const UA_ContentFilterElement *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENT], dst, offset); }
static UA_INLINE UA_StatusCode UA_ContentFilterElement_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ContentFilterElement *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENT]); }

/* UA_ContentFilter */
static UA_INLINE UA_StatusCode UA_ContentFilter_encodeBinary(const UA_ContentFilter *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CONTENTFILTER], dst, offset); }
static UA_INLINE UA_StatusCode UA_ContentFilter_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ContentFilter *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CONTENTFILTER]); }

/* UA_ContentFilterElementResult */
static UA_INLINE UA_StatusCode UA_ContentFilterElementResult_encodeBinary(const UA_ContentFilterElementResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENTRESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_ContentFilterElementResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ContentFilterElementResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CONTENTFILTERELEMENTRESULT]); }

/* UA_ContentFilterResult */
static UA_INLINE UA_StatusCode UA_ContentFilterResult_encodeBinary(const UA_ContentFilterResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CONTENTFILTERRESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_ContentFilterResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ContentFilterResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CONTENTFILTERRESULT]); }

/* UA_ParsingResult */
static UA_INLINE UA_StatusCode UA_ParsingResult_encodeBinary(const UA_ParsingResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_PARSINGRESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_ParsingResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ParsingResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_PARSINGRESULT]); }

/* UA_QueryFirstRequest */
static UA_INLINE UA_StatusCode UA_QueryFirstRequest_encodeBinary(const UA_QueryFirstRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_QUERYFIRSTREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_QueryFirstRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_QueryFirstRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_QUERYFIRSTREQUEST]); }

/* UA_QueryFirstResponse */
static UA_INLINE UA_StatusCode UA_QueryFirstResponse_encodeBinary(const UA_QueryFirstResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_QUERYFIRSTRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_QueryFirstResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_QueryFirstResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_QUERYFIRSTRESPONSE]); }

/* UA_QueryNextRequest */
static UA_INLINE UA_StatusCode UA_QueryNextRequest_encodeBinary(const UA_QueryNextRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_QUERYNEXTREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_QueryNextRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_QueryNextRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_QUERYNEXTREQUEST]); }

/* UA_QueryNextResponse */
static UA_INLINE UA_StatusCode UA_QueryNextResponse_encodeBinary(const UA_QueryNextResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_QUERYNEXTRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_QueryNextResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_QueryNextResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_QUERYNEXTRESPONSE]); }

/* UA_TimestampsToReturn */
static UA_INLINE UA_StatusCode UA_TimestampsToReturn_encodeBinary(const UA_TimestampsToReturn *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_TimestampsToReturn_decodeBinary(const UA_ByteString *src, size_t *offset, UA_TimestampsToReturn *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ReadValueId */
static UA_INLINE UA_StatusCode UA_ReadValueId_encodeBinary(const UA_ReadValueId *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_READVALUEID], dst, offset); }
static UA_INLINE UA_StatusCode UA_ReadValueId_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ReadValueId *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_READVALUEID]); }

/* UA_ReadRequest */
static UA_INLINE UA_StatusCode UA_ReadRequest_encodeBinary(const UA_ReadRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_READREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_ReadRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ReadRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_READREQUEST]); }

/* UA_ReadResponse */
static UA_INLINE UA_StatusCode UA_ReadResponse_encodeBinary(const UA_ReadResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_READRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_ReadResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ReadResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_READRESPONSE]); }

/* UA_WriteValue */
static UA_INLINE UA_StatusCode UA_WriteValue_encodeBinary(const UA_WriteValue *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_WRITEVALUE], dst, offset); }
static UA_INLINE UA_StatusCode UA_WriteValue_decodeBinary(const UA_ByteString *src, size_t *offset, UA_WriteValue *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_WRITEVALUE]); }

/* UA_WriteRequest */
static UA_INLINE UA_StatusCode UA_WriteRequest_encodeBinary(const UA_WriteRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_WRITEREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_WriteRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_WriteRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_WRITEREQUEST]); }

/* UA_WriteResponse */
static UA_INLINE UA_StatusCode UA_WriteResponse_encodeBinary(const UA_WriteResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_WRITERESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_WriteResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_WriteResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_WRITERESPONSE]); }

/* UA_CallMethodRequest */
static UA_INLINE UA_StatusCode UA_CallMethodRequest_encodeBinary(const UA_CallMethodRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CallMethodRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CallMethodRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST]); }

/* UA_CallMethodResult */
static UA_INLINE UA_StatusCode UA_CallMethodResult_encodeBinary(const UA_CallMethodResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLMETHODRESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_CallMethodResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CallMethodResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLMETHODRESULT]); }

/* UA_CallRequest */
static UA_INLINE UA_StatusCode UA_CallRequest_encodeBinary(const UA_CallRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CallRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CallRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLREQUEST]); }

/* UA_CallResponse */
static UA_INLINE UA_StatusCode UA_CallResponse_encodeBinary(const UA_CallResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CALLRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_CallResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CallResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CALLRESPONSE]); }

/* UA_MonitoringMode */
static UA_INLINE UA_StatusCode UA_MonitoringMode_encodeBinary(const UA_MonitoringMode *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_MonitoringMode_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MonitoringMode *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_MonitoringParameters */
static UA_INLINE UA_StatusCode UA_MonitoringParameters_encodeBinary(const UA_MonitoringParameters *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS], dst, offset); }
static UA_INLINE UA_StatusCode UA_MonitoringParameters_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MonitoringParameters *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITORINGPARAMETERS]); }

/* UA_MonitoredItemCreateRequest */
static UA_INLINE UA_StatusCode UA_MonitoredItemCreateRequest_encodeBinary(const UA_MonitoredItemCreateRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_MonitoredItemCreateRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MonitoredItemCreateRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATEREQUEST]); }

/* UA_MonitoredItemCreateResult */
static UA_INLINE UA_StatusCode UA_MonitoredItemCreateResult_encodeBinary(const UA_MonitoredItemCreateResult *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT], dst, offset); }
static UA_INLINE UA_StatusCode UA_MonitoredItemCreateResult_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MonitoredItemCreateResult *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_MONITOREDITEMCREATERESULT]); }

/* UA_CreateMonitoredItemsRequest */
static UA_INLINE UA_StatusCode UA_CreateMonitoredItemsRequest_encodeBinary(const UA_CreateMonitoredItemsRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CreateMonitoredItemsRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CreateMonitoredItemsRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSREQUEST]); }

/* UA_CreateMonitoredItemsResponse */
static UA_INLINE UA_StatusCode UA_CreateMonitoredItemsResponse_encodeBinary(const UA_CreateMonitoredItemsResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_CreateMonitoredItemsResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CreateMonitoredItemsResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATEMONITOREDITEMSRESPONSE]); }

/* UA_CreateSubscriptionRequest */
static UA_INLINE UA_StatusCode UA_CreateSubscriptionRequest_encodeBinary(const UA_CreateSubscriptionRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_CreateSubscriptionRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CreateSubscriptionRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONREQUEST]); }

/* UA_CreateSubscriptionResponse */
static UA_INLINE UA_StatusCode UA_CreateSubscriptionResponse_encodeBinary(const UA_CreateSubscriptionResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_CreateSubscriptionResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_CreateSubscriptionResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_CREATESUBSCRIPTIONRESPONSE]); }

/* UA_SetPublishingModeRequest */
static UA_INLINE UA_StatusCode UA_SetPublishingModeRequest_encodeBinary(const UA_SetPublishingModeRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_SetPublishingModeRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SetPublishingModeRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODEREQUEST]); }

/* UA_SetPublishingModeResponse */
static UA_INLINE UA_StatusCode UA_SetPublishingModeResponse_encodeBinary(const UA_SetPublishingModeResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_SetPublishingModeResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SetPublishingModeResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SETPUBLISHINGMODERESPONSE]); }

/* UA_NotificationMessage */
static UA_INLINE UA_StatusCode UA_NotificationMessage_encodeBinary(const UA_NotificationMessage *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE], dst, offset); }
static UA_INLINE UA_StatusCode UA_NotificationMessage_decodeBinary(const UA_ByteString *src, size_t *offset, UA_NotificationMessage *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_NOTIFICATIONMESSAGE]); }

/* UA_SubscriptionAcknowledgement */
static UA_INLINE UA_StatusCode UA_SubscriptionAcknowledgement_encodeBinary(const UA_SubscriptionAcknowledgement *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT], dst, offset); }
static UA_INLINE UA_StatusCode UA_SubscriptionAcknowledgement_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SubscriptionAcknowledgement *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT]); }

/* UA_PublishRequest */
static UA_INLINE UA_StatusCode UA_PublishRequest_encodeBinary(const UA_PublishRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_PUBLISHREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_PublishRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_PublishRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_PUBLISHREQUEST]); }

/* UA_PublishResponse */
static UA_INLINE UA_StatusCode UA_PublishResponse_encodeBinary(const UA_PublishResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_PublishResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_PublishResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_PUBLISHRESPONSE]); }

/* UA_DeleteSubscriptionsRequest */
static UA_INLINE UA_StatusCode UA_DeleteSubscriptionsRequest_encodeBinary(const UA_DeleteSubscriptionsRequest *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteSubscriptionsRequest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteSubscriptionsRequest *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSREQUEST]); }

/* UA_DeleteSubscriptionsResponse */
static UA_INLINE UA_StatusCode UA_DeleteSubscriptionsResponse_encodeBinary(const UA_DeleteSubscriptionsResponse *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE], dst, offset); }
static UA_INLINE UA_StatusCode UA_DeleteSubscriptionsResponse_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DeleteSubscriptionsResponse *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_DELETESUBSCRIPTIONSRESPONSE]); }

/* UA_BuildInfo */
static UA_INLINE UA_StatusCode UA_BuildInfo_encodeBinary(const UA_BuildInfo *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_BUILDINFO], dst, offset); }
static UA_INLINE UA_StatusCode UA_BuildInfo_decodeBinary(const UA_ByteString *src, size_t *offset, UA_BuildInfo *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_BUILDINFO]); }

/* UA_ServerState */
static UA_INLINE UA_StatusCode UA_ServerState_encodeBinary(const UA_ServerState *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_ServerState_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ServerState *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_ServerStatusDataType */
static UA_INLINE UA_StatusCode UA_ServerStatusDataType_encodeBinary(const UA_ServerStatusDataType *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE], dst, offset); }
static UA_INLINE UA_StatusCode UA_ServerStatusDataType_decodeBinary(const UA_ByteString *src, size_t *offset, UA_ServerStatusDataType *dst) { return UA_decodeBinary(src, offset, dst, &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE]); }

/*********************************** amalgamated original file "C:/open62541/build/src_generated/ua_transport_generated.h" ***********************************/

/**
* @file ua_transport_generated.h
*
* @brief Autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/open62541/tools/generate_datatypes.py
* on host opcua-PC by user opcua at 2015-11-18 11:21:45
*/


#ifdef __cplusplus
extern "C" {
#endif

#ifdef UA_INTERNAL
#endif


/**
* @ingroup types
*
* @defgroup ua_transport_generated Autogenerated ua_transport Types
*
* @brief Data structures that are autogenerated from an XML-Schema.
*
* @{
*/

#define UA_TRANSPORT_COUNT 10

extern UA_EXPORT const UA_DataType *UA_TRANSPORT;


/** @brief Message Type and whether the message contains the final chunk */
typedef enum { 
    UA_MESSAGETYPEANDFINAL_ACKF = 1179337537,
    UA_MESSAGETYPEANDFINAL_CLOF = 1179601987,
    UA_MESSAGETYPEANDFINAL_HELF = 1179403592,
    UA_MESSAGETYPEANDFINAL_MSGF = 1179079501,
    UA_MESSAGETYPEANDFINAL_OPNF = 1179537487
} UA_MessageTypeAndFinal;
#define UA_TRANSPORT_MESSAGETYPEANDFINAL 0
static UA_INLINE UA_MessageTypeAndFinal * UA_MessageTypeAndFinal_new(void) { UA_MessageTypeAndFinal *v = (UA_MessageTypeAndFinal*)UA_Int32_new(); return v; }
static UA_INLINE void UA_MessageTypeAndFinal_init(UA_MessageTypeAndFinal *p) { UA_Int32_init((UA_Int32*)p); }
static UA_INLINE void UA_MessageTypeAndFinal_delete(UA_MessageTypeAndFinal *p) { UA_Int32_delete((UA_Int32*)p); }
static UA_INLINE void UA_MessageTypeAndFinal_deleteMembers(UA_MessageTypeAndFinal *p) { }
static UA_INLINE UA_StatusCode UA_MessageTypeAndFinal_copy(const UA_MessageTypeAndFinal *src, UA_MessageTypeAndFinal *dst) { return UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst); }

/** @brief TCP Header */
typedef struct {
    UA_UInt32 messageTypeAndFinal;
    UA_UInt32 messageSize;
} UA_TcpMessageHeader;
#define UA_TRANSPORT_TCPMESSAGEHEADER 1
static UA_INLINE void UA_TcpMessageHeader_init(UA_TcpMessageHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE void UA_TcpMessageHeader_delete(UA_TcpMessageHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE void UA_TcpMessageHeader_deleteMembers(UA_TcpMessageHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE UA_TcpMessageHeader * UA_TcpMessageHeader_new(void) { return (UA_TcpMessageHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE UA_StatusCode UA_TcpMessageHeader_copy(const UA_TcpMessageHeader *src, UA_TcpMessageHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }

/** @brief Hello Message */
typedef struct {
    UA_UInt32 protocolVersion;
    UA_UInt32 receiveBufferSize;
    UA_UInt32 sendBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
    UA_String endpointUrl;
} UA_TcpHelloMessage;
#define UA_TRANSPORT_TCPHELLOMESSAGE 2
static UA_INLINE void UA_TcpHelloMessage_init(UA_TcpHelloMessage *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE void UA_TcpHelloMessage_delete(UA_TcpHelloMessage *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE void UA_TcpHelloMessage_deleteMembers(UA_TcpHelloMessage *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE UA_TcpHelloMessage * UA_TcpHelloMessage_new(void) { return (UA_TcpHelloMessage *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE UA_StatusCode UA_TcpHelloMessage_copy(const UA_TcpHelloMessage *src, UA_TcpHelloMessage *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }

/** @brief Acknowledge Message */
typedef struct {
    UA_UInt32 protocolVersion;
    UA_UInt32 receiveBufferSize;
    UA_UInt32 sendBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
} UA_TcpAcknowledgeMessage;
#define UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE 3
static UA_INLINE void UA_TcpAcknowledgeMessage_init(UA_TcpAcknowledgeMessage *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE void UA_TcpAcknowledgeMessage_delete(UA_TcpAcknowledgeMessage *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE void UA_TcpAcknowledgeMessage_deleteMembers(UA_TcpAcknowledgeMessage *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE UA_TcpAcknowledgeMessage * UA_TcpAcknowledgeMessage_new(void) { return (UA_TcpAcknowledgeMessage *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE UA_StatusCode UA_TcpAcknowledgeMessage_copy(const UA_TcpAcknowledgeMessage *src, UA_TcpAcknowledgeMessage *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }

/** @brief Secure Layer Sequence Header */
typedef struct {
    UA_TcpMessageHeader messageHeader;
    UA_UInt32 secureChannelId;
} UA_SecureConversationMessageHeader;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER 4
static UA_INLINE void UA_SecureConversationMessageHeader_init(UA_SecureConversationMessageHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE void UA_SecureConversationMessageHeader_delete(UA_SecureConversationMessageHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE void UA_SecureConversationMessageHeader_deleteMembers(UA_SecureConversationMessageHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE UA_SecureConversationMessageHeader * UA_SecureConversationMessageHeader_new(void) { return (UA_SecureConversationMessageHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageHeader_copy(const UA_SecureConversationMessageHeader *src, UA_SecureConversationMessageHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }

/** @brief Security Header */
typedef struct {
    UA_ByteString securityPolicyUri;
    UA_ByteString senderCertificate;
    UA_ByteString receiverCertificateThumbprint;
} UA_AsymmetricAlgorithmSecurityHeader;
#define UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER 5
static UA_INLINE void UA_AsymmetricAlgorithmSecurityHeader_init(UA_AsymmetricAlgorithmSecurityHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_AsymmetricAlgorithmSecurityHeader_delete(UA_AsymmetricAlgorithmSecurityHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(UA_AsymmetricAlgorithmSecurityHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_AsymmetricAlgorithmSecurityHeader * UA_AsymmetricAlgorithmSecurityHeader_new(void) { return (UA_AsymmetricAlgorithmSecurityHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_StatusCode UA_AsymmetricAlgorithmSecurityHeader_copy(const UA_AsymmetricAlgorithmSecurityHeader *src, UA_AsymmetricAlgorithmSecurityHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }

/** @brief Secure Layer Symmetric Algorithm Header */
typedef struct {
    UA_UInt32 tokenId;
} UA_SymmetricAlgorithmSecurityHeader;
#define UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER 6
static UA_INLINE void UA_SymmetricAlgorithmSecurityHeader_init(UA_SymmetricAlgorithmSecurityHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_SymmetricAlgorithmSecurityHeader_delete(UA_SymmetricAlgorithmSecurityHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_SymmetricAlgorithmSecurityHeader_deleteMembers(UA_SymmetricAlgorithmSecurityHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_SymmetricAlgorithmSecurityHeader * UA_SymmetricAlgorithmSecurityHeader_new(void) { return (UA_SymmetricAlgorithmSecurityHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_StatusCode UA_SymmetricAlgorithmSecurityHeader_copy(const UA_SymmetricAlgorithmSecurityHeader *src, UA_SymmetricAlgorithmSecurityHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }

/** @brief Secure Layer Sequence Header */
typedef struct {
    UA_UInt32 sequenceNumber;
    UA_UInt32 requestId;
} UA_SequenceHeader;
#define UA_TRANSPORT_SEQUENCEHEADER 7
static UA_INLINE void UA_SequenceHeader_init(UA_SequenceHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE void UA_SequenceHeader_delete(UA_SequenceHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE void UA_SequenceHeader_deleteMembers(UA_SequenceHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE UA_SequenceHeader * UA_SequenceHeader_new(void) { return (UA_SequenceHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE UA_StatusCode UA_SequenceHeader_copy(const UA_SequenceHeader *src, UA_SequenceHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }

/** @brief Secure Conversation Message Footer */
typedef struct {
    UA_Int32 paddingSize;
    UA_Byte *padding;
    UA_Byte signature;
} UA_SecureConversationMessageFooter;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER 8
static UA_INLINE void UA_SecureConversationMessageFooter_init(UA_SecureConversationMessageFooter *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE void UA_SecureConversationMessageFooter_delete(UA_SecureConversationMessageFooter *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE void UA_SecureConversationMessageFooter_deleteMembers(UA_SecureConversationMessageFooter *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE UA_SecureConversationMessageFooter * UA_SecureConversationMessageFooter_new(void) { return (UA_SecureConversationMessageFooter *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageFooter_copy(const UA_SecureConversationMessageFooter *src, UA_SecureConversationMessageFooter *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }

/** @brief Secure Conversation Message Abort Body */
typedef struct {
    UA_UInt32 error;
    UA_String reason;
} UA_SecureConversationMessageAbortBody;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY 9
static UA_INLINE void UA_SecureConversationMessageAbortBody_init(UA_SecureConversationMessageAbortBody *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE void UA_SecureConversationMessageAbortBody_delete(UA_SecureConversationMessageAbortBody *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE void UA_SecureConversationMessageAbortBody_deleteMembers(UA_SecureConversationMessageAbortBody *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE UA_SecureConversationMessageAbortBody * UA_SecureConversationMessageAbortBody_new(void) { return (UA_SecureConversationMessageAbortBody *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageAbortBody_copy(const UA_SecureConversationMessageAbortBody *src, UA_SecureConversationMessageAbortBody *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }

/// @} /* end of group */

#ifdef __cplusplus
} // extern "C"
#endif


/*********************************** amalgamated original file "C:/open62541/build/src_generated/ua_transport_generated_encoding_binary.h" ***********************************/

/**
* @file ua_transport_generated_encoding_binary.h
*
* @brief Binary encoding for autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/open62541/tools/generate_datatypes.py
* on host opcua-PC by user opcua at 2015-11-18 11:21:45
*/


/* UA_MessageTypeAndFinal */
static UA_INLINE UA_StatusCode UA_MessageTypeAndFinal_encodeBinary(const UA_MessageTypeAndFinal *src, UA_ByteString *dst, size_t *offset) { return UA_Int32_encodeBinary((const UA_Int32*)src, dst, offset); }
static UA_INLINE UA_StatusCode UA_MessageTypeAndFinal_decodeBinary(const UA_ByteString *src, size_t *offset, UA_MessageTypeAndFinal *dst) { return UA_Int32_decodeBinary(src, offset, (UA_Int32*)dst); }

/* UA_TcpMessageHeader */
static UA_INLINE UA_StatusCode UA_TcpMessageHeader_encodeBinary(const UA_TcpMessageHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_TcpMessageHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_TcpMessageHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }

/* UA_TcpHelloMessage */
static UA_INLINE UA_StatusCode UA_TcpHelloMessage_encodeBinary(const UA_TcpHelloMessage *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE], dst, offset); }
static UA_INLINE UA_StatusCode UA_TcpHelloMessage_decodeBinary(const UA_ByteString *src, size_t *offset, UA_TcpHelloMessage *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }

/* UA_TcpAcknowledgeMessage */
static UA_INLINE UA_StatusCode UA_TcpAcknowledgeMessage_encodeBinary(const UA_TcpAcknowledgeMessage *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE], dst, offset); }
static UA_INLINE UA_StatusCode UA_TcpAcknowledgeMessage_decodeBinary(const UA_ByteString *src, size_t *offset, UA_TcpAcknowledgeMessage *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }

/* UA_SecureConversationMessageHeader */
static UA_INLINE UA_StatusCode UA_SecureConversationMessageHeader_encodeBinary(const UA_SecureConversationMessageHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SecureConversationMessageHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }

/* UA_AsymmetricAlgorithmSecurityHeader */
static UA_INLINE UA_StatusCode UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(const UA_AsymmetricAlgorithmSecurityHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_AsymmetricAlgorithmSecurityHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }

/* UA_SymmetricAlgorithmSecurityHeader */
static UA_INLINE UA_StatusCode UA_SymmetricAlgorithmSecurityHeader_encodeBinary(const UA_SymmetricAlgorithmSecurityHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_SymmetricAlgorithmSecurityHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SymmetricAlgorithmSecurityHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }

/* UA_SequenceHeader */
static UA_INLINE UA_StatusCode UA_SequenceHeader_encodeBinary(const UA_SequenceHeader *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER], dst, offset); }
static UA_INLINE UA_StatusCode UA_SequenceHeader_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SequenceHeader *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }

/* UA_SecureConversationMessageFooter */
static UA_INLINE UA_StatusCode UA_SecureConversationMessageFooter_encodeBinary(const UA_SecureConversationMessageFooter *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER], dst, offset); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageFooter_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SecureConversationMessageFooter *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }

/* UA_SecureConversationMessageAbortBody */
static UA_INLINE UA_StatusCode UA_SecureConversationMessageAbortBody_encodeBinary(const UA_SecureConversationMessageAbortBody *src, UA_ByteString *dst, size_t *offset) { return UA_encodeBinary(src, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY], dst, offset); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageAbortBody_decodeBinary(const UA_ByteString *src, size_t *offset, UA_SecureConversationMessageAbortBody *dst) { return UA_decodeBinary(src, offset, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }

/*********************************** amalgamated original file "C:/open62541/src/ua_securechannel.h" ***********************************/



/**
 *  @ingroup communication
 *
 * @{
 */

struct UA_Session;
typedef struct UA_Session UA_Session;

struct SessionEntry {
    LIST_ENTRY(SessionEntry) pointers;
    UA_Session *session; // Just a pointer. The session is held in the session manager or the client
};

struct UA_SecureChannel {
    UA_MessageSecurityMode  securityMode;
    UA_ChannelSecurityToken securityToken; // the channelId is contained in the securityToken
    UA_ChannelSecurityToken nextSecurityToken; // the channelId is contained in the securityToken
    UA_AsymmetricAlgorithmSecurityHeader clientAsymAlgSettings;
    UA_AsymmetricAlgorithmSecurityHeader serverAsymAlgSettings;
    UA_ByteString  clientNonce;
    UA_ByteString  serverNonce;
    UA_UInt32      sequenceNumber;
    UA_Connection *connection;
    LIST_HEAD(session_pointerlist, SessionEntry) sessions;
};

void UA_SecureChannel_init(UA_SecureChannel *channel);
void UA_SecureChannel_deleteMembersCleanup(UA_SecureChannel *channel);

UA_StatusCode UA_SecureChannel_generateNonce(UA_ByteString *nonce);

void UA_SecureChannel_attachSession(UA_SecureChannel *channel, UA_Session *session);
void UA_SecureChannel_detachSession(UA_SecureChannel *channel, UA_Session *session);
UA_Session * UA_SecureChannel_getSession(UA_SecureChannel *channel, UA_NodeId *token);

UA_StatusCode UA_SecureChannel_sendBinaryMessage(UA_SecureChannel *channel, UA_UInt32 requestId,
                                                  const void *content, const UA_DataType *contentType);

void UA_SecureChannel_revolveTokens(UA_SecureChannel *channel);
/** @} */


/*********************************** amalgamated original file "C:/open62541/src/server/ua_nodes.h" ***********************************/



#define UA_TYPE_HANDLING_FUNCTIONS(TYPE)                             \
    TYPE UA_EXPORT * TYPE##_new(void);                               \
    void UA_EXPORT TYPE##_init(TYPE * p);                            \
    void UA_EXPORT TYPE##_delete(TYPE * p);                          \
    void UA_EXPORT TYPE##_deleteMembers(TYPE * p);                   \
    UA_StatusCode UA_EXPORT TYPE##_copy(const TYPE *src, TYPE *dst);

#define UA_STANDARD_NODEMEMBERS                 \
    UA_NodeId nodeId;                           \
    UA_NodeClass nodeClass;                     \
    UA_QualifiedName browseName;                \
    UA_LocalizedText displayName;               \
    UA_LocalizedText description;               \
    UA_UInt32 writeMask;                        \
    UA_UInt32 userWriteMask;                    \
    UA_Int32 referencesSize;                    \
    UA_ReferenceNode *references;

typedef struct {
    UA_STANDARD_NODEMEMBERS
} UA_Node;

void UA_Node_deleteAnyNodeClass(UA_Node *node);
UA_Node * UA_Node_copyAnyNodeClass(const UA_Node *node);

/**************/
/* ObjectNode */
/**************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Byte eventNotifier;
    void *instanceHandle;
} UA_ObjectNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ObjectNode)

/******************/
/* ObjectTypeNode */
/******************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean isAbstract;
    UA_ObjectLifecycleManagement lifecycleManagement;
} UA_ObjectTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ObjectTypeNode)

typedef enum {
    UA_VALUESOURCE_VARIANT,
    UA_VALUESOURCE_DATASOURCE
} UA_ValueSource;

/****************/
/* VariableNode */
/****************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Int32 valueRank; /**< n >= 1: the value is an array with the specified number of dimensions.
                             n = 0: the value is an array with one or more dimensions.
                             n = -1: the value is a scalar.
                             n = -2: the value can be a scalar or an array with any number of dimensions.
                             n = -3:  the value can be a scalar or a one dimensional array. */
    UA_ValueSource valueSource;
    union {
        struct {
        UA_Variant value;
        UA_ValueCallback callback;
        } variant;
        UA_DataSource dataSource;
    } value;
    /* <--- similar to variabletypenodes up to there--->*/
    UA_Byte accessLevel;
    UA_Byte userAccessLevel;
    UA_Double minimumSamplingInterval;
    UA_Boolean historizing;
} UA_VariableNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_VariableNode)
/** Make a copy but leave out the references and the variable */
UA_StatusCode UA_VariableNode_copyWithoutRefsAndVariable(const UA_VariableNode *src, UA_VariableNode *dst);

/********************/
/* VariableTypeNode */
/********************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Int32 valueRank;
    UA_ValueSource valueSource;
    union {
        struct {
            UA_Variant value;
            UA_ValueCallback callback;
        } variant;
        UA_DataSource dataSource;
    } value;
    /* <--- similar to variablenodes up to there--->*/
    UA_Boolean isAbstract;
} UA_VariableTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_VariableTypeNode)

/*********************/
/* ReferenceTypeNode */
/*********************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean isAbstract;
    UA_Boolean symmetric;
    UA_LocalizedText inverseName;
} UA_ReferenceTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ReferenceTypeNode)

/***********************/
/* ReferenceMethodNode */
/***********************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean executable;
    UA_Boolean userExecutable;
#ifdef ENABLE_METHODCALLS
    void *methodHandle;
    UA_MethodCallback attachedMethod;
#endif
} UA_MethodNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_MethodNode)

/************/
/* ViewNode */
/************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Byte eventNotifier;
    /* <-- the same as objectnode until here --> */
    UA_Boolean containsNoLoops;
} UA_ViewNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_ViewNode)

/****************/
/* DataTypeNode */
/****************/

typedef struct {
    UA_STANDARD_NODEMEMBERS
    UA_Boolean isAbstract;
} UA_DataTypeNode;
UA_TYPE_HANDLING_FUNCTIONS(UA_DataTypeNode)


/*********************************** amalgamated original file "C:/open62541/src/ua_session.h" ***********************************/



#define MAXCONTINUATIONPOINTS 5

#ifdef ENABLE_SUBSCRIPTIONS
#endif

/**
 *  @ingroup communication
 *
 * @{
 */

struct ContinuationPointEntry {
    LIST_ENTRY(ContinuationPointEntry) pointers;
    UA_ByteString        identifier;
    UA_BrowseDescription browseDescription;
    UA_Int32            continuationIndex;
    UA_UInt32            maxReferences;
};

struct UA_Session {
    UA_ApplicationDescription clientDescription;
    UA_Boolean        activated;
    UA_String         sessionName;
    UA_NodeId         authenticationToken;
    UA_NodeId         sessionId;
    UA_UInt32         maxRequestMessageSize;
    UA_UInt32         maxResponseMessageSize;
    UA_Int64          timeout; // [ms]
    UA_DateTime       validTill;
    #ifdef ENABLE_SUBSCRIPTIONS
        UA_SubscriptionManager subscriptionManager;
    #endif
    UA_SecureChannel *channel;
    UA_UInt16 availableContinuationPoints;
    LIST_HEAD(ContinuationPointList, ContinuationPointEntry) continuationPoints;
};

extern UA_Session anonymousSession; ///< If anonymous access is allowed, this session is used internally (Session ID: 0)
extern UA_Session adminSession; ///< Local access to the services (for startup and maintenance) uses this Session with all possible access rights (Session ID: 1)

void UA_Session_init(UA_Session *session);
void UA_Session_deleteMembersCleanup(UA_Session *session, UA_Server *server);

/** If any activity on a session happens, the timeout is extended */
void UA_Session_updateLifetime(UA_Session *session);

/** @} */


/*********************************** amalgamated original file "C:/open62541/src/server/ua_nodestore.h" ***********************************/



/**
 * @ingroup server
 *
 * @defgroup nodestore NodeStore
 *
 * @brief Stores the nodes in the address space. Internally, it is based on a
 * hash-map that maps nodes to their nodeid.
 *
 * Nodes need to be allocated on the heap before adding them to the nodestore
 * with. When adding, the node is copied to a new (managed) location in the
 * nodestore and the original memory is freed. The nodes in the nodestore are
 * immutable. To change the content of a node, it needs to be replaced as a
 * whole.
 *
 * Every node you _get from the nodestore needs to be _released when it is no
 * longer needed. In the background, reference counting is used to know if
 * somebody still uses the node in multi-threaded environments.
 *
 * @{
 */

struct UA_NodeStore;
typedef struct UA_NodeStore UA_NodeStore;

/** Create a new nodestore */
UA_NodeStore * UA_NodeStore_new(void);

/** Delete the nodestore and all nodes in it */
void UA_NodeStore_delete(UA_NodeStore *ns);

/**
 * Inserts a new node into the nodestore. If the nodeid is zero, then a fresh
 * numeric nodeid from nodestore 1 is assigned. The memory of the original node
 * is freed and the content is moved to a managed (immutable) node. If inserted
 * is not NULL, then a pointer to the managed node is returned (and must be
 * released).
 */
UA_StatusCode UA_NodeStore_insert(UA_NodeStore *ns, UA_Node *node, const UA_Node **inserted);

/**
 * Replace an existing node in the nodestore. If the node was already replaced,
 * UA_STATUSCODE_BADINTERNALERROR is returned. If inserted is not NULL, a
 * pointer to the managed (immutable) node is returned.
 */
UA_StatusCode UA_NodeStore_replace(UA_NodeStore *ns, const UA_Node *oldNode, UA_Node *node, const UA_Node **inserted);

/**
 * Remove a node from the nodestore. Always succeeds, even if the node was not
 * found.
 */
UA_StatusCode UA_NodeStore_remove(UA_NodeStore *ns, const UA_NodeId *nodeid);

/**
 * Retrieve a managed node (read-only) from the nodestore. Nodes are reference-
 * counted (for garbage collection) and immutable. They can only be replaced
 * entirely. After the node is no longer used, it needs to be released to decrease
 * the reference count.
 */
const UA_Node * UA_NodeStore_get(const UA_NodeStore *ns, const UA_NodeId *nodeid);

/**
 * Release a managed node. Do never call this with a node that isn't managed by a
 * nodestore.
 */
void UA_NodeStore_release(const UA_Node *managed);

/**
 * A function that can be evaluated on all entries in a nodestore via
 * UA_NodeStore_iterate. Note that the visitor is read-only on the nodes.
 */
typedef void (*UA_NodeStore_nodeVisitor)(const UA_Node *node);

/** Iterate over all nodes in a nodestore. */
void UA_NodeStore_iterate(const UA_NodeStore *ns, UA_NodeStore_nodeVisitor visitor);

/** @} */


/*********************************** amalgamated original file "C:/open62541/src/server/ua_session_manager.h" ***********************************/



typedef struct session_list_entry {
    LIST_ENTRY(session_list_entry) pointers;
    UA_Session session;
} session_list_entry;

typedef struct UA_SessionManager {
    LIST_HEAD(session_list, session_list_entry) sessions; // doubly-linked list of sessions
    UA_UInt32    maxSessionCount;
    UA_Int32     lastSessionId;
    UA_UInt32    currentSessionCount;
    UA_DateTime  maxSessionLifeTime;    // time in [ms]
} UA_SessionManager;

UA_StatusCode
UA_SessionManager_init(UA_SessionManager *sessionManager, UA_UInt32 maxSessionCount,
                       UA_UInt32 maxSessionLifeTime, UA_UInt32 startSessionId);

void UA_SessionManager_deleteMembers(UA_SessionManager *sessionManager, UA_Server *server);

void UA_SessionManager_cleanupTimedOut(UA_SessionManager *sessionManager, UA_Server *server, UA_DateTime now);

UA_StatusCode
UA_SessionManager_createSession(UA_SessionManager *sessionManager,
                                UA_SecureChannel *channel, const UA_CreateSessionRequest *request,
                                UA_Session **session);

UA_StatusCode
UA_SessionManager_removeSession(UA_SessionManager *sessionManager,
                                UA_Server *server, const UA_NodeId *token);

UA_Session *
UA_SessionManager_getSession(UA_SessionManager *sessionManager, const UA_NodeId *token);


/*********************************** amalgamated original file "C:/open62541/src/server/ua_securechannel_manager.h" ***********************************/



typedef struct channel_list_entry {
    UA_SecureChannel channel;
    LIST_ENTRY(channel_list_entry) pointers;
} channel_list_entry;

typedef struct UA_SecureChannelManager {
    LIST_HEAD(channel_list, channel_list_entry) channels; // doubly-linked list of channels
    size_t    maxChannelCount;
    size_t currentChannelCount;
    UA_DateTime maxChannelLifetime;
    UA_MessageSecurityMode securityMode;
    UA_DateTime channelLifeTime;
    UA_Int32    lastChannelId;
    UA_UInt32   lastTokenId;
} UA_SecureChannelManager;

UA_StatusCode
UA_SecureChannelManager_init(UA_SecureChannelManager *cm, size_t maxChannelCount,
                             UA_UInt32 tokenLifetime, UA_UInt32 startChannelId,
                             UA_UInt32 startTokenId);

void UA_SecureChannelManager_deleteMembers(UA_SecureChannelManager *cm);

void UA_SecureChannelManager_cleanupTimedOut(UA_SecureChannelManager *cm, UA_DateTime now);

UA_StatusCode
UA_SecureChannelManager_open(UA_SecureChannelManager *cm, UA_Connection *conn,
                             const UA_OpenSecureChannelRequest *request,
                             UA_OpenSecureChannelResponse *response);

UA_StatusCode
UA_SecureChannelManager_renew(UA_SecureChannelManager *cm, UA_Connection *conn,
                              const UA_OpenSecureChannelRequest *request,
                              UA_OpenSecureChannelResponse *response);

UA_SecureChannel *
UA_SecureChannelManager_get(UA_SecureChannelManager *cm, UA_UInt32 channelId);

UA_StatusCode
UA_SecureChannelManager_close(UA_SecureChannelManager *cm, UA_UInt32 channelId);


/*********************************** amalgamated original file "C:/open62541/src/server/ua_server_internal.h" ***********************************/



#ifdef ENABLE_SUBSCRIPTIONS
#endif

#define PRODUCT_URI "http://open62541.org"
#define ANONYMOUS_POLICY "open62541-anonymous-policy"
#define USERNAME_POLICY "open62541-username-policy"

#ifdef UA_EXTERNAL_NAMESPACES
/** Mapping of namespace-id and url to an external nodestore. For namespaces
    that have no mapping defined, the internal nodestore is used by default. */
typedef struct UA_ExternalNamespace {
	UA_UInt16 index;
	UA_String url;
	UA_ExternalNodeStore externalNodeStore;
} UA_ExternalNamespace;
#endif

struct UA_Server {
    /* Config */
    UA_ServerConfig config;
    UA_Logger logger;
    UA_UInt32 random_seed;

    /* Meta */
    UA_DateTime startTime;
    UA_DateTime buildDate;
    UA_ApplicationDescription description;
    UA_Int32 endpointDescriptionsSize;
    UA_EndpointDescription *endpointDescriptions;

    /* Communication */
    size_t networkLayersSize;
    UA_ServerNetworkLayer **networkLayers;

    /* Security */
    UA_ByteString serverCertificate;
    UA_SecureChannelManager secureChannelManager;
    UA_SessionManager sessionManager;

    /* Address Space */
    UA_NodeStore *nodestore;
    size_t namespacesSize;
    UA_String *namespaces;

#ifdef UA_EXTERNAL_NAMESPACES
    size_t externalNamespacesSize;
    UA_ExternalNamespace *externalNamespaces;
#endif
     
    /* Jobs with a repetition interval */
    LIST_HEAD(RepeatedJobsList, RepeatedJobs) repeatedJobs;
    
#ifdef UA_MULTITHREADING
    /* Dispatch queue head for the worker threads (the tail should not be in the same cache line) */
	struct cds_wfcq_head dispatchQueue_head;

    UA_Boolean *running;
    UA_UInt16 nThreads;
    UA_UInt32 **workerCounters;
    pthread_t *thr;

    struct cds_lfs_stack mainLoopJobs; /* Work that shall be executed only in the main loop and not
                                          by worker threads */
    struct DelayedJobs *delayedJobs;

    pthread_cond_t dispatchQueue_condition; /* so the workers don't spin if the queue is empty */
    /* Dispatch queue tail for the worker threads */
	struct cds_wfcq_tail dispatchQueue_tail;
#endif
};

/* The node is assumed to be "finished", i.e. no instantiation from inheritance is necessary */
void UA_Server_addExistingNode(UA_Server *server, UA_Session *session, UA_Node *node,
                               const UA_NodeId *parentNodeId, const UA_NodeId *referenceTypeId,
                               UA_AddNodesResult *result);

typedef UA_StatusCode (*UA_EditNodeCallback)(UA_Server *server, UA_Session*, UA_Node*, const void*);

/* Calls callback on the node. In the multithreaded case, the node is copied before and replaced in
   the nodestore. */
UA_StatusCode UA_Server_editNode(UA_Server *server, UA_Session *session, const UA_NodeId *nodeId,
                                 UA_EditNodeCallback callback, const void *data);

void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, const UA_ByteString *msg);

UA_StatusCode UA_Server_addDelayedJob(UA_Server *server, UA_Job job);

void UA_Server_deleteAllRepeatedJobs(UA_Server *server);


/*********************************** amalgamated original file "C:/open62541/src/server/ua_services.h" ***********************************/



/**
 * @ingroup server
 * @defgroup services Services
 *
 * @brief The UA services that can be called from a remote user
 *
 * @{
 */

/**
 * @name Discovery Service Set
 *
 * This Service Set defines Services used to discover the Endpoints implemented
 * by a Server and to read the security configuration for those Endpoints.
 *
 * @{
 */
void Service_FindServers(UA_Server *server, const UA_FindServersRequest *request,
                         UA_FindServersResponse *response);
/**
 * Returns the Endpoints supported by a Server and all of the configuration
 * information required to establish a SecureChannel and a Session.
 */
void Service_GetEndpoints(UA_Server *server, const UA_GetEndpointsRequest *request,
                          UA_GetEndpointsResponse *response);
// Service_RegisterServer
/** @} */

/**
 * @name SecureChannel Service Set
 *
 * This Service Set defines Services used to open a communication channel that
 * ensures the confidentiality and Integrity of all Messages exchanged with the
 * Server.
 *
 * @{
 */

/**
 * Open or renew a SecureChannel that can be used to ensure Confidentiality and
 * Integrity for Message exchange during a Session.
 */
void Service_OpenSecureChannel(UA_Server *server, UA_Connection *connection,
                               const UA_OpenSecureChannelRequest *request,
                               UA_OpenSecureChannelResponse *response);

/** Used to terminate a SecureChannel. */
void Service_CloseSecureChannel(UA_Server *server, UA_Int32 channelId);

/** @} */

/**
 * @name Session Service Set
 *
 * This Service Set defines Services for an application layer connection
 * establishment in the context of a Session.
 *
 * @{
 */

/**
 * Used by an OPC UA Client to create a Session and the Server returns two
 * values which uniquely identify the Session. The first value is the sessionId
 * which is used to identify the Session in the audit logs and in the Server's
 * address space. The second is the authenticationToken which is used to
 * associate an incoming request with a Session.
 */
void Service_CreateSession(UA_Server *server, UA_SecureChannel *channel,
                           const UA_CreateSessionRequest *request, UA_CreateSessionResponse *response);

/**
 * Used by the Client to submit its SoftwareCertificates to the Server for
 * validation and to specify the identity of the user associated with the
 * Session. This Service request shall be issued by the Client before it issues
 * any other Service request after CreateSession. Failure to do so shall cause
 * the Server to close the Session.
 */
void Service_ActivateSession(UA_Server *server, UA_SecureChannel *channel,
                             const UA_ActivateSessionRequest *request, UA_ActivateSessionResponse *response);

/** Used to terminate a Session. */
void Service_CloseSession(UA_Server *server, UA_Session *session, const UA_CloseSessionRequest *request,
                          UA_CloseSessionResponse *response);
// Service_Cancel
/** @} */

/**
 * @name NodeManagement Service Set
 *
 * This Service Set defines Services to add and delete AddressSpace Nodes and References between
 * them. All added Nodes continue to exist in the AddressSpace even if the Client that created them
 * disconnects from the Server.
 *
 * @{
 */

/** Used to add one or more Nodes into the AddressSpace hierarchy. */
void Service_AddNodes(UA_Server *server, UA_Session *session, const UA_AddNodesRequest *request,
                      UA_AddNodesResponse *response);
void Service_AddNodes_single(UA_Server *server, UA_Session *session, UA_AddNodesItem *item,
                             UA_NodeAttributes *attr, UA_AddNodesResult *result);

/** Used to add one or more References to one or more Nodes. */
void Service_AddReferences(UA_Server *server, UA_Session *session, const UA_AddReferencesRequest *request,
                           UA_AddReferencesResponse *response);
UA_StatusCode Service_AddReferences_single(UA_Server *server, UA_Session *session,
                                           const UA_AddReferencesItem *item);

/** Used to delete one or more Nodes from the AddressSpace. */
void Service_DeleteNodes(UA_Server *server, UA_Session *session, const UA_DeleteNodesRequest *request,
                         UA_DeleteNodesResponse *response);
UA_StatusCode Service_DeleteNodes_single(UA_Server *server, UA_Session *session, const UA_NodeId *nodeId,
                                         UA_Boolean deleteReferences);

/** Used to delete one or more References of a Node. */
void Service_DeleteReferences(UA_Server *server, UA_Session *session,
                              const UA_DeleteReferencesRequest *request,
                              UA_DeleteReferencesResponse *response);
UA_StatusCode Service_DeleteReferences_single(UA_Server *server, UA_Session *session,
                                              const UA_DeleteReferencesItem *item);

/** @} */

/**
 * @name View Service Set
 *
 * Clients use the browse Services of the View Service Set to navigate through
 * the AddressSpace or through a View which is a subset of the AddressSpace.
 *
 * @{
 */

/**
 * Used to discover the References of a specified Node. The browse can be
 * further limited by the use of a View. This Browse Service also supports a
 * primitive filtering capability.
 */
void Service_Browse(UA_Server *server, UA_Session *session, const UA_BrowseRequest *request,
                    UA_BrowseResponse *response);

void Service_Browse_single(UA_Server *server, UA_Session *session, struct ContinuationPointEntry *cp,
                           const UA_BrowseDescription *descr, UA_UInt32 maxrefs, UA_BrowseResult *result);

/**
 * Used to request the next set of Browse or BrowseNext response information
 * that is too large to be sent in a single response. "Too large" in this
 * context means that the Server is not able to return a larger response or that
 * the number of results to return exceeds the maximum number of results to
 * return that was specified by the Client in the original Browse request.
 */
void Service_BrowseNext(UA_Server *server, UA_Session *session, const UA_BrowseNextRequest *request,
                        UA_BrowseNextResponse *response);

/** Used to translate textual node paths to their respective ids. */
void Service_TranslateBrowsePathsToNodeIds(UA_Server *server, UA_Session *session,
                                           const UA_TranslateBrowsePathsToNodeIdsRequest *request,
                                           UA_TranslateBrowsePathsToNodeIdsResponse *response);
void Service_TranslateBrowsePathsToNodeIds_single(UA_Server *server, UA_Session *session,
                                                  const UA_BrowsePath *path, UA_BrowsePathResult *result);

void Service_RegisterNodes(UA_Server *server, UA_Session *session, const UA_RegisterNodesRequest *request,
                           UA_RegisterNodesResponse *response);
void Service_UnregisterNodes(UA_Server *server, UA_Session *session, const UA_UnregisterNodesRequest *request,
                             UA_UnregisterNodesResponse *response);
/** @} */

/**
 * @name Query Service Set
 *
 * This Service Set is used to issue a Query to a Server. OPC UA Query is
 * generic in that it provides an underlying storage mechanism independent Query
 * capability that can be used to access a wide variety of OPC UA data stores
 * and information management systems. OPC UA Query permits a Client to access
 * data maintained by a Server without any knowledge of the logical schema used
 * for internal storage of the data. Knowledge of the AddressSpace is
 * sufficient.
 *
 * @{
 */
// Service_QueryFirst
// Service_QueryNext
/** @} */

/**
 * @name Attribute Service Set
 *
 * This Service Set provides Services to access Attributes that are part of
 * Nodes.
 *
 * @{
 */

/* Mock-Up of the function signature for Unit Tests */
#ifdef BUILD_UNIT_TESTS
UA_StatusCode parse_numericrange(const UA_String *str, UA_NumericRange *range);
#endif

/**
 * Used to read one or more Attributes of one or more Nodes. For constructed
 * Attribute values whose elements are indexed, such as an array, this Service
 * allows Clients to read the entire set of indexed values as a composite, to
 * read individual elements or to read ranges of elements of the composite.
 */
void Service_Read(UA_Server *server, UA_Session *session, const UA_ReadRequest *request,
                  UA_ReadResponse *response);
void Service_Read_single(UA_Server *server, UA_Session *session, UA_TimestampsToReturn timestamps,
                         const UA_ReadValueId *id, UA_DataValue *v);

// Service_HistoryRead
/**
 * Used to write one or more Attributes of one or more Nodes. For constructed
 * Attribute values whose elements are indexed, such as an array, this Service
 * allows Clients to write the entire set of indexed values as a composite, to
 * write individual elements or to write ranges of elements of the composite.
 */
void Service_Write(UA_Server *server, UA_Session *session, const UA_WriteRequest *request,
                   UA_WriteResponse *response);

/** Single attribute writes are exposed to the userspace. The wvalue may be destroyed (deleteMembers) */
UA_StatusCode Service_Write_single(UA_Server *server, UA_Session *session, UA_WriteValue *wvalue);

// Service_HistoryUpdate
/** @} */

/**
 * @name Method Service Set
 *
 * The Method Service Set defines the means to invoke methods. A method shall be
 * a component of an Object.
 *
 * @{
 */
#ifdef ENABLE_METHODCALLS
void Service_Call(UA_Server *server, UA_Session *session, const UA_CallRequest *request,
                  UA_CallResponse *response);
#endif
/** @} */

#ifdef ENABLE_SUBSCRIPTIONS
/**
 * @name MonitoredItem Service Set
 *
 * Clients define MonitoredItems to subscribe to data and Events. Each
 * MonitoredItem identifies the item to be monitored and the Subscription to use
 * to send Notifications. The item to be monitored may be any Node Attribute.
 *
 * @{
 */

/*
 * Used to create and add one or more MonitoredItems to a Subscription. A
 * MonitoredItem is deleted automatically by the Server when the Subscription is
 * deleted. Deleting a MonitoredItem causes its entire set of triggered item
 * links to be deleted, but has no effect on the MonitoredItems referenced by
 * the triggered items.
 */
void Service_CreateMonitoredItems(UA_Server *server, UA_Session *session,
                                       const UA_CreateMonitoredItemsRequest *request, 
                                       UA_CreateMonitoredItemsResponse *response);
// Service_ModifyMonitoredItems
// Service_SetMonitoringMode
// Service_SetTriggering
void Service_DeleteMonitoredItems(UA_Server *server, UA_Session *session,
                                  const UA_DeleteMonitoredItemsRequest *request,
                                  UA_DeleteMonitoredItemsResponse *response);
                                      
/** @} */

/**
 * @name Subscription Service Set
 *
 * Subscriptions are used to report Notifications to the Client.
 *
 * @{
 */
    
void Service_CreateSubscription(UA_Server *server, UA_Session *session,
                                const UA_CreateSubscriptionRequest *request,
                                UA_CreateSubscriptionResponse *response);

void Service_ModifySubscription(UA_Server *server, UA_Session *session,
                                const UA_ModifySubscriptionRequest *request,
                                UA_ModifySubscriptionResponse *response);

void Service_DeleteSubscriptions(UA_Server *server, UA_Session *session,
                                 const UA_DeleteSubscriptionsRequest *request,
                                 UA_DeleteSubscriptionsResponse *response);
                                     
void Service_Publish(UA_Server *server, UA_Session *session,
                     const UA_PublishRequest *request, UA_PublishResponse *response);
                         
// Service_ModifySubscription
// Service_SetPublishingMode
// UA_Int32 Service_SetPublishingMode(UA_Server *server, UA_Session *session,
                                  // const UA_SetPublishingModeRequest *request,
                                  // UA_SetPublishingModeResponse *response);
// Service_Republish
// Service_TransferSubscription
// Service_DeleteSubscription
/** @} */
#endif

/** @} */

/*********************************** amalgamated original file "C:/open62541/src/client/ua_client_internal.h" ***********************************/



/**************************/
/* Subscriptions Handling */
/**************************/

#ifdef ENABLE_SUBSCRIPTIONS

typedef struct UA_Client_NotificationsAckNumber_s {
    UA_SubscriptionAcknowledgement subAck;
    LIST_ENTRY(UA_Client_NotificationsAckNumber_s) listEntry;
} UA_Client_NotificationsAckNumber;

typedef struct UA_Client_MonitoredItem_s {
    UA_UInt32          MonitoredItemId;
    UA_UInt32          MonitoringMode;
    UA_NodeId          monitoredNodeId;
    UA_UInt32          AttributeID;
    UA_UInt32          ClientHandle;
    UA_UInt32          SamplingInterval;
    UA_UInt32          QueueSize;
    UA_Boolean         DiscardOldest;
    void               (*handler)(UA_UInt32 handle, UA_DataValue *value);
    LIST_ENTRY(UA_Client_MonitoredItem_s)  listEntry;
} UA_Client_MonitoredItem;

typedef struct UA_Client_Subscription_s {
    UA_UInt32    LifeTime;
    UA_Int32     KeepAliveCount;
    UA_DateTime  PublishingInterval;
    UA_UInt32    SubscriptionID;
    UA_Int32     NotificationsPerPublish;
    UA_UInt32    Priority;
    LIST_ENTRY(UA_Client_Subscription_s) listEntry;
    LIST_HEAD(UA_ListOfClientMonitoredItems, UA_Client_MonitoredItem_s) MonitoredItems;
} UA_Client_Subscription;

#endif

/**********/
/* Client */
/**********/

typedef enum {
    UA_CLIENTSTATE_READY,
    UA_CLIENTSTATE_CONNECTED,
    UA_CLIENTSTATE_ERRORED
} UA_Client_State;

struct UA_Client {
    /* State */ //maybe it should be visible to user
    UA_Client_State state;

    /* Connection */
    UA_Connection connection;
    UA_SecureChannel channel;
    UA_String endpointUrl;
    UA_UInt32 requestId;

    /* Session */
    UA_UserTokenPolicy token;
    UA_NodeId sessionId;
    UA_NodeId authenticationToken;
    UA_UInt32 requestHandle;
    
#ifdef ENABLE_SUBSCRIPTIONS
    UA_Int32 monitoredItemHandles;
    LIST_HEAD(UA_ListOfUnacknowledgedNotificationNumbers, UA_Client_NotificationsAckNumber_s) pendingNotificationsAcks;
    LIST_HEAD(UA_ListOfClientSubscriptionItems, UA_Client_Subscription_s) subscriptions;
#endif
    
    /* Config */
    UA_Logger logger;
    UA_ClientConfig config;
    UA_DateTime scExpiresAt;
};


/*********************************** amalgamated original file "C:/open62541/src/server/ua_nodestore_hash.inc" ***********************************/

typedef UA_UInt32 hash_t;

static hash_t mod(hash_t h, hash_t size) { return h % size; }
static hash_t mod2(hash_t h, hash_t size) { return 1 + (h % (size - 2)); }

/* Based on Murmur-Hash 3 by Austin Appleby (public domain, freely usable) */
static hash_t hash_array(const UA_Byte *data, UA_UInt32 len, UA_UInt32 seed) {
    if(data == NULL)
        return 0;

    const int32_t   nblocks = len / 4;
    const uint32_t *blocks;
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m  = 5;
    static const uint32_t n  = 0xe6546b64;
    hash_t hash = seed;
    /* Somce compilers emit a warning when casting from a byte array to ints. */
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
#endif
    blocks = (const uint32_t *)data;
#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic pop
#endif
    for(int32_t i = 0;i < nblocks;i++) {
        uint32_t k = blocks[i];
        k    *= c1;
        k     = (k << r1) | (k >> (32 - r1));
        k    *= c2;
        hash ^= k;
        hash  = ((hash << r2) | (hash >> (32 - r2))) * m + n;
    }

    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);
    uint32_t       k1   = 0;
    switch(len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1   ^= tail[0];
        k1   *= c1;
        k1    = (k1 << r1) | (k1 >> (32 - r1));
        k1   *= c2;
        hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    return hash;
}

static hash_t hash(const UA_NodeId *n) {
    switch(n->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        /*  Knuth's multiplicative hashing */
        return (n->identifier.numeric + n->namespaceIndex) * 2654435761;   // mod(2^32) is implicit
    case UA_NODEIDTYPE_STRING:
        return hash_array(n->identifier.string.data, n->identifier.string.length, n->namespaceIndex);
    case UA_NODEIDTYPE_GUID:
        return hash_array((const UA_Byte *)&(n->identifier.guid), sizeof(UA_Guid), n->namespaceIndex);
    case UA_NODEIDTYPE_BYTESTRING:
        return hash_array((const UA_Byte *)n->identifier.byteString.data, n->identifier.byteString.length, n->namespaceIndex);
    default:
        UA_assert(UA_FALSE);
        return 0;
    }
}


/*********************************** amalgamated original file "C:/open62541/src/ua_types.c" ***********************************/



/* Helper Macros */
#define UA_TYPE_DEFAULT(TYPE)                                  \
    TYPE * TYPE##_new() {                                      \
        TYPE *p = UA_malloc(sizeof(TYPE));                     \
        if(p) TYPE##_init(p);                                  \
        return p;                                              \
    }                                                          \
    void TYPE##_delete(TYPE *p) {                              \
        TYPE##_deleteMembers(p);                               \
        UA_free(p);                                            \
    }

/* static variables */
UA_EXPORT const UA_String UA_STRING_NULL = {.length = -1, .data = (UA_Byte*)0 };
UA_EXPORT const UA_ByteString UA_BYTESTRING_NULL = {.length = -1, .data = (UA_Byte*)0 };
UA_EXPORT const UA_NodeId UA_NODEID_NULL = {0, UA_NODEIDTYPE_NUMERIC, {0}};
UA_EXPORT const UA_ExpandedNodeId UA_EXPANDEDNODEID_NULL = {
    .nodeId = { .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0 },
    .namespaceUri = {.length = -1, .data = (UA_Byte*)0}, .serverIndex = 0 };

/***************************/
/* Random Number Generator */
/***************************/

static UA_THREAD_LOCAL pcg32_random_t UA_rng = PCG32_INITIALIZER;

UA_EXPORT void UA_random_seed(UA_UInt64 seed) {
    pcg32_srandom_r(&UA_rng, seed, UA_DateTime_now());
}

UA_EXPORT UA_UInt32 UA_random(void) {
    return (UA_UInt32)pcg32_random_r(&UA_rng);
}

/*****************/
/* Builtin Types */
/*****************/

/* Boolean */
UA_TYPE_DEFAULT(UA_Boolean)

/* SByte */
UA_TYPE_DEFAULT(UA_SByte)

/* Byte */
UA_TYPE_DEFAULT(UA_Byte)

/* Int16 */
UA_TYPE_DEFAULT(UA_Int16)

/* UInt16 */
UA_TYPE_DEFAULT(UA_UInt16)

/* Int32 */
UA_TYPE_DEFAULT(UA_Int32)

/* UInt32 */
UA_TYPE_DEFAULT(UA_UInt32)

/* Int64 */
UA_TYPE_DEFAULT(UA_Int64)

/* UInt64 */
UA_TYPE_DEFAULT(UA_UInt64)

/* Float */
UA_TYPE_DEFAULT(UA_Float)

/* Double */
UA_TYPE_DEFAULT(UA_Double)

/* String */
UA_TYPE_DEFAULT(UA_String)

void UA_String_init(UA_String *p) {
    p->length = -1;
    p->data   = NULL;
}

void UA_String_deleteMembers(UA_String *p) {
    UA_free(p->data);
    p->data = NULL;
    p->length = -1;
}

UA_StatusCode UA_String_copy(UA_String const *src, UA_String *dst) {
    UA_String_init(dst);
    if(src->length > 0) {
        if(!(dst->data = UA_malloc((UA_UInt32)src->length)))
            return UA_STATUSCODE_BADOUTOFMEMORY;
        memcpy((void *)dst->data, src->data, (UA_UInt32)src->length);
    }
    dst->length = src->length;
    return UA_STATUSCODE_GOOD;
}

UA_String UA_String_fromChars(char const src[]) {
    UA_String str;
    size_t length = strlen(src);
    if(length == 0) {
        str.length = 0;
        str.data = NULL;
        return str;
    }
    str.data = UA_malloc(length);
    if(!str.data) {
        str.length = -1;
        return str;
    }
    memcpy(str.data, src, length);
    str.length = length;
    return str;
}

UA_Boolean UA_String_equal(const UA_String *string1, const UA_String *string2) {
    if(string1->length <= 0 && string2->length <= 0)
        return UA_TRUE;
    if(string1->length != string2->length)
        return UA_FALSE;

    // casts are needed to overcome signed warnings
    UA_Int32 is = memcmp((char const *)string1->data, (char const *)string2->data, (size_t)string1->length);
    return (is == 0) ? UA_TRUE : UA_FALSE;
}

/* DateTime */
UA_TYPE_DEFAULT(UA_DateTime)
#define UNIX_EPOCH_BIAS_SEC 11644473600LL // Number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
#define HUNDRED_NANOSEC_PER_USEC 10LL
#define HUNDRED_NANOSEC_PER_SEC (HUNDRED_NANOSEC_PER_USEC * 1000000LL)

#ifdef __MINGW32__
#ifndef _TIMEZONE_DEFINED
#define _TIMEZONE_DEFINED
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
#endif
#endif
#ifdef _WIN32
static const unsigned __int64 epoch = 116444736000000000;
int gettimeofday(struct timeval *tp, struct timezone *tzp);
int gettimeofday(struct timeval *tp, struct timezone *tzp) {
    FILETIME       ft;
    SYSTEMTIME     st;
    ULARGE_INTEGER ul;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    ul.LowPart  = ft.dwLowDateTime;
    ul.HighPart = ft.dwHighDateTime;
    tp->tv_sec  = (long)((ul.QuadPart - epoch) / 10000000L);
    tp->tv_usec = st.wMilliseconds * 1000;
    return 0;
}
#endif

UA_DateTime UA_DateTime_now() {
    UA_DateTime    dateTime;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    dateTime = (tv.tv_sec + UNIX_EPOCH_BIAS_SEC)
               * HUNDRED_NANOSEC_PER_SEC + tv.tv_usec * HUNDRED_NANOSEC_PER_USEC;
    return dateTime;
}

UA_DateTimeStruct UA_DateTime_toStruct(UA_DateTime atime) {
    UA_DateTimeStruct dateTimeStruct;
    //calcualting the the milli-, micro- and nanoseconds
    dateTimeStruct.nanoSec  = (UA_UInt16)((atime % 10) * 100);
    dateTimeStruct.microSec = (UA_UInt16)((atime % 10000) / 10);
    dateTimeStruct.milliSec = (UA_UInt16)((atime % 10000000) / 10000);

    //calculating the unix time with #include <time.h>
    time_t secSinceUnixEpoch = (atime/10000000) - UNIX_EPOCH_BIAS_SEC;
    struct tm ts = *gmtime(&secSinceUnixEpoch);
    dateTimeStruct.sec    = (UA_UInt16)ts.tm_sec;
    dateTimeStruct.min    = (UA_UInt16)ts.tm_min;
    dateTimeStruct.hour   = (UA_UInt16)ts.tm_hour;
    dateTimeStruct.day    = (UA_UInt16)ts.tm_mday;
    dateTimeStruct.month  = (UA_UInt16)(ts.tm_mon + 1);
    dateTimeStruct.year   = (UA_UInt16)(ts.tm_year + 1900);
    return dateTimeStruct;
}

static void printNumber(UA_UInt16 n, UA_Byte *pos, size_t digits) {
    for(size_t i = 0; i < digits; i++) {
        pos[digits-i-1] = (n % 10) + '0';
        n = n / 10;
    }
}

UA_StatusCode UA_DateTime_toString(UA_DateTime atime, UA_String *timeString) {
    // length of the string is 31 (incl. \0 at the end)
    if(!(timeString->data = UA_malloc(32)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    timeString->length = 31;

    UA_DateTimeStruct tSt = UA_DateTime_toStruct(atime);
    printNumber(tSt.month, timeString->data, 2);
    timeString->data[2] = '/';
    printNumber(tSt.day, &timeString->data[3], 2);
    timeString->data[5] = '/';
    printNumber(tSt.year, &timeString->data[6], 4);
    timeString->data[10] = ' ';
    printNumber(tSt.hour, &timeString->data[11], 2);
    timeString->data[13] = ':';
    printNumber(tSt.min, &timeString->data[14], 2);
    timeString->data[16] = ':';
    printNumber(tSt.sec, &timeString->data[17], 2);
    timeString->data[19] = '.';
    printNumber(tSt.milliSec, &timeString->data[20], 3);
    timeString->data[23] = '.';
    printNumber(tSt.microSec, &timeString->data[24], 3);
    timeString->data[27] = '.';
    printNumber(tSt.nanoSec, &timeString->data[28], 3);
    return UA_STATUSCODE_GOOD;
}

/* Guid */
UA_TYPE_DEFAULT(UA_Guid)
UA_Boolean UA_Guid_equal(const UA_Guid *g1, const UA_Guid *g2) {
    if(memcmp(g1, g2, sizeof(UA_Guid)) == 0)
        return UA_TRUE;
    return UA_FALSE;
}

UA_Guid UA_Guid_random(UA_UInt32 *seed) {
    UA_Guid result;
    result.data1 = (UA_UInt32)pcg32_random_r(&UA_rng);
    UA_UInt32 r = (UA_UInt32)pcg32_random_r(&UA_rng);
    result.data2 = (UA_UInt16) r;
    result.data3 = (UA_UInt16) (r >> 16);
    r = (UA_UInt32)pcg32_random_r(&UA_rng);
    result.data4[0] = (UA_Byte)r;
    result.data4[1] = (UA_Byte)(r >> 4);
    result.data4[2] = (UA_Byte)(r >> 8);
    result.data4[3] = (UA_Byte)(r >> 12);
    r = (UA_UInt32)pcg32_random_r(&UA_rng);
    result.data4[4] = (UA_Byte)r;
    result.data4[5] = (UA_Byte)(r >> 4);
    result.data4[6] = (UA_Byte)(r >> 8);
    result.data4[7] = (UA_Byte)(r >> 12);
    return result;
}

/* ByteString */
UA_StatusCode UA_ByteString_newMembers(UA_ByteString *p, UA_Int32 length) {
    if(length > 0) {
        if(!(p->data = UA_malloc((UA_UInt32)length)))
            return UA_STATUSCODE_BADOUTOFMEMORY;
        p->length = length;
    } else {
        p->data = NULL;
        if(length == 0)
            p->length = 0;
        else
            p->length = -1;
    }
    return UA_STATUSCODE_GOOD;
}

/* XmlElement */

/* NodeId */
UA_TYPE_DEFAULT(UA_NodeId)

void UA_NodeId_init(UA_NodeId *p) {
    p->identifierType = UA_NODEIDTYPE_NUMERIC;
    p->namespaceIndex = 0;
    memset(&p->identifier, 0, sizeof(p->identifier));
}

UA_StatusCode UA_NodeId_copy(UA_NodeId const *src, UA_NodeId *dst) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    switch(src->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        *dst = *src;
        return UA_STATUSCODE_GOOD;
    case UA_NODEIDTYPE_STRING: // Table 6, second entry
        retval |= UA_String_copy(&src->identifier.string, &dst->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID: // Table 6, third entry
        retval |= UA_Guid_copy(&src->identifier.guid, &dst->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING: // Table 6, "OPAQUE"
        retval |= UA_ByteString_copy(&src->identifier.byteString, &dst->identifier.byteString);
        break;
    default:
        UA_NodeId_init(dst);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    dst->namespaceIndex = src->namespaceIndex;
    dst->identifierType = src->identifierType;
    if(retval) {
        UA_NodeId_deleteMembers(dst);
        UA_NodeId_init(dst);
    }
    return retval;
}

void UA_NodeId_deleteMembers(UA_NodeId *p) {
    switch(p->identifierType) {
    case UA_NODEIDTYPE_STRING:
    case UA_NODEIDTYPE_BYTESTRING:
        UA_ByteString_deleteMembers(&p->identifier.byteString);
        break;
    default:
        break;
    }
}

UA_Boolean UA_NodeId_equal(const UA_NodeId *n1, const UA_NodeId *n2) {
	if(n1->namespaceIndex != n2->namespaceIndex || n1->identifierType!=n2->identifierType)
        return UA_FALSE;

    switch(n1->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(n1->identifier.numeric == n2->identifier.numeric)
            return UA_TRUE;
        else
            return UA_FALSE;

    case UA_NODEIDTYPE_STRING:
        return UA_String_equal(&n1->identifier.string, &n2->identifier.string);

    case UA_NODEIDTYPE_GUID:
        return UA_Guid_equal(&n1->identifier.guid, &n2->identifier.guid);

    case UA_NODEIDTYPE_BYTESTRING:
        return UA_ByteString_equal(&n1->identifier.byteString, &n2->identifier.byteString);
    }
    return UA_FALSE;
}

UA_Boolean UA_NodeId_isNull(const UA_NodeId *p) {
    switch(p->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(p->namespaceIndex != 0 || p->identifier.numeric != 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_STRING:
        if(p->namespaceIndex != 0 || p->identifier.string.length > 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_GUID:
        if(p->namespaceIndex != 0 ||
           memcmp(&p->identifier.guid, (char[sizeof(UA_Guid)]) { 0 }, sizeof(UA_Guid)) != 0)
            return UA_FALSE;
        break;

    case UA_NODEIDTYPE_BYTESTRING:
        if(p->namespaceIndex != 0 || p->identifier.byteString.length > 0)
            return UA_FALSE;
        break;

    default:
        return UA_FALSE;
    }
    return UA_TRUE;
}

/* ExpandedNodeId */
UA_TYPE_DEFAULT(UA_ExpandedNodeId)

void UA_ExpandedNodeId_deleteMembers(UA_ExpandedNodeId *p) {
    UA_NodeId_deleteMembers(&p->nodeId);
    UA_String_deleteMembers(&p->namespaceUri);
}

void UA_ExpandedNodeId_init(UA_ExpandedNodeId *p) {
    UA_NodeId_init(&p->nodeId);
    UA_String_init(&p->namespaceUri);
    p->serverIndex = 0;
}

UA_StatusCode UA_ExpandedNodeId_copy(UA_ExpandedNodeId const *src, UA_ExpandedNodeId *dst) {
    UA_StatusCode retval = UA_NodeId_copy(&src->nodeId, &dst->nodeId);
    retval |= UA_String_copy(&src->namespaceUri, &dst->namespaceUri);
    dst->serverIndex = src->serverIndex;
    if(retval) {
        UA_ExpandedNodeId_deleteMembers(dst);
        UA_ExpandedNodeId_init(dst);
    }
    return retval;
}

/* StatusCode */

/* QualifiedName */
UA_TYPE_DEFAULT(UA_QualifiedName)

void UA_QualifiedName_deleteMembers(UA_QualifiedName *p) {
    UA_String_deleteMembers(&p->name);
}

void UA_QualifiedName_init(UA_QualifiedName *p) {
    UA_String_init(&p->name);
    p->namespaceIndex = 0;
}

UA_StatusCode UA_QualifiedName_copy(UA_QualifiedName const *src, UA_QualifiedName *dst) {
    UA_StatusCode retval = UA_String_copy(&src->name, &dst->name);
    dst->namespaceIndex = src->namespaceIndex;
    if(retval) {
        UA_QualifiedName_deleteMembers(dst);
        UA_QualifiedName_init(dst);
    }
    return retval;
}

/* LocalizedText */
UA_TYPE_DEFAULT(UA_LocalizedText)

void UA_LocalizedText_deleteMembers(UA_LocalizedText *p) {
    UA_String_deleteMembers(&p->locale);
    UA_String_deleteMembers(&p->text);
}

void UA_LocalizedText_init(UA_LocalizedText *p) {
    UA_String_init(&p->locale);
    UA_String_init(&p->text);
}

UA_StatusCode UA_LocalizedText_copy(UA_LocalizedText const *src, UA_LocalizedText *dst) {
    UA_StatusCode retval = UA_String_copy(&src->locale, &dst->locale);
    retval |= UA_String_copy(&src->text, &dst->text);
    if(retval) {
        UA_LocalizedText_deleteMembers(dst);
        UA_LocalizedText_init(dst);
    }
    return retval;
}

/* ExtensionObject */
UA_TYPE_DEFAULT(UA_ExtensionObject)

void UA_ExtensionObject_deleteMembers(UA_ExtensionObject *p) {
    UA_NodeId_deleteMembers(&p->typeId);
    UA_ByteString_deleteMembers(&p->body);
}

void UA_ExtensionObject_init(UA_ExtensionObject *p) {
    UA_NodeId_init(&p->typeId);
    p->encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED;
    UA_ByteString_init(&p->body);
}

UA_StatusCode UA_ExtensionObject_copy(UA_ExtensionObject const *src, UA_ExtensionObject *dst) {
    UA_StatusCode retval = UA_ByteString_copy(&src->body, &dst->body);
    retval |= UA_NodeId_copy(&src->typeId, &dst->typeId);
    dst->encoding = src->encoding;
    if(retval) {
        UA_ExtensionObject_deleteMembers(dst);
        UA_ExtensionObject_init(dst);
    }
    return retval;
}

/* DataValue */
UA_TYPE_DEFAULT(UA_DataValue)

void UA_DataValue_deleteMembers(UA_DataValue *p) {
    UA_Variant_deleteMembers(&p->value);
}

void UA_DataValue_init(UA_DataValue *p) {
    memset(p, 0, sizeof(UA_DataValue));
    UA_Variant_init(&p->value);
}

UA_StatusCode UA_DataValue_copy(UA_DataValue const *src, UA_DataValue *dst) {
    memcpy(dst, src, sizeof(UA_DataValue));
    UA_Variant_init(&dst->value);
    UA_StatusCode retval = UA_Variant_copy(&src->value, &dst->value);
    if(retval) {
        UA_DataValue_deleteMembers(dst);
        UA_DataValue_init(dst);
    }
    return retval;
}

/* Variant */
UA_TYPE_DEFAULT(UA_Variant)

void UA_Variant_init(UA_Variant *p) {
    p->storageType = UA_VARIANT_DATA;
    p->arrayLength = -1;
    p->data = NULL;
    p->arrayDimensions = NULL;
    p->arrayDimensionsSize = -1;
    p->type = &UA_TYPES[UA_TYPES_BOOLEAN];
}

void UA_Variant_deleteMembers(UA_Variant *p) {
    if(p->storageType != UA_VARIANT_DATA)
        return;
    if(p->data) {
        if(p->arrayLength == -1)
            p->arrayLength = 1;
        UA_Array_delete(p->data, p->type, p->arrayLength);
        p->data = NULL;
        p->arrayLength = -1;
    }
    if(p->arrayDimensions) {
        UA_free(p->arrayDimensions);
        p->arrayDimensions = NULL;
        p->arrayDimensionsSize = -1;
    }
}

UA_StatusCode UA_Variant_copy(UA_Variant const *src, UA_Variant *dst) {
    UA_Variant_init(dst);
    UA_Int32 elements = src->arrayLength;
    if(UA_Variant_isScalar(src))
        elements = 1;
    UA_StatusCode retval = UA_Array_copy(src->data, &dst->data, src->type, elements);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Variant_deleteMembers(dst);
        UA_Variant_init(dst);
        return retval;
    }
    dst->arrayLength = src->arrayLength;
    dst->type = src->type;
    dst->storageType = UA_VARIANT_DATA;

    if(src->arrayDimensions) {
        retval |= UA_Array_copy(src->arrayDimensions, (void **)&dst->arrayDimensions,
                                &UA_TYPES[UA_TYPES_INT32], src->arrayDimensionsSize);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Variant_deleteMembers(dst);
            UA_Variant_init(dst);
            return retval;
        }
        dst->arrayDimensionsSize = src->arrayDimensionsSize;
    }
    return retval;
}

UA_Boolean UA_Variant_isScalar(const UA_Variant *v) {
    return (v->data != NULL && v->arrayLength == -1);
}

/**
 * Tests if a range is compatible with a variant. If yes, the following values are set:
 * - total: how many elements are in the range
 * - block: how big is each contiguous block of elements in the variant that maps into the range
 * - stride: how many elements are between the blocks (beginning to beginning)
 * - first: where does the first block begin
 */
static UA_StatusCode
testRangeWithVariant(const UA_Variant *v, const UA_NumericRange range, size_t *total,
                     size_t *block, size_t *stride, size_t *first) {
    /* Test the integrity of the source variant dimensions */
    UA_Int32 dims_count = 1;
    UA_Int32 elements = 1;
    const UA_Int32 *dims = &v->arrayLength; // default: the array has only one dimension
    if(v->arrayDimensionsSize > 0) {
        dims_count = v->arrayDimensionsSize;
        dims = v->arrayDimensions;
        for(UA_Int32 i = 0; i < dims_count; i++)
            elements *= dims[i];
        if(elements != v->arrayLength)
            return UA_STATUSCODE_BADINTERNALERROR;
    }

    /* Test the integrity of the range */
    size_t count = 1;
    if(range.dimensionsSize != dims_count)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;
    for(UA_Int32 i = 0; i < dims_count; i++) {
        if(range.dimensions[i].min > range.dimensions[i].max)
            return UA_STATUSCODE_BADINDEXRANGEINVALID;
        if(range.dimensions[i].max > (UA_UInt32)*(dims+i))
            return UA_STATUSCODE_BADINDEXRANGENODATA;
        count *= (range.dimensions[i].max - range.dimensions[i].min) + 1;
    }

    /* Compute the block size and the position of the first element */
    size_t b = 1, s = elements, f = 0;
    size_t running_dimssize = 1; // elements per block of dimensions k to k_max
    UA_Boolean found_contiguous = UA_FALSE;
    for(UA_Int32 k = dims_count - 1; k >= 0; k--) {
        if(!found_contiguous && (range.dimensions[k].min != 0 ||
                                 range.dimensions[k].max + 1 != (UA_UInt32)dims[k])) {
            found_contiguous = UA_TRUE;
            b = (range.dimensions[k].max - range.dimensions[k].min + 1) * running_dimssize;
            s = dims[k] * running_dimssize;
        } 
        f += running_dimssize * range.dimensions[k].min;
        running_dimssize *= dims[k];
    }
    *total = count;
    *block = b;
    *stride = s;
    *first = f;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_Variant_copyRange(const UA_Variant *src, UA_Variant *dst, const UA_NumericRange range) {
    size_t count, block, stride, first;
    UA_StatusCode retval = testRangeWithVariant(src, range, &count, &block, &stride, &first);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_Variant_init(dst);
    size_t elem_size = src->type->memSize;
    dst->data = UA_malloc(elem_size * count);
    if(!dst->data)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    /* Copy the range */
    size_t block_count = count / block;
    uintptr_t nextdst = (uintptr_t)dst->data;
    uintptr_t nextsrc = (uintptr_t)src->data + (elem_size * first);
    if(src->type->fixedSize) {
        for(size_t i = 0; i < block_count; i++) {
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block);
            nextdst += block * elem_size;
            nextsrc += stride * elem_size;
        }
    } else {
        for(size_t i = 0; i < block_count; i++) {
            for(size_t j = 0; j < block && retval == UA_STATUSCODE_GOOD; j++) {
                retval = UA_copy((const void*)nextsrc, (void*)nextdst, src->type);
                nextdst += elem_size;
                nextsrc += elem_size;
            }
            nextsrc += (stride - block) * elem_size;
        }
        if(retval != UA_STATUSCODE_GOOD) {
            size_t copied = ((nextdst - elem_size) - (uintptr_t)dst->data) / elem_size;
            UA_Array_delete(dst->data, src->type, copied);
            return retval;
        }
    }

    /* Copy the range dimensions */
    if(src->arrayDimensionsSize > 0) {
        dst->arrayDimensions = UA_malloc(sizeof(UA_Int32) * src->arrayDimensionsSize);
        if(!dst->arrayDimensions) {
            UA_Array_delete(dst->data, src->type, count);
            return UA_STATUSCODE_BADOUTOFMEMORY;
        }
        for(UA_Int32 k = 0; k < src->arrayDimensionsSize; k++)
            dst->arrayDimensions[k] = range.dimensions[k].max - range.dimensions[k].min + 1;
        dst->arrayDimensionsSize = src->arrayDimensionsSize;
    }
    dst->arrayLength = count;
    dst->type = src->type;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_Variant_setRange(UA_Variant *v, void * UA_RESTRICT dataArray,
                    UA_Int32 dataArraySize, const UA_NumericRange range) {
    size_t count, block, stride, first;
    UA_StatusCode retval = testRangeWithVariant(v, range, &count, &block, &stride, &first);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if((UA_Int32)count != dataArraySize)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;

    size_t block_count = count / block;
    size_t elem_size = v->type->memSize;
    uintptr_t nextdst = (uintptr_t)v->data + (first * elem_size);
    uintptr_t nextsrc = (uintptr_t)dataArray;
    for(size_t i = 0; i < block_count; i++) {
        if(!v->type->fixedSize) {
            for(size_t j = 0; j < block; j++) {
                UA_deleteMembers((void*)nextdst, v->type);
                nextdst += elem_size;
            }
            nextdst -= block * elem_size;
        }
        memcpy((void*)nextdst, (void*)nextsrc, elem_size * block);
        nextsrc += block * elem_size;
        nextdst += stride * elem_size;
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_Variant_setRangeCopy(UA_Variant *v, const void *dataArray, UA_Int32 dataArraySize,
                        const UA_NumericRange range) {
    size_t count, block, stride, first;
    UA_StatusCode retval = testRangeWithVariant(v, range, &count, &block, &stride, &first);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if((UA_Int32)count != dataArraySize)
        return UA_STATUSCODE_BADINDEXRANGEINVALID;

    size_t block_count = count / block;
    size_t elem_size = v->type->memSize;
    uintptr_t nextdst = (uintptr_t)v->data + (first * elem_size);
    uintptr_t nextsrc = (uintptr_t)dataArray;
    if(v->type->fixedSize) {
        for(size_t i = 0; i < block_count; i++) {
            memcpy((void*)nextdst, (void*)nextsrc, elem_size * block);
            nextsrc += block * elem_size;
            nextdst += stride * elem_size;
        }
    } else {
        for(size_t i = 0; i < block_count; i++) {
            for(size_t j = 0; j < block; j++) {
                UA_deleteMembers((void*)nextdst, v->type);
                UA_copy((void*)nextsrc, (void*)nextdst, v->type);
                nextdst += elem_size;
                nextsrc += elem_size;
            }
            nextdst += (stride - block) * elem_size;
        }
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setScalar(UA_Variant *v, void * UA_RESTRICT p, const UA_DataType *type) {
    return UA_Variant_setArray(v, p, -1, type);
}

UA_StatusCode UA_Variant_setScalarCopy(UA_Variant *v, const void *p, const UA_DataType *type) {
    void *new = UA_malloc(type->memSize);
    if(!new)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    UA_StatusCode retval = UA_copy(p, new, type);
	if(retval != UA_STATUSCODE_GOOD) {
		UA_delete(new, type);
		return retval;
	}
    return UA_Variant_setArray(v, new, -1, type);
}

UA_StatusCode UA_Variant_setArray(UA_Variant *v, void * UA_RESTRICT array, UA_Int32 elements,
                                  const UA_DataType *type) {
    v->type = type;
    v->arrayLength = elements;
    v->data = array;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Variant_setArrayCopy(UA_Variant *v, const void *array, UA_Int32 elements,
                                      const UA_DataType *type) {
    void *new;
    UA_StatusCode retval = UA_Array_copy(array, &new, type, elements);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    return UA_Variant_setArray(v, new, elements, type);
}

/* DiagnosticInfo */
UA_TYPE_DEFAULT(UA_DiagnosticInfo)

void UA_DiagnosticInfo_deleteMembers(UA_DiagnosticInfo *p) {
    UA_String_deleteMembers(&p->additionalInfo);
    if(p->hasInnerDiagnosticInfo && p->innerDiagnosticInfo) {
        UA_DiagnosticInfo_delete(p->innerDiagnosticInfo);
        p->innerDiagnosticInfo = NULL;
    }
}

void UA_DiagnosticInfo_init(UA_DiagnosticInfo *p) {
    memset(p, 0, sizeof(UA_DiagnosticInfo));
    UA_String_init(&p->additionalInfo);
}

UA_StatusCode UA_DiagnosticInfo_copy(UA_DiagnosticInfo const *src, UA_DiagnosticInfo *dst) {
    memcpy(dst, src, sizeof(UA_DiagnosticInfo));
    UA_String_init(&dst->additionalInfo);
    dst->innerDiagnosticInfo = NULL;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(src->hasAdditionalInfo)
       retval = UA_String_copy(&src->additionalInfo, &dst->additionalInfo);
    if(src->hasInnerDiagnosticInfo && src->innerDiagnosticInfo) {
        if((dst->innerDiagnosticInfo = UA_malloc(sizeof(UA_DiagnosticInfo)))) {
            retval |= UA_DiagnosticInfo_copy(src->innerDiagnosticInfo, dst->innerDiagnosticInfo);
            dst->hasInnerDiagnosticInfo = src->hasInnerDiagnosticInfo;
        } else {
            retval |= UA_STATUSCODE_BADOUTOFMEMORY;
        }
    }
    if(retval != UA_STATUSCODE_GOOD) {
        UA_DiagnosticInfo_deleteMembers(dst);
        UA_DiagnosticInfo_init(dst);
    }
    return retval;
}

/*******************/
/* Structure Types */
/*******************/

void UA_init(void *p, const UA_DataType *type) {
    /* builtin types */
    switch(type->typeIndex + (0x8000 * !type->namespaceZero)) {
    case UA_TYPES_BOOLEAN:
    case UA_TYPES_SBYTE:
    case UA_TYPES_BYTE:
        *(UA_Byte*)p = 0;
        return;
    case UA_TYPES_INT16:
    case UA_TYPES_UINT16:
        *(UA_Int16*)p = 0;
        return;
    case UA_TYPES_INT32:
    case UA_TYPES_UINT32:
    case UA_TYPES_STATUSCODE:
    case UA_TYPES_FLOAT:
        *(UA_Int32*)p = 0;
        return;
    case UA_TYPES_INT64:
    case UA_TYPES_UINT64:
    case UA_TYPES_DOUBLE:
    case UA_TYPES_DATETIME:
        *(UA_Int64*)p = 0;
        return;
    case UA_TYPES_GUID:
        UA_Guid_init((UA_Guid*)p);
        return;
    case UA_TYPES_NODEID:
        UA_NodeId_init((UA_NodeId*)p);
        return;
    case UA_TYPES_EXTENSIONOBJECT:
        UA_ExtensionObject_init((UA_ExtensionObject*)p);
        return;
    case UA_TYPES_DATAVALUE:
        UA_DataValue_init((UA_DataValue*)p);
        return;
    case UA_TYPES_VARIANT:
        UA_Variant_init((UA_Variant*)p);
        return;
    case UA_TYPES_DIAGNOSTICINFO:
        UA_DiagnosticInfo_init((UA_DiagnosticInfo*)p);
        return;
    }

    /* structured types */
    uintptr_t ptr = (uintptr_t)p;
    UA_Byte membersSize = type->membersSize;
    for(size_t i = 0; i < membersSize; i++) {
        const UA_DataTypeMember *member = &type->members[i];
        if(!member->isArray) {
            const UA_DataType *memberType;
            if(member->namespaceZero)
                memberType = &UA_TYPES[member->memberTypeIndex];
            else
                memberType = &type[member->memberTypeIndex - type->typeIndex];
            ptr += member->padding;
            UA_init((void*)ptr, memberType);
            ptr += memberType->memSize;
        } else {
            ptr += (member->padding >> 3);
            *((UA_Int32*)ptr) = -1;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            *((void**)ptr) = NULL;
            ptr += sizeof(void*);
        }
    }
}

void * UA_new(const UA_DataType *type) {
    void *p = UA_malloc(type->memSize);
    if(p)
        UA_init(p, type);
    return p;
}

UA_StatusCode UA_copy(const void *src, void *dst, const UA_DataType *type) {
    if(type->fixedSize) {
        memcpy(dst, src, type->memSize);
        return UA_STATUSCODE_GOOD;
    }
    switch(type->typeIndex + (0x8000 * !type->namespaceZero)) {
    case UA_TYPES_NODEID:
        return UA_NodeId_copy((const UA_NodeId*)src, (UA_NodeId*)dst);
    case UA_TYPES_EXTENSIONOBJECT:
        return UA_ExtensionObject_copy((const UA_ExtensionObject*)src, (UA_ExtensionObject*)dst);
    case UA_TYPES_DATAVALUE:
        return UA_DataValue_copy((const UA_DataValue*)src, (UA_DataValue*)dst);
    case UA_TYPES_VARIANT:
        return UA_Variant_copy((const UA_Variant*)src, (UA_Variant*)dst);
    case UA_TYPES_DIAGNOSTICINFO:
        return UA_DiagnosticInfo_copy((const UA_DiagnosticInfo*)src, (UA_DiagnosticInfo*)dst);
    }

    UA_init(dst, type);
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    uintptr_t ptrs = (uintptr_t)src;
    uintptr_t ptrd = (uintptr_t)dst;
    UA_Byte membersSize = type->membersSize;
    for(size_t i = 0; i < membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &type->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &type[member->memberTypeIndex - type->typeIndex];
        if(!member->isArray) {
            ptrs += member->padding;
            ptrd += member->padding;
            retval = UA_copy((const void*)ptrs, (void*)ptrd, memberType);
            ptrs += memberType->memSize;
            ptrd += memberType->memSize;
        } else {
            ptrs += (member->padding >> 3);
            ptrd += (member->padding >> 3);
            UA_Int32 *dstNoElements = (UA_Int32*)ptrd;
            const UA_Int32 elements = *((const UA_Int32*)ptrs);
            ptrs += sizeof(UA_Int32) + (member->padding & 0x07);
            ptrd += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_copy(*(void* const*)ptrs, (void**)ptrd, memberType, elements);
            if(retval == UA_STATUSCODE_GOOD)
                *dstNoElements = elements;
            ptrs += sizeof(void*);
            ptrd += sizeof(void*);
        }
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_deleteMembers(dst, type);
    return retval;
}

void UA_deleteMembers(void *p, const UA_DataType *type) {
    if(type->fixedSize)
        return;

    /* builtin types */
    switch(type->typeIndex + (0x8000 * !type->namespaceZero)) {
    case UA_TYPES_NODEID:
        UA_NodeId_deleteMembers((UA_NodeId*)p);
        return;
    case UA_TYPES_EXTENSIONOBJECT:
        UA_ExtensionObject_deleteMembers((UA_ExtensionObject*)p);
        return;
    case UA_TYPES_DATAVALUE:
        UA_DataValue_deleteMembers((UA_DataValue*)p);
        return;
    case UA_TYPES_VARIANT:
        UA_Variant_deleteMembers((UA_Variant*)p);
        return;
    case UA_TYPES_DIAGNOSTICINFO:
        UA_DiagnosticInfo_deleteMembers((UA_DiagnosticInfo*)p);
        return;
    }

    /* structured types */
    uintptr_t ptr = (uintptr_t)p;
    for(size_t i = 0; i < type->membersSize; i++) {
        const UA_DataTypeMember *member = &type->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &type[member->memberTypeIndex - type->typeIndex];
        if(!member->isArray) {
            ptr += member->padding;
            UA_deleteMembers((void*)ptr, memberType);
            ptr += memberType->memSize;
        } else {
            ptr += (member->padding >> 3);
            UA_Int32 elements = *(UA_Int32*)ptr;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            UA_Array_delete(*(void**)ptr, memberType, elements);
            *(void**)ptr = NULL;
            ptr += sizeof(void*);
        }
    }
}

void UA_delete(void *p, const UA_DataType *type) {
    UA_deleteMembers(p, type);
    UA_free(p);
}

/******************/
/* Array Handling */
/******************/

void* UA_Array_new(const UA_DataType *type, UA_Int32 elements) {
    if((UA_Int32)type->memSize * elements < 0 || type->memSize * elements > MAX_ARRAY_SIZE )
        return NULL;

    if(type->fixedSize)
        return UA_calloc(elements, type->memSize);

    void *p = UA_malloc(type->memSize * (size_t)elements);
    if(!p)
        return p;

    uintptr_t ptr = (uintptr_t)p;
    for(int i = 0; i<elements; i++) {
        UA_init((void*)ptr, type);
        ptr += type->memSize;
    }
    return p;
}

UA_StatusCode UA_Array_copy(const void *src, void **dst, const UA_DataType *type, UA_Int32 elements) {
    if(elements <= 0) {
        *dst = NULL;
        return UA_STATUSCODE_GOOD;
    }

    if(!(*dst = UA_malloc((size_t)elements * type->memSize)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    if(type->fixedSize) {
        memcpy(*dst, src, type->memSize * (size_t)elements);
        return UA_STATUSCODE_GOOD;
    }

    uintptr_t ptrs = (uintptr_t)src;
    uintptr_t ptrd = (uintptr_t)*dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    for(int i = 0; i < elements; i++) {
        retval |= UA_copy((void*)ptrs, (void*)ptrd, type);
        ptrs += type->memSize;
        ptrd += type->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_Array_delete(*dst, type, elements);
    return retval;
}

void UA_Array_delete(void *p, const UA_DataType *type, UA_Int32 elements) {
    if(!type->fixedSize) {
        uintptr_t ptr = (uintptr_t)p;
        for(UA_Int32 i = 0; i < elements; i++) {
            UA_deleteMembers((void*)ptr, type);
            ptr += type->memSize;
        }
    }
    UA_free(p);
}

/*********************************** amalgamated original file "C:/open62541/src/ua_types_encoding_binary.c" ***********************************/


/******************/
/* Array Handling */
/******************/

static UA_StatusCode UA_Array_encodeBinary(const void *src, UA_Int32 noElements, const UA_DataType *dataType,
                                           UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if(noElements <= -1)
        noElements = -1;
    UA_StatusCode retval = UA_Int32_encodeBinary(&noElements, dst, offset);
    if(retval)
        return retval;

#ifndef UA_NON_LITTLEENDIAN_ARCHITECTURE
    if(dataType->zeroCopyable) {
        if(noElements <= 0)
            return UA_STATUSCODE_GOOD;
        if((size_t) dst->length < *offset + (dataType->memSize * (size_t) noElements))
            return UA_STATUSCODE_BADENCODINGERROR;
        memcpy(&dst->data[*offset], src, dataType->memSize * (size_t) noElements);
        *offset += dataType->memSize * (size_t) noElements;
        return retval;
    }
#endif

    uintptr_t ptr = (uintptr_t) src;
    for(UA_Int32 i = 0; i < noElements && retval == UA_STATUSCODE_GOOD; i++) {
        retval = UA_encodeBinary((const void*) ptr, dataType, dst, offset);
        ptr += dataType->memSize;
    }
    return retval;
}

static UA_StatusCode UA_Array_decodeBinary(const UA_ByteString *src, size_t *UA_RESTRICT offset,
                                           UA_Int32 noElements_signed, void **dst, const UA_DataType *dataType) {
    if(noElements_signed <= 0) {
        *dst = NULL;
        return UA_STATUSCODE_GOOD;
    }

    size_t noElements = (size_t)noElements_signed; //is guaranteed positive

    if(dataType->memSize * noElements > MAX_ARRAY_SIZE)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    /* filter out arrays that can obviously not be parsed, because the message is too small */
    if(*offset + ((dataType->memSize * noElements) / 32) > (UA_UInt32) src->length)
        return UA_STATUSCODE_BADDECODINGERROR;

    *dst = UA_calloc(1, dataType->memSize * noElements);
    if(!*dst)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_StatusCode retval = UA_STATUSCODE_GOOD;

#ifndef UA_NON_LITTLEENDIAN_ARCHITECTURE
    if(dataType->zeroCopyable) {
        if((size_t) src->length < *offset + (dataType->memSize * (size_t) noElements))
            return UA_STATUSCODE_BADDECODINGERROR;
        memcpy(*dst, &src->data[*offset], dataType->memSize * (size_t) noElements);
        *offset += dataType->memSize * (size_t) noElements;
        return retval;
    }
#endif

    uintptr_t ptr = (uintptr_t) *dst;
    size_t i = 0;
    for(i = 0; i < noElements && retval == UA_STATUSCODE_GOOD; i++) {
        retval = UA_decodeBinary(src, offset, (void*) ptr, dataType);
        ptr += dataType->memSize;
    }
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Array_delete(*dst, dataType, i);
        *dst = NULL;
    }
    return retval;
}

/*****************/
/* Builtin Types */
/*****************/

/* Boolean */
UA_StatusCode UA_Boolean_encodeBinary(const UA_Boolean *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_Boolean)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    dst->data[*offset] = (UA_Byte) *src;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Boolean_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_Boolean *dst) {
    if((UA_Int32) (*offset + sizeof(UA_Boolean)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    *dst = (src->data[*offset] > 0) ? UA_TRUE : UA_FALSE;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

/* Byte */
UA_StatusCode UA_Byte_encodeBinary(const UA_Byte *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_Byte)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    dst->data[*offset] = (UA_Byte) *src;
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Byte_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_Byte *dst) {
    if((UA_Int32) (*offset + sizeof(UA_Byte)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    *dst = src->data[*offset];
    ++(*offset);
    return UA_STATUSCODE_GOOD;
}

/* UInt16 */
UA_StatusCode UA_UInt16_encodeBinary(UA_UInt16 const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_UInt16)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

#if defined(UA_NON_LITTLEENDIAN_ARCHITECTURE) && !defined(UA_ALIGNED_MEMORY_ACCESS)
    UA_UInt16 le_uint16 = htole16(*src);
    src = &le_uint16;
#endif

#ifdef UA_ALIGNED_MEMORY_ACCESS
    dst->data[(*offset)++] = (*src & 0x00FF) >> 0;
    dst->data[(*offset)++] = (*src & 0xFF00) >> 8;
#else
    *(UA_UInt16*) &dst->data[*offset] = *src;
    *offset += 2;
#endif
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt16_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_UInt16 *dst) {
    if((UA_Int32) (*offset + sizeof(UA_UInt16)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;

#ifdef UA_ALIGNED_MEMORY_ACCESS
    *dst = (UA_UInt16) src->data[(*offset)++] << 0;
    *dst |= (UA_UInt16) src->data[(*offset)++] << 8;
#else
    *dst = *((UA_UInt16*) &src->data[*offset]);
    *offset += 2;
#endif

#if defined(UA_NON_LITTLEENDIAN_ARCHITECTURE) && !defined(UA_ALIGNED_MEMORY_ACCESS)
    *dst = le16toh(*dst);
#endif
    return UA_STATUSCODE_GOOD;
}

/* UInt32 */
UA_StatusCode UA_UInt32_encodeBinary(UA_UInt32 const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_UInt32)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

#if defined(UA_NON_LITTLEENDIAN_ARCHITECTURE) && !defined(UA_ALIGNED_MEMORY_ACCESS)
    UA_UInt32 le_uint32 = htole32(*src);
    src = &le_uint32;
#endif

#ifdef UA_ALIGNED_MEMORY_ACCESS
    dst->data[(*offset)++] = (*src & 0x000000FF) >> 0;
    dst->data[(*offset)++] = (*src & 0x0000FF00) >> 8;
    dst->data[(*offset)++] = (*src & 0x00FF0000) >> 16;
    dst->data[(*offset)++] = (*src & 0xFF000000) >> 24;
#else
    *(UA_UInt32*) &dst->data[*offset] = *src;
    *offset += 4;
#endif
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt32_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_UInt32 *dst) {
    if((UA_Int32) (*offset + sizeof(UA_UInt32)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;

#ifdef UA_ALIGNED_MEMORY_ACCESS
    *dst = (UA_UInt32)((UA_Byte)(src->data[(*offset)++] & 0xFF));
    *dst |= (UA_UInt32)((UA_Byte)(src->data[(*offset)++] & 0xFF) << 8);
    *dst |= (UA_UInt32)((UA_Byte)(src->data[(*offset)++] & 0xFF) << 16);
    *dst |= (UA_UInt32)((UA_Byte)(src->data[(*offset)++] & 0xFF) << 24);
#else
    *dst = *((UA_UInt32*) &src->data[*offset]);
    *offset += 4;
#endif

#if defined(UA_NON_LITTLEENDIAN_ARCHITECTURE) && !defined(UA_ALIGNED_MEMORY_ACCESS)
    *dst = le32toh(*dst);
#endif
    return UA_STATUSCODE_GOOD;
}

/* UInt64 */
UA_StatusCode UA_UInt64_encodeBinary(UA_UInt64 const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_UInt64)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

#if defined(UA_NON_LITTLEENDIAN_ARCHITECTURE) && !defined(UA_ALIGNED_MEMORY_ACCESS)
    UA_UInt64 le_uint64 = htole64(*src);
    src = &le_uint64;
#endif

#ifdef UA_ALIGNED_MEMORY_ACCESS
    dst->data[(*offset)++] = (*src & 0x00000000000000FF) >> 0;
    dst->data[(*offset)++] = (*src & 0x000000000000FF00) >> 8;
    dst->data[(*offset)++] = (*src & 0x0000000000FF0000) >> 16;
    dst->data[(*offset)++] = (*src & 0x00000000FF000000) >> 24;
    dst->data[(*offset)++] = (*src & 0x000000FF00000000) >> 32;
    dst->data[(*offset)++] = (*src & 0x0000FF0000000000) >> 40;
    dst->data[(*offset)++] = (*src & 0x00FF000000000000) >> 48;
    dst->data[(*offset)++] = (*src & 0xFF00000000000000) >> 56;
#else
    *(UA_UInt64*) &dst->data[*offset] = *src;
    *offset += 8;
#endif
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_UInt64_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_UInt64 *dst) {
    if((UA_Int32) (*offset + sizeof(UA_UInt64)) > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;

#ifdef UA_ALIGNED_MEMORY_ACCESS
    *dst = (UA_UInt64) src->data[(*offset)++];
    *dst |= (UA_UInt64) src->data[(*offset)++] << 8;
    *dst |= (UA_UInt64) src->data[(*offset)++] << 16;
    *dst |= (UA_UInt64) src->data[(*offset)++] << 24;
    *dst |= (UA_UInt64) src->data[(*offset)++] << 32;
    *dst |= (UA_UInt64) src->data[(*offset)++] << 40;
    *dst |= (UA_UInt64) src->data[(*offset)++] << 48;
    *dst |= (UA_UInt64) src->data[(*offset)++] << 56;
#else
    *dst = *((UA_UInt64*) &src->data[*offset]);
    *offset += 8;
#endif

#if defined(UA_NON_LITTLEENDIAN_ARCHITECTURE) && !defined(UA_ALIGNED_MEMORY_ACCESS)
    *dst = le64toh(*dst);
#endif
    return UA_STATUSCODE_GOOD;
}

#ifdef UA_MIXED_ENDIAN
/* Float */
UA_Byte UA_FLOAT_ZERO[] = { 0x00, 0x00, 0x00, 0x00 };
UA_StatusCode UA_Float_decodeBinary(UA_ByteString const *src, size_t *offset, UA_Float * dst) {
    if(*offset + sizeof(UA_Float) > (UA_UInt32) src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_Float mantissa;
    UA_UInt32 biasedExponent;
    UA_Float sign;
    if(memcmp(&src->data[*offset], UA_FLOAT_ZERO, 4) == 0)
        return UA_Int32_decodeBinary(src, offset, (UA_Int32 *) dst);

    mantissa = (UA_Float) (src->data[*offset] & 0xFF); // bits 0-7
    mantissa = (mantissa / 256.0) + (UA_Float) (src->data[*offset + 1] & 0xFF); // bits 8-15
    mantissa = (mantissa / 256.0) + (UA_Float) (src->data[*offset + 2] & 0x7F); // bits 16-22
    biasedExponent = (src->data[*offset + 2] & 0x80) >> 7; // bits 23
    biasedExponent |= (src->data[*offset + 3] & 0x7F) << 1; // bits 24-30
    sign = (src->data[*offset + 3] & 0x80) ? -1.0 : 1.0; // bit 31
    if(biasedExponent >= 127)
        *dst = (UA_Float) sign * (1 << (biasedExponent - 127)) * (1.0 + mantissa / 128.0);
    else
        *dst = (UA_Float) sign * 2.0 * (1.0 + mantissa / 128.0) / ((UA_Float) (biasedExponent - 127));
    *offset += 4;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Float_encodeBinary(UA_Float const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_Float)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;
    UA_Float srcFloat = *src;
    dst->data[(*offset)++] = (UA_Byte) (((UA_Int32) srcFloat & 0xFF000000) >> 24);
    dst->data[(*offset)++] = (UA_Byte) (((UA_Int32) srcFloat & 0x00FF0000) >> 16);
    dst->data[(*offset)++] = (UA_Byte) (((UA_Int32) srcFloat & 0x0000FF00) >> 8);
    dst->data[(*offset)++] = (UA_Byte) ((UA_Int32)  srcFloat & 0x000000FF);
    return UA_STATUSCODE_GOOD;
}

/* Double */
UA_Byte UA_DOUBLE_ZERO[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
UA_StatusCode UA_Double_decodeBinary(UA_ByteString const *src, size_t *offset, UA_Double * dst) {
    if(*offset + sizeof(UA_Double) > (UA_UInt32) src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_Byte *dstBytes = (UA_Byte*)dst;
    UA_Double db = 0;
    memcpy(&db, &(src->data[*offset]),sizeof(UA_Double));
    dstBytes[4] = src->data[(*offset)++];
    dstBytes[5] = src->data[(*offset)++];
    dstBytes[6] = src->data[(*offset)++];
    dstBytes[7] = src->data[(*offset)++];
    dstBytes[0] = src->data[(*offset)++];
    dstBytes[1] = src->data[(*offset)++];
    dstBytes[2] = src->data[(*offset)++];
    dstBytes[3] = src->data[(*offset)++];

/*
    UA_Double sign;
    UA_Double mantissa;
    UA_UInt32 biasedExponent;
    if(memcmp(&src->data[*offset], UA_DOUBLE_ZERO, 8) == 0)
        return UA_Int64_decodeBinary(src, offset, (UA_Int64 *) dst);
    mantissa = (UA_Double) (src->data[*offset] & 0xFF); // bits 0-7
    mantissa = (mantissa / 256.0) + (UA_Double) (src->data[*offset + 1] & 0xFF); // bits 8-15
    mantissa = (mantissa / 256.0) + (UA_Double) (src->data[*offset + 2] & 0xFF); // bits 16-23
    mantissa = (mantissa / 256.0) + (UA_Double) (src->data[*offset + 3] & 0xFF); // bits 24-31
    mantissa = (mantissa / 256.0) + (UA_Double) (src->data[*offset + 4] & 0xFF); // bits 32-39
    mantissa = (mantissa / 256.0) + (UA_Double) (src->data[*offset + 5] & 0xFF); // bits 40-47
    mantissa = (mantissa / 256.0) + (UA_Double) (src->data[*offset + 6] & 0x0F); // bits 48-51
    biasedExponent = (src->data[*offset + 6] & 0xF0) >> 4; // bits 52-55
    biasedExponent |= ((UA_UInt32) (src->data[*offset + 7] & 0x7F)) << 4; // bits 56-62
    sign = (src->data[*offset + 7] & 0x80) ? -1.0 : 1.0; // bit 63
    if(biasedExponent >= 1023)
        *dst = (UA_Double) sign * (1 << (biasedExponent - 1023))
                * (1.0 + mantissa / 8.0);
    else
        *dst = (UA_Double) sign * 2.0 * (1.0 + mantissa / 8.0)
                / ((UA_Double) (biasedExponent - 1023));
    *offset += 8;
    *offset */
    return UA_STATUSCODE_GOOD;
}

/*expecting double in ieee754 format */
UA_StatusCode UA_Double_encodeBinary(UA_Double const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    if((UA_Int32) (*offset + sizeof(UA_Double)) > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

    /* ARM7TDMI Half Little Endian Byte order for Double 3 2 1 0 7 6 5 4 */
    UA_Byte srcDouble[sizeof(UA_Double)];
    memcpy(&srcDouble,src,sizeof(UA_Double));
    dst->data[(*offset)++] = srcDouble[4];
    dst->data[(*offset)++] = srcDouble[5];
    dst->data[(*offset)++] = srcDouble[6];
    dst->data[(*offset)++] = srcDouble[7];
    dst->data[(*offset)++] = srcDouble[0];
    dst->data[(*offset)++] = srcDouble[1];
    dst->data[(*offset)++] = srcDouble[2];
    dst->data[(*offset)++] = srcDouble[3];
    return UA_STATUSCODE_GOOD;
}
#endif /* UA_MIXED_ENDIAN */

/* String */
UA_StatusCode UA_String_encodeBinary(UA_String const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    UA_Int32 end = *offset + 4;
    if(src->length > 0)
        end += src->length;
    if(end > dst->length)
        return UA_STATUSCODE_BADENCODINGERROR;

    UA_StatusCode retval = UA_Int32_encodeBinary(&src->length, dst, offset);
    if(src->length > 0) {
        memcpy(&dst->data[*offset], src->data, src->length);
        *offset += src->length;
    }
    return retval;
}

UA_StatusCode UA_String_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_String *dst) {
    UA_String_init(dst);
    UA_Int32 length;
    if(UA_Int32_decodeBinary(src, offset, &length))
        return UA_STATUSCODE_BADINTERNALERROR;
    if(length <= 0) {
        if(length == 0)
            dst->length = 0;
        else
            dst->length = -1;
        return UA_STATUSCODE_GOOD;
    }
    if((UA_Int32) *offset + length > src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    if(!(dst->data = UA_malloc(length)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    memcpy(dst->data, &src->data[*offset], length);
    dst->length = length;
    *offset += length;
    return UA_STATUSCODE_GOOD;
}

/* Guid */
UA_StatusCode UA_Guid_encodeBinary(UA_Guid const *src, UA_ByteString * dst, size_t *UA_RESTRICT offset) {
    UA_StatusCode retval = UA_UInt32_encodeBinary(&src->data1, dst, offset);
    retval |= UA_UInt16_encodeBinary(&src->data2, dst, offset);
    retval |= UA_UInt16_encodeBinary(&src->data3, dst, offset);
    for(UA_Int32 i = 0; i < 8; i++)
        retval |= UA_Byte_encodeBinary(&src->data4[i], dst, offset);
    return retval;
}

UA_StatusCode UA_Guid_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_Guid * dst) {
    // This could be done with a single memcpy (if the compiler does no fancy realigning of structs)
    UA_StatusCode retval = UA_UInt32_decodeBinary(src, offset, &dst->data1);
    retval |= UA_UInt16_decodeBinary(src, offset, &dst->data2);
    retval |= UA_UInt16_decodeBinary(src, offset, &dst->data3);
    for(size_t i = 0; i < 8; i++)
        retval |= UA_Byte_decodeBinary(src, offset, &dst->data4[i]);
    if(retval)
        UA_Guid_deleteMembers(dst);
    return retval;
}

/* NodeId */
#define UA_NODEIDTYPE_TWOBYTE 0
#define UA_NODEIDTYPE_FOURBYTE 1

UA_StatusCode UA_NodeId_encodeBinary(UA_NodeId const *src, UA_ByteString * dst, size_t *UA_RESTRICT offset) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    // temporary variables for endian-save code
    UA_Byte srcByte;
    UA_UInt16 srcUInt16;
    UA_UInt32 srcUInt32;
    switch (src->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(src->identifier.numeric > UA_UINT16_MAX
                || src->namespaceIndex > UA_BYTE_MAX) {
            srcByte = UA_NODEIDTYPE_NUMERIC;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
            srcUInt32 = src->identifier.numeric;
            retval |= UA_UInt32_encodeBinary(&srcUInt32, dst, offset);
        } else if(src->identifier.numeric > UA_BYTE_MAX
                || src->namespaceIndex > 0) {
            /* UA_NODEIDTYPE_FOURBYTE */
            srcByte = UA_NODEIDTYPE_FOURBYTE;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            srcByte = (UA_Byte)src->namespaceIndex;
            srcUInt16 = src->identifier.numeric;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            retval |= UA_UInt16_encodeBinary(&srcUInt16, dst, offset);
        } else {
            /* UA_NODEIDTYPE_TWOBYTE */
            srcByte = UA_NODEIDTYPE_TWOBYTE;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
            srcByte = src->identifier.numeric;
            retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        }
        break;
    case UA_NODEIDTYPE_STRING:
        srcByte = UA_NODEIDTYPE_STRING;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_String_encodeBinary(&src->identifier.string, dst, offset);
        break;
    case UA_NODEIDTYPE_GUID:
        srcByte = UA_NODEIDTYPE_GUID;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_Guid_encodeBinary(&src->identifier.guid, dst, offset);
        break;
    case UA_NODEIDTYPE_BYTESTRING:
        srcByte = UA_NODEIDTYPE_BYTESTRING;
        retval |= UA_Byte_encodeBinary(&srcByte, dst, offset);
        retval |= UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
        retval |= UA_ByteString_encodeBinary(&src->identifier.byteString, dst,
                offset);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return retval;
}

UA_StatusCode UA_NodeId_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_NodeId *dst) {
    // temporary variables to overcome decoder's non-endian-saveness for datatypes with different length
    UA_Byte dstByte = 0;
    UA_UInt16 dstUInt16 = 0;
    UA_Byte encodingByte = 0;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingByte);
    if(retval) {
        UA_NodeId_init(dst);
        return retval;
    }

    switch (encodingByte) {
    case UA_NODEIDTYPE_TWOBYTE: // Table 7
        dst->identifierType = UA_NODEIDTYPE_NUMERIC;
        retval = UA_Byte_decodeBinary(src, offset, &dstByte);
        dst->identifier.numeric = dstByte;
        dst->namespaceIndex = 0; // default namespace
        break;
    case UA_NODEIDTYPE_FOURBYTE: // Table 8
        dst->identifierType = UA_NODEIDTYPE_NUMERIC;
        retval |= UA_Byte_decodeBinary(src, offset, &dstByte);
        dst->namespaceIndex = dstByte;
        retval |= UA_UInt16_decodeBinary(src, offset, &dstUInt16);
        dst->identifier.numeric = dstUInt16;
        break;
    case UA_NODEIDTYPE_NUMERIC: // Table 6, first entry
        dst->identifierType = UA_NODEIDTYPE_NUMERIC;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_UInt32_decodeBinary(src, offset, &dst->identifier.numeric);
        break;
    case UA_NODEIDTYPE_STRING: // Table 6, second entry
        dst->identifierType = UA_NODEIDTYPE_STRING;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_String_decodeBinary(src, offset, &dst->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID: // Table 6, third entry
        dst->identifierType = UA_NODEIDTYPE_GUID;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_Guid_decodeBinary(src, offset, &dst->identifier.guid);
        break;
    case UA_NODEIDTYPE_BYTESTRING: // Table 6, "OPAQUE"
        dst->identifierType = UA_NODEIDTYPE_BYTESTRING;
        retval |= UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
        retval |= UA_ByteString_decodeBinary(src, offset,
                &dst->identifier.byteString);
        break;
    default:
        UA_NodeId_init(dst);
        retval |= UA_STATUSCODE_BADINTERNALERROR; // the client sends an encodingByte we do not recognize
        break;
    }
    if(retval)
        UA_NodeId_deleteMembers(dst);
    return retval;
}

/* ExpandedNodeId */
#define UA_EXPANDEDNODEID_NAMESPACEURI_FLAG 0x80
#define UA_EXPANDEDNODEID_SERVERINDEX_FLAG 0x40

UA_StatusCode UA_ExpandedNodeId_encodeBinary(UA_ExpandedNodeId const *src, UA_ByteString *dst,
                                             size_t *UA_RESTRICT offset) {
    UA_Byte flags = 0;
    UA_UInt32 start = *offset;
    UA_StatusCode retval = UA_NodeId_encodeBinary(&src->nodeId, dst, offset);
    if(src->namespaceUri.length > 0) {
        // TODO: Set namespaceIndex to 0 in the nodeid as the namespaceUri takes precedence
        retval |= UA_String_encodeBinary(&src->namespaceUri, dst, offset);
        flags |= UA_EXPANDEDNODEID_NAMESPACEURI_FLAG;
    }
    if(src->serverIndex > 0) {
        retval |= UA_UInt32_encodeBinary(&src->serverIndex, dst, offset);
        flags |= UA_EXPANDEDNODEID_SERVERINDEX_FLAG;
    }
    if(flags != 0)
        dst->data[start] |= flags;
    return retval;
}

UA_StatusCode UA_ExpandedNodeId_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset,
                                             UA_ExpandedNodeId *dst) {
    UA_ExpandedNodeId_init(dst);
    // get encodingflags and leave a "clean" nodeidtype
    if((UA_Int32) *offset >= src->length)
        return UA_STATUSCODE_BADDECODINGERROR;
    UA_Byte encodingByte = src->data[*offset];
    src->data[*offset] = encodingByte & ~(UA_EXPANDEDNODEID_NAMESPACEURI_FLAG |
    UA_EXPANDEDNODEID_SERVERINDEX_FLAG);
    UA_StatusCode retval = UA_NodeId_decodeBinary(src, offset, &dst->nodeId);
    if(encodingByte & UA_EXPANDEDNODEID_NAMESPACEURI_FLAG) {
        dst->nodeId.namespaceIndex = 0;
        retval |= UA_String_decodeBinary(src, offset, &dst->namespaceUri);
    }
    if(encodingByte & UA_EXPANDEDNODEID_SERVERINDEX_FLAG)
        retval |= UA_UInt32_decodeBinary(src, offset, &dst->serverIndex);
    if(retval)
        UA_ExpandedNodeId_deleteMembers(dst);
    return retval;
}

/* QualifiedName */
UA_StatusCode UA_QualifiedName_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset,
                                            UA_QualifiedName *dst) {
    UA_QualifiedName_init(dst);
    UA_StatusCode retval = UA_UInt16_decodeBinary(src, offset, &dst->namespaceIndex);
    retval |= UA_String_decodeBinary(src, offset, &dst->name);
    if(retval)
        UA_QualifiedName_deleteMembers(dst);
    return retval;
}

UA_StatusCode UA_QualifiedName_encodeBinary(UA_QualifiedName const *src, UA_ByteString* dst,
                                            size_t *UA_RESTRICT offset) {
    UA_StatusCode retval = UA_UInt16_encodeBinary(&src->namespaceIndex, dst, offset);
    retval |= UA_String_encodeBinary(&src->name, dst, offset);
    return retval;
}

/* LocalizedText */
#define UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE 0x01
#define UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT 0x02

UA_StatusCode UA_LocalizedText_encodeBinary(UA_LocalizedText const *src, UA_ByteString *dst,
                                            size_t *UA_RESTRICT offset) {
    UA_Byte encodingMask = 0;
    if(src->locale.data != NULL)
        encodingMask |=
        UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE;
    if(src->text.data != NULL)
        encodingMask |=
        UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT;
    UA_StatusCode retval = UA_Byte_encodeBinary(&encodingMask, dst, offset);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE)
        retval |= UA_String_encodeBinary(&src->locale, dst, offset);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT)
        retval |= UA_String_encodeBinary(&src->text, dst, offset);
    return retval;
}

UA_StatusCode UA_LocalizedText_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset,
                                            UA_LocalizedText *dst) {
    UA_LocalizedText_init(dst);
    UA_Byte encodingMask = 0;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingMask);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_LOCALE)
        retval |= UA_String_decodeBinary(src, offset, &dst->locale);
    if(encodingMask & UA_LOCALIZEDTEXT_ENCODINGMASKTYPE_TEXT)
        retval |= UA_String_decodeBinary(src, offset, &dst->text);
    if(retval)
        UA_LocalizedText_deleteMembers(dst);
    return retval;
}

/* ExtensionObject */
UA_StatusCode UA_ExtensionObject_encodeBinary(UA_ExtensionObject const *src, UA_ByteString *dst,
                                              size_t *UA_RESTRICT offset) {
    UA_StatusCode retval = UA_NodeId_encodeBinary(&src->typeId, dst, offset);
    retval |= UA_Byte_encodeBinary((const UA_Byte*) &src->encoding, dst, offset);
    switch (src->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
        retval |= UA_ByteString_encodeBinary(&src->body, dst, offset);
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        retval |= UA_ByteString_encodeBinary(&src->body, dst, offset);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return retval;
}

UA_StatusCode UA_ExtensionObject_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset,
                                              UA_ExtensionObject *dst) {
    UA_ExtensionObject_init(dst);
    UA_Byte encoding = 0;
    UA_StatusCode retval = UA_NodeId_decodeBinary(src, offset, &dst->typeId);
    retval |= UA_Byte_decodeBinary(src, offset, &encoding);
    dst->encoding = encoding;
    retval |= UA_String_copy(&UA_STRING_NULL, (UA_String *) &dst->body);
    if(retval) {
        UA_ExtensionObject_init(dst);
        return retval;
    }
    switch (dst->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        retval |= UA_ByteString_decodeBinary(src, offset, &dst->body);
        break;
    default:
        UA_ExtensionObject_deleteMembers(dst);
        return UA_STATUSCODE_BADDECODINGERROR;
    }
    if(retval)
        UA_ExtensionObject_deleteMembers(dst);
    return retval;
}

/* DataValue */
UA_StatusCode UA_DataValue_encodeBinary(UA_DataValue const *src, UA_ByteString *dst,
                                        size_t *UA_RESTRICT offset) {
    UA_StatusCode retval = UA_Byte_encodeBinary((const UA_Byte*) src, dst, offset);
    if(src->hasValue)
        retval |= UA_Variant_encodeBinary(&src->value, dst, offset);
    if(src->hasStatus)
        retval |= UA_StatusCode_encodeBinary(&src->status, dst, offset);
    if(src->hasSourceTimestamp)
        retval |= UA_DateTime_encodeBinary(&src->sourceTimestamp, dst, offset);
    if(src->hasSourcePicoseconds)
        retval |= UA_Int16_encodeBinary(&src->sourcePicoseconds, dst, offset);
    if(src->hasServerTimestamp)
        retval |= UA_DateTime_encodeBinary(&src->serverTimestamp, dst, offset);
    if(src->hasServerPicoseconds)
        retval |= UA_Int16_encodeBinary(&src->serverPicoseconds, dst, offset);
    return retval;
}

#define MAX_PICO_SECONDS 1000
UA_StatusCode UA_DataValue_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset,
                                        UA_DataValue *dst) {
    UA_DataValue_init(dst);
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*) dst);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if(dst->hasValue)
        retval |= UA_Variant_decodeBinary(src, offset, &dst->value);
    if(dst->hasStatus)
        retval |= UA_StatusCode_decodeBinary(src, offset, &dst->status);
    if(dst->hasSourceTimestamp)
        retval |= UA_DateTime_decodeBinary(src, offset, &dst->sourceTimestamp);
    if(dst->hasSourcePicoseconds) {
        retval |= UA_Int16_decodeBinary(src, offset, &dst->sourcePicoseconds);
        if(dst->sourcePicoseconds > MAX_PICO_SECONDS)
            dst->sourcePicoseconds = MAX_PICO_SECONDS;
    }
    if(dst->hasServerTimestamp)
        retval |= UA_DateTime_decodeBinary(src, offset, &dst->serverTimestamp);
    if(dst->hasServerPicoseconds) {
        retval |= UA_Int16_decodeBinary(src, offset, &dst->serverPicoseconds);
        if(dst->serverPicoseconds > MAX_PICO_SECONDS)
            dst->serverPicoseconds = MAX_PICO_SECONDS;
    }
    if(retval)
        UA_DataValue_deleteMembers(dst);
    return retval;
}

/* Variant */
/* We can store all data types in a variant internally. But for communication we wrap them in an
 * ExtensionObject if they are not builtin. */

enum UA_VARIANT_ENCODINGMASKTYPE_enum {
    UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK = 0x3F, // bits 0:5
    UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS = (0x01 << 6), // bit 6
    UA_VARIANT_ENCODINGMASKTYPE_ARRAY = (0x01 << 7)       // bit 7
};

UA_StatusCode UA_Variant_encodeBinary(UA_Variant const *src, UA_ByteString *dst, size_t *UA_RESTRICT offset) {
    UA_Boolean isArray = src->arrayLength != -1 || !src->data; // a single element is not an array
    UA_Boolean hasDimensions = isArray && src->arrayDimensions != NULL;
    UA_Boolean isBuiltin = src->type->namespaceZero && UA_IS_BUILTIN(src->type->typeIndex);
    UA_Byte encodingByte = 0;
    if(isArray) {
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_ARRAY;
        if(hasDimensions)
            encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS;
    }

    UA_NodeId typeId;
    UA_NodeId_init(&typeId);
    if(isBuiltin)
        /* Do an extra lookup. E.g. UA_ServerState is encoded as UA_UINT32. The
           typeindex points to the true type. */
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK &
            (UA_Byte) UA_TYPES[src->type->typeIndex].typeId.identifier.numeric;
    else {
        encodingByte |= UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK & (UA_Byte) 22; // wrap in an extensionobject
        typeId = src->type->typeId;
        if(typeId.identifierType != UA_NODEIDTYPE_NUMERIC)
            return UA_STATUSCODE_BADINTERNALERROR;
        typeId.identifier.numeric += UA_ENCODINGOFFSET_BINARY;
    }

    UA_Int32 numToEncode = src->arrayLength;
    UA_StatusCode retval = UA_Byte_encodeBinary(&encodingByte, dst, offset);
    if(isArray)
        retval |= UA_Int32_encodeBinary(&src->arrayLength, dst, offset);
    else
        numToEncode = 1;

    uintptr_t ptr = (uintptr_t) src->data;
    ptrdiff_t memSize = src->type->memSize;
    for(UA_Int32 i = 0; i < numToEncode; i++) {
        size_t oldoffset; // before encoding the actual content
        if(!isBuiltin) {
            /* The type is wrapped inside an extensionobject */
            retval |= UA_NodeId_encodeBinary(&typeId, dst, offset);
            UA_Byte eoEncoding =
                    UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;
            retval |= UA_Byte_encodeBinary(&eoEncoding, dst, offset);
            *offset += 4;
            oldoffset = *offset;
        }
        retval |= UA_encodeBinary((void*) ptr, src->type, dst, offset);
        if(!isBuiltin) {
            /* Jump back and print the length of the extension object */
            UA_Int32 encodingLength = *offset - oldoffset;
            oldoffset -= 4;
            retval |= UA_Int32_encodeBinary(&encodingLength, dst, &oldoffset);
        }
        ptr += memSize;
    }
    if(hasDimensions)
        retval |= UA_Array_encodeBinary(src->arrayDimensions, src->arrayDimensionsSize,
                                        &UA_TYPES[UA_TYPES_INT32], dst, offset);
    return retval;
}

/* The resulting variant always has the storagetype UA_VARIANT_DATA. Currently,
 we only support ns0 types (todo: attach typedescriptions to datatypenodes) */
UA_StatusCode UA_Variant_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset, UA_Variant *dst) {
    UA_Variant_init(dst);
    UA_Byte encodingByte;
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, &encodingByte);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_Boolean isArray = encodingByte & UA_VARIANT_ENCODINGMASKTYPE_ARRAY;
    size_t typeIndex = (encodingByte & UA_VARIANT_ENCODINGMASKTYPE_TYPEID_MASK) - 1;
    if(typeIndex > 24) /* must be builtin */
        return UA_STATUSCODE_BADDECODINGERROR;

    if(isArray || typeIndex != UA_TYPES_EXTENSIONOBJECT) {
        /* an array or a single builtin */
        const UA_DataType *dataType = &UA_TYPES[typeIndex];
        UA_Int32 arraySize = -1;
        if(isArray) {
            retval |= UA_Int32_decodeBinary(src, offset, &arraySize);
            if(retval != UA_STATUSCODE_GOOD)
                return retval;
        }
        retval |= UA_Array_decodeBinary(src, offset, (!isArray && arraySize==-1) ? 1: arraySize, &dst->data, dataType);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        dst->arrayLength = arraySize; // for deleteMembers
        dst->type = dataType;
    } else {
        /* a single extensionobject */
        size_t oldoffset = *offset;
        UA_NodeId typeId;
        if((retval |= UA_NodeId_decodeBinary(src, offset, &typeId)) != UA_STATUSCODE_GOOD)
            return retval;
        UA_Byte EOencodingByte;
        if((retval |= UA_Byte_decodeBinary(src, offset, &EOencodingByte)) != UA_STATUSCODE_GOOD) {
            UA_NodeId_deleteMembers(&typeId);
            return retval;
        }
        const UA_DataType *dataType = NULL;
        if(typeId.namespaceIndex == 0 && EOencodingByte == UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING) {
            for(typeIndex = 0;typeIndex < UA_TYPES_COUNT; typeIndex++) {
                if(UA_NodeId_equal(&typeId, &UA_TYPES[typeIndex].typeId)) {
                    dataType = &UA_TYPES[typeIndex];
                    break;
                }
            }
        }
        UA_NodeId_deleteMembers(&typeId);
        if(!dataType) {
            *offset = oldoffset;
            dataType = &UA_TYPES[UA_TYPES_EXTENSIONOBJECT];
        }
        dst->data = UA_malloc(dataType->memSize);
        if((retval |= UA_decodeBinary(src, offset, dst->data, dataType)) != UA_STATUSCODE_GOOD)
            return retval;
        dst->type = dataType;
        dst->arrayLength = -1;
    }

    /* array dimensions */
    if(isArray && (encodingByte & UA_VARIANT_ENCODINGMASKTYPE_DIMENSIONS)) {
        retval |= UA_Int32_decodeBinary(src, offset, &dst->arrayDimensionsSize);
        if(retval == UA_STATUSCODE_GOOD)
            retval |= UA_Array_decodeBinary(src, offset, dst->arrayDimensionsSize,
                                            (void**)&dst->arrayDimensions, &UA_TYPES[UA_TYPES_INT32]);
        if(retval != UA_STATUSCODE_GOOD) {
            dst->arrayDimensionsSize = -1;
            UA_Variant_deleteMembers(dst);
            return retval;
        }
    }
    return UA_STATUSCODE_GOOD;
}

/* DiagnosticInfo */
UA_StatusCode UA_DiagnosticInfo_encodeBinary(const UA_DiagnosticInfo *src, UA_ByteString * dst,
                                             size_t *UA_RESTRICT offset) {
    UA_StatusCode retval = UA_Byte_encodeBinary((const UA_Byte *) src, dst, offset);
    if(src->hasSymbolicId)
        retval |= UA_Int32_encodeBinary(&src->symbolicId, dst, offset);
    if(src->hasNamespaceUri)
        retval |= UA_Int32_encodeBinary(&src->namespaceUri, dst, offset);
    if(src->hasLocalizedText)
        retval |= UA_Int32_encodeBinary(&src->localizedText, dst, offset);
    if(src->hasLocale)
        retval |= UA_Int32_encodeBinary(&src->locale, dst, offset);
    if(src->hasAdditionalInfo)
        retval |= UA_String_encodeBinary(&src->additionalInfo, dst, offset);
    if(src->hasInnerStatusCode)
        retval |= UA_StatusCode_encodeBinary(&src->innerStatusCode, dst, offset);
    if(src->hasInnerDiagnosticInfo)
        retval |= UA_DiagnosticInfo_encodeBinary(src->innerDiagnosticInfo, dst, offset);
    return retval;
}

UA_StatusCode UA_DiagnosticInfo_decodeBinary(UA_ByteString const *src, size_t *UA_RESTRICT offset,
                                             UA_DiagnosticInfo *dst) {
    UA_DiagnosticInfo_init(dst);
    UA_StatusCode retval = UA_Byte_decodeBinary(src, offset, (UA_Byte*) dst);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    if(dst->hasSymbolicId)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->symbolicId);
    if(dst->hasNamespaceUri)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->namespaceUri);
    if(dst->hasLocalizedText)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->localizedText);
    if(dst->hasLocale)
        retval |= UA_Int32_decodeBinary(src, offset, &dst->locale);
    if(dst->hasAdditionalInfo)
        retval |= UA_String_decodeBinary(src, offset, &dst->additionalInfo);
    if(dst->hasInnerStatusCode)
        retval |= UA_StatusCode_decodeBinary(src, offset, &dst->innerStatusCode);
    if(dst->hasInnerDiagnosticInfo) {
        // innerDiagnosticInfo is a pointer to struct, therefore allocate
        dst->innerDiagnosticInfo = UA_malloc(sizeof(UA_DiagnosticInfo));
        if(dst->innerDiagnosticInfo) {
            if(UA_DiagnosticInfo_decodeBinary(src, offset,
                                              dst->innerDiagnosticInfo) != UA_STATUSCODE_GOOD) {
                UA_free(dst->innerDiagnosticInfo);
                dst->innerDiagnosticInfo = NULL;
                retval |= UA_STATUSCODE_BADINTERNALERROR;
            }
        } else {
            retval |= UA_STATUSCODE_BADOUTOFMEMORY;
        }
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_DiagnosticInfo_deleteMembers(dst);
    return retval;
}

/********************/
/* Structured Types */
/********************/

UA_StatusCode UA_encodeBinary(const void *src, const UA_DataType *dataType, UA_ByteString *dst,
                              size_t *UA_RESTRICT offset) {
    /* builtin types */
    switch(dataType->typeIndex + (0x8000 * !dataType->namespaceZero)) {
    case UA_TYPES_BOOLEAN:
    case UA_TYPES_SBYTE:
    case UA_TYPES_BYTE:
        return UA_Byte_encodeBinary((const UA_Byte*)src, dst, offset);
    case UA_TYPES_INT16:
    case UA_TYPES_UINT16:
        return UA_UInt16_encodeBinary((const UA_UInt16*)src, dst, offset);
    case UA_TYPES_INT32:
    case UA_TYPES_UINT32:
    case UA_TYPES_STATUSCODE:
        return UA_UInt32_encodeBinary((const UA_UInt32*)src, dst, offset);
    case UA_TYPES_INT64:
    case UA_TYPES_UINT64:
    case UA_TYPES_DATETIME:
        return UA_UInt64_encodeBinary((const UA_UInt64*)src, dst, offset);
    case UA_TYPES_FLOAT:
        return UA_Float_encodeBinary((const UA_Float*)src, dst, offset);
    case UA_TYPES_DOUBLE:
        return UA_Double_encodeBinary((const UA_Double*)src, dst, offset);
    case UA_TYPES_GUID:
        return UA_Guid_encodeBinary((const UA_Guid*)src, dst, offset);
    case UA_TYPES_NODEID:
        return UA_NodeId_encodeBinary((const UA_NodeId*)src, dst, offset);
    case UA_TYPES_EXPANDEDNODEID:
        return UA_ExpandedNodeId_encodeBinary((const UA_ExpandedNodeId*)src, dst, offset);
    case UA_TYPES_LOCALIZEDTEXT:
        return UA_LocalizedText_encodeBinary((const UA_LocalizedText*)src, dst, offset);
    case UA_TYPES_EXTENSIONOBJECT:
        return UA_ExtensionObject_encodeBinary((const UA_ExtensionObject*)src, dst, offset);
    case UA_TYPES_DATAVALUE:
        return UA_DataValue_encodeBinary((const UA_DataValue*)src, dst, offset);
    case UA_TYPES_VARIANT:
        return UA_Variant_encodeBinary((const UA_Variant*)src, dst, offset);
    case UA_TYPES_DIAGNOSTICINFO:
        return UA_DiagnosticInfo_encodeBinary((const UA_DiagnosticInfo*)src, dst, offset);
    }

    /* structured type */
    uintptr_t ptr = (uintptr_t)src;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i = 0; i < membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];
        if(!member->isArray) {
            ptr += member->padding;
            retval = UA_encodeBinary((const void*) ptr, memberType, dst, offset);
            ptr += memberType->memSize;
        } else {
            ptr += (member->padding >> 3);
            const UA_Int32 noElements = *((const UA_Int32*) ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_encodeBinary(*(void * const *) ptr, noElements, memberType, dst, offset);
            ptr += sizeof(void*);
        }
    }
    return retval;
}

UA_StatusCode UA_decodeBinary(const UA_ByteString *src, size_t *UA_RESTRICT offset,
                              void *dst, const UA_DataType *dataType) {
    /* builtin types */
    switch(dataType->typeIndex + (0x8000 * !dataType->namespaceZero)) {
    case UA_TYPES_BOOLEAN:
    case UA_TYPES_SBYTE:
    case UA_TYPES_BYTE:
        return UA_Byte_decodeBinary(src, offset, (UA_Byte*)dst);
    case UA_TYPES_INT16:
    case UA_TYPES_UINT16:
        return UA_Int16_decodeBinary(src, offset, (UA_Int16*)dst);
    case UA_TYPES_INT32:
    case UA_TYPES_UINT32:
    case UA_TYPES_STATUSCODE:
        return UA_UInt32_decodeBinary(src, offset, (UA_UInt32*)dst);
    case UA_TYPES_INT64:
    case UA_TYPES_UINT64:
    case UA_TYPES_DATETIME:
        return UA_UInt64_decodeBinary(src, offset, (UA_UInt64*)dst);
    case UA_TYPES_FLOAT:
        return UA_Float_decodeBinary(src, offset, (UA_Float*)dst);
    case UA_TYPES_DOUBLE:
        return UA_Double_decodeBinary(src, offset, (UA_Double*)dst);
    case UA_TYPES_GUID:
        return UA_Guid_decodeBinary(src, offset, (UA_Guid*)dst);
    case UA_TYPES_NODEID:
        return UA_NodeId_decodeBinary(src, offset, (UA_NodeId*)dst);
    case UA_TYPES_EXPANDEDNODEID:
        return UA_ExpandedNodeId_decodeBinary(src, offset, (UA_ExpandedNodeId*)dst);
    case UA_TYPES_LOCALIZEDTEXT:
        return UA_LocalizedText_decodeBinary(src, offset, (UA_LocalizedText*)dst);
    case UA_TYPES_EXTENSIONOBJECT:
        return UA_ExtensionObject_decodeBinary(src, offset, (UA_ExtensionObject*)dst);
    case UA_TYPES_DATAVALUE:
        return UA_DataValue_decodeBinary(src, offset, (UA_DataValue*)dst);
    case UA_TYPES_VARIANT:
        return UA_Variant_decodeBinary(src, offset, (UA_Variant*)dst);
    case UA_TYPES_DIAGNOSTICINFO:
        return UA_DiagnosticInfo_decodeBinary(src, offset, (UA_DiagnosticInfo*)dst);
    }

    UA_init(dst, dataType);
    /* structured types */
    uintptr_t ptr = (uintptr_t) dst;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i = 0; i < membersSize && retval == UA_STATUSCODE_GOOD; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];
        if(!member->isArray) {
            ptr += member->padding;
            retval = UA_decodeBinary(src, offset, (void*)ptr, memberType);
            ptr += memberType->memSize;
        } else {
            ptr += (member->padding >> 3);
            UA_Int32 *noElements = (UA_Int32*) ptr;
            UA_Int32 tempNoElements = 0;
            retval = UA_Int32_decodeBinary(src, offset, &tempNoElements);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            retval = UA_Array_decodeBinary(src, offset, tempNoElements, (void**)ptr, memberType);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            *noElements = tempNoElements;
            ptr += sizeof(void*);
        }
    }
    if(retval != UA_STATUSCODE_GOOD)
        UA_deleteMembers(dst, dataType);
    return retval;
}

/******************/
/* CalcSizeBinary */
/******************/

static size_t UA_Array_calcSizeBinary(const void *p, UA_Int32 elements, const UA_DataType *dataType) {
    if (elements <= 0)
        return 4;
    size_t size = 4; // the array size encoding
    if (dataType->fixedSize) {
        size += elements * UA_calcSizeBinary(p, dataType);
        return size;
    }
    uintptr_t ptr = (uintptr_t) p;
    for (int i = 0; i < elements; i++) {
        size += UA_calcSizeBinary((void*) ptr, dataType);
        ptr += dataType->memSize;
    }
    return size;
}

static size_t UA_String_calcSizeBinary(UA_String const *string) {
    if (string->length > 0)
        return sizeof(UA_Int32) + string->length;
    else
        return sizeof(UA_Int32);
}

static size_t UA_NodeId_calcSizeBinary(UA_NodeId const *p) {
    size_t length = 0;
    switch (p->identifierType) {
    case UA_NODEIDTYPE_NUMERIC:
        if(p->identifier.numeric > UA_UINT16_MAX || p->namespaceIndex > UA_BYTE_MAX)
            length = sizeof(UA_Byte) + sizeof(UA_UInt16) + sizeof(UA_UInt32);
        else if(p->identifier.numeric > UA_BYTE_MAX || p->namespaceIndex > 0)
            length = 4; /* UA_NODEIDTYPE_FOURBYTE */
        else
            length = 2; /* UA_NODEIDTYPE_TWOBYTE*/
        break;
    case UA_NODEIDTYPE_STRING:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + UA_String_calcSizeBinary(&p->identifier.string);
        break;
    case UA_NODEIDTYPE_GUID:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) + 128;
        break;
    case UA_NODEIDTYPE_BYTESTRING:
        length = sizeof(UA_Byte) + sizeof(UA_UInt16) +
            UA_String_calcSizeBinary((const UA_String*)&p->identifier.byteString);
        break;
    default:
        UA_assert(UA_FALSE); // this must never happen
        break;
    }
    return length;
}

static size_t UA_ExpandedNodeId_calcSizeBinary(UA_ExpandedNodeId const *p) {
    size_t length = UA_NodeId_calcSizeBinary(&p->nodeId);
    if(p->namespaceUri.length > 0)
        length += UA_String_calcSizeBinary(&p->namespaceUri);
    if(p->serverIndex > 0)
        length += sizeof(UA_UInt32);
    return length;
}


static size_t UA_QualifiedName_calcSizeBinary(UA_QualifiedName const *p) {
    size_t length = sizeof(UA_UInt16); //qualifiedName->namespaceIndex
    // length += sizeof(UA_UInt16); //qualifiedName->reserved
    length += UA_String_calcSizeBinary(&p->name); //qualifiedName->name
    return length;
}

static size_t UA_LocalizedText_calcSizeBinary(UA_LocalizedText const *p) {
    size_t length = 1; // for encodingMask
    if(p->locale.data != NULL)
        length += UA_String_calcSizeBinary(&p->locale);
    if(p->text.data != NULL)
        length += UA_String_calcSizeBinary(&p->text);
    return length;
}

static size_t UA_ExtensionObject_calcSizeBinary(UA_ExtensionObject const *p) {
    size_t length = UA_NodeId_calcSizeBinary(&p->typeId);
    length += 1; // encoding
    switch (p->encoding) {
    case UA_EXTENSIONOBJECT_ENCODINGMASK_NOBODYISENCODED:
        break;
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING:
    case UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISXML:
        length += UA_String_calcSizeBinary((const UA_String*)&p->body);
        break;
    default:
        UA_assert(UA_FALSE);
    }
    return length;
}

static size_t UA_Variant_calcSizeBinary(UA_Variant const *p) {
    UA_Boolean isArray = p->arrayLength != -1 || !p->data; // a single element is not an array
    UA_Boolean hasDimensions = isArray && p->arrayDimensions != NULL;
    UA_Boolean isBuiltin = p->type->namespaceZero && UA_IS_BUILTIN(p->type->typeIndex);
    size_t length = sizeof(UA_Byte); //p->encodingMask
    UA_Int32 arrayLength = p->arrayLength;
    if(!isArray) {
        arrayLength = 1;
        length += UA_calcSizeBinary(p->data, p->type);
    } else
        length += UA_Array_calcSizeBinary(p->data, arrayLength, p->type);

    // if the type is not builtin, we encode it as an extensionobject
    if(!isBuiltin) {
        if(arrayLength > 0)
            length += 9 * arrayLength; // overhead for extensionobjects: 4 byte nodeid + 1 byte
                                       // encoding + 4 byte bytestring length
    }
    if(hasDimensions)
        length += UA_Array_calcSizeBinary(p->arrayDimensions,
                                          p->arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);
    return length;
}

static size_t UA_DataValue_calcSizeBinary(UA_DataValue const *p) {
    size_t length = sizeof(UA_Byte);
    if(p->hasValue)
        length += UA_Variant_calcSizeBinary(&p->value);
    if(p->hasStatus)
        length += sizeof(UA_UInt32);
    if(p->hasSourceTimestamp)
        length += sizeof(UA_DateTime);
    if(p->hasSourcePicoseconds)
        length += sizeof(UA_Int16);
    if(p->hasServerTimestamp)
        length += sizeof(UA_DateTime);
    if(p->hasServerPicoseconds)
        length += sizeof(UA_Int16);
    return length;
}

static size_t UA_DiagnosticInfo_calcSizeBinary(UA_DiagnosticInfo const *p) {
    size_t length = sizeof(UA_Byte);
    if(p->hasSymbolicId)
        length += sizeof(UA_Int32);
    if(p->hasNamespaceUri)
        length += sizeof(UA_Int32);
    if(p->hasLocalizedText)
        length += sizeof(UA_Int32);
    if(p->hasLocale)
        length += sizeof(UA_Int32);
    if(p->hasAdditionalInfo)
        length += UA_String_calcSizeBinary(&p->additionalInfo);
    if(p->hasInnerStatusCode)
        length += sizeof(UA_StatusCode);
    if(p->hasInnerDiagnosticInfo)
        length += UA_DiagnosticInfo_calcSizeBinary(p->innerDiagnosticInfo);
    return length;
}

size_t UA_calcSizeBinary(const void *p, const UA_DataType *dataType) {
    /* builtin types */
    switch(dataType->typeIndex + (0x8000 * !dataType->namespaceZero)) {
    case UA_TYPES_BOOLEAN:
    case UA_TYPES_SBYTE:
    case UA_TYPES_BYTE:
        return 1;
    case UA_TYPES_INT16:
    case UA_TYPES_UINT16:
        return 2;
    case UA_TYPES_INT32:
    case UA_TYPES_UINT32:
    case UA_TYPES_STATUSCODE:
    case UA_TYPES_FLOAT:
        return 4;
    case UA_TYPES_INT64:
    case UA_TYPES_UINT64:
    case UA_TYPES_DOUBLE:
    case UA_TYPES_DATETIME:
        return 8;
    case UA_TYPES_GUID:
        return 16;
    case UA_TYPES_NODEID:
        return UA_NodeId_calcSizeBinary((const UA_NodeId*)p);
    case UA_TYPES_EXPANDEDNODEID:
        return UA_ExpandedNodeId_calcSizeBinary((const UA_ExpandedNodeId*)p);
    case UA_TYPES_LOCALIZEDTEXT:
        return UA_LocalizedText_calcSizeBinary((const UA_LocalizedText*)p);
    case UA_TYPES_EXTENSIONOBJECT:
        return UA_ExtensionObject_calcSizeBinary((const UA_ExtensionObject*)p);
    case UA_TYPES_DATAVALUE:
        return UA_DataValue_calcSizeBinary((const UA_DataValue*)p);
    case UA_TYPES_VARIANT:
        return UA_Variant_calcSizeBinary((const UA_Variant*)p);
    case UA_TYPES_DIAGNOSTICINFO:
        return UA_DiagnosticInfo_calcSizeBinary((const UA_DiagnosticInfo*)p);
    }

    /* structured types */
    size_t size = 0;
    uintptr_t ptr = (uintptr_t) p;
    UA_Byte membersSize = dataType->membersSize;
    for(size_t i = 0; i < membersSize; i++) {
        const UA_DataTypeMember *member = &dataType->members[i];
        const UA_DataType *memberType;
        if(member->namespaceZero)
            memberType = &UA_TYPES[member->memberTypeIndex];
        else
            memberType = &dataType[member->memberTypeIndex - dataType->typeIndex];
        if(!member->isArray) {
            ptr += member->padding;
            size += UA_calcSizeBinary((const void*)ptr, memberType);
            ptr += memberType->memSize;
        } else {
            ptr += (member->padding >> 3);
            const UA_Int32 elements = *((const UA_Int32*) ptr);
            ptr += sizeof(UA_Int32) + (member->padding & 0x07);
            size += UA_Array_calcSizeBinary(*(void * const *)ptr, elements, memberType);
            ptr += sizeof(void*);
            continue;
        }
    }
    return size;
}

/*********************************** amalgamated original file "C:/open62541/build/src_generated/ua_types_generated.c" ***********************************/

/**
* @file ua_types_generated.c
*
* @brief Autogenerated data types
*
* Generated from Opc.Ua.Types.bsd with script C:/open62541/tools/generate_datatypes.py
* on host opcua-PC by user opcua at 2015-11-18 11:21:45
*/


const UA_DataType *UA_TYPES = (UA_DataType[]){

/* UA_Boolean */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1}, .memSize = sizeof(UA_Boolean), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_BOOLEAN },

/* UA_SByte */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 2}, .memSize = sizeof(UA_SByte), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_SBYTE },

/* UA_Byte */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 3}, .memSize = sizeof(UA_Byte), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_BYTE },

/* UA_Int16 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 4}, .memSize = sizeof(UA_Int16), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_INT16 },

/* UA_UInt16 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 5}, .memSize = sizeof(UA_UInt16), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_UINT16 },

/* UA_Int32 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 6}, .memSize = sizeof(UA_Int32), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_INT32 },

/* UA_UInt32 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 7}, .memSize = sizeof(UA_UInt32), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_UINT32 },

/* UA_Int64 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 8}, .memSize = sizeof(UA_Int64), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_INT64 },

/* UA_UInt64 */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 9}, .memSize = sizeof(UA_UInt64), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_UINT64 },

/* UA_Float */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 10}, .memSize = sizeof(UA_Float), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_FLOAT },

/* UA_Double */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 11}, .memSize = sizeof(UA_Double), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_DOUBLE },

/* UA_String */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 12}, .memSize = sizeof(UA_String), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_STRING },

/* UA_DateTime */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 13}, .memSize = sizeof(UA_DateTime), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_DATETIME },

/* UA_Guid */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 14}, .memSize = sizeof(UA_Guid), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_FALSE, .membersSize = 0, .typeIndex = UA_TYPES_GUID },

/* UA_ByteString */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 15}, .memSize = sizeof(UA_ByteString), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_BYTESTRING },

/* UA_XmlElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 16}, .memSize = sizeof(UA_XmlElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_String, data) - sizeof(UA_Int32), .isArray = UA_TRUE }}, .typeIndex = UA_TYPES_XMLELEMENT },

/* UA_NodeId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 17}, .memSize = sizeof(UA_NodeId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 0, .typeIndex = UA_TYPES_NODEID },

/* UA_ExpandedNodeId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 18}, .memSize = sizeof(UA_ExpandedNodeId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 3, .members = {
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE },
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ExpandedNodeId, namespaceUri) - sizeof(UA_NodeId), .isArray = UA_FALSE },
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ExpandedNodeId, serverIndex) - offsetof(UA_ExpandedNodeId, namespaceUri) - sizeof(UA_String), .isArray = UA_FALSE }},
.typeIndex = UA_TYPES_EXPANDEDNODEID },

/* UA_StatusCode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 19}, .memSize = sizeof(UA_StatusCode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 0, .typeIndex = UA_TYPES_STATUSCODE },

/* UA_QualifiedName */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 20}, .memSize = sizeof(UA_QualifiedName), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 2, .members = {
	{.memberTypeIndex = UA_TYPES_UINT16, .namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE },
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QualifiedName, name)-sizeof(UA_UInt16), .isArray = UA_FALSE }},
.typeIndex = UA_TYPES_QUALIFIEDNAME },

/* UA_LocalizedText */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 21}, .memSize = sizeof(UA_LocalizedText), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 2, .members = {
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE },
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_LocalizedText, text)-sizeof(UA_String), .isArray = UA_FALSE }},
.typeIndex = UA_TYPES_LOCALIZEDTEXT },

/* UA_ExtensionObject */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 22}, .memSize = sizeof(UA_ExtensionObject), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 0, .typeIndex = UA_TYPES_EXTENSIONOBJECT },

/* UA_DataValue */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 23}, .memSize = sizeof(UA_DataValue), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 0, .typeIndex = UA_TYPES_DATAVALUE },

/* UA_Variant */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 24}, .memSize = sizeof(UA_Variant), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 0, .typeIndex = UA_TYPES_VARIANT },

/* UA_DiagnosticInfo */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 25}, .memSize = sizeof(UA_DiagnosticInfo), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .membersSize = 0, .typeIndex = UA_TYPES_DIAGNOSTICINFO },

/* UA_IdType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 256}, .memSize = sizeof(UA_IdType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_NodeClass */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 257}, .memSize = sizeof(UA_NodeClass), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReferenceNode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 285}, .memSize = sizeof(UA_ReferenceNode), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCENODE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, isInverse) - (offsetof(UA_ReferenceNode, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceNode, targetId) - (offsetof(UA_ReferenceNode, isInverse) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_Argument */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 296}, .memSize = sizeof(UA_Argument), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ARGUMENT, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_Argument, name) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_Argument, dataType) - (offsetof(UA_Argument, name) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_INT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_Argument, valueRank) - (offsetof(UA_Argument, dataType) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_Argument, arrayDimensionsSize) - (offsetof(UA_Argument, valueRank) + sizeof(UA_Int32))) << 3) + (offsetof(UA_Argument, arrayDimensions) - sizeof(UA_Int32) - offsetof(UA_Argument, arrayDimensionsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_Argument, description) - (offsetof(UA_Argument, arrayDimensions) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_ApplicationType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 307}, .memSize = sizeof(UA_ApplicationType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ApplicationDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 308}, .memSize = sizeof(UA_ApplicationDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, productUri) - (offsetof(UA_ApplicationDescription, applicationUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationName) - (offsetof(UA_ApplicationDescription, productUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, applicationType) - (offsetof(UA_ApplicationDescription, applicationName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, gatewayServerUri) - (offsetof(UA_ApplicationDescription, applicationType) + sizeof(UA_ApplicationType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ApplicationDescription, discoveryProfileUri) - (offsetof(UA_ApplicationDescription, gatewayServerUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ApplicationDescription, discoveryUrlsSize) - (offsetof(UA_ApplicationDescription, discoveryProfileUri) + sizeof(UA_String))) << 3) + (offsetof(UA_ApplicationDescription, discoveryUrls) - sizeof(UA_Int32) - offsetof(UA_ApplicationDescription, discoveryUrlsSize)), .isArray = UA_TRUE }, }},

/* UA_RequestHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 389}, .memSize = sizeof(UA_RequestHeader), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REQUESTHEADER, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, authenticationToken) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, timestamp) - (offsetof(UA_RequestHeader, authenticationToken) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, requestHandle) - (offsetof(UA_RequestHeader, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, returnDiagnostics) - (offsetof(UA_RequestHeader, requestHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, auditEntryId) - (offsetof(UA_RequestHeader, returnDiagnostics) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, timeoutHint) - (offsetof(UA_RequestHeader, auditEntryId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RequestHeader, additionalHeader) - (offsetof(UA_RequestHeader, timeoutHint) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ResponseHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 392}, .memSize = sizeof(UA_ResponseHeader), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RESPONSEHEADER, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, timestamp) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, requestHandle) - (offsetof(UA_ResponseHeader, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, serviceResult) - (offsetof(UA_ResponseHeader, requestHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, serviceDiagnostics) - (offsetof(UA_ResponseHeader, serviceResult) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ResponseHeader, stringTableSize) - (offsetof(UA_ResponseHeader, serviceDiagnostics) + sizeof(UA_DiagnosticInfo))) << 3) + (offsetof(UA_ResponseHeader, stringTable) - sizeof(UA_Int32) - offsetof(UA_ResponseHeader, stringTableSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ResponseHeader, additionalHeader) - (offsetof(UA_ResponseHeader, stringTable) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_ServiceFault */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 395}, .memSize = sizeof(UA_ServiceFault), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SERVICEFAULT, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServiceFault, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_FindServersRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 420}, .memSize = sizeof(UA_FindServersRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_FINDSERVERSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersRequest, endpointUrl) - (offsetof(UA_FindServersRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersRequest, localeIdsSize) - (offsetof(UA_FindServersRequest, endpointUrl) + sizeof(UA_String))) << 3) + (offsetof(UA_FindServersRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_FindServersRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersRequest, serverUrisSize) - (offsetof(UA_FindServersRequest, localeIds) + sizeof(void*))) << 3) + (offsetof(UA_FindServersRequest, serverUris) - sizeof(UA_Int32) - offsetof(UA_FindServersRequest, serverUrisSize)), .isArray = UA_TRUE }, }},

/* UA_FindServersResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 423}, .memSize = sizeof(UA_FindServersResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_FINDSERVERSRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_FindServersResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_FindServersResponse, serversSize) - (offsetof(UA_FindServersResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_FindServersResponse, servers) - sizeof(UA_Int32) - offsetof(UA_FindServersResponse, serversSize)), .isArray = UA_TRUE }, }},

/* UA_MessageSecurityMode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 302}, .memSize = sizeof(UA_MessageSecurityMode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UserTokenType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 303}, .memSize = sizeof(UA_UserTokenType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_UserTokenPolicy */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 304}, .memSize = sizeof(UA_UserTokenPolicy), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERTOKENPOLICY, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, policyId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_USERTOKENTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, tokenType) - (offsetof(UA_UserTokenPolicy, policyId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, issuedTokenType) - (offsetof(UA_UserTokenPolicy, tokenType) + sizeof(UA_UserTokenType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, issuerEndpointUrl) - (offsetof(UA_UserTokenPolicy, issuedTokenType) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserTokenPolicy, securityPolicyUri) - (offsetof(UA_UserTokenPolicy, issuerEndpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_EndpointDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 312}, .memSize = sizeof(UA_EndpointDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .membersSize = 8,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, endpointUrl) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, server) - (offsetof(UA_EndpointDescription, endpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, serverCertificate) - (offsetof(UA_EndpointDescription, server) + sizeof(UA_ApplicationDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MESSAGESECURITYMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityMode) - (offsetof(UA_EndpointDescription, serverCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityPolicyUri) - (offsetof(UA_EndpointDescription, securityMode) + sizeof(UA_MessageSecurityMode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_USERTOKENPOLICY, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_EndpointDescription, userIdentityTokensSize) - (offsetof(UA_EndpointDescription, securityPolicyUri) + sizeof(UA_String))) << 3) + (offsetof(UA_EndpointDescription, userIdentityTokens) - sizeof(UA_Int32) - offsetof(UA_EndpointDescription, userIdentityTokensSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, transportProfileUri) - (offsetof(UA_EndpointDescription, userIdentityTokens) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_EndpointDescription, securityLevel) - (offsetof(UA_EndpointDescription, transportProfileUri) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_GetEndpointsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 426}, .memSize = sizeof(UA_GetEndpointsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_GETENDPOINTSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsRequest, endpointUrl) - (offsetof(UA_GetEndpointsRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsRequest, localeIdsSize) - (offsetof(UA_GetEndpointsRequest, endpointUrl) + sizeof(UA_String))) << 3) + (offsetof(UA_GetEndpointsRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsRequest, profileUrisSize) - (offsetof(UA_GetEndpointsRequest, localeIds) + sizeof(void*))) << 3) + (offsetof(UA_GetEndpointsRequest, profileUris) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsRequest, profileUrisSize)), .isArray = UA_TRUE }, }},

/* UA_GetEndpointsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 429}, .memSize = sizeof(UA_GetEndpointsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_GETENDPOINTSRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_GetEndpointsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_GetEndpointsResponse, endpointsSize) - (offsetof(UA_GetEndpointsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_GetEndpointsResponse, endpoints) - sizeof(UA_Int32) - offsetof(UA_GetEndpointsResponse, endpointsSize)), .isArray = UA_TRUE }, }},

/* UA_SecurityTokenRequestType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 315}, .memSize = sizeof(UA_SecurityTokenRequestType), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ChannelSecurityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 441}, .memSize = sizeof(UA_ChannelSecurityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_ChannelSecurityToken) == 20, .typeIndex = UA_TYPES_CHANNELSECURITYTOKEN, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, channelId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, tokenId) - (offsetof(UA_ChannelSecurityToken, channelId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, createdAt) - (offsetof(UA_ChannelSecurityToken, tokenId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ChannelSecurityToken, revisedLifetime) - (offsetof(UA_ChannelSecurityToken, createdAt) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, }},

/* UA_OpenSecureChannelRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 444}, .memSize = sizeof(UA_OpenSecureChannelRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OPENSECURECHANNELREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, clientProtocolVersion) - (offsetof(UA_OpenSecureChannelRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SECURITYTOKENREQUESTTYPE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestType) - (offsetof(UA_OpenSecureChannelRequest, clientProtocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MESSAGESECURITYMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, securityMode) - (offsetof(UA_OpenSecureChannelRequest, requestType) + sizeof(UA_SecurityTokenRequestType)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, clientNonce) - (offsetof(UA_OpenSecureChannelRequest, securityMode) + sizeof(UA_MessageSecurityMode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelRequest, requestedLifetime) - (offsetof(UA_OpenSecureChannelRequest, clientNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_OpenSecureChannelResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 447}, .memSize = sizeof(UA_OpenSecureChannelResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OPENSECURECHANNELRESPONSE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, serverProtocolVersion) - (offsetof(UA_OpenSecureChannelResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_CHANNELSECURITYTOKEN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, securityToken) - (offsetof(UA_OpenSecureChannelResponse, serverProtocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_OpenSecureChannelResponse, serverNonce) - (offsetof(UA_OpenSecureChannelResponse, securityToken) + sizeof(UA_ChannelSecurityToken)), .isArray = UA_FALSE }, }},

/* UA_CloseSecureChannelRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 450}, .memSize = sizeof(UA_CloseSecureChannelRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESECURECHANNELREQUEST, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSecureChannelRequest, requestHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_CloseSecureChannelResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 453}, .memSize = sizeof(UA_CloseSecureChannelResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESECURECHANNELRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSecureChannelResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_SignedSoftwareCertificate */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 344}, .memSize = sizeof(UA_SignedSoftwareCertificate), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignedSoftwareCertificate, certificateData) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignedSoftwareCertificate, signature) - (offsetof(UA_SignedSoftwareCertificate, certificateData) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_SignatureData */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 456}, .memSize = sizeof(UA_SignatureData), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SIGNATUREDATA, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignatureData, algorithm) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SignatureData, signature) - (offsetof(UA_SignatureData, algorithm) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_CreateSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 459}, .memSize = sizeof(UA_CreateSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESESSIONREQUEST, .membersSize = 9,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_APPLICATIONDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientDescription) - (offsetof(UA_CreateSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, serverUri) - (offsetof(UA_CreateSessionRequest, clientDescription) + sizeof(UA_ApplicationDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, endpointUrl) - (offsetof(UA_CreateSessionRequest, serverUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, sessionName) - (offsetof(UA_CreateSessionRequest, endpointUrl) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientNonce) - (offsetof(UA_CreateSessionRequest, sessionName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, clientCertificate) - (offsetof(UA_CreateSessionRequest, clientNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, requestedSessionTimeout) - (offsetof(UA_CreateSessionRequest, clientCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionRequest, maxResponseMessageSize) - (offsetof(UA_CreateSessionRequest, requestedSessionTimeout) + sizeof(UA_Double)), .isArray = UA_FALSE }, }},

/* UA_CreateSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 462}, .memSize = sizeof(UA_CreateSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESESSIONRESPONSE, .membersSize = 10,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, sessionId) - (offsetof(UA_CreateSessionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, authenticationToken) - (offsetof(UA_CreateSessionResponse, sessionId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, revisedSessionTimeout) - (offsetof(UA_CreateSessionResponse, authenticationToken) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverNonce) - (offsetof(UA_CreateSessionResponse, revisedSessionTimeout) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverCertificate) - (offsetof(UA_CreateSessionResponse, serverNonce) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ENDPOINTDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateSessionResponse, serverEndpointsSize) - (offsetof(UA_CreateSessionResponse, serverCertificate) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_CreateSessionResponse, serverEndpoints) - sizeof(UA_Int32) - offsetof(UA_CreateSessionResponse, serverEndpointsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateSessionResponse, serverSoftwareCertificatesSize) - (offsetof(UA_CreateSessionResponse, serverEndpoints) + sizeof(void*))) << 3) + (offsetof(UA_CreateSessionResponse, serverSoftwareCertificates) - sizeof(UA_Int32) - offsetof(UA_CreateSessionResponse, serverSoftwareCertificatesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, serverSignature) - (offsetof(UA_CreateSessionResponse, serverSoftwareCertificates) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSessionResponse, maxRequestMessageSize) - (offsetof(UA_CreateSessionResponse, serverSignature) + sizeof(UA_SignatureData)), .isArray = UA_FALSE }, }},

/* UA_UserIdentityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 316}, .memSize = sizeof(UA_UserIdentityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERIDENTITYTOKEN, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserIdentityToken, policyId) - 0, .isArray = UA_FALSE }, }},

/* UA_AnonymousIdentityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 319}, .memSize = sizeof(UA_AnonymousIdentityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ANONYMOUSIDENTITYTOKEN, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AnonymousIdentityToken, policyId) - 0, .isArray = UA_FALSE }, }},

/* UA_UserNameIdentityToken */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 322}, .memSize = sizeof(UA_UserNameIdentityToken), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_USERNAMEIDENTITYTOKEN, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, policyId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, userName) - (offsetof(UA_UserNameIdentityToken, policyId) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, password) - (offsetof(UA_UserNameIdentityToken, userName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UserNameIdentityToken, encryptionAlgorithm) - (offsetof(UA_UserNameIdentityToken, password) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_ActivateSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 465}, .memSize = sizeof(UA_ActivateSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ACTIVATESESSIONREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, clientSignature) - (offsetof(UA_ActivateSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNEDSOFTWARECERTIFICATE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionRequest, clientSoftwareCertificatesSize) - (offsetof(UA_ActivateSessionRequest, clientSignature) + sizeof(UA_SignatureData))) << 3) + (offsetof(UA_ActivateSessionRequest, clientSoftwareCertificates) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionRequest, clientSoftwareCertificatesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionRequest, localeIdsSize) - (offsetof(UA_ActivateSessionRequest, clientSoftwareCertificates) + sizeof(void*))) << 3) + (offsetof(UA_ActivateSessionRequest, localeIds) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionRequest, localeIdsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, userIdentityToken) - (offsetof(UA_ActivateSessionRequest, localeIds) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SIGNATUREDATA, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionRequest, userTokenSignature) - (offsetof(UA_ActivateSessionRequest, userIdentityToken) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, }},

/* UA_ActivateSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 468}, .memSize = sizeof(UA_ActivateSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ACTIVATESESSIONRESPONSE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ActivateSessionResponse, serverNonce) - (offsetof(UA_ActivateSessionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionResponse, resultsSize) - (offsetof(UA_ActivateSessionResponse, serverNonce) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_ActivateSessionResponse, results) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ActivateSessionResponse, diagnosticInfosSize) - (offsetof(UA_ActivateSessionResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_ActivateSessionResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ActivateSessionResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CloseSessionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 471}, .memSize = sizeof(UA_CloseSessionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESESSIONREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionRequest, deleteSubscriptions) - (offsetof(UA_CloseSessionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, }},

/* UA_CloseSessionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 474}, .memSize = sizeof(UA_CloseSessionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CLOSESESSIONRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CloseSessionResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_NodeAttributesMask */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 348}, .memSize = sizeof(UA_NodeAttributesMask), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_NodeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 349}, .memSize = sizeof(UA_NodeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NODEATTRIBUTES, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, displayName) - (offsetof(UA_NodeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, description) - (offsetof(UA_NodeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, writeMask) - (offsetof(UA_NodeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeAttributes, userWriteMask) - (offsetof(UA_NodeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ObjectAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 352}, .memSize = sizeof(UA_ObjectAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OBJECTATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, displayName) - (offsetof(UA_ObjectAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, description) - (offsetof(UA_ObjectAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, writeMask) - (offsetof(UA_ObjectAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, userWriteMask) - (offsetof(UA_ObjectAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectAttributes, eventNotifier) - (offsetof(UA_ObjectAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_VariableAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 355}, .memSize = sizeof(UA_VariableAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VARIABLEATTRIBUTES, .membersSize = 13,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, displayName) - (offsetof(UA_VariableAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, description) - (offsetof(UA_VariableAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, writeMask) - (offsetof(UA_VariableAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, userWriteMask) - (offsetof(UA_VariableAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, value) - (offsetof(UA_VariableAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, dataType) - (offsetof(UA_VariableAttributes, value) + sizeof(UA_Variant)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_INT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, valueRank) - (offsetof(UA_VariableAttributes, dataType) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_VariableAttributes, arrayDimensionsSize) - (offsetof(UA_VariableAttributes, valueRank) + sizeof(UA_Int32))) << 3) + (offsetof(UA_VariableAttributes, arrayDimensions) - sizeof(UA_Int32) - offsetof(UA_VariableAttributes, arrayDimensionsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, accessLevel) - (offsetof(UA_VariableAttributes, arrayDimensions) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, userAccessLevel) - (offsetof(UA_VariableAttributes, accessLevel) + sizeof(UA_Byte)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, minimumSamplingInterval) - (offsetof(UA_VariableAttributes, userAccessLevel) + sizeof(UA_Byte)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableAttributes, historizing) - (offsetof(UA_VariableAttributes, minimumSamplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, }},

/* UA_MethodAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 358}, .memSize = sizeof(UA_MethodAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_METHODATTRIBUTES, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, displayName) - (offsetof(UA_MethodAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, description) - (offsetof(UA_MethodAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, writeMask) - (offsetof(UA_MethodAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, userWriteMask) - (offsetof(UA_MethodAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, executable) - (offsetof(UA_MethodAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MethodAttributes, userExecutable) - (offsetof(UA_MethodAttributes, executable) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_ObjectTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 361}, .memSize = sizeof(UA_ObjectTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_OBJECTTYPEATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, displayName) - (offsetof(UA_ObjectTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, description) - (offsetof(UA_ObjectTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, writeMask) - (offsetof(UA_ObjectTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, userWriteMask) - (offsetof(UA_ObjectTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ObjectTypeAttributes, isAbstract) - (offsetof(UA_ObjectTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_VariableTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 364}, .memSize = sizeof(UA_VariableTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VARIABLETYPEATTRIBUTES, .membersSize = 10,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, displayName) - (offsetof(UA_VariableTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, description) - (offsetof(UA_VariableTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, writeMask) - (offsetof(UA_VariableTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, userWriteMask) - (offsetof(UA_VariableTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, value) - (offsetof(UA_VariableTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, dataType) - (offsetof(UA_VariableTypeAttributes, value) + sizeof(UA_Variant)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_INT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, valueRank) - (offsetof(UA_VariableTypeAttributes, dataType) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_VariableTypeAttributes, arrayDimensionsSize) - (offsetof(UA_VariableTypeAttributes, valueRank) + sizeof(UA_Int32))) << 3) + (offsetof(UA_VariableTypeAttributes, arrayDimensions) - sizeof(UA_Int32) - offsetof(UA_VariableTypeAttributes, arrayDimensionsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_VariableTypeAttributes, isAbstract) - (offsetof(UA_VariableTypeAttributes, arrayDimensions) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_ReferenceTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 367}, .memSize = sizeof(UA_ReferenceTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCETYPEATTRIBUTES, .membersSize = 8,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, displayName) - (offsetof(UA_ReferenceTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, description) - (offsetof(UA_ReferenceTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, writeMask) - (offsetof(UA_ReferenceTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, userWriteMask) - (offsetof(UA_ReferenceTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, isAbstract) - (offsetof(UA_ReferenceTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, symmetric) - (offsetof(UA_ReferenceTypeAttributes, isAbstract) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceTypeAttributes, inverseName) - (offsetof(UA_ReferenceTypeAttributes, symmetric) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_DataTypeAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 370}, .memSize = sizeof(UA_DataTypeAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DATATYPEATTRIBUTES, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DataTypeAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DataTypeAttributes, displayName) - (offsetof(UA_DataTypeAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DataTypeAttributes, description) - (offsetof(UA_DataTypeAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DataTypeAttributes, writeMask) - (offsetof(UA_DataTypeAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DataTypeAttributes, userWriteMask) - (offsetof(UA_DataTypeAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DataTypeAttributes, isAbstract) - (offsetof(UA_DataTypeAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_ViewAttributes */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 373}, .memSize = sizeof(UA_ViewAttributes), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VIEWATTRIBUTES, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, specifiedAttributes) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, displayName) - (offsetof(UA_ViewAttributes, specifiedAttributes) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, description) - (offsetof(UA_ViewAttributes, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, writeMask) - (offsetof(UA_ViewAttributes, description) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, userWriteMask) - (offsetof(UA_ViewAttributes, writeMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, containsNoLoops) - (offsetof(UA_ViewAttributes, userWriteMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewAttributes, eventNotifier) - (offsetof(UA_ViewAttributes, containsNoLoops) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_AddNodesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 376}, .memSize = sizeof(UA_AddNodesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESITEM, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, parentNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, referenceTypeId) - (offsetof(UA_AddNodesItem, parentNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, requestedNewNodeId) - (offsetof(UA_AddNodesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, browseName) - (offsetof(UA_AddNodesItem, requestedNewNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, nodeClass) - (offsetof(UA_AddNodesItem, browseName) + sizeof(UA_QualifiedName)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, nodeAttributes) - (offsetof(UA_AddNodesItem, nodeClass) + sizeof(UA_NodeClass)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesItem, typeDefinition) - (offsetof(UA_AddNodesItem, nodeAttributes) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, }},

/* UA_AddNodesResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 483}, .memSize = sizeof(UA_AddNodesResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResult, addedNodeId) - (offsetof(UA_AddNodesResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, }},

/* UA_AddNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 486}, .memSize = sizeof(UA_AddNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDNODESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesRequest, nodesToAddSize) - (offsetof(UA_AddNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_AddNodesRequest, nodesToAdd) - sizeof(UA_Int32) - offsetof(UA_AddNodesRequest, nodesToAddSize)), .isArray = UA_TRUE }, }},

/* UA_AddNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 489}, .memSize = sizeof(UA_AddNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDNODESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDNODESRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesResponse, resultsSize) - (offsetof(UA_AddNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_AddNodesResponse, results) - sizeof(UA_Int32) - offsetof(UA_AddNodesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddNodesResponse, diagnosticInfosSize) - (offsetof(UA_AddNodesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_AddNodesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_AddNodesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_AddReferencesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 379}, .memSize = sizeof(UA_AddReferencesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESITEM, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, sourceNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, referenceTypeId) - (offsetof(UA_AddReferencesItem, sourceNodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, isForward) - (offsetof(UA_AddReferencesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetServerUri) - (offsetof(UA_AddReferencesItem, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetNodeId) - (offsetof(UA_AddReferencesItem, targetServerUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesItem, targetNodeClass) - (offsetof(UA_AddReferencesItem, targetNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_AddReferencesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 492}, .memSize = sizeof(UA_AddReferencesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_ADDREFERENCESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesRequest, referencesToAddSize) - (offsetof(UA_AddReferencesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_AddReferencesRequest, referencesToAdd) - sizeof(UA_Int32) - offsetof(UA_AddReferencesRequest, referencesToAddSize)), .isArray = UA_TRUE }, }},

/* UA_AddReferencesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 495}, .memSize = sizeof(UA_AddReferencesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_ADDREFERENCESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AddReferencesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesResponse, resultsSize) - (offsetof(UA_AddReferencesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_AddReferencesResponse, results) - sizeof(UA_Int32) - offsetof(UA_AddReferencesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_AddReferencesResponse, diagnosticInfosSize) - (offsetof(UA_AddReferencesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_AddReferencesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_AddReferencesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteNodesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 382}, .memSize = sizeof(UA_DeleteNodesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESITEM, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesItem, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesItem, deleteTargetReferences) - (offsetof(UA_DeleteNodesItem, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, }},

/* UA_DeleteNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 498}, .memSize = sizeof(UA_DeleteNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DELETENODESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesRequest, nodesToDeleteSize) - (offsetof(UA_DeleteNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteNodesRequest, nodesToDelete) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesRequest, nodesToDeleteSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 501}, .memSize = sizeof(UA_DeleteNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETENODESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesResponse, resultsSize) - (offsetof(UA_DeleteNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteNodesResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteNodesResponse, diagnosticInfosSize) - (offsetof(UA_DeleteNodesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteNodesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteNodesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteReferencesItem */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 385}, .memSize = sizeof(UA_DeleteReferencesItem), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESITEM, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, sourceNodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, referenceTypeId) - (offsetof(UA_DeleteReferencesItem, sourceNodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, isForward) - (offsetof(UA_DeleteReferencesItem, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, targetNodeId) - (offsetof(UA_DeleteReferencesItem, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesItem, deleteBidirectional) - (offsetof(UA_DeleteReferencesItem, targetNodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_DeleteReferencesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 504}, .memSize = sizeof(UA_DeleteReferencesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DELETEREFERENCESITEM, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesRequest, referencesToDeleteSize) - (offsetof(UA_DeleteReferencesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteReferencesRequest, referencesToDelete) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesRequest, referencesToDeleteSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteReferencesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 507}, .memSize = sizeof(UA_DeleteReferencesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETEREFERENCESRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteReferencesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesResponse, resultsSize) - (offsetof(UA_DeleteReferencesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteReferencesResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteReferencesResponse, diagnosticInfosSize) - (offsetof(UA_DeleteReferencesResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteReferencesResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteReferencesResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseDirection */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 510}, .memSize = sizeof(UA_BrowseDirection), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ViewDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 511}, .memSize = sizeof(UA_ViewDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_VIEWDESCRIPTION, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, viewId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, timestamp) - (offsetof(UA_ViewDescription, viewId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ViewDescription, viewVersion) - (offsetof(UA_ViewDescription, timestamp) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, }},

/* UA_BrowseDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 514}, .memSize = sizeof(UA_BrowseDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEDESCRIPTION, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEDIRECTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, browseDirection) - (offsetof(UA_BrowseDescription, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, referenceTypeId) - (offsetof(UA_BrowseDescription, browseDirection) + sizeof(UA_BrowseDirection)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, includeSubtypes) - (offsetof(UA_BrowseDescription, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, nodeClassMask) - (offsetof(UA_BrowseDescription, includeSubtypes) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseDescription, resultMask) - (offsetof(UA_BrowseDescription, nodeClassMask) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_BrowseResultMask */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 517}, .memSize = sizeof(UA_BrowseResultMask), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReferenceDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 518}, .memSize = sizeof(UA_ReferenceDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REFERENCEDESCRIPTION, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, isForward) - (offsetof(UA_ReferenceDescription, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, nodeId) - (offsetof(UA_ReferenceDescription, isForward) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, browseName) - (offsetof(UA_ReferenceDescription, nodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, displayName) - (offsetof(UA_ReferenceDescription, browseName) + sizeof(UA_QualifiedName)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODECLASS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, nodeClass) - (offsetof(UA_ReferenceDescription, displayName) + sizeof(UA_LocalizedText)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReferenceDescription, typeDefinition) - (offsetof(UA_ReferenceDescription, nodeClass) + sizeof(UA_NodeClass)), .isArray = UA_FALSE }, }},

/* UA_BrowseResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 522}, .memSize = sizeof(UA_BrowseResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSERESULT, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResult, continuationPoint) - (offsetof(UA_BrowseResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_REFERENCEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResult, referencesSize) - (offsetof(UA_BrowseResult, continuationPoint) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_BrowseResult, references) - sizeof(UA_Int32) - offsetof(UA_BrowseResult, referencesSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 525}, .memSize = sizeof(UA_BrowseRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VIEWDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, view) - (offsetof(UA_BrowseRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseRequest, requestedMaxReferencesPerNode) - (offsetof(UA_BrowseRequest, view) + sizeof(UA_ViewDescription)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseRequest, nodesToBrowseSize) - (offsetof(UA_BrowseRequest, requestedMaxReferencesPerNode) + sizeof(UA_UInt32))) << 3) + (offsetof(UA_BrowseRequest, nodesToBrowse) - sizeof(UA_Int32) - offsetof(UA_BrowseRequest, nodesToBrowseSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 528}, .memSize = sizeof(UA_BrowseResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResponse, resultsSize) - (offsetof(UA_BrowseResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_BrowseResponse, results) - sizeof(UA_Int32) - offsetof(UA_BrowseResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseResponse, diagnosticInfosSize) - (offsetof(UA_BrowseResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_BrowseResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_BrowseResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseNextRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 531}, .memSize = sizeof(UA_BrowseNextRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSENEXTREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseNextRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseNextRequest, releaseContinuationPoints) - (offsetof(UA_BrowseNextRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseNextRequest, continuationPointsSize) - (offsetof(UA_BrowseNextRequest, releaseContinuationPoints) + sizeof(UA_Boolean))) << 3) + (offsetof(UA_BrowseNextRequest, continuationPoints) - sizeof(UA_Int32) - offsetof(UA_BrowseNextRequest, continuationPointsSize)), .isArray = UA_TRUE }, }},

/* UA_BrowseNextResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 534}, .memSize = sizeof(UA_BrowseNextResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSENEXTRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowseNextResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseNextResponse, resultsSize) - (offsetof(UA_BrowseNextResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_BrowseNextResponse, results) - sizeof(UA_Int32) - offsetof(UA_BrowseNextResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowseNextResponse, diagnosticInfosSize) - (offsetof(UA_BrowseNextResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_BrowseNextResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_BrowseNextResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_RelativePathElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 537}, .memSize = sizeof(UA_RelativePathElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RELATIVEPATHELEMENT, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, referenceTypeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, isInverse) - (offsetof(UA_RelativePathElement, referenceTypeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, includeSubtypes) - (offsetof(UA_RelativePathElement, isInverse) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RelativePathElement, targetName) - (offsetof(UA_RelativePathElement, includeSubtypes) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_RelativePath */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 540}, .memSize = sizeof(UA_RelativePath), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_RELATIVEPATH, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RELATIVEPATHELEMENT, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_RelativePath, elements) - sizeof(UA_Int32) - offsetof(UA_RelativePath, elementsSize)), .isArray = UA_TRUE }, }},

/* UA_BrowsePath */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 543}, .memSize = sizeof(UA_BrowsePath), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATH, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePath, startingNode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_RELATIVEPATH, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePath, relativePath) - (offsetof(UA_BrowsePath, startingNode) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, }},

/* UA_BrowsePathTarget */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 546}, .memSize = sizeof(UA_BrowsePathTarget), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATHTARGET, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathTarget, targetId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathTarget, remainingPathIndex) - (offsetof(UA_BrowsePathTarget, targetId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, }},

/* UA_BrowsePathResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 549}, .memSize = sizeof(UA_BrowsePathResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BROWSEPATHRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BrowsePathResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATHTARGET, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_BrowsePathResult, targetsSize) - (offsetof(UA_BrowsePathResult, statusCode) + sizeof(UA_StatusCode))) << 3) + (offsetof(UA_BrowsePathResult, targets) - sizeof(UA_Int32) - offsetof(UA_BrowsePathResult, targetsSize)), .isArray = UA_TRUE }, }},

/* UA_TranslateBrowsePathsToNodeIdsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 552}, .memSize = sizeof(UA_TranslateBrowsePathsToNodeIdsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATH, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePathsSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePaths) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsRequest, browsePathsSize)), .isArray = UA_TRUE }, }},

/* UA_TranslateBrowsePathsToNodeIdsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 555}, .memSize = sizeof(UA_TranslateBrowsePathsToNodeIdsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BROWSEPATHRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, resultsSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, results) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfosSize) - (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_TranslateBrowsePathsToNodeIdsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_RegisterNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 558}, .memSize = sizeof(UA_RegisterNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REGISTERNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RegisterNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_RegisterNodesRequest, nodesToRegisterSize) - (offsetof(UA_RegisterNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_RegisterNodesRequest, nodesToRegister) - sizeof(UA_Int32) - offsetof(UA_RegisterNodesRequest, nodesToRegisterSize)), .isArray = UA_TRUE }, }},

/* UA_RegisterNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 561}, .memSize = sizeof(UA_RegisterNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_REGISTERNODESRESPONSE, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_RegisterNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_RegisterNodesResponse, registeredNodeIdsSize) - (offsetof(UA_RegisterNodesResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_RegisterNodesResponse, registeredNodeIds) - sizeof(UA_Int32) - offsetof(UA_RegisterNodesResponse, registeredNodeIdsSize)), .isArray = UA_TRUE }, }},

/* UA_UnregisterNodesRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 564}, .memSize = sizeof(UA_UnregisterNodesRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_UNREGISTERNODESREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UnregisterNodesRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_UnregisterNodesRequest, nodesToUnregisterSize) - (offsetof(UA_UnregisterNodesRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_UnregisterNodesRequest, nodesToUnregister) - sizeof(UA_Int32) - offsetof(UA_UnregisterNodesRequest, nodesToUnregisterSize)), .isArray = UA_TRUE }, }},

/* UA_UnregisterNodesResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 567}, .memSize = sizeof(UA_UnregisterNodesResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_UNREGISTERNODESRESPONSE, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_UnregisterNodesResponse, responseHeader) - 0, .isArray = UA_FALSE }, }},

/* UA_QueryDataDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 570}, .memSize = sizeof(UA_QueryDataDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_QUERYDATADESCRIPTION, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RELATIVEPATH, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryDataDescription, relativePath) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryDataDescription, attributeId) - (offsetof(UA_QueryDataDescription, relativePath) + sizeof(UA_RelativePath)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryDataDescription, indexRange) - (offsetof(UA_QueryDataDescription, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_NodeTypeDescription */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 573}, .memSize = sizeof(UA_NodeTypeDescription), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NODETYPEDESCRIPTION, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeTypeDescription, typeDefinitionNode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NodeTypeDescription, includeSubTypes) - (offsetof(UA_NodeTypeDescription, typeDefinitionNode) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUERYDATADESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_NodeTypeDescription, dataToReturnSize) - (offsetof(UA_NodeTypeDescription, includeSubTypes) + sizeof(UA_Boolean))) << 3) + (offsetof(UA_NodeTypeDescription, dataToReturn) - sizeof(UA_Int32) - offsetof(UA_NodeTypeDescription, dataToReturnSize)), .isArray = UA_TRUE }, }},

/* UA_FilterOperator */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 576}, .memSize = sizeof(UA_FilterOperator), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_QueryDataSet */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 577}, .memSize = sizeof(UA_QueryDataSet), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_QUERYDATASET, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryDataSet, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXPANDEDNODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryDataSet, typeDefinitionNode) - (offsetof(UA_QueryDataSet, nodeId) + sizeof(UA_ExpandedNodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_QueryDataSet, valuesSize) - (offsetof(UA_QueryDataSet, typeDefinitionNode) + sizeof(UA_ExpandedNodeId))) << 3) + (offsetof(UA_QueryDataSet, values) - sizeof(UA_Int32) - offsetof(UA_QueryDataSet, valuesSize)), .isArray = UA_TRUE }, }},

/* UA_ContentFilterElement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 583}, .memSize = sizeof(UA_ContentFilterElement), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CONTENTFILTERELEMENT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_FILTEROPERATOR, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ContentFilterElement, filterOperator) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ContentFilterElement, filterOperandsSize) - (offsetof(UA_ContentFilterElement, filterOperator) + sizeof(UA_FilterOperator))) << 3) + (offsetof(UA_ContentFilterElement, filterOperands) - sizeof(UA_Int32) - offsetof(UA_ContentFilterElement, filterOperandsSize)), .isArray = UA_TRUE }, }},

/* UA_ContentFilter */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 586}, .memSize = sizeof(UA_ContentFilter), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CONTENTFILTER, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_CONTENTFILTERELEMENT, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_ContentFilter, elements) - sizeof(UA_Int32) - offsetof(UA_ContentFilter, elementsSize)), .isArray = UA_TRUE }, }},

/* UA_ContentFilterElementResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 604}, .memSize = sizeof(UA_ContentFilterElementResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CONTENTFILTERELEMENTRESULT, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ContentFilterElementResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ContentFilterElementResult, operandStatusCodesSize) - (offsetof(UA_ContentFilterElementResult, statusCode) + sizeof(UA_StatusCode))) << 3) + (offsetof(UA_ContentFilterElementResult, operandStatusCodes) - sizeof(UA_Int32) - offsetof(UA_ContentFilterElementResult, operandStatusCodesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ContentFilterElementResult, operandDiagnosticInfosSize) - (offsetof(UA_ContentFilterElementResult, operandStatusCodes) + sizeof(void*))) << 3) + (offsetof(UA_ContentFilterElementResult, operandDiagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ContentFilterElementResult, operandDiagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_ContentFilterResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 607}, .memSize = sizeof(UA_ContentFilterResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CONTENTFILTERRESULT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_CONTENTFILTERELEMENTRESULT, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_ContentFilterResult, elementResults) - sizeof(UA_Int32) - offsetof(UA_ContentFilterResult, elementResultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ContentFilterResult, elementDiagnosticInfosSize) - (offsetof(UA_ContentFilterResult, elementResults) + sizeof(void*))) << 3) + (offsetof(UA_ContentFilterResult, elementDiagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ContentFilterResult, elementDiagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_ParsingResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 610}, .memSize = sizeof(UA_ParsingResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PARSINGRESULT, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ParsingResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ParsingResult, dataStatusCodesSize) - (offsetof(UA_ParsingResult, statusCode) + sizeof(UA_StatusCode))) << 3) + (offsetof(UA_ParsingResult, dataStatusCodes) - sizeof(UA_Int32) - offsetof(UA_ParsingResult, dataStatusCodesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ParsingResult, dataDiagnosticInfosSize) - (offsetof(UA_ParsingResult, dataStatusCodes) + sizeof(void*))) << 3) + (offsetof(UA_ParsingResult, dataDiagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ParsingResult, dataDiagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_QueryFirstRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 613}, .memSize = sizeof(UA_QueryFirstRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_QUERYFIRSTREQUEST, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VIEWDESCRIPTION, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstRequest, view) - (offsetof(UA_QueryFirstRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODETYPEDESCRIPTION, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_QueryFirstRequest, nodeTypesSize) - (offsetof(UA_QueryFirstRequest, view) + sizeof(UA_ViewDescription))) << 3) + (offsetof(UA_QueryFirstRequest, nodeTypes) - sizeof(UA_Int32) - offsetof(UA_QueryFirstRequest, nodeTypesSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_CONTENTFILTER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstRequest, filter) - (offsetof(UA_QueryFirstRequest, nodeTypes) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstRequest, maxDataSetsToReturn) - (offsetof(UA_QueryFirstRequest, filter) + sizeof(UA_ContentFilter)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstRequest, maxReferencesToReturn) - (offsetof(UA_QueryFirstRequest, maxDataSetsToReturn) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_QueryFirstResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 616}, .memSize = sizeof(UA_QueryFirstResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_QUERYFIRSTRESPONSE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUERYDATASET, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_QueryFirstResponse, queryDataSetsSize) - (offsetof(UA_QueryFirstResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_QueryFirstResponse, queryDataSets) - sizeof(UA_Int32) - offsetof(UA_QueryFirstResponse, queryDataSetsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstResponse, continuationPoint) - (offsetof(UA_QueryFirstResponse, queryDataSets) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_PARSINGRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_QueryFirstResponse, parsingResultsSize) - (offsetof(UA_QueryFirstResponse, continuationPoint) + sizeof(UA_ByteString))) << 3) + (offsetof(UA_QueryFirstResponse, parsingResults) - sizeof(UA_Int32) - offsetof(UA_QueryFirstResponse, parsingResultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_QueryFirstResponse, diagnosticInfosSize) - (offsetof(UA_QueryFirstResponse, parsingResults) + sizeof(void*))) << 3) + (offsetof(UA_QueryFirstResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_QueryFirstResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_CONTENTFILTERRESULT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryFirstResponse, filterResult) - (offsetof(UA_QueryFirstResponse, diagnosticInfos) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_QueryNextRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 619}, .memSize = sizeof(UA_QueryNextRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_QUERYNEXTREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryNextRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryNextRequest, releaseContinuationPoint) - (offsetof(UA_QueryNextRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryNextRequest, continuationPoint) - (offsetof(UA_QueryNextRequest, releaseContinuationPoint) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_QueryNextResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 622}, .memSize = sizeof(UA_QueryNextResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_QUERYNEXTRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryNextResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUERYDATASET, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_QueryNextResponse, queryDataSetsSize) - (offsetof(UA_QueryNextResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_QueryNextResponse, queryDataSets) - sizeof(UA_Int32) - offsetof(UA_QueryNextResponse, queryDataSetsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_QueryNextResponse, revisedContinuationPoint) - (offsetof(UA_QueryNextResponse, queryDataSets) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_TimestampsToReturn */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 625}, .memSize = sizeof(UA_TimestampsToReturn), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ReadValueId */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 626}, .memSize = sizeof(UA_ReadValueId), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READVALUEID, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, attributeId) - (offsetof(UA_ReadValueId, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, indexRange) - (offsetof(UA_ReadValueId, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_QUALIFIEDNAME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadValueId, dataEncoding) - (offsetof(UA_ReadValueId, indexRange) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_ReadRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 629}, .memSize = sizeof(UA_ReadRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, maxAge) - (offsetof(UA_ReadRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_TIMESTAMPSTORETURN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadRequest, timestampsToReturn) - (offsetof(UA_ReadRequest, maxAge) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_READVALUEID, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadRequest, nodesToReadSize) - (offsetof(UA_ReadRequest, timestampsToReturn) + sizeof(UA_TimestampsToReturn))) << 3) + (offsetof(UA_ReadRequest, nodesToRead) - sizeof(UA_Int32) - offsetof(UA_ReadRequest, nodesToReadSize)), .isArray = UA_TRUE }, }},

/* UA_ReadResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 632}, .memSize = sizeof(UA_ReadResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_READRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ReadResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATAVALUE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadResponse, resultsSize) - (offsetof(UA_ReadResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_ReadResponse, results) - sizeof(UA_Int32) - offsetof(UA_ReadResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_ReadResponse, diagnosticInfosSize) - (offsetof(UA_ReadResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_ReadResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_ReadResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_WriteValue */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 668}, .memSize = sizeof(UA_WriteValue), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITEVALUE, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, nodeId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, attributeId) - (offsetof(UA_WriteValue, nodeId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, indexRange) - (offsetof(UA_WriteValue, attributeId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATAVALUE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteValue, value) - (offsetof(UA_WriteValue, indexRange) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_WriteRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 671}, .memSize = sizeof(UA_WriteRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITEREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_WRITEVALUE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteRequest, nodesToWriteSize) - (offsetof(UA_WriteRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_WriteRequest, nodesToWrite) - sizeof(UA_Int32) - offsetof(UA_WriteRequest, nodesToWriteSize)), .isArray = UA_TRUE }, }},

/* UA_WriteResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 674}, .memSize = sizeof(UA_WriteResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_WRITERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_WriteResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteResponse, resultsSize) - (offsetof(UA_WriteResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_WriteResponse, results) - sizeof(UA_Int32) - offsetof(UA_WriteResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_WriteResponse, diagnosticInfosSize) - (offsetof(UA_WriteResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_WriteResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_WriteResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CallMethodRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 704}, .memSize = sizeof(UA_CallMethodRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CALLMETHODREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CallMethodRequest, objectId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NODEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CallMethodRequest, methodId) - (offsetof(UA_CallMethodRequest, objectId) + sizeof(UA_NodeId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallMethodRequest, inputArgumentsSize) - (offsetof(UA_CallMethodRequest, methodId) + sizeof(UA_NodeId))) << 3) + (offsetof(UA_CallMethodRequest, inputArguments) - sizeof(UA_Int32) - offsetof(UA_CallMethodRequest, inputArgumentsSize)), .isArray = UA_TRUE }, }},

/* UA_CallMethodResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 707}, .memSize = sizeof(UA_CallMethodResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CALLMETHODRESULT, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CallMethodResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallMethodResult, inputArgumentResultsSize) - (offsetof(UA_CallMethodResult, statusCode) + sizeof(UA_StatusCode))) << 3) + (offsetof(UA_CallMethodResult, inputArgumentResults) - sizeof(UA_Int32) - offsetof(UA_CallMethodResult, inputArgumentResultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallMethodResult, inputArgumentDiagnosticInfosSize) - (offsetof(UA_CallMethodResult, inputArgumentResults) + sizeof(void*))) << 3) + (offsetof(UA_CallMethodResult, inputArgumentDiagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_CallMethodResult, inputArgumentDiagnosticInfosSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_VARIANT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallMethodResult, outputArgumentsSize) - (offsetof(UA_CallMethodResult, inputArgumentDiagnosticInfos) + sizeof(void*))) << 3) + (offsetof(UA_CallMethodResult, outputArguments) - sizeof(UA_Int32) - offsetof(UA_CallMethodResult, outputArgumentsSize)), .isArray = UA_TRUE }, }},

/* UA_CallRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 710}, .memSize = sizeof(UA_CallRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CALLREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CallRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_CALLMETHODREQUEST, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallRequest, methodsToCallSize) - (offsetof(UA_CallRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_CallRequest, methodsToCall) - sizeof(UA_Int32) - offsetof(UA_CallRequest, methodsToCallSize)), .isArray = UA_TRUE }, }},

/* UA_CallResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 713}, .memSize = sizeof(UA_CallResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CALLRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CallResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_CALLMETHODRESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallResponse, resultsSize) - (offsetof(UA_CallResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_CallResponse, results) - sizeof(UA_Int32) - offsetof(UA_CallResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CallResponse, diagnosticInfosSize) - (offsetof(UA_CallResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_CallResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_CallResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_MonitoringMode */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 716}, .memSize = sizeof(UA_MonitoringMode), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_MonitoringParameters */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 740}, .memSize = sizeof(UA_MonitoringParameters), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITORINGPARAMETERS, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, clientHandle) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, samplingInterval) - (offsetof(UA_MonitoringParameters, clientHandle) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, filter) - (offsetof(UA_MonitoringParameters, samplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, queueSize) - (offsetof(UA_MonitoringParameters, filter) + sizeof(UA_ExtensionObject)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoringParameters, discardOldest) - (offsetof(UA_MonitoringParameters, queueSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_MonitoredItemCreateRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 743}, .memSize = sizeof(UA_MonitoredItemCreateRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITOREDITEMCREATEREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_READVALUEID, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, itemToMonitor) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITORINGMODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, monitoringMode) - (offsetof(UA_MonitoredItemCreateRequest, itemToMonitor) + sizeof(UA_ReadValueId)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITORINGPARAMETERS, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateRequest, requestedParameters) - (offsetof(UA_MonitoredItemCreateRequest, monitoringMode) + sizeof(UA_MonitoringMode)), .isArray = UA_FALSE }, }},

/* UA_MonitoredItemCreateResult */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 746}, .memSize = sizeof(UA_MonitoredItemCreateResult), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_MONITOREDITEMCREATERESULT, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, statusCode) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, monitoredItemId) - (offsetof(UA_MonitoredItemCreateResult, statusCode) + sizeof(UA_StatusCode)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, revisedSamplingInterval) - (offsetof(UA_MonitoredItemCreateResult, monitoredItemId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, revisedQueueSize) - (offsetof(UA_MonitoredItemCreateResult, revisedSamplingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_MonitoredItemCreateResult, filterResult) - (offsetof(UA_MonitoredItemCreateResult, revisedQueueSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_CreateMonitoredItemsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 749}, .memSize = sizeof(UA_CreateMonitoredItemsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATEMONITOREDITEMSREQUEST, .membersSize = 4,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, subscriptionId) - (offsetof(UA_CreateMonitoredItemsRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_TIMESTAMPSTORETURN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsRequest, timestampsToReturn) - (offsetof(UA_CreateMonitoredItemsRequest, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITOREDITEMCREATEREQUEST, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsRequest, itemsToCreateSize) - (offsetof(UA_CreateMonitoredItemsRequest, timestampsToReturn) + sizeof(UA_TimestampsToReturn))) << 3) + (offsetof(UA_CreateMonitoredItemsRequest, itemsToCreate) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsRequest, itemsToCreateSize)), .isArray = UA_TRUE }, }},

/* UA_CreateMonitoredItemsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 752}, .memSize = sizeof(UA_CreateMonitoredItemsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATEMONITOREDITEMSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateMonitoredItemsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_MONITOREDITEMCREATERESULT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsResponse, resultsSize) - (offsetof(UA_CreateMonitoredItemsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_CreateMonitoredItemsResponse, results) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfosSize) - (offsetof(UA_CreateMonitoredItemsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_CreateMonitoredItemsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_CreateSubscriptionRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 785}, .memSize = sizeof(UA_CreateSubscriptionRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESUBSCRIPTIONREQUEST, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedPublishingInterval) - (offsetof(UA_CreateSubscriptionRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedLifetimeCount) - (offsetof(UA_CreateSubscriptionRequest, requestedPublishingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, requestedMaxKeepAliveCount) - (offsetof(UA_CreateSubscriptionRequest, requestedLifetimeCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, maxNotificationsPerPublish) - (offsetof(UA_CreateSubscriptionRequest, requestedMaxKeepAliveCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, publishingEnabled) - (offsetof(UA_CreateSubscriptionRequest, maxNotificationsPerPublish) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionRequest, priority) - (offsetof(UA_CreateSubscriptionRequest, publishingEnabled) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, }},

/* UA_CreateSubscriptionResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 788}, .memSize = sizeof(UA_CreateSubscriptionResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_CREATESUBSCRIPTIONRESPONSE, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, subscriptionId) - (offsetof(UA_CreateSubscriptionResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DOUBLE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedPublishingInterval) - (offsetof(UA_CreateSubscriptionResponse, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedLifetimeCount) - (offsetof(UA_CreateSubscriptionResponse, revisedPublishingInterval) + sizeof(UA_Double)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_CreateSubscriptionResponse, revisedMaxKeepAliveCount) - (offsetof(UA_CreateSubscriptionResponse, revisedLifetimeCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SetPublishingModeRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 797}, .memSize = sizeof(UA_SetPublishingModeRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SETPUBLISHINGMODEREQUEST, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeRequest, publishingEnabled) - (offsetof(UA_SetPublishingModeRequest, requestHeader) + sizeof(UA_RequestHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeRequest, subscriptionIdsSize) - (offsetof(UA_SetPublishingModeRequest, publishingEnabled) + sizeof(UA_Boolean))) << 3) + (offsetof(UA_SetPublishingModeRequest, subscriptionIds) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeRequest, subscriptionIdsSize)), .isArray = UA_TRUE }, }},

/* UA_SetPublishingModeResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 800}, .memSize = sizeof(UA_SetPublishingModeResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SETPUBLISHINGMODERESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SetPublishingModeResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeResponse, resultsSize) - (offsetof(UA_SetPublishingModeResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_SetPublishingModeResponse, results) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_SetPublishingModeResponse, diagnosticInfosSize) - (offsetof(UA_SetPublishingModeResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_SetPublishingModeResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_SetPublishingModeResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_NotificationMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 803}, .memSize = sizeof(UA_NotificationMessage), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_NOTIFICATIONMESSAGE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NotificationMessage, sequenceNumber) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_NotificationMessage, publishTime) - (offsetof(UA_NotificationMessage, sequenceNumber) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_EXTENSIONOBJECT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_NotificationMessage, notificationDataSize) - (offsetof(UA_NotificationMessage, publishTime) + sizeof(UA_DateTime))) << 3) + (offsetof(UA_NotificationMessage, notificationData) - sizeof(UA_Int32) - offsetof(UA_NotificationMessage, notificationDataSize)), .isArray = UA_TRUE }, }},

/* UA_SubscriptionAcknowledgement */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 821}, .memSize = sizeof(UA_SubscriptionAcknowledgement), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SubscriptionAcknowledgement) == 8, .typeIndex = UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SubscriptionAcknowledgement, subscriptionId) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SubscriptionAcknowledgement, sequenceNumber) - (offsetof(UA_SubscriptionAcknowledgement, subscriptionId) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_PublishRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 824}, .memSize = sizeof(UA_PublishRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PUBLISHREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SUBSCRIPTIONACKNOWLEDGEMENT, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishRequest, subscriptionAcknowledgementsSize) - (offsetof(UA_PublishRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_PublishRequest, subscriptionAcknowledgements) - sizeof(UA_Int32) - offsetof(UA_PublishRequest, subscriptionAcknowledgementsSize)), .isArray = UA_TRUE }, }},

/* UA_PublishResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 827}, .memSize = sizeof(UA_PublishResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_PUBLISHRESPONSE, .membersSize = 7,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, subscriptionId) - (offsetof(UA_PublishResponse, responseHeader) + sizeof(UA_ResponseHeader)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, availableSequenceNumbersSize) - (offsetof(UA_PublishResponse, subscriptionId) + sizeof(UA_UInt32))) << 3) + (offsetof(UA_PublishResponse, availableSequenceNumbers) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, availableSequenceNumbersSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BOOLEAN, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, moreNotifications) - (offsetof(UA_PublishResponse, availableSequenceNumbers) + sizeof(void*)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_NOTIFICATIONMESSAGE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_PublishResponse, notificationMessage) - (offsetof(UA_PublishResponse, moreNotifications) + sizeof(UA_Boolean)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, resultsSize) - (offsetof(UA_PublishResponse, notificationMessage) + sizeof(UA_NotificationMessage))) << 3) + (offsetof(UA_PublishResponse, results) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_PublishResponse, diagnosticInfosSize) - (offsetof(UA_PublishResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_PublishResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_PublishResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteSubscriptionsRequest */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 845}, .memSize = sizeof(UA_DeleteSubscriptionsRequest), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETESUBSCRIPTIONSREQUEST, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_REQUESTHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteSubscriptionsRequest, requestHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteSubscriptionsRequest, subscriptionIdsSize) - (offsetof(UA_DeleteSubscriptionsRequest, requestHeader) + sizeof(UA_RequestHeader))) << 3) + (offsetof(UA_DeleteSubscriptionsRequest, subscriptionIds) - sizeof(UA_Int32) - offsetof(UA_DeleteSubscriptionsRequest, subscriptionIdsSize)), .isArray = UA_TRUE }, }},

/* UA_DeleteSubscriptionsResponse */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 848}, .memSize = sizeof(UA_DeleteSubscriptionsResponse), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_DELETESUBSCRIPTIONSRESPONSE, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_RESPONSEHEADER, .namespaceZero = UA_TRUE, .padding = offsetof(UA_DeleteSubscriptionsResponse, responseHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STATUSCODE, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteSubscriptionsResponse, resultsSize) - (offsetof(UA_DeleteSubscriptionsResponse, responseHeader) + sizeof(UA_ResponseHeader))) << 3) + (offsetof(UA_DeleteSubscriptionsResponse, results) - sizeof(UA_Int32) - offsetof(UA_DeleteSubscriptionsResponse, resultsSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_DIAGNOSTICINFO, .namespaceZero = UA_TRUE, .padding = ((offsetof(UA_DeleteSubscriptionsResponse, diagnosticInfosSize) - (offsetof(UA_DeleteSubscriptionsResponse, results) + sizeof(void*))) << 3) + (offsetof(UA_DeleteSubscriptionsResponse, diagnosticInfos) - sizeof(UA_Int32) - offsetof(UA_DeleteSubscriptionsResponse, diagnosticInfosSize)), .isArray = UA_TRUE }, }},

/* UA_BuildInfo */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 338}, .memSize = sizeof(UA_BuildInfo), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_BUILDINFO, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, productUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, manufacturerName) - (offsetof(UA_BuildInfo, productUri) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, productName) - (offsetof(UA_BuildInfo, manufacturerName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, softwareVersion) - (offsetof(UA_BuildInfo, productName) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, buildNumber) - (offsetof(UA_BuildInfo, softwareVersion) + sizeof(UA_String)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_BuildInfo, buildDate) - (offsetof(UA_BuildInfo, buildNumber) + sizeof(UA_String)), .isArray = UA_FALSE }, }},

/* UA_ServerState */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 852}, .memSize = sizeof(UA_ServerState), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_ServerStatusDataType */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 862}, .memSize = sizeof(UA_ServerStatusDataType), .namespaceZero = UA_TRUE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TYPES_SERVERSTATUSDATATYPE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, startTime) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_DATETIME, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, currentTime) - (offsetof(UA_ServerStatusDataType, startTime) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_SERVERSTATE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, state) - (offsetof(UA_ServerStatusDataType, currentTime) + sizeof(UA_DateTime)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BUILDINFO, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, buildInfo) - (offsetof(UA_ServerStatusDataType, state) + sizeof(UA_ServerState)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, secondsTillShutdown) - (offsetof(UA_ServerStatusDataType, buildInfo) + sizeof(UA_BuildInfo)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_LOCALIZEDTEXT, .namespaceZero = UA_TRUE, .padding = offsetof(UA_ServerStatusDataType, shutdownReason) - (offsetof(UA_ServerStatusDataType, secondsTillShutdown) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},
};


/*********************************** amalgamated original file "C:/open62541/build/src_generated/ua_transport_generated.c" ***********************************/

/**
* @file ua_transport_generated.c
*
* @brief Autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/open62541/tools/generate_datatypes.py
* on host opcua-PC by user opcua at 2015-11-18 11:21:45
*/


const UA_DataType *UA_TRANSPORT = (UA_DataType[]){

/* UA_MessageTypeAndFinal */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_MessageTypeAndFinal), .namespaceZero = UA_TRUE, .fixedSize = UA_TRUE, .zeroCopyable = UA_TRUE, .membersSize = 1,
	.members = {{.memberTypeIndex = UA_TYPES_INT32,.namespaceZero = UA_TRUE, .padding = 0, .isArray = UA_FALSE }}, .typeIndex = UA_TYPES_INT32 },

/* UA_TcpMessageHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpMessageHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_TcpMessageHeader) == 8, .typeIndex = UA_TRANSPORT_TCPMESSAGEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpMessageHeader, messageTypeAndFinal) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpMessageHeader, messageSize) - (offsetof(UA_TcpMessageHeader, messageTypeAndFinal) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_TcpHelloMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpHelloMessage), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_TCPHELLOMESSAGE, .membersSize = 6,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, protocolVersion) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, receiveBufferSize) - (offsetof(UA_TcpHelloMessage, protocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, sendBufferSize) - (offsetof(UA_TcpHelloMessage, receiveBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, maxMessageSize) - (offsetof(UA_TcpHelloMessage, sendBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, maxChunkCount) - (offsetof(UA_TcpHelloMessage, maxMessageSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpHelloMessage, endpointUrl) - (offsetof(UA_TcpHelloMessage, maxChunkCount) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_TcpAcknowledgeMessage */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_TcpAcknowledgeMessage), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_TcpAcknowledgeMessage) == 20, .typeIndex = UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE, .membersSize = 5,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, protocolVersion) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, receiveBufferSize) - (offsetof(UA_TcpAcknowledgeMessage, protocolVersion) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, sendBufferSize) - (offsetof(UA_TcpAcknowledgeMessage, receiveBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, maxMessageSize) - (offsetof(UA_TcpAcknowledgeMessage, sendBufferSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_TcpAcknowledgeMessage, maxChunkCount) - (offsetof(UA_TcpAcknowledgeMessage, maxMessageSize) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SecureConversationMessageHeader) == 12, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TRANSPORT_TCPMESSAGEHEADER, .namespaceZero = UA_FALSE, .padding = offsetof(UA_SecureConversationMessageHeader, messageHeader) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageHeader, secureChannelId) - (offsetof(UA_SecureConversationMessageHeader, messageHeader) + sizeof(UA_TcpMessageHeader)), .isArray = UA_FALSE }, }},

/* UA_AsymmetricAlgorithmSecurityHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_AsymmetricAlgorithmSecurityHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER, .membersSize = 3,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, securityPolicyUri) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, senderCertificate) - (offsetof(UA_AsymmetricAlgorithmSecurityHeader, securityPolicyUri) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_BYTESTRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_AsymmetricAlgorithmSecurityHeader, receiverCertificateThumbprint) - (offsetof(UA_AsymmetricAlgorithmSecurityHeader, senderCertificate) + sizeof(UA_ByteString)), .isArray = UA_FALSE }, }},

/* UA_SymmetricAlgorithmSecurityHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SymmetricAlgorithmSecurityHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SymmetricAlgorithmSecurityHeader) == 4, .typeIndex = UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER, .membersSize = 1,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SymmetricAlgorithmSecurityHeader, tokenId) - 0, .isArray = UA_FALSE }, }},

/* UA_SequenceHeader */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SequenceHeader), .namespaceZero = UA_FALSE, .fixedSize = UA_TRUE, .zeroCopyable = sizeof(UA_SequenceHeader) == 8, .typeIndex = UA_TRANSPORT_SEQUENCEHEADER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SequenceHeader, sequenceNumber) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SequenceHeader, requestId) - (offsetof(UA_SequenceHeader, sequenceNumber) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageFooter */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageFooter), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = (offsetof(UA_SecureConversationMessageFooter, padding) - sizeof(UA_Int32) - offsetof(UA_SecureConversationMessageFooter, paddingSize)), .isArray = UA_TRUE }, 
	{.memberTypeIndex = UA_TYPES_BYTE, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageFooter, signature) - (offsetof(UA_SecureConversationMessageFooter, padding) + sizeof(void*)), .isArray = UA_FALSE }, }},

/* UA_SecureConversationMessageAbortBody */
{.typeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0}, .memSize = sizeof(UA_SecureConversationMessageAbortBody), .namespaceZero = UA_FALSE, .fixedSize = UA_FALSE, .zeroCopyable = UA_FALSE, .typeIndex = UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY, .membersSize = 2,
	.members={
	{.memberTypeIndex = UA_TYPES_UINT32, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageAbortBody, error) - 0, .isArray = UA_FALSE }, 
	{.memberTypeIndex = UA_TYPES_STRING, .namespaceZero = UA_TRUE, .padding = offsetof(UA_SecureConversationMessageAbortBody, reason) - (offsetof(UA_SecureConversationMessageAbortBody, error) + sizeof(UA_UInt32)), .isArray = UA_FALSE }, }},
};


/*********************************** amalgamated original file "C:/open62541/src/ua_connection.c" ***********************************/


// max message size is 64k
const UA_ConnectionConfig UA_ConnectionConfig_standard =
    {.protocolVersion = 0, .sendBufferSize = 65536, .recvBufferSize  = 65536,
     .maxMessageSize = 65536, .maxChunkCount   = 1};

void UA_Connection_init(UA_Connection *connection) {
    connection->state = UA_CONNECTION_CLOSED;
    connection->localConf = UA_ConnectionConfig_standard;
    connection->remoteConf = UA_ConnectionConfig_standard;
    connection->channel = NULL;
    connection->sockfd = 0;
    connection->handle = NULL;
    UA_ByteString_init(&connection->incompleteMessage);
    connection->send = NULL;
    connection->close = NULL;
    connection->recv = NULL;
    connection->getSendBuffer = NULL;
    connection->releaseSendBuffer = NULL;
    connection->releaseRecvBuffer = NULL;
}

void UA_Connection_deleteMembers(UA_Connection *connection) {
    UA_ByteString_deleteMembers(&connection->incompleteMessage);
}

UA_Job UA_Connection_completeMessages(UA_Connection *connection, UA_ByteString received) {
    UA_Job job = (UA_Job){.type = UA_JOBTYPE_NOTHING};
    if(received.length == -1)
        return job;

    UA_ByteString current;
    if(connection->incompleteMessage.length <= 0)
        current = received;
    else {
        /* concat the existing incomplete message with the new message */
        current.data = UA_realloc(connection->incompleteMessage.data,
                                  connection->incompleteMessage.length + received.length);
        if(!current.data) {
            /* not enough memory */
            UA_ByteString_deleteMembers(&connection->incompleteMessage);
            connection->releaseRecvBuffer(connection, &received);
            return job;
        }
        memcpy(current.data + connection->incompleteMessage.length, received.data, received.length);
        current.length = connection->incompleteMessage.length + received.length;
        connection->releaseRecvBuffer(connection, &received);
        UA_ByteString_init(&connection->incompleteMessage);
    }

    /* the while loop sets pos to the first element after the last complete message. if a message
       contains garbage, the buffer length is set to contain only the "good" messages before. */
    size_t pos = 0;
    while(current.length - pos >= 16) {
        UA_UInt32 msgtype = current.data[pos] + (current.data[pos+1] << 8) + (current.data[pos+2] << 16);
        if(msgtype != ('M' + ('S' << 8) + ('G' << 16)) &&
           msgtype != ('O' + ('P' << 8) + ('N' << 16)) &&
           msgtype != ('H' + ('E' << 8) + ('L' << 16)) &&
           msgtype != ('A' + ('C' << 8) + ('K' << 16)) &&
           msgtype != ('C' + ('L' << 8) + ('O' << 16))) {
            /* the message type is not recognized. throw the remaining bytestring away */
            current.length = pos;
            break;
        }
        UA_Int32 length = 0;
        size_t length_pos = pos + 4;
        UA_StatusCode retval = UA_Int32_decodeBinary(&current, &length_pos, &length);
        if(retval != UA_STATUSCODE_GOOD || length < 16 ||
           length > (UA_Int32)connection->localConf.maxMessageSize) {
            /* the message size is not allowed. throw the remaining bytestring away */
            current.length = pos;
            break;
        }
        if(length + (UA_Int32)pos > current.length)
            break; /* the message is incomplete. keep the beginning */
        pos += length;
    }

    if(current.length == 0) {
        /* throw everything away */
        if(current.data == received.data)
            connection->releaseRecvBuffer(connection, &received);
        else
            UA_ByteString_deleteMembers(&current);
        return job;
    }

    if(pos == 0) {
        /* no complete message in current */
        if(current.data == received.data) {
            /* copy the data into the connection */
            UA_ByteString_copy(&current, &connection->incompleteMessage);
            connection->releaseRecvBuffer(connection, &received);
        } else {
            /* the data is already copied off the network stack */
            connection->incompleteMessage = current;
        }
        return job;
    }

    if(current.length != (UA_Int32)pos) {
        /* there is an incomplete message at the end of current */
        connection->incompleteMessage.data = UA_malloc(current.length - pos);
        if(connection->incompleteMessage.data) {
            memcpy(connection->incompleteMessage.data, &current.data[pos], current.length - pos);
            connection->incompleteMessage.length = current.length - pos;
        }
        current.length = pos;
    }

    job.job.binaryMessage.message = current;
    job.job.binaryMessage.connection = connection;
    if(current.data == received.data)
        job.type = UA_JOBTYPE_BINARYMESSAGE_NETWORKLAYER;
    else
        job.type = UA_JOBTYPE_BINARYMESSAGE_ALLOCATED;
    return job;
}

void UA_Connection_detachSecureChannel(UA_Connection *connection) {
#ifdef UA_MULTITHREADING
    UA_SecureChannel *channel = connection->channel;
    if(channel)
        uatomic_cmpxchg(&channel->connection, connection, NULL);
    uatomic_set(&connection->channel, NULL);
#else
    if(connection->channel)
        connection->channel->connection = NULL;
    connection->channel = NULL;
#endif
}

void UA_Connection_attachSecureChannel(UA_Connection *connection, UA_SecureChannel *channel) {
#ifdef UA_MULTITHREADING
    if(uatomic_cmpxchg(&channel->connection, NULL, connection) == NULL)
        uatomic_set(&connection->channel, channel);
#else
    if(channel->connection != NULL)
        return;
    channel->connection = connection;
    connection->channel = channel;
#endif
}

/*********************************** amalgamated original file "C:/open62541/src/ua_securechannel.c" ***********************************/


void UA_SecureChannel_init(UA_SecureChannel *channel) {
    UA_MessageSecurityMode_init(&channel->securityMode);
    UA_ChannelSecurityToken_init(&channel->securityToken);
    UA_ChannelSecurityToken_init(&channel->nextSecurityToken);
    UA_AsymmetricAlgorithmSecurityHeader_init(&channel->clientAsymAlgSettings);
    UA_AsymmetricAlgorithmSecurityHeader_init(&channel->serverAsymAlgSettings);
    UA_ByteString_init(&channel->clientNonce);
    UA_ByteString_init(&channel->serverNonce);
    channel->sequenceNumber = 0;
    channel->connection = NULL;
    LIST_INIT(&channel->sessions);
}

void UA_SecureChannel_deleteMembersCleanup(UA_SecureChannel *channel) {
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&channel->serverAsymAlgSettings);
    UA_ByteString_deleteMembers(&channel->serverNonce);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&channel->clientAsymAlgSettings);
    UA_ByteString_deleteMembers(&channel->clientNonce);
    UA_ChannelSecurityToken_deleteMembers(&channel->securityToken); //FIXME: not really needed
    UA_ChannelSecurityToken_deleteMembers(&channel->nextSecurityToken); //FIXME: not really needed
    UA_Connection *c = channel->connection;
    if(c) {
        UA_Connection_detachSecureChannel(c);
        if(c->close)
            c->close(c);
    }
    /* just remove the pointers and free the linked list (not the sessions) */
    struct SessionEntry *se, *temp;
    LIST_FOREACH_SAFE(se, &channel->sessions, pointers, temp) {
        if(se->session)
            se->session->channel = NULL;
        LIST_REMOVE(se, pointers);
        UA_free(se);
    }
}

//TODO implement real nonce generator - DUMMY function
UA_StatusCode UA_SecureChannel_generateNonce(UA_ByteString *nonce) {
    if(!(nonce->data = UA_malloc(1)))
        return UA_STATUSCODE_BADOUTOFMEMORY;
    nonce->length  = 1;
    nonce->data[0] = 'a';
    return UA_STATUSCODE_GOOD;
}

void UA_SecureChannel_attachSession(UA_SecureChannel *channel, UA_Session *session) {
    struct SessionEntry *se = UA_malloc(sizeof(struct SessionEntry));
    if(!se)
        return;
    se->session = session;
#ifdef UA_MULTITHREADING
    if(uatomic_cmpxchg(&session->channel, NULL, channel) != NULL) {
        UA_free(se);
        return;
    }
#else
    if(session->channel != NULL) {
        UA_free(se);
        return;
    }
    session->channel = channel;
#endif
    LIST_INSERT_HEAD(&channel->sessions, se, pointers);
}

void UA_SecureChannel_detachSession(UA_SecureChannel *channel, UA_Session *session) {
    if(session)
        session->channel = NULL;
    struct SessionEntry *se, *temp;
    LIST_FOREACH_SAFE(se, &channel->sessions, pointers, temp) {
        if(se->session != session)
            continue;
        LIST_REMOVE(se, pointers);
        UA_free(se);
        break;
    }
}

UA_Session * UA_SecureChannel_getSession(UA_SecureChannel *channel, UA_NodeId *token) {
    struct SessionEntry *se;
    LIST_FOREACH(se, &channel->sessions, pointers) {
        if(UA_NodeId_equal(&se->session->authenticationToken, token))
            break;
    }
    if(!se)
        return NULL;
    return se->session;
}

void UA_SecureChannel_revolveTokens(UA_SecureChannel *channel){
    if(channel->nextSecurityToken.tokenId==0) //no security token issued
        return;

    //FIXME: not thread-safe
    //swap tokens
    memcpy(&channel->securityToken, &channel->nextSecurityToken, sizeof(UA_ChannelSecurityToken));
    UA_ChannelSecurityToken_init(&channel->nextSecurityToken);
}

UA_StatusCode UA_SecureChannel_sendBinaryMessage(UA_SecureChannel *channel, UA_UInt32 requestId,
                                                  const void *content,
                                                  const UA_DataType *contentType) {
    UA_Connection *connection = channel->connection;
    if(!connection)
        return UA_STATUSCODE_BADINTERNALERROR;

    UA_NodeId typeId = contentType->typeId;
    if(typeId.identifierType != UA_NODEIDTYPE_NUMERIC)
        return UA_STATUSCODE_BADINTERNALERROR;
    typeId.identifier.numeric += UA_ENCODINGOFFSET_BINARY;

    UA_SecureConversationMessageHeader respHeader;
    respHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_MSGF;
    respHeader.messageHeader.messageSize = 0;
    respHeader.secureChannelId = channel->securityToken.channelId;

    UA_SymmetricAlgorithmSecurityHeader symSecHeader;
    symSecHeader.tokenId = channel->securityToken.tokenId;

    UA_SequenceHeader seqHeader;
    seqHeader.requestId = requestId;

    UA_ByteString message;
    UA_StatusCode retval = connection->getSendBuffer(connection, connection->remoteConf.recvBufferSize,
                                                     &message);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    size_t messagePos = 24; // after the headers
    retval |= UA_NodeId_encodeBinary(&typeId, &message, &messagePos);
    retval |= UA_encodeBinary(content, contentType, &message, &messagePos);

    if(retval != UA_STATUSCODE_GOOD) {
        connection->releaseSendBuffer(connection, &message);
        return retval;
    }

    /* now write the header with the size */
    respHeader.messageHeader.messageSize = messagePos;
#ifndef UA_MULTITHREADING
    seqHeader.sequenceNumber = ++channel->sequenceNumber;
#else
    seqHeader.sequenceNumber = uatomic_add_return(&channel->sequenceNumber, 1);
#endif

    messagePos = 0;
    UA_SecureConversationMessageHeader_encodeBinary(&respHeader, &message, &messagePos);
    UA_SymmetricAlgorithmSecurityHeader_encodeBinary(&symSecHeader, &message, &messagePos);
    UA_SequenceHeader_encodeBinary(&seqHeader, &message, &messagePos);
    message.length = respHeader.messageHeader.messageSize;

    retval = connection->send(connection, &message);
    return retval;
}

/*********************************** amalgamated original file "C:/open62541/src/ua_session.c" ***********************************/


UA_Session anonymousSession = {
    .clientDescription =  {.applicationUri = {-1, NULL}, .productUri = {-1, NULL},
                           .applicationName = {.locale = {-1, NULL}, .text = {-1, NULL}},
                           .applicationType = UA_APPLICATIONTYPE_CLIENT,
                           .gatewayServerUri = {-1, NULL}, .discoveryProfileUri = {-1, NULL},
                           .discoveryUrlsSize = -1, .discoveryUrls = NULL},
    .sessionName = {sizeof("Anonymous Session")-1, (UA_Byte*)"Anonymous Session"},
    .authenticationToken = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
                            .identifier.numeric = 0}, 
    .sessionId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 0},
    .maxRequestMessageSize = UA_UINT32_MAX, .maxResponseMessageSize = UA_UINT32_MAX,
    .timeout = UA_INT64_MAX, .validTill = UA_INT64_MAX, .channel = NULL,
    .continuationPoints = {NULL}};

UA_Session adminSession = {
    .clientDescription =  {.applicationUri = {-1, NULL}, .productUri = {-1, NULL},
                           .applicationName = {.locale = {-1, NULL}, .text = {-1, NULL}},
                           .applicationType = UA_APPLICATIONTYPE_CLIENT,
                           .gatewayServerUri = {-1, NULL}, .discoveryProfileUri = {-1, NULL},
                           .discoveryUrlsSize = -1, .discoveryUrls = NULL},
    .sessionName = {sizeof("Administrator Session")-1, (UA_Byte*)"Administrator Session"},
    .authenticationToken = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
                            .identifier.numeric = 1},
    .sessionId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC, .identifier.numeric = 1},
    .maxRequestMessageSize = UA_UINT32_MAX, .maxResponseMessageSize = UA_UINT32_MAX,
    .timeout = UA_INT64_MAX, .validTill = UA_INT64_MAX, .channel = NULL,
    .continuationPoints = {NULL}};

void UA_Session_init(UA_Session *session) {
    UA_ApplicationDescription_init(&session->clientDescription);
    session->activated = UA_FALSE;
    UA_NodeId_init(&session->authenticationToken);
    UA_NodeId_init(&session->sessionId);
    UA_String_init(&session->sessionName);
    session->maxRequestMessageSize  = 0;
    session->maxResponseMessageSize = 0;
    session->timeout = 0;
    UA_DateTime_init(&session->validTill);
    session->channel = NULL;
#ifdef ENABLE_SUBSCRIPTIONS
    SubscriptionManager_init(session);
#endif
    session->availableContinuationPoints = MAXCONTINUATIONPOINTS;
    LIST_INIT(&session->continuationPoints);
}

void UA_Session_deleteMembersCleanup(UA_Session *session, UA_Server* server) {
    UA_ApplicationDescription_deleteMembers(&session->clientDescription);
    UA_NodeId_deleteMembers(&session->authenticationToken);
    UA_NodeId_deleteMembers(&session->sessionId);
    UA_String_deleteMembers(&session->sessionName);
    struct ContinuationPointEntry *cp, *temp;
    LIST_FOREACH_SAFE(cp, &session->continuationPoints, pointers, temp) {
        LIST_REMOVE(cp, pointers);
        UA_ByteString_deleteMembers(&cp->identifier);
        UA_BrowseDescription_deleteMembers(&cp->browseDescription);
        UA_free(cp);
    }
    if(session->channel)
        UA_SecureChannel_detachSession(session->channel, session);
#ifdef ENABLE_SUBSCRIPTIONS
    SubscriptionManager_deleteMembers(session, server);
#endif
}

void UA_Session_updateLifetime(UA_Session *session) {
    session->validTill = UA_DateTime_now() + session->timeout * 10000; //timeout in ms
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_server.c" ***********************************/


#ifdef ENABLE_GENERATE_NAMESPACE0
#endif

const UA_EXPORT UA_ServerConfig UA_ServerConfig_standard = {
    .Login_enableAnonymous = UA_TRUE,
    .Login_enableUsernamePassword = UA_TRUE,
    .Login_usernames = (char *[]){"user1","user2"},
    .Login_passwords = (char *[]){"password","password1"},
    .Login_loginsCount = 2,
    .Application_applicationURI = "urn:unconfigured:open62541:open62541Server",
    .Application_applicationName = "open62541" };

static const UA_NodeId nodeIdHasSubType = {
    .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
    .identifier.numeric = UA_NS0ID_HASSUBTYPE};
static const UA_NodeId nodeIdHasTypeDefinition = {
    .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
    .identifier.numeric = UA_NS0ID_HASTYPEDEFINITION};
static const UA_NodeId nodeIdHasComponent = {
    .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
    .identifier.numeric = UA_NS0ID_HASCOMPONENT};
static const UA_NodeId nodeIdHasProperty = {
    .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
    .identifier.numeric = UA_NS0ID_HASPROPERTY};
static const UA_NodeId nodeIdOrganizes = {
    .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
    .identifier.numeric = UA_NS0ID_ORGANIZES};

static const UA_ExpandedNodeId expandedNodeIdBaseDataVariabletype = {
    .nodeId = {.namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
               .identifier.numeric = UA_NS0ID_BASEDATAVARIABLETYPE},
    .namespaceUri = {.length = -1, .data = NULL}, .serverIndex = 0};

#ifndef ENABLE_GENERATE_NAMESPACE0
static const UA_NodeId nodeIdNonHierarchicalReferences = {
        .namespaceIndex = 0, .identifierType = UA_NODEIDTYPE_NUMERIC,
        .identifier.numeric = UA_NS0ID_NONHIERARCHICALREFERENCES};
#endif

/**********************/
/* Namespace Handling */
/**********************/

#ifdef UA_EXTERNAL_NAMESPACES
static void UA_ExternalNamespace_init(UA_ExternalNamespace *ens) {
    ens->index = 0;
    UA_String_init(&ens->url);
}

static void UA_ExternalNamespace_deleteMembers(UA_ExternalNamespace *ens) {
    UA_String_deleteMembers(&ens->url);
    ens->externalNodeStore.destroy(ens->externalNodeStore.ensHandle);
}

static void UA_Server_deleteExternalNamespaces(UA_Server *server) {
	for(UA_UInt32 i = 0; i < server->externalNamespacesSize; i++){
		UA_ExternalNamespace_deleteMembers(&(server->externalNamespaces[i]));
	}
	if(server->externalNamespacesSize > 0){
		UA_free(server->externalNamespaces);
		server->externalNamespaces = NULL;
		server->externalNamespacesSize = 0;
	}
}

UA_StatusCode UA_EXPORT
UA_Server_addExternalNamespace(UA_Server *server, UA_UInt16 namespaceIndex,
                               const UA_String *url, UA_ExternalNodeStore *nodeStore) {
	if(nodeStore == NULL)
		return UA_STATUSCODE_BADARGUMENTSMISSING;

    UA_UInt32 size = server->externalNamespacesSize;
	//do not allow double indices
	for(UA_UInt32 i = 0; i < size; i++) {
		if(server->externalNamespaces[i].index == namespaceIndex)
			return UA_STATUSCODE_BADINDEXRANGEINVALID;
	}
    server->externalNamespaces =
        UA_realloc(server->externalNamespaces, sizeof(UA_ExternalNamespace) * (size+1));
    server->externalNamespaces[size].externalNodeStore = *nodeStore;
    server->externalNamespaces[size].index = namespaceIndex;
    UA_String_copy(url, &server->externalNamespaces[size].url);
    server->externalNamespacesSize++;
    return UA_STATUSCODE_GOOD;
}
#endif /* UA_EXTERNAL_NAMESPACES*/

UA_UInt16 UA_Server_addNamespace(UA_Server *server, const char* name) {
    server->namespaces = UA_realloc(server->namespaces,
                                    sizeof(UA_String) * (server->namespacesSize+1));
    server->namespaces[server->namespacesSize] = UA_STRING_ALLOC(name);
    server->namespacesSize++;
    return (UA_UInt16)server->namespacesSize - 1;
}

UA_StatusCode UA_Server_deleteNode(UA_Server *server, const UA_NodeId nodeId,
                                   UA_Boolean deleteReferences) {
    return Service_DeleteNodes_single(server, &adminSession, &nodeId, deleteReferences);
}

UA_StatusCode
UA_Server_deleteReference(UA_Server *server, const UA_NodeId sourceNodeId, const UA_NodeId referenceTypeId,
                          UA_Boolean isForward, const UA_ExpandedNodeId targetNodeId,
                          UA_Boolean deleteBidirectional) {
    UA_DeleteReferencesItem item;
    item.sourceNodeId = sourceNodeId;
    item.referenceTypeId = referenceTypeId;
    item.isForward = isForward;
    item.targetNodeId = targetNodeId;
    item.deleteBidirectional = deleteBidirectional;
    return Service_DeleteReferences_single(server, &adminSession, &item);
}

UA_StatusCode
UA_Server_forEachChildNodeCall(UA_Server *server, UA_NodeId parentNodeId,
                               UA_NodeIteratorCallback callback, void *handle) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    const UA_Node *parent = UA_NodeStore_get(server->nodestore, &parentNodeId);
    if(!parent)
        return UA_STATUSCODE_BADNODEIDINVALID;
    
    for(int i=0; i<parent->referencesSize; i++) {
        UA_ReferenceNode *ref = &parent->references[i];
        retval |= callback(ref->targetId.nodeId, ref->isInverse,
                           ref->referenceTypeId, handle);
    }
    
    UA_NodeStore_release(parent);
    return retval;
}

UA_StatusCode
UA_Server_addReference(UA_Server *server, const UA_NodeId sourceId,
                       const UA_NodeId refTypeId, const UA_ExpandedNodeId targetId,
                       UA_Boolean isForward) {
    UA_AddReferencesItem item;
    UA_AddReferencesItem_init(&item);
    item.sourceNodeId = sourceId;
    item.referenceTypeId = refTypeId;
    item.isForward = isForward;
    item.targetNodeId = targetId;
    return Service_AddReferences_single(server, &adminSession, &item);
}

static UA_AddNodesResult
addNodeInternal(UA_Server *server, UA_Node *node, const UA_NodeId parentNodeId,
                const UA_NodeId referenceTypeId) {
    UA_AddNodesResult res;
    UA_AddNodesResult_init(&res);
    UA_Server_addExistingNode(server, &adminSession, node, &parentNodeId,
                              &referenceTypeId, &res);
    return res;
}

UA_StatusCode
__UA_Server_addNode(UA_Server *server, const UA_NodeClass nodeClass,
                    const UA_NodeId requestedNewNodeId, const UA_NodeId parentNodeId,
                    const UA_NodeId referenceTypeId, const UA_QualifiedName browseName,
                    const UA_NodeId typeDefinition, const UA_NodeAttributes *attr,
                    const UA_DataType *attributeType, UA_NodeId *outNewNodeId) {
    UA_AddNodesResult result;
    UA_AddNodesResult_init(&result);

    UA_AddNodesItem item;
    UA_AddNodesItem_init(&item);
    result.statusCode = UA_QualifiedName_copy(&browseName, &item.browseName);
    item.nodeClass = nodeClass;
    result.statusCode |= UA_NodeId_copy(&parentNodeId, &item.parentNodeId.nodeId);
    result.statusCode |= UA_NodeId_copy(&referenceTypeId, &item.referenceTypeId);
    result.statusCode |= UA_NodeId_copy(&requestedNewNodeId,
                                        &item.requestedNewNodeId.nodeId);
    result.statusCode |= UA_NodeId_copy(&typeDefinition, &item.typeDefinition.nodeId);
    UA_NodeAttributes *attrCopy = UA_alloca(attributeType->memSize);
    result.statusCode |= UA_copy(attr, attrCopy, attributeType);
    if(result.statusCode == UA_STATUSCODE_GOOD)
        Service_AddNodes_single(server, &adminSession, &item, attrCopy, &result);

    if(outNewNodeId && result.statusCode == UA_STATUSCODE_GOOD)
        *outNewNodeId = result.addedNodeId;
    else
        UA_AddNodesResult_deleteMembers(&result);
    UA_AddNodesItem_deleteMembers(&item);
    UA_deleteMembers(attrCopy, attributeType);
    return result.statusCode;
}

/*****************/
/* Configuration */
/*****************/

void UA_Server_addNetworkLayer(UA_Server *server, UA_ServerNetworkLayer *networkLayer) {
    UA_ServerNetworkLayer **newlayers =
        UA_realloc(server->networkLayers, sizeof(void*)*(server->networkLayersSize+1));
    if(!newlayers) {
        UA_LOG_ERROR(server->logger, UA_LOGCATEGORY_SERVER, "Networklayer added");
        return;
    }
    server->networkLayers = newlayers;
    server->networkLayers[server->networkLayersSize] = networkLayer;
    server->networkLayersSize++;

    if(server->description.discoveryUrlsSize < 0)
        server->description.discoveryUrlsSize = 0;
    UA_String* newUrls;
    newUrls = UA_realloc(server->description.discoveryUrls,
                         sizeof(UA_String)*(server->description.discoveryUrlsSize+1));
    if(!newUrls) {
        UA_LOG_ERROR(server->logger, UA_LOGCATEGORY_SERVER, "Adding discoveryUrl");
        return;
    }
    server->description.discoveryUrls = newUrls;
    UA_String_copy(&networkLayer->discoveryUrl,
                   &server->description.discoveryUrls[server->description.discoveryUrlsSize]);
    server->description.discoveryUrlsSize++;
    for(UA_Int32 i = 0; i < server->endpointDescriptionsSize; i++) {
        if(!server->endpointDescriptions[i].endpointUrl.data)
            UA_String_copy(&networkLayer->discoveryUrl,
                           &server->endpointDescriptions[i].endpointUrl);
    }
}

void UA_Server_setServerCertificate(UA_Server *server, UA_ByteString certificate) {
    for(UA_Int32 i = 0; i < server->endpointDescriptionsSize; i++)
        UA_ByteString_copy(&certificate,
                           &server->endpointDescriptions[i].serverCertificate);
}

void UA_Server_setLogger(UA_Server *server, UA_Logger logger) {
    server->logger = logger;
}

/**********/
/* Server */
/**********/

/* The server needs to be stopped before it can be deleted */
void UA_Server_delete(UA_Server *server) {
    // Delete the timed work
    UA_Server_deleteAllRepeatedJobs(server);

    // Delete all internal data
    UA_ApplicationDescription_deleteMembers(&server->description);
    UA_SecureChannelManager_deleteMembers(&server->secureChannelManager);
    UA_SessionManager_deleteMembers(&server->sessionManager, server);
    UA_NodeStore_delete(server->nodestore);
#ifdef UA_EXTERNAL_NAMESPACES
    UA_Server_deleteExternalNamespaces(server);
#endif
    UA_ByteString_deleteMembers(&server->serverCertificate);
    UA_Array_delete(server->namespaces, &UA_TYPES[UA_TYPES_STRING],
                    server->namespacesSize);
    UA_Array_delete(server->endpointDescriptions,
                    &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION],
                    server->endpointDescriptionsSize);

    // Delete the network layers
    for(size_t i = 0; i < server->networkLayersSize; i++) {
        UA_String_deleteMembers(&server->networkLayers[i]->discoveryUrl);
        server->networkLayers[i]->deleteMembers(server->networkLayers[i]);
        UA_free(server->networkLayers[i]);
    }
    UA_free(server->networkLayers);

#ifdef UA_MULTITHREADING
    /* so the workers don't spin if the queue is empty */
    pthread_cond_destroy(&server->dispatchQueue_condition);
    rcu_barrier(); // wait for all scheduled call_rcu work to complete
   	rcu_unregister_thread();
#endif
    UA_free(server);
}

/* Recurring cleanup. Removing unused and timed-out channels and sessions */
static void UA_Server_cleanup(UA_Server *server, void *nothing) {
    UA_DateTime now = UA_DateTime_now();
    UA_SessionManager_cleanupTimedOut(&server->sessionManager, server, now);
    UA_SecureChannelManager_cleanupTimedOut(&server->secureChannelManager, now);
}

#define MANUFACTURER_NAME "open62541"
#define PRODUCT_NAME "open62541 OPC UA Server"
#define STRINGIFY(x) #x //some magic
#define TOSTRING(x) STRINGIFY(x) //some magic
#define SOFTWARE_VERSION TOSTRING(VERSION)
#define BUILD_NUMBER "0"

static void getBuildInfo(const UA_Server* server, UA_BuildInfo *buildInfo) {
    buildInfo->productUri = UA_STRING_ALLOC(PRODUCT_URI);
    buildInfo->manufacturerName = UA_STRING_ALLOC(MANUFACTURER_NAME);
    buildInfo->productName = UA_STRING_ALLOC(PRODUCT_NAME);
    buildInfo->softwareVersion = UA_STRING_ALLOC(SOFTWARE_VERSION);
    buildInfo->buildNumber = UA_STRING_ALLOC(BUILD_NUMBER);
    buildInfo->buildDate = server->buildDate;
}

static UA_StatusCode
readStatus(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp,
           const UA_NumericRange *range, UA_DataValue *value) {
    if(range) {
        value->hasStatus = UA_TRUE;
        value->status = UA_STATUSCODE_BADINDEXRANGEINVALID;
        return UA_STATUSCODE_GOOD;
    }

    UA_Server *server = (UA_Server*)handle;
    UA_ServerStatusDataType *status = UA_ServerStatusDataType_new();
    status->startTime = server->startTime;
    status->currentTime = UA_DateTime_now();
    status->state = UA_SERVERSTATE_RUNNING;
    getBuildInfo(server, &status->buildInfo);
    status->secondsTillShutdown = 0;

    value->value.type = &UA_TYPES[UA_TYPES_SERVERSTATUSDATATYPE];
    value->value.arrayLength = -1;
    value->value.data = status;
    value->value.arrayDimensionsSize = -1;
    value->value.arrayDimensions = NULL;
    value->hasValue = UA_TRUE;
    if(sourceTimeStamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
readNamespaces(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimestamp,
               const UA_NumericRange *range, UA_DataValue *value) {
    if(range) {
        value->hasStatus = UA_TRUE;
        value->status = UA_STATUSCODE_BADINDEXRANGEINVALID;
        return UA_STATUSCODE_GOOD;
    }
    UA_Server *server = (UA_Server*)handle;
    UA_StatusCode retval;
    retval = UA_Variant_setArrayCopy(&value->value, server->namespaces,
                                     server->namespacesSize, &UA_TYPES[UA_TYPES_STRING]);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    value->hasValue = UA_TRUE;
    if(sourceTimestamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = UA_DateTime_now();
    }
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
readCurrentTime(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp,
                const UA_NumericRange *range, UA_DataValue *value) {
    if(range) {
        value->hasStatus = UA_TRUE;
        value->status = UA_STATUSCODE_BADINDEXRANGEINVALID;
        return UA_STATUSCODE_GOOD;
    }
    UA_DateTime currentTime = UA_DateTime_now();
    UA_StatusCode retval = UA_Variant_setScalarCopy(&value->value, &currentTime, &UA_TYPES[UA_TYPES_DATETIME]);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    value->hasValue = UA_TRUE;
    if(sourceTimeStamp) {
        value->hasSourceTimestamp = UA_TRUE;
        value->sourceTimestamp = currentTime;
    }
    return UA_STATUSCODE_GOOD;
}

static void copyNames(UA_Node *node, char *name) {
    node->browseName = UA_QUALIFIEDNAME_ALLOC(0, name);
    node->displayName = UA_LOCALIZEDTEXT_ALLOC("en_US", name);
    node->description = UA_LOCALIZEDTEXT_ALLOC("en_US", name);
}

static void
addDataTypeNode(UA_Server *server, char* name, UA_UInt32 datatypeid, UA_Int32 parent) {
    UA_DataTypeNode *datatype = UA_DataTypeNode_new();
    copyNames((UA_Node*)datatype, name);
    datatype->nodeId.identifier.numeric = datatypeid;
    addNodeInternal(server, (UA_Node*)datatype, UA_NODEID_NUMERIC(0, parent), nodeIdOrganizes);
}

static void
addObjectTypeNode(UA_Server *server, char* name, UA_UInt32 objecttypeid,
                  UA_Int32 parent, UA_Int32 parentreference) {
    UA_ObjectTypeNode *objecttype = UA_ObjectTypeNode_new();
    copyNames((UA_Node*)objecttype, name);
    objecttype->nodeId.identifier.numeric = objecttypeid;
    addNodeInternal(server, (UA_Node*)objecttype, UA_NODEID_NUMERIC(0, parent),
                    UA_NODEID_NUMERIC(0, parentreference));
}

static UA_VariableTypeNode*
createVariableTypeNode(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                       UA_Int32 parent, UA_Boolean abstract) {
    UA_VariableTypeNode *variabletype = UA_VariableTypeNode_new();
    copyNames((UA_Node*)variabletype, name);
    variabletype->nodeId.identifier.numeric = variabletypeid;
    variabletype->isAbstract = abstract;
    variabletype->value.variant.value.type = &UA_TYPES[UA_TYPES_VARIANT];
    return variabletype;
}

static void
addVariableTypeNode_organized(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                              UA_Int32 parent, UA_Boolean abstract) {
    UA_VariableTypeNode *variabletype = createVariableTypeNode(server, name, variabletypeid, parent, abstract);
    addNodeInternal(server, (UA_Node*)variabletype, UA_NODEID_NUMERIC(0, parent), nodeIdOrganizes);
}

static void
addVariableTypeNode_subtype(UA_Server *server, char* name, UA_UInt32 variabletypeid,
                            UA_Int32 parent, UA_Boolean abstract) {
    UA_VariableTypeNode *variabletype =
        createVariableTypeNode(server, name, variabletypeid, parent, abstract);
    addNodeInternal(server, (UA_Node*)variabletype, UA_NODEID_NUMERIC(0, parent), nodeIdHasSubType);
}

UA_Server * UA_Server_new(UA_ServerConfig config) {
    UA_Server *server = UA_malloc(sizeof(UA_Server));
    if(!server)
        return NULL;

    //FIXME: config contains strings, for now its okay, but consider copying them aswell
    server->config = config;

    LIST_INIT(&server->repeatedJobs);
#ifdef UA_MULTITHREADING
    rcu_init();
   	rcu_register_thread();
    cds_wfcq_init(&server->dispatchQueue_head, &server->dispatchQueue_tail);
    cds_lfs_init(&server->mainLoopJobs);
    server->delayedJobs = NULL;
#endif

    // logger
    server->logger = NULL;

    // random seed
    server->random_seed = (UA_UInt32)UA_DateTime_now();

    // networklayers
    server->networkLayers = NULL;
    server->networkLayersSize = 0;

    UA_ByteString_init(&server->serverCertificate);

    // mockup application description
    UA_ApplicationDescription_init(&server->description);
    server->description.productUri = UA_STRING_ALLOC(PRODUCT_URI);
    server->description.applicationUri =
        UA_STRING_ALLOC(server->config.Application_applicationURI);
    server->description.discoveryUrlsSize = 0;

    server->description.applicationName =
        UA_LOCALIZEDTEXT_ALLOC("en_US", server->config.Application_applicationName);
    server->description.applicationType = UA_APPLICATIONTYPE_SERVER;

#ifdef UA_EXTERNAL_NAMESPACES
    server->externalNamespacesSize = 0;
    server->externalNamespaces = NULL;
#endif

    /* ns0 and ns1 */
    server->namespaces = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 2);
    server->namespaces[0] = UA_STRING_ALLOC("http://opcfoundation.org/UA/");
    UA_String_copy(&server->description.applicationUri, &server->namespaces[1]);
    server->namespacesSize = 2;

    server->endpointDescriptions = NULL;
    server->endpointDescriptionsSize = 0;

    UA_EndpointDescription *endpoint = UA_EndpointDescription_new(); // todo: check return code
    if(endpoint) {
        endpoint->securityMode = UA_MESSAGESECURITYMODE_NONE;
        endpoint->securityPolicyUri =
            UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");
        endpoint->transportProfileUri =
            UA_STRING_ALLOC("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");

        int size = 0;
        if(server->config.Login_enableAnonymous){
            size++;
        }
        if(server->config.Login_enableUsernamePassword){
            size++;
        }
        endpoint->userIdentityTokensSize = size;
        endpoint->userIdentityTokens = UA_Array_new(&UA_TYPES[UA_TYPES_USERTOKENPOLICY], size);

        int currentIndex = 0;
        if(server->config.Login_enableAnonymous){
            UA_UserTokenPolicy_init(&endpoint->userIdentityTokens[currentIndex]);
            endpoint->userIdentityTokens[currentIndex].tokenType = UA_USERTOKENTYPE_ANONYMOUS;
            endpoint->userIdentityTokens[currentIndex].policyId = UA_STRING_ALLOC(ANONYMOUS_POLICY); // defined per server
            currentIndex++;
        }

        if(server->config.Login_enableUsernamePassword){
            UA_UserTokenPolicy_init(&endpoint->userIdentityTokens[currentIndex]);
            endpoint->userIdentityTokens[currentIndex].tokenType = UA_USERTOKENTYPE_USERNAME;
            endpoint->userIdentityTokens[currentIndex].policyId = UA_STRING_ALLOC(USERNAME_POLICY); // defined per server
            currentIndex++;
        }

        /* The standard says "the HostName specified in the Server Certificate is the
           same as the HostName contained in the endpointUrl provided in the
           EndpointDescription */
        /* UA_String_copy(endpointUrl, &endpoint->endpointUrl); */
        /* UA_String_copy(&server->serverCertificate, &endpoint->serverCertificate); */
        UA_ApplicationDescription_copy(&server->description, &endpoint->server);
        server->endpointDescriptions = endpoint;
        server->endpointDescriptionsSize = 1;
    } 

#define MAXCHANNELCOUNT 100
#define STARTCHANNELID 1
#define TOKENLIFETIME 600000 //this is in milliseconds //600000 seems to be the minimal allowet time for UaExpert
#define STARTTOKENID 1
    UA_SecureChannelManager_init(&server->secureChannelManager, MAXCHANNELCOUNT,
                                 TOKENLIFETIME, STARTCHANNELID, STARTTOKENID);

#define MAXSESSIONCOUNT 1000
#define MAXSESSIONLIFETIME 10000
#define STARTSESSIONID 1
    UA_SessionManager_init(&server->sessionManager, MAXSESSIONCOUNT, MAXSESSIONLIFETIME, STARTSESSIONID);

    server->nodestore = UA_NodeStore_new();

    UA_Job cleanup = {.type = UA_JOBTYPE_METHODCALL,
                      .job.methodCall = {.method = UA_Server_cleanup, .data = NULL} };
    UA_Server_addRepeatedJob(server, cleanup, 10000, NULL);

    /**********************/
    /* Server Information */
    /**********************/

    server->startTime = UA_DateTime_now();
    static struct tm ct;
    ct.tm_year = (__DATE__[7] - '0') * 1000 + (__DATE__[8] - '0') * 100 +
        (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0')- 1900;

    if(__DATE__[0]=='J' && __DATE__[1]=='a' && __DATE__[2]=='n') ct.tm_mon = 1-1;
    else if(__DATE__[0]=='F' && __DATE__[1]=='e' && __DATE__[2]=='b') ct.tm_mon = 2-1;
    else if(__DATE__[0]=='M' && __DATE__[1]=='a' && __DATE__[2]=='r') ct.tm_mon = 3-1;
    else if(__DATE__[0]=='A' && __DATE__[1]=='p' && __DATE__[2]=='r') ct.tm_mon = 4-1;
    else if(__DATE__[0]=='M' && __DATE__[1]=='a' && __DATE__[2]=='y') ct.tm_mon = 5-1;
    else if(__DATE__[0]=='J' && __DATE__[1]=='u' && __DATE__[2]=='n') ct.tm_mon = 6-1;
    else if(__DATE__[0]=='J' && __DATE__[1]=='u' && __DATE__[2]=='l') ct.tm_mon = 7-1;
    else if(__DATE__[0]=='A' && __DATE__[1]=='u' && __DATE__[2]=='g') ct.tm_mon = 8-1;
    else if(__DATE__[0]=='S' && __DATE__[1]=='e' && __DATE__[2]=='p') ct.tm_mon = 9-1;
    else if(__DATE__[0]=='O' && __DATE__[1]=='c' && __DATE__[2]=='t') ct.tm_mon = 10-1;
    else if(__DATE__[0]=='N' && __DATE__[1]=='o' && __DATE__[2]=='v') ct.tm_mon = 11-1;
    else if(__DATE__[0]=='D' && __DATE__[1]=='e' && __DATE__[2]=='c') ct.tm_mon = 12-1;

    // special case to handle __DATE__ not inserting leading zero on day of month
    // if Day of month is less than 10 - it inserts a blank character
    // this results in a negative number for tm_mday

    if(__DATE__[4] == ' ')
        ct.tm_mday = __DATE__[5]-'0';
    else
        ct.tm_mday = (__DATE__[4]-'0')*10 + (__DATE__[5]-'0');
    ct.tm_hour = ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0');
    ct.tm_min = ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0');
    ct.tm_sec = ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0');
    ct.tm_isdst = -1; // information is not available.

    //FIXME: next 3 lines are copy-pasted from ua_types.c
#define UNIX_EPOCH_BIAS_SEC 11644473600LL // Number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
#define HUNDRED_NANOSEC_PER_USEC 10LL
#define HUNDRED_NANOSEC_PER_SEC (HUNDRED_NANOSEC_PER_USEC * 1000000LL)
    server->buildDate = (mktime(&ct) + UNIX_EPOCH_BIAS_SEC) * HUNDRED_NANOSEC_PER_SEC;
    
    /**************/
    /* References */
    /**************/
#ifndef ENABLE_GENERATE_NAMESPACE0
    /* Bootstrap by manually inserting "references" and "hassubtype" */
    UA_ReferenceTypeNode *references = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)references, "References");
    references->nodeId.identifier.numeric = UA_NS0ID_REFERENCES;
    references->isAbstract = UA_TRUE;
    references->symmetric = UA_TRUE;
    references->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "References");
    /* The reference to root is later inserted */
    UA_NodeStore_insert(server->nodestore, (UA_Node*)references, NULL);

    UA_ReferenceTypeNode *hassubtype = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hassubtype, "HasSubtype");
    hassubtype->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "HasSupertype");
    hassubtype->nodeId.identifier.numeric = UA_NS0ID_HASSUBTYPE;
    hassubtype->isAbstract = UA_FALSE;
    hassubtype->symmetric = UA_FALSE;
    /* The reference to root is later inserted */
    UA_NodeStore_insert(server->nodestore, (UA_Node*)hassubtype, NULL);

    /* Continue adding reference types with normal "addnode" */
    UA_ReferenceTypeNode *hierarchicalreferences = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hierarchicalreferences, "Hierarchicalreferences");
    hierarchicalreferences->nodeId.identifier.numeric = UA_NS0ID_HIERARCHICALREFERENCES;
    hierarchicalreferences->isAbstract = UA_TRUE;
    hierarchicalreferences->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hierarchicalreferences,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_REFERENCES), nodeIdHasSubType);

    UA_ReferenceTypeNode *nonhierarchicalreferences = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)nonhierarchicalreferences, "NonHierarchicalReferences");
    nonhierarchicalreferences->nodeId.identifier.numeric = UA_NS0ID_NONHIERARCHICALREFERENCES;
    nonhierarchicalreferences->isAbstract = UA_TRUE;
    nonhierarchicalreferences->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)nonhierarchicalreferences,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_REFERENCES), nodeIdHasSubType);

    UA_ReferenceTypeNode *haschild = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haschild, "HasChild");
    haschild->nodeId.identifier.numeric = UA_NS0ID_HASCHILD;
    haschild->isAbstract = UA_TRUE;
    haschild->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)haschild,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES), nodeIdHasSubType);

    UA_ReferenceTypeNode *organizes = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)organizes, "Organizes");
    organizes->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "OrganizedBy");
    organizes->nodeId.identifier.numeric = UA_NS0ID_ORGANIZES;
    organizes->isAbstract = UA_FALSE;
    organizes->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)organizes,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES), nodeIdHasSubType);

    UA_ReferenceTypeNode *haseventsource = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haseventsource, "HasEventSource");
    haseventsource->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "EventSourceOf");
    haseventsource->nodeId.identifier.numeric = UA_NS0ID_HASEVENTSOURCE;
    haseventsource->isAbstract = UA_FALSE;
    haseventsource->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)haseventsource,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES), nodeIdHasSubType);

    UA_ReferenceTypeNode *hasmodellingrule = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasmodellingrule, "HasModellingRule");
    hasmodellingrule->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "ModellingRuleOf");
    hasmodellingrule->nodeId.identifier.numeric = UA_NS0ID_HASMODELLINGRULE;
    hasmodellingrule->isAbstract = UA_FALSE;
    hasmodellingrule->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasmodellingrule, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *hasencoding = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasencoding, "HasEncoding");
    hasencoding->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "EncodingOf");
    hasencoding->nodeId.identifier.numeric = UA_NS0ID_HASENCODING;
    hasencoding->isAbstract = UA_FALSE;
    hasencoding->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasencoding, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *hasdescription = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasdescription, "HasDescription");
    hasdescription->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "DescriptionOf");
    hasdescription->nodeId.identifier.numeric = UA_NS0ID_HASDESCRIPTION;
    hasdescription->isAbstract = UA_FALSE;
    hasdescription->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasdescription, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *hastypedefinition = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hastypedefinition, "HasTypeDefinition");
    hastypedefinition->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "TypeDefinitionOf");
    hastypedefinition->nodeId.identifier.numeric = UA_NS0ID_HASTYPEDEFINITION;
    hastypedefinition->isAbstract = UA_FALSE;
    hastypedefinition->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hastypedefinition, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *generatesevent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)generatesevent, "GeneratesEvent");
    generatesevent->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "GeneratedBy");
    generatesevent->nodeId.identifier.numeric = UA_NS0ID_GENERATESEVENT;
    generatesevent->isAbstract = UA_FALSE;
    generatesevent->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)generatesevent, nodeIdNonHierarchicalReferences,
                    nodeIdHasSubType);

    UA_ReferenceTypeNode *aggregates = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)aggregates, "Aggregates");
    // Todo: Is there an inverse name?
    aggregates->nodeId.identifier.numeric = UA_NS0ID_AGGREGATES;
    aggregates->isAbstract = UA_TRUE;
    aggregates->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)aggregates,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_HASCHILD), nodeIdHasSubType);

    // complete bootstrap of hassubtype
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCHILD), nodeIdHasSubType,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_TRUE);

    UA_ReferenceTypeNode *hasproperty = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasproperty, "HasProperty");
    hasproperty->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "PropertyOf");
    hasproperty->nodeId.identifier.numeric = UA_NS0ID_HASPROPERTY;
    hasproperty->isAbstract = UA_FALSE;
    hasproperty->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasproperty,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_AGGREGATES), nodeIdHasSubType);

    UA_ReferenceTypeNode *hascomponent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hascomponent, "HasComponent");
    hascomponent->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "ComponentOf");
    hascomponent->nodeId.identifier.numeric = UA_NS0ID_HASCOMPONENT;
    hascomponent->isAbstract = UA_FALSE;
    hascomponent->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hascomponent,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_AGGREGATES), nodeIdHasSubType);

    UA_ReferenceTypeNode *hasnotifier = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasnotifier, "HasNotifier");
    hasnotifier->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "NotifierOf");
    hasnotifier->nodeId.identifier.numeric = UA_NS0ID_HASNOTIFIER;
    hasnotifier->isAbstract = UA_FALSE;
    hasnotifier->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasnotifier, UA_NODEID_NUMERIC(0, UA_NS0ID_HASEVENTSOURCE),
                    nodeIdHasSubType);

    UA_ReferenceTypeNode *hasorderedcomponent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasorderedcomponent, "HasOrderedComponent");
    hasorderedcomponent->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "OrderedComponentOf");
    hasorderedcomponent->nodeId.identifier.numeric = UA_NS0ID_HASORDEREDCOMPONENT;
    hasorderedcomponent->isAbstract = UA_FALSE;
    hasorderedcomponent->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasorderedcomponent, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                    nodeIdHasSubType);

    UA_ReferenceTypeNode *hasmodelparent = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hasmodelparent, "HasModelParent");
    hasmodelparent->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "ModelParentOf");
    hasmodelparent->nodeId.identifier.numeric = UA_NS0ID_HASMODELPARENT;
    hasmodelparent->isAbstract = UA_FALSE;
    hasmodelparent->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hasmodelparent, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *fromstate = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)fromstate, "FromState");
    fromstate->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "ToTransition");
    fromstate->nodeId.identifier.numeric = UA_NS0ID_FROMSTATE;
    fromstate->isAbstract = UA_FALSE;
    fromstate->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)fromstate, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *tostate = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)tostate, "ToState");
    tostate->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "FromTransition");
    tostate->nodeId.identifier.numeric = UA_NS0ID_TOSTATE;
    tostate->isAbstract = UA_FALSE;
    tostate->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)tostate, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *hascause = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hascause, "HasCause");
    hascause->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "MayBeCausedBy");
    hascause->nodeId.identifier.numeric = UA_NS0ID_HASCAUSE;
    hascause->isAbstract = UA_FALSE;
    hascause->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hascause, nodeIdNonHierarchicalReferences, nodeIdHasSubType);
    
    UA_ReferenceTypeNode *haseffect = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)haseffect, "HasEffect");
    haseffect->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "MayBeEffectedBy");
    haseffect->nodeId.identifier.numeric = UA_NS0ID_HASEFFECT;
    haseffect->isAbstract = UA_FALSE;
    haseffect->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)haseffect, nodeIdNonHierarchicalReferences, nodeIdHasSubType);

    UA_ReferenceTypeNode *hashistoricalconfiguration = UA_ReferenceTypeNode_new();
    copyNames((UA_Node*)hashistoricalconfiguration, "HasHistoricalConfiguration");
    hashistoricalconfiguration->inverseName = UA_LOCALIZEDTEXT_ALLOC("en_US", "HistoricalConfigurationOf");
    hashistoricalconfiguration->nodeId.identifier.numeric = UA_NS0ID_HASHISTORICALCONFIGURATION;
    hashistoricalconfiguration->isAbstract = UA_FALSE;
    hashistoricalconfiguration->symmetric  = UA_FALSE;
    addNodeInternal(server, (UA_Node*)hashistoricalconfiguration,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_AGGREGATES), nodeIdHasSubType);

    /*****************/
    /* Basic Folders */
    /*****************/

    UA_ObjectNode *root = UA_ObjectNode_new();
    copyNames((UA_Node*)root, "Root");
    root->nodeId.identifier.numeric = UA_NS0ID_ROOTFOLDER;
    UA_NodeStore_insert(server->nodestore, (UA_Node*)root, NULL);

    UA_ObjectNode *objects = UA_ObjectNode_new();
    copyNames((UA_Node*)objects, "Objects");
    objects->nodeId.identifier.numeric = UA_NS0ID_OBJECTSFOLDER;
    addNodeInternal(server, (UA_Node*)objects, UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
                    nodeIdOrganizes);

    UA_ObjectNode *types = UA_ObjectNode_new();
    copyNames((UA_Node*)types, "Types");
    types->nodeId.identifier.numeric = UA_NS0ID_TYPESFOLDER;
    addNodeInternal(server, (UA_Node*)types, UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
                    nodeIdOrganizes);

    UA_ObjectNode *views = UA_ObjectNode_new();
    copyNames((UA_Node*)views, "Views");
    views->nodeId.identifier.numeric = UA_NS0ID_VIEWSFOLDER;
    addNodeInternal(server, (UA_Node*)views, UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER),
                    nodeIdOrganizes);

    UA_ObjectNode *referencetypes = UA_ObjectNode_new();
    copyNames((UA_Node*)referencetypes, "ReferenceTypes");
    referencetypes->nodeId.identifier.numeric = UA_NS0ID_REFERENCETYPESFOLDER;
    addNodeInternal(server, (UA_Node*)referencetypes, UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                    nodeIdOrganizes);

    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_REFERENCETYPESFOLDER), nodeIdOrganizes,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_REFERENCES), UA_TRUE);

    /**********************/
    /* Basic Object Types */
    /**********************/

    UA_ObjectNode *objecttypes = UA_ObjectNode_new();
    copyNames((UA_Node*)objecttypes, "ObjectTypes");
    objecttypes->nodeId.identifier.numeric = UA_NS0ID_OBJECTTYPESFOLDER;
    addNodeInternal(server, (UA_Node*)objecttypes, UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                    nodeIdOrganizes);

    addObjectTypeNode(server, "BaseObjectType", UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_OBJECTTYPESFOLDER,
                      UA_NS0ID_ORGANIZES);
    addObjectTypeNode(server, "FolderType", UA_NS0ID_FOLDERTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTTYPESFOLDER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_VIEWSFOLDER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_REFERENCETYPESFOLDER),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    addObjectTypeNode(server, "ServerType", UA_NS0ID_SERVERTYPE, UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "ServerDiagnosticsType", UA_NS0ID_SERVERDIAGNOSTICSTYPE,
                      UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "ServerCapatilitiesType", UA_NS0ID_SERVERCAPABILITIESTYPE,
                      UA_NS0ID_BASEOBJECTTYPE, UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "ServerStatusType", UA_NS0ID_SERVERSTATUSTYPE, UA_NS0ID_BASEOBJECTTYPE,
                      UA_NS0ID_HASSUBTYPE);
    addObjectTypeNode(server, "BuildInfoType", UA_NS0ID_BUILDINFOTYPE, UA_NS0ID_BASEOBJECTTYPE,
                      UA_NS0ID_HASSUBTYPE);

    /**************/
    /* Data Types */
    /**************/

    UA_ObjectNode *datatypes = UA_ObjectNode_new();
    copyNames((UA_Node*)datatypes, "DataTypes");
    datatypes->nodeId.identifier.numeric = UA_NS0ID_DATATYPESFOLDER;
    addNodeInternal(server, (UA_Node*)datatypes, UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER), nodeIdOrganizes);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_DATATYPESFOLDER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);

    addDataTypeNode(server, "BaseDataType", UA_NS0ID_BASEDATATYPE, UA_NS0ID_DATATYPESFOLDER);
    addDataTypeNode(server, "Boolean", UA_NS0ID_BOOLEAN, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Number", UA_NS0ID_NUMBER, UA_NS0ID_BASEDATATYPE);
        addDataTypeNode(server, "Float", UA_NS0ID_FLOAT, UA_NS0ID_NUMBER);
        addDataTypeNode(server, "Double", UA_NS0ID_DOUBLE, UA_NS0ID_NUMBER);
        addDataTypeNode(server, "Integer", UA_NS0ID_INTEGER, UA_NS0ID_NUMBER);
            addDataTypeNode(server, "SByte", UA_NS0ID_SBYTE, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "Int16", UA_NS0ID_INT16, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "Int32", UA_NS0ID_INT32, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "Int64", UA_NS0ID_INT64, UA_NS0ID_INTEGER);
            addDataTypeNode(server, "UInteger", UA_NS0ID_UINTEGER, UA_NS0ID_INTEGER);
                addDataTypeNode(server, "Byte", UA_NS0ID_BYTE, UA_NS0ID_UINTEGER);
                addDataTypeNode(server, "UInt16", UA_NS0ID_UINT16, UA_NS0ID_UINTEGER);
                addDataTypeNode(server, "UInt32", UA_NS0ID_UINT32, UA_NS0ID_UINTEGER);
                addDataTypeNode(server, "UInt64", UA_NS0ID_UINT64, UA_NS0ID_UINTEGER);
    addDataTypeNode(server, "String", UA_NS0ID_STRING, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DateTime", UA_NS0ID_DATETIME, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Guid", UA_NS0ID_GUID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "ByteString", UA_NS0ID_BYTESTRING, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "XmlElement", UA_NS0ID_XMLELEMENT, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "NodeId", UA_NS0ID_NODEID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "ExpandedNodeId", UA_NS0ID_EXPANDEDNODEID, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "StatusCode", UA_NS0ID_STATUSCODE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "QualifiedName", UA_NS0ID_QUALIFIEDNAME, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "LocalizedText", UA_NS0ID_LOCALIZEDTEXT, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Structure", UA_NS0ID_STRUCTURE, UA_NS0ID_BASEDATATYPE);
        addDataTypeNode(server, "ServerStatusDataType", UA_NS0ID_SERVERSTATUSDATATYPE, UA_NS0ID_STRUCTURE);
        addDataTypeNode(server, "BuildInfo", UA_NS0ID_BUILDINFO, UA_NS0ID_STRUCTURE);
    addDataTypeNode(server, "DataValue", UA_NS0ID_DATAVALUE, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "DiagnosticInfo", UA_NS0ID_DIAGNOSTICINFO, UA_NS0ID_BASEDATATYPE);
    addDataTypeNode(server, "Enumeration", UA_NS0ID_ENUMERATION, UA_NS0ID_BASEDATATYPE);
        addDataTypeNode(server, "ServerState", UA_NS0ID_SERVERSTATE, UA_NS0ID_ENUMERATION);

    UA_ObjectNode *variabletypes = UA_ObjectNode_new();
    copyNames((UA_Node*)variabletypes, "VariableTypes");
    variabletypes->nodeId.identifier.numeric = UA_NS0ID_VARIABLETYPESFOLDER;
    addNodeInternal(server, (UA_Node*)variabletypes, UA_NODEID_NUMERIC(0, UA_NS0ID_TYPESFOLDER),
                    nodeIdOrganizes);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_VARIABLETYPESFOLDER),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE), UA_TRUE);
    addVariableTypeNode_organized(server, "BaseVariableType", UA_NS0ID_BASEVARIABLETYPE,
                                  UA_NS0ID_VARIABLETYPESFOLDER, UA_TRUE);
    addVariableTypeNode_subtype(server, "BaseDataVariableType", UA_NS0ID_BASEDATAVARIABLETYPE,
                                UA_NS0ID_BASEVARIABLETYPE, UA_FALSE);
    addVariableTypeNode_subtype(server, "PropertyType", UA_NS0ID_PROPERTYTYPE,
                                UA_NS0ID_BASEVARIABLETYPE, UA_FALSE);
#endif

#ifdef ENABLE_GENERATE_NAMESPACE0
    //load the generated namespace
    ua_namespaceinit_generated(server);
#endif

    /*********************/
    /* The Server Object */
    /*********************/

    UA_ObjectNode *servernode = UA_ObjectNode_new();
    copyNames((UA_Node*)servernode, "Server");
    servernode->nodeId.identifier.numeric = UA_NS0ID_SERVER;
    addNodeInternal(server, (UA_Node*)servernode, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                    nodeIdOrganizes);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERTYPE), UA_TRUE);

    UA_VariableNode *namespaceArray = UA_VariableNode_new();
    copyNames((UA_Node*)namespaceArray, "NamespaceArray");
    namespaceArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_NAMESPACEARRAY;
    namespaceArray->valueSource = UA_VALUESOURCE_DATASOURCE;
    namespaceArray->value.dataSource = (UA_DataSource) {.handle = server, .read = readNamespaces,
                                                        .write = NULL};
    namespaceArray->valueRank = 1;
    namespaceArray->minimumSamplingInterval = 1.0;
    addNodeInternal(server, (UA_Node*)namespaceArray, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), nodeIdHasProperty);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE),
                           UA_TRUE);

    UA_VariableNode *serverArray = UA_VariableNode_new();
    copyNames((UA_Node*)serverArray, "ServerArray");
    serverArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERARRAY;
    serverArray->value.variant.value.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 1);
    serverArray->value.variant.value.arrayLength = 1;
    serverArray->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    *(UA_String *)serverArray->value.variant.value.data =
        UA_STRING_ALLOC(server->config.Application_applicationURI);
    serverArray->valueRank = 1;
    serverArray->minimumSamplingInterval = 1.0;
    addNodeInternal(server, (UA_Node*)serverArray, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), nodeIdHasProperty);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERARRAY), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), UA_TRUE);

    UA_ObjectNode *servercapablities = UA_ObjectNode_new();
    copyNames((UA_Node*)servercapablities, "ServerCapabilities");
    servercapablities->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES;
    addNodeInternal(server, (UA_Node*)servercapablities, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
                    nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES),
                           nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERCAPABILITIESTYPE), UA_TRUE);

    UA_VariableNode *localeIdArray = UA_VariableNode_new();
    copyNames((UA_Node*)localeIdArray, "LocaleIdArray");
    localeIdArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY;
    localeIdArray->value.variant.value.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], 1);
    localeIdArray->value.variant.value.arrayLength = 1;
    localeIdArray->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    *(UA_String *)localeIdArray->value.variant.value.data = UA_STRING_ALLOC("en");
    localeIdArray->valueRank = 1;
    localeIdArray->minimumSamplingInterval = 1.0;
    addNodeInternal(server, (UA_Node*)localeIdArray,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES), nodeIdHasProperty);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES_LOCALEIDARRAY),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), UA_TRUE);

    UA_VariableNode *maxBrowseContinuationPoints = UA_VariableNode_new();
    copyNames((UA_Node*)maxBrowseContinuationPoints, "MaxBrowseContinuationPoints");
    maxBrowseContinuationPoints->nodeId.identifier.numeric =
        UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXBROWSECONTINUATIONPOINTS;
    maxBrowseContinuationPoints->value.variant.value.data = UA_UInt16_new();
    *((UA_UInt16*)maxBrowseContinuationPoints->value.variant.value.data) = MAXCONTINUATIONPOINTS;
    maxBrowseContinuationPoints->value.variant.value.type = &UA_TYPES[UA_TYPES_UINT16];
    addNodeInternal(server, (UA_Node*)maxBrowseContinuationPoints,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES), nodeIdHasProperty);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES_MAXBROWSECONTINUATIONPOINTS),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), UA_TRUE);

    /** ServerProfileArray **/
#define MAX_PROFILEARRAY 16 //a *magic* limit to the number of supported profiles
#define ADDPROFILEARRAY(x) profileArray[profileArraySize++] = UA_STRING_ALLOC(x)
    UA_String profileArray[MAX_PROFILEARRAY];
    UA_UInt16 profileArraySize = 0;
    ADDPROFILEARRAY("http://opcfoundation.org/UA-Profile/Server/NanoEmbeddedDevice");

#ifdef ENABLE_SERVICESET_NODEMANAGEMENT
    ADDPROFILEARRAY("http://opcfoundation.org/UA-Profile/Server/NodeManagement");
#endif
#ifdef ENABLE_SERVICESET_METHOD
    ADDPROFILEARRAY("http://opcfoundation.org/UA-Profile/Server/Methods");
#endif
#ifdef ENABLE_SUBSCRIPTIONS
    ADDPROFILEARRAY("http://opcfoundation.org/UA-Profile/Server/EmbeddedDataChangeSubscription");
#endif

    UA_VariableNode *serverProfileArray = UA_VariableNode_new();
    copyNames((UA_Node*)serverProfileArray, "ServerProfileArray");
    serverProfileArray->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERCAPABILITIES_SERVERPROFILEARRAY;
    serverProfileArray->value.variant.value.arrayLength = profileArraySize;
    serverProfileArray->value.variant.value.data = UA_Array_new(&UA_TYPES[UA_TYPES_STRING], profileArraySize);
    serverProfileArray->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    for(UA_UInt16 i=0;i<profileArraySize;i++)
        ((UA_String *)serverProfileArray->value.variant.value.data)[i] = profileArray[i];
    serverProfileArray->valueRank = 1;
    serverProfileArray->minimumSamplingInterval = 1.0;
    addNodeInternal(server, (UA_Node*)serverProfileArray,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES), nodeIdHasProperty);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERCAPABILITIES_SERVERPROFILEARRAY),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE), UA_TRUE);

    UA_ObjectNode *serverdiagnostics = UA_ObjectNode_new();
    copyNames((UA_Node*)serverdiagnostics, "ServerDiagnostics");
    serverdiagnostics->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERDIAGNOSTICS;
    addNodeInternal(server, (UA_Node*)serverdiagnostics,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERDIAGNOSTICS),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERDIAGNOSTICSTYPE), UA_TRUE);

    UA_VariableNode *enabledFlag = UA_VariableNode_new();
    copyNames((UA_Node*)enabledFlag, "EnabledFlag");
    enabledFlag->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERDIAGNOSTICS_ENABLEDFLAG;
    enabledFlag->value.variant.value.data = UA_Boolean_new(); //initialized as false
    enabledFlag->value.variant.value.type = &UA_TYPES[UA_TYPES_BOOLEAN];
    enabledFlag->valueRank = 1;
    enabledFlag->minimumSamplingInterval = 1.0;
    addNodeInternal(server, (UA_Node*)enabledFlag,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERDIAGNOSTICS), nodeIdHasProperty);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERDIAGNOSTICS_ENABLEDFLAG),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_PROPERTYTYPE),
                           UA_TRUE);

    UA_VariableNode *serverstatus = UA_VariableNode_new();
    copyNames((UA_Node*)serverstatus, "ServerStatus");
    serverstatus->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS);
    serverstatus->valueSource = UA_VALUESOURCE_DATASOURCE;
    serverstatus->value.dataSource = (UA_DataSource) {.handle = server, .read = readStatus, .write = NULL};
    addNodeInternal(server, (UA_Node*)serverstatus, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), nodeIdHasTypeDefinition,
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_SERVERSTATUSTYPE), UA_TRUE);

    UA_VariableNode *starttime = UA_VariableNode_new();
    copyNames((UA_Node*)starttime, "StartTime");
    starttime->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STARTTIME);
    starttime->value.variant.value.storageType = UA_VARIANT_DATA_NODELETE;
    starttime->value.variant.value.data = &server->startTime;
    starttime->value.variant.value.type = &UA_TYPES[UA_TYPES_DATETIME];
    addNodeInternal(server, (UA_Node*)starttime, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
                    nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STARTTIME),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *currenttime = UA_VariableNode_new();
    copyNames((UA_Node*)currenttime, "CurrentTime");
    currenttime->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    currenttime->valueSource = UA_VALUESOURCE_DATASOURCE;
    currenttime->value.dataSource = (UA_DataSource) {.handle = NULL, .read = readCurrentTime,
                                                     .write = NULL};
    addNodeInternal(server, (UA_Node*)currenttime,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *state = UA_VariableNode_new();
    UA_ServerState *stateEnum = UA_ServerState_new();
    *stateEnum = UA_SERVERSTATE_RUNNING;
    copyNames((UA_Node*)state, "State");
    state->nodeId.identifier.numeric = UA_NS0ID_SERVER_SERVERSTATUS_STATE;
    state->value.variant.value.type = &UA_TYPES[UA_TYPES_SERVERSTATE];
    state->value.variant.value.arrayLength = -1;
    state->value.variant.value.data = stateEnum; // points into the other object.
    addNodeInternal(server, (UA_Node*)state, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS),
                    nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STATE),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *buildinfo = UA_VariableNode_new();
    copyNames((UA_Node*)buildinfo, "BuildInfo");
    buildinfo->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO);
    buildinfo->value.variant.value.data = UA_BuildInfo_new();
    buildinfo->value.variant.value.type = &UA_TYPES[UA_TYPES_BUILDINFO];
    getBuildInfo(server, (UA_BuildInfo*)buildinfo->value.variant.value.data);
    addNodeInternal(server, (UA_Node*)buildinfo,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO),
                           nodeIdHasTypeDefinition, UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_BUILDINFOTYPE),
                           UA_TRUE);

    UA_VariableNode *producturi = UA_VariableNode_new();
    copyNames((UA_Node*)producturi, "ProductUri");
    producturi->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTURI);
    producturi->value.variant.value.data = UA_String_new();
    *((UA_String*)producturi->value.variant.value.data) = UA_STRING_ALLOC(PRODUCT_URI);
    producturi->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    addNodeInternal(server, (UA_Node*)producturi,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTURI),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *manufacturername = UA_VariableNode_new();
    copyNames((UA_Node*)manufacturername, "ManufacturererName");
    manufacturername->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_MANUFACTURERNAME);
    manufacturername->value.variant.value.data = UA_String_new();
    *((UA_String*)manufacturername->value.variant.value.data) = UA_STRING_ALLOC(MANUFACTURER_NAME);
    manufacturername->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    addNodeInternal(server, (UA_Node*)manufacturername,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), nodeIdHasComponent);
    UA_Server_addReference(server,
                           UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_MANUFACTURERNAME),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *productname = UA_VariableNode_new();
    copyNames((UA_Node*)productname, "ProductName");
    productname->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTNAME);
    productname->value.variant.value.data = UA_String_new();
    *((UA_String*)productname->value.variant.value.data) = UA_STRING_ALLOC(PRODUCT_NAME);
    productname->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    addNodeInternal(server, (UA_Node*)productname,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_PRODUCTNAME),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *softwareversion = UA_VariableNode_new();
    copyNames((UA_Node*)softwareversion, "SoftwareVersion");
    softwareversion->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_SOFTWAREVERSION);
    softwareversion->value.variant.value.data = UA_String_new();
    *((UA_String*)softwareversion->value.variant.value.data) = UA_STRING_ALLOC(SOFTWARE_VERSION);
    softwareversion->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    addNodeInternal(server, (UA_Node*)softwareversion,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_SOFTWAREVERSION),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *buildnumber = UA_VariableNode_new();
    copyNames((UA_Node*)buildnumber, "BuildNumber");
    buildnumber->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER);
    buildnumber->value.variant.value.data = UA_String_new();
    *((UA_String*)buildnumber->value.variant.value.data) = UA_STRING_ALLOC(BUILD_NUMBER);
    buildnumber->value.variant.value.type = &UA_TYPES[UA_TYPES_STRING];
    addNodeInternal(server, (UA_Node*)buildnumber,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *builddate = UA_VariableNode_new();
    copyNames((UA_Node*)builddate, "BuildDate");
    builddate->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDDATE);
    builddate->value.variant.value.storageType = UA_VARIANT_DATA_NODELETE;
    builddate->value.variant.value.data = &server->buildDate;
    builddate->value.variant.value.type = &UA_TYPES[UA_TYPES_DATETIME];
    addNodeInternal(server, (UA_Node*)builddate,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_BUILDINFO_BUILDNUMBER),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *secondstillshutdown = UA_VariableNode_new();
    copyNames((UA_Node*)secondstillshutdown, "SecondsTillShutdown");
    secondstillshutdown->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SECONDSTILLSHUTDOWN);
    secondstillshutdown->value.variant.value.data = UA_UInt32_new();
    secondstillshutdown->value.variant.value.type = &UA_TYPES[UA_TYPES_UINT32];
    addNodeInternal(server, (UA_Node*)secondstillshutdown,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SECONDSTILLSHUTDOWN),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);

    UA_VariableNode *shutdownreason = UA_VariableNode_new();
    copyNames((UA_Node*)shutdownreason, "ShutdownReason");
    shutdownreason->nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SHUTDOWNREASON);
    shutdownreason->value.variant.value.data = UA_LocalizedText_new();
    shutdownreason->value.variant.value.type = &UA_TYPES[UA_TYPES_LOCALIZEDTEXT];
    addNodeInternal(server, (UA_Node*)shutdownreason,
                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS), nodeIdHasComponent);
    UA_Server_addReference(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_SHUTDOWNREASON),
                           nodeIdHasTypeDefinition, expandedNodeIdBaseDataVariabletype, UA_TRUE);
    return server;
}

UA_StatusCode
__UA_Server_writeAttribute(UA_Server *server, const UA_NodeId nodeId,
                             const UA_AttributeId attributeId, const UA_DataType *type,
                             const void *value) {
    UA_WriteValue wvalue;
    UA_WriteValue_init(&wvalue);
    wvalue.nodeId = nodeId;
    wvalue.attributeId = attributeId;
    if(attributeId != UA_ATTRIBUTEID_VALUE)
        UA_Variant_setScalarCopy(&wvalue.value.value, value, type);
    else
        UA_Variant_copy(value, &wvalue.value.value);
    wvalue.value.hasValue = UA_TRUE;
    UA_StatusCode retval = Service_Write_single(server, &adminSession, &wvalue);
    UA_NodeId_init(&wvalue.nodeId);
    UA_WriteValue_deleteMembers(&wvalue);
    return retval;
}

UA_StatusCode
UA_Server_writeValueAttribute_move(UA_Server *server, const UA_NodeId nodeId, UA_Variant *value) {
    UA_WriteValue wvalue;
    UA_WriteValue_init(&wvalue);
    wvalue.nodeId = nodeId;
    wvalue.attributeId = UA_ATTRIBUTEID_VALUE;
    wvalue.value.value = *value;
    UA_Variant_init(value);
    wvalue.value.hasValue = UA_TRUE;
    UA_StatusCode retval = Service_Write_single(server, &adminSession, &wvalue);
    UA_NodeId_init(&wvalue.nodeId);
    UA_WriteValue_deleteMembers(&wvalue);
    return retval;
}

static UA_StatusCode setValueCallback(UA_Server *server, UA_Session *session, UA_VariableNode *node,
                                      UA_ValueCallback *callback) {
    if(node->nodeClass != UA_NODECLASS_VARIABLE)
        return UA_STATUSCODE_BADNODECLASSINVALID;
    node->value.variant.callback = *callback;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_EXPORT
UA_Server_setVariableNode_valueCallback(UA_Server *server, const UA_NodeId nodeId,
                                        const UA_ValueCallback callback) {
    return UA_Server_editNode(server, &adminSession, &nodeId,
                              (UA_EditNodeCallback)setValueCallback, &callback);
}

static UA_StatusCode
setDataSource(UA_Server *server, UA_Session *session,
              UA_VariableNode* node, UA_DataSource *dataSource) {
    if(node->nodeClass != UA_NODECLASS_VARIABLE)
        return UA_STATUSCODE_BADNODECLASSINVALID;
    if(node->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&node->value.variant.value);
    node->value.dataSource = *dataSource;
    node->valueSource = UA_VALUESOURCE_DATASOURCE;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_Server_setVariableNode_dataSource(UA_Server *server, const UA_NodeId nodeId,
                                     const UA_DataSource dataSource) {
    return UA_Server_editNode(server, &adminSession, &nodeId,
                              (UA_EditNodeCallback)setDataSource, &dataSource);
}

static UA_StatusCode
setObjectTypeLifecycleManagement(UA_Server *server, UA_Session *session, UA_ObjectTypeNode* node,
                                 UA_ObjectLifecycleManagement *olm) {
    if(node->nodeClass != UA_NODECLASS_OBJECTTYPE)
        return UA_STATUSCODE_BADNODECLASSINVALID;
    node->lifecycleManagement = *olm;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_EXPORT
UA_Server_setObjectTypeNode_instanceLifecycleManagement(UA_Server *server, UA_NodeId nodeId,
                                                        UA_ObjectLifecycleManagement olm) {
    return UA_Server_editNode(server, &adminSession, &nodeId,
                              (UA_EditNodeCallback)setObjectTypeLifecycleManagement, &olm);
}

UA_StatusCode
__UA_Server_readAttribute(UA_Server *server, const UA_NodeId nodeId,
                          const UA_AttributeId attributeId, void *v) {
    UA_ReadValueId item;
    UA_ReadValueId_init(&item);
    item.nodeId = nodeId;
    item.attributeId = attributeId;
    UA_DataValue dv;
    UA_DataValue_init(&dv);
    Service_Read_single(server, &adminSession, UA_TIMESTAMPSTORETURN_NEITHER,
                        &item, &dv);
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(dv.hasStatus)
        retval = dv.hasStatus;
    else if(!dv.hasValue)
        retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_DataValue_deleteMembers(&dv);
        return retval;
    }
    if(attributeId == UA_ATTRIBUTEID_VALUE)
        memcpy(v, &dv.value, sizeof(UA_Variant));
    else {
        memcpy(v, dv.value.data, dv.value.type->memSize);
        dv.value.data = NULL;
        dv.value.arrayLength = -1;
        UA_Variant_deleteMembers(&dv.value);
    }
    return UA_STATUSCODE_GOOD;
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_server_binary.c" ***********************************/


/** Max size of messages that are allocated on the stack */
#define MAX_STACK_MESSAGE 65536

static void processHEL(UA_Connection *connection, const UA_ByteString *msg, size_t *pos) {
    UA_TcpHelloMessage helloMessage;
    if(UA_TcpHelloMessage_decodeBinary(msg, pos, &helloMessage) != UA_STATUSCODE_GOOD) {
        connection->close(connection);
        return;
    }

    connection->remoteConf.maxChunkCount = helloMessage.maxChunkCount;
    connection->remoteConf.maxMessageSize = helloMessage.maxMessageSize;
    connection->remoteConf.protocolVersion = helloMessage.protocolVersion;
    connection->remoteConf.recvBufferSize = helloMessage.receiveBufferSize;
    if(connection->localConf.sendBufferSize > helloMessage.receiveBufferSize)
        connection->localConf.sendBufferSize = helloMessage.receiveBufferSize;
    if(connection->localConf.recvBufferSize > helloMessage.sendBufferSize)
        connection->localConf.recvBufferSize = helloMessage.sendBufferSize;
    connection->remoteConf.sendBufferSize = helloMessage.sendBufferSize;
    connection->state = UA_CONNECTION_ESTABLISHED;
    UA_TcpHelloMessage_deleteMembers(&helloMessage);

    // build acknowledge response
    UA_TcpAcknowledgeMessage ackMessage;
    ackMessage.protocolVersion = connection->localConf.protocolVersion;
    ackMessage.receiveBufferSize = connection->localConf.recvBufferSize;
    ackMessage.sendBufferSize = connection->localConf.sendBufferSize;
    ackMessage.maxMessageSize = connection->localConf.maxMessageSize;
    ackMessage.maxChunkCount = connection->localConf.maxChunkCount;

    UA_TcpMessageHeader ackHeader;
    ackHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_ACKF;
    ackHeader.messageSize =  8 + 20; /* ackHeader + ackMessage */

    UA_ByteString ack_msg;
    if(connection->getSendBuffer(connection, connection->remoteConf.recvBufferSize,
                                 &ack_msg) != UA_STATUSCODE_GOOD)
        return;

    size_t tmpPos = 0;
    UA_TcpMessageHeader_encodeBinary(&ackHeader, &ack_msg, &tmpPos);
    UA_TcpAcknowledgeMessage_encodeBinary(&ackMessage, &ack_msg, &tmpPos);
    ack_msg.length = ackHeader.messageSize;
    connection->send(connection, &ack_msg);
}

static void processOPN(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg, size_t *pos) {
    if(connection->state != UA_CONNECTION_ESTABLISHED) {
        connection->close(connection);
        return;
    }

    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);

    UA_AsymmetricAlgorithmSecurityHeader asymHeader;
    retval |= UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(msg, pos, &asymHeader);

    UA_SequenceHeader seqHeader;
    retval |= UA_SequenceHeader_decodeBinary(msg, pos, &seqHeader);

    UA_NodeId requestType;
    retval |= UA_NodeId_decodeBinary(msg, pos, &requestType);

    UA_OpenSecureChannelRequest r;
    retval |= UA_OpenSecureChannelRequest_decodeBinary(msg, pos, &r);

    if(retval != UA_STATUSCODE_GOOD || requestType.identifier.numeric != 446) {
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_SequenceHeader_deleteMembers(&seqHeader);
        UA_NodeId_deleteMembers(&requestType);
        UA_OpenSecureChannelRequest_deleteMembers(&r);
        connection->close(connection);
        return;
    }

    UA_OpenSecureChannelResponse p;
    UA_OpenSecureChannelResponse_init(&p);
    Service_OpenSecureChannel(server, connection, &r, &p);
    UA_OpenSecureChannelRequest_deleteMembers(&r);

    UA_SecureChannel *channel = connection->channel;
    if(!channel) {
        connection->close(connection);
        UA_OpenSecureChannelResponse_deleteMembers(&p);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        return;
    }

    /* send the response with an asymmetric security header */
#ifndef UA_MULTITHREADING
    seqHeader.sequenceNumber = ++channel->sequenceNumber;
#else
    seqHeader.sequenceNumber = uatomic_add_return(&channel->sequenceNumber, 1);
#endif

    UA_SecureConversationMessageHeader respHeader;
    respHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_OPNF;
    respHeader.messageHeader.messageSize = 0;
    respHeader.secureChannelId = p.securityToken.channelId;

    UA_NodeId responseType = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELRESPONSE +
                                               UA_ENCODINGOFFSET_BINARY);

    UA_ByteString resp_msg;
    retval = connection->getSendBuffer(connection, connection->remoteConf.recvBufferSize, &resp_msg);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_OpenSecureChannelResponse_deleteMembers(&p);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        return;
    }
        
    size_t tmpPos = 12; /* skip the secureconversationmessageheader for now */
    retval |= UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(&asymHeader, &resp_msg, &tmpPos); // just mirror back
    retval |= UA_SequenceHeader_encodeBinary(&seqHeader, &resp_msg, &tmpPos); // just mirror back
    retval |= UA_NodeId_encodeBinary(&responseType, &resp_msg, &tmpPos);
    retval |= UA_OpenSecureChannelResponse_encodeBinary(&p, &resp_msg, &tmpPos);

    if(retval != UA_STATUSCODE_GOOD) {
        connection->releaseSendBuffer(connection, &resp_msg);
        connection->close(connection);
    } else {
        respHeader.messageHeader.messageSize = tmpPos;
        tmpPos = 0;
        UA_SecureConversationMessageHeader_encodeBinary(&respHeader, &resp_msg, &tmpPos);
        resp_msg.length = respHeader.messageHeader.messageSize;
        connection->send(connection, &resp_msg);
    }

    UA_OpenSecureChannelResponse_deleteMembers(&p);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
}

static void init_response_header(const UA_RequestHeader *p, UA_ResponseHeader *r) {
    r->requestHandle = p->requestHandle;
    r->stringTableSize = 0;
    r->timestamp = UA_DateTime_now();
}

/* The request/response are casted to the header (first element of their struct) */
static void invoke_service(UA_Server *server, UA_SecureChannel *channel, UA_UInt32 requestId,
                           UA_RequestHeader *request, const UA_DataType *responseType,
                           void (*service)(UA_Server*, UA_Session*, void*, void*)) {
    UA_ResponseHeader *response = UA_alloca(responseType->memSize);
    UA_init(response, responseType);
    init_response_header(request, response);
    /* try to get the session from the securechannel first */
    UA_Session *session = UA_SecureChannel_getSession(channel, &request->authenticationToken);
#ifdef EXTENSION_STATELESS
    if(request->authenticationToken.namespaceIndex == 0
            && request->authenticationToken.identifierType == UA_NODEIDTYPE_NUMERIC
            && request->authenticationToken.identifier.numeric == 0
    && (responseType->typeIndex == UA_TYPES_READRESPONSE
            || responseType->typeIndex == UA_TYPES_WRITERESPONSE
            || responseType->typeIndex == UA_TYPES_BROWSERESPONSE)
    ){
        session = &anonymousSession;
        service(server, session, request, response);
    }else{
#endif
    if(!session || session->channel != channel) {
        response->serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
    } else if(session->activated == UA_FALSE) {
        response->serviceResult = UA_STATUSCODE_BADSESSIONNOTACTIVATED;
        /* /\* the session is invalidated FIXME: do this delayed*\/ */
        /* UA_SessionManager_removeSession(&server->sessionManager, server, &request->authenticationToken); */
    } else {
        UA_Session_updateLifetime(session);
        service(server, session, request, response);
    }
#ifdef EXTENSION_STATELESS
    }
#endif
    UA_StatusCode retval = UA_SecureChannel_sendBinaryMessage(channel, requestId, response, responseType);
    if(retval != UA_STATUSCODE_GOOD) {
        if(retval == UA_STATUSCODE_BADENCODINGLIMITSEXCEEDED)
            response->serviceResult = UA_STATUSCODE_BADRESPONSETOOLARGE;
        else
            response->serviceResult = retval;
        UA_SecureChannel_sendBinaryMessage(channel, requestId, response, &UA_TYPES[UA_TYPES_SERVICEFAULT]);
    }
    UA_deleteMembers(response, responseType);
}

#define INVOKE_SERVICE(REQUEST, RESPONSETYPE) do {                      \
        UA_##REQUEST##Request p;                                        \
        if(UA_##REQUEST##Request_decodeBinary(msg, pos, &p))            \
            return;                                                     \
        invoke_service(server, clientChannel, sequenceHeader.requestId, \
                       &p.requestHeader, &UA_TYPES[RESPONSETYPE],       \
                       (void (*)(UA_Server*, UA_Session*, void*,void*))Service_##REQUEST); \
        UA_##REQUEST##Request_deleteMembers(&p);                        \
} while(0)

static void processMSG(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg, size_t *pos) {
    /* Read in the securechannel */
    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);
    if(retval != UA_STATUSCODE_GOOD)
        return;

    /* the anonymous channel is used e.g. to allow getEndpoints without a channel */
    UA_SecureChannel *clientChannel = connection->channel;
    UA_SecureChannel anonymousChannel;
    if(!clientChannel) {
        UA_SecureChannel_init(&anonymousChannel);
        anonymousChannel.connection = connection;
        clientChannel = &anonymousChannel;
    }

    /* Read the security header */
    UA_UInt32 tokenId = 0;
    UA_SequenceHeader sequenceHeader;
    retval = UA_UInt32_decodeBinary(msg, pos, &tokenId);
    retval |= UA_SequenceHeader_decodeBinary(msg, pos, &sequenceHeader);
#ifndef EXTENSION_STATELESS
    if(retval != UA_STATUSCODE_GOOD || tokenId == 0) // 0 is invalid
        return;
#else
    if(retval != UA_STATUSCODE_GOOD)
        return;
#endif

    if(clientChannel != &anonymousChannel && tokenId != clientChannel->securityToken.tokenId) {
        if(tokenId != clientChannel->nextSecurityToken.tokenId) {
            /* close the securechannel but keep the connection open */
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                        "Request with a wrong security token. Closing the SecureChannel %i.",
                        clientChannel->securityToken.channelId);
            Service_CloseSecureChannel(server, clientChannel->securityToken.channelId);
            return;
        }
        UA_SecureChannel_revolveTokens(clientChannel);
    }

    /* Read the request type */
    UA_NodeId requestType;
    if(UA_NodeId_decodeBinary(msg, pos, &requestType) != UA_STATUSCODE_GOOD)
        return;
    if(requestType.identifierType != UA_NODEIDTYPE_NUMERIC) {
        UA_NodeId_deleteMembers(&requestType);
        return;
    }

    switch(requestType.identifier.numeric - UA_ENCODINGOFFSET_BINARY) {
    case UA_NS0ID_GETENDPOINTSREQUEST: {
        if(clientChannel == &anonymousChannel)
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_NETWORK, "Processing GetEndpointsRequest on Connection %i",
                         connection->sockfd);
        else
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL, "Processing GetEndpointsRequest on SecureChannel %i",
                         clientChannel->securityToken.channelId);
        UA_GetEndpointsRequest p;
        UA_GetEndpointsResponse r;
        if(UA_GetEndpointsRequest_decodeBinary(msg, pos, &p))
            return;
        UA_GetEndpointsResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_GetEndpoints(server, &p, &r);
        UA_GetEndpointsRequest_deleteMembers(&p);
        UA_SecureChannel_sendBinaryMessage(clientChannel, sequenceHeader.requestId, &r,
                                           &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE]);
        UA_GetEndpointsResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_FINDSERVERSREQUEST: {
        if(clientChannel == &anonymousChannel)
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_NETWORK, "Processing FindServerRequest on Connection %i",
                         connection->sockfd);
        else
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL, "Processing FindServerRequest on SecureChannel %i",
                         clientChannel->securityToken.channelId);
        UA_FindServersRequest  p;
        UA_FindServersResponse r;
        if(UA_FindServersRequest_decodeBinary(msg, pos, &p))
            return;
        UA_FindServersResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_FindServers(server, &p, &r);
        UA_FindServersRequest_deleteMembers(&p);
        UA_SecureChannel_sendBinaryMessage(clientChannel, sequenceHeader.requestId, &r,
                                           &UA_TYPES[UA_TYPES_FINDSERVERSRESPONSE]);
        UA_FindServersResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_CREATESESSIONREQUEST: {
        UA_CreateSessionRequest  p;
        UA_CreateSessionResponse r;
        if(UA_CreateSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_CreateSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_CreateSession(server, clientChannel, &p, &r);
        UA_CreateSessionRequest_deleteMembers(&p);
        UA_SecureChannel_sendBinaryMessage(clientChannel, sequenceHeader.requestId, &r,
                                           &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE]);
        UA_CreateSessionResponse_deleteMembers(&r);
        break;
    }

    case UA_NS0ID_ACTIVATESESSIONREQUEST: {
        UA_ActivateSessionRequest  p;
        UA_ActivateSessionResponse r;
        if(UA_ActivateSessionRequest_decodeBinary(msg, pos, &p))
            return;
        UA_ActivateSessionResponse_init(&r);
        init_response_header(&p.requestHeader, &r.responseHeader);
        Service_ActivateSession(server, clientChannel, &p, &r);
        UA_ActivateSessionRequest_deleteMembers(&p);
        UA_SecureChannel_sendBinaryMessage(clientChannel, sequenceHeader.requestId, &r,
                                           &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE]);
        UA_ActivateSessionResponse_deleteMembers(&r);
        break;
    }
    
    case UA_NS0ID_CLOSESESSIONREQUEST:
        INVOKE_SERVICE(CloseSession, UA_TYPES_CLOSESESSIONRESPONSE);
        break;
    case UA_NS0ID_READREQUEST:
        INVOKE_SERVICE(Read, UA_TYPES_READRESPONSE);
        break;
    case UA_NS0ID_WRITEREQUEST:
        INVOKE_SERVICE(Write, UA_TYPES_WRITERESPONSE);
        break;
    case UA_NS0ID_BROWSEREQUEST:
        INVOKE_SERVICE(Browse, UA_TYPES_BROWSERESPONSE);
        break;
    case UA_NS0ID_BROWSENEXTREQUEST:
        INVOKE_SERVICE(BrowseNext, UA_TYPES_BROWSENEXTRESPONSE);
        break;
    case UA_NS0ID_REGISTERNODESREQUEST:
        INVOKE_SERVICE(RegisterNodes, UA_TYPES_REGISTERNODESRESPONSE);
        break;
    case UA_NS0ID_UNREGISTERNODESREQUEST:
        INVOKE_SERVICE(UnregisterNodes, UA_TYPES_UNREGISTERNODESRESPONSE);
        break;
    case UA_NS0ID_TRANSLATEBROWSEPATHSTONODEIDSREQUEST:
        INVOKE_SERVICE(TranslateBrowsePathsToNodeIds, UA_TYPES_TRANSLATEBROWSEPATHSTONODEIDSRESPONSE);
        break;
#ifdef ENABLE_SUBSCRIPTIONS
    case UA_NS0ID_CREATESUBSCRIPTIONREQUEST:
        INVOKE_SERVICE(CreateSubscription, UA_TYPES_CREATESUBSCRIPTIONRESPONSE);
        break;
    case UA_NS0ID_PUBLISHREQUEST:
        INVOKE_SERVICE(Publish, UA_TYPES_PUBLISHRESPONSE);
        break;
    case UA_NS0ID_MODIFYSUBSCRIPTIONREQUEST:
        INVOKE_SERVICE(ModifySubscription, UA_TYPES_MODIFYSUBSCRIPTIONRESPONSE);
        break;
    case UA_NS0ID_DELETESUBSCRIPTIONSREQUEST:
        INVOKE_SERVICE(DeleteSubscriptions, UA_TYPES_DELETESUBSCRIPTIONSRESPONSE);
        break;
    case UA_NS0ID_CREATEMONITOREDITEMSREQUEST:
        INVOKE_SERVICE(CreateMonitoredItems, UA_TYPES_CREATEMONITOREDITEMSRESPONSE);
        break;
    case UA_NS0ID_DELETEMONITOREDITEMSREQUEST:
        INVOKE_SERVICE(DeleteMonitoredItems, UA_TYPES_DELETEMONITOREDITEMSRESPONSE);
        break;
#endif
#ifdef ENABLE_METHODCALLS
    case UA_NS0ID_CALLREQUEST:
        INVOKE_SERVICE(Call, UA_TYPES_CALLRESPONSE);
	break;
#endif
#ifdef ENABLE_NODEMANAGEMENT
    case UA_NS0ID_ADDNODESREQUEST:
        INVOKE_SERVICE(AddNodes, UA_TYPES_ADDNODESRESPONSE);
        break;
    case UA_NS0ID_ADDREFERENCESREQUEST:
        INVOKE_SERVICE(AddReferences, UA_TYPES_ADDREFERENCESRESPONSE);
        break;
    case UA_NS0ID_DELETENODESREQUEST:
        INVOKE_SERVICE(DeleteNodes, UA_TYPES_DELETENODESRESPONSE);
        break;
    case UA_NS0ID_DELETEREFERENCESREQUEST:
        INVOKE_SERVICE(DeleteReferences, UA_TYPES_DELETEREFERENCESRESPONSE);
        break;
#endif
    default: {
        if(requestType.namespaceIndex == 0 && requestType.identifier.numeric==787)
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_NETWORK,
                        "Client requested a subscription that are not supported, the message will be skipped");
        else
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_NETWORK, "Unknown request: NodeId(ns=%d, i=%d)",
                        requestType.namespaceIndex, requestType.identifier.numeric);
        UA_RequestHeader p;
        UA_ServiceFault r;
        if(UA_RequestHeader_decodeBinary(msg, pos, &p) != UA_STATUSCODE_GOOD)
            return;
        UA_ServiceFault_init(&r);
        init_response_header(&p, &r.responseHeader);
        r.responseHeader.serviceResult = UA_STATUSCODE_BADSERVICEUNSUPPORTED;
        UA_SecureChannel_sendBinaryMessage(clientChannel, sequenceHeader.requestId, &r,
                                           &UA_TYPES[UA_TYPES_SERVICEFAULT]);
        UA_RequestHeader_deleteMembers(&p);
        UA_ServiceFault_deleteMembers(&r);
        break;
    }
    }
}

static void processCLO(UA_Connection *connection, UA_Server *server, const UA_ByteString *msg, size_t *pos) {
    UA_UInt32 secureChannelId;
    UA_StatusCode retval = UA_UInt32_decodeBinary(msg, pos, &secureChannelId);
    if(retval != UA_STATUSCODE_GOOD || !connection->channel ||
       connection->channel->securityToken.channelId != secureChannelId)
        return;
    Service_CloseSecureChannel(server, secureChannelId);
}

/**
 * process binary message received from Connection
 * dose not modify UA_ByteString you have to free it youself.
 * use of connection->getSendBuffer() and connection->sent() to answer Message
 */
void UA_Server_processBinaryMessage(UA_Server *server, UA_Connection *connection, const UA_ByteString *msg) {
    size_t pos = 0;
    UA_TcpMessageHeader tcpMessageHeader;
    do {
        if(UA_TcpMessageHeader_decodeBinary(msg, &pos, &tcpMessageHeader)) {
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_NETWORK, "Decoding of message header failed");
            connection->close(connection);
            break;
        }

        size_t targetpos = pos - 8 + tcpMessageHeader.messageSize;
        switch(tcpMessageHeader.messageTypeAndFinal & 0xffffff) {
        case UA_MESSAGETYPEANDFINAL_HELF & 0xffffff:
            processHEL(connection, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_OPNF & 0xffffff:
            processOPN(connection, server, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_MSGF & 0xffffff:
#ifndef EXTENSION_STATELESS
            if(connection->state != UA_CONNECTION_ESTABLISHED) {
                connection->close(connection);
                return;
            }
#endif
            processMSG(connection, server, msg, &pos);
            break;
        case UA_MESSAGETYPEANDFINAL_CLOF & 0xffffff:
            processCLO(connection, server, msg, &pos);
            connection->close(connection);
            return;
        default:
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_NETWORK, "Unknown request type on Connection %i", connection->sockfd);
        }

        UA_TcpMessageHeader_deleteMembers(&tcpMessageHeader);
        if(pos != targetpos) {
            UA_LOG_INFO(server->logger, UA_LOGCATEGORY_NETWORK,
                        "Message on Connection %i was not entirely processed. Arrived at position %i, skip after the announced length to position %i",
                        connection->sockfd, pos, targetpos);
            pos = targetpos;
        }
    } while(msg->length > (UA_Int32)pos);
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_nodes.c" ***********************************/


/* UA_Node */
static void UA_Node_init(UA_Node *p) {
	UA_NodeId_init(&p->nodeId);
	UA_NodeClass_init(&p->nodeClass);
	UA_QualifiedName_init(&p->browseName);
	UA_LocalizedText_init(&p->displayName);
	UA_LocalizedText_init(&p->description);
	p->writeMask = 0;
	p->userWriteMask = 0;
	p->referencesSize = -1;
	p->references = NULL;
}

static void UA_Node_deleteMembers(UA_Node *p) {
	UA_NodeId_deleteMembers(&p->nodeId);
	UA_QualifiedName_deleteMembers(&p->browseName);
	UA_LocalizedText_deleteMembers(&p->displayName);
	UA_LocalizedText_deleteMembers(&p->description);
	UA_Array_delete(p->references, &UA_TYPES[UA_TYPES_REFERENCENODE], p->referencesSize);
}

static UA_StatusCode UA_Node_copy(const UA_Node *src, UA_Node *dst) {
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	UA_Node_init(dst);
	retval |= UA_NodeId_copy(&src->nodeId, &dst->nodeId);
	dst->nodeClass = src->nodeClass;
	retval |= UA_QualifiedName_copy(&src->browseName, &dst->browseName);
	retval |= UA_LocalizedText_copy(&src->displayName, &dst->displayName);
	retval |= UA_LocalizedText_copy(&src->description, &dst->description);
	dst->writeMask = src->writeMask;
	dst->userWriteMask = src->userWriteMask;
	dst->referencesSize = src->referencesSize;
	retval |= UA_Array_copy(src->references, (void**)&dst->references, &UA_TYPES[UA_TYPES_REFERENCENODE],
                            src->referencesSize);
	if(retval)
    	UA_Node_deleteMembers(dst);
	return retval;
}

void UA_Node_deleteAnyNodeClass(UA_Node *node) {
    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        UA_ObjectNode_delete((UA_ObjectNode*)node);
        break;
    case UA_NODECLASS_VARIABLE:
        UA_VariableNode_delete((UA_VariableNode*)node);
        break;
    case UA_NODECLASS_METHOD:
        UA_MethodNode_delete((UA_MethodNode*)node);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        UA_ObjectTypeNode_delete((UA_ObjectTypeNode*)node);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        UA_VariableTypeNode_delete((UA_VariableTypeNode*)node);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        UA_ReferenceTypeNode_delete((UA_ReferenceTypeNode*)node);
        break;
    case UA_NODECLASS_DATATYPE:
        UA_DataTypeNode_delete((UA_DataTypeNode*)node);
        break;
    case UA_NODECLASS_VIEW:
        UA_ViewNode_delete((UA_ViewNode*)node);
        break;
    default:
        break;
    }
}

typedef UA_Node *(*UA_NewNodeFunction)(void);
typedef UA_StatusCode (*UA_CopyNodeFunction)(const UA_Node *src, UA_Node *dst);
typedef void (*UA_DeleteNodeFunction)(UA_Node *p);

UA_Node * UA_Node_copyAnyNodeClass(const UA_Node *node) {
    UA_NewNodeFunction newNode;
    UA_CopyNodeFunction copyNode;
    UA_DeleteNodeFunction deleteNode;

    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        newNode = (UA_NewNodeFunction)UA_ObjectNode_new;
        copyNode = (UA_CopyNodeFunction)UA_ObjectNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_ObjectNode_delete;
        break;
    case UA_NODECLASS_VARIABLE:
        newNode = (UA_NewNodeFunction)UA_VariableNode_new;
        copyNode = (UA_CopyNodeFunction)UA_VariableNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_VariableNode_delete;
        break;
    case UA_NODECLASS_METHOD:
        newNode = (UA_NewNodeFunction)UA_MethodNode_new;
        copyNode = (UA_CopyNodeFunction)UA_MethodNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_MethodNode_delete;
        break;
    case UA_NODECLASS_OBJECTTYPE:
        newNode = (UA_NewNodeFunction)UA_ObjectTypeNode_new;
        copyNode = (UA_CopyNodeFunction)UA_ObjectTypeNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_ObjectTypeNode_delete;
        break;
    case UA_NODECLASS_VARIABLETYPE:
        newNode = (UA_NewNodeFunction)UA_VariableTypeNode_new;
        copyNode = (UA_CopyNodeFunction)UA_VariableTypeNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_VariableTypeNode_delete;
        break;
    case UA_NODECLASS_REFERENCETYPE:
        newNode = (UA_NewNodeFunction)UA_ReferenceTypeNode_new;
        copyNode = (UA_CopyNodeFunction)UA_ReferenceTypeNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_ReferenceTypeNode_delete;
        break;
    case UA_NODECLASS_DATATYPE:
        newNode = (UA_NewNodeFunction)UA_DataTypeNode_new;
        copyNode = (UA_CopyNodeFunction)UA_DataTypeNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_DataTypeNode_delete;
        break;
    case UA_NODECLASS_VIEW:
        newNode = (UA_NewNodeFunction)UA_ViewNode_new;
        copyNode = (UA_CopyNodeFunction)UA_ViewNode_copy;
        deleteNode = (UA_DeleteNodeFunction)UA_ViewNode_delete;
        break;
    default:
        return NULL;
        break;
    }

    UA_Node *copy = newNode();
    if(!copy)
        return NULL;
    if(copyNode(node, copy) != UA_STATUSCODE_GOOD) {
        deleteNode(copy);
        return NULL;
    }
    return copy;
}

/* UA_ObjectNode */
void UA_ObjectNode_init(UA_ObjectNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_OBJECT;
    p->eventNotifier = 0;
    p->instanceHandle = NULL;
}

UA_ObjectNode * UA_ObjectNode_new(void) {
    UA_ObjectNode *p = (UA_ObjectNode*)UA_malloc(sizeof(UA_ObjectNode));
    if(p)
        UA_ObjectNode_init(p);
    return p;
}

void UA_ObjectNode_deleteMembers(UA_ObjectNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ObjectNode_delete(UA_ObjectNode *p) {
    UA_ObjectNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ObjectNode_copy(const UA_ObjectNode *src, UA_ObjectNode *dst) {
    dst->eventNotifier = src->eventNotifier;
    dst->instanceHandle = src->instanceHandle;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_ObjectTypeNode */
void UA_ObjectTypeNode_init(UA_ObjectTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_OBJECTTYPE;
    p->isAbstract = UA_FALSE;
    p->lifecycleManagement = (UA_ObjectLifecycleManagement)
        {.constructor = NULL, .destructor = NULL};
}

UA_ObjectTypeNode * UA_ObjectTypeNode_new(void) {
    UA_ObjectTypeNode *p = (UA_ObjectTypeNode*)UA_malloc(sizeof(UA_ObjectTypeNode));
    if(p)
        UA_ObjectTypeNode_init(p);
    return p;
}

void UA_ObjectTypeNode_deleteMembers(UA_ObjectTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ObjectTypeNode_delete(UA_ObjectTypeNode *p) {
    UA_ObjectTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ObjectTypeNode_copy(const UA_ObjectTypeNode *src, UA_ObjectTypeNode *dst) {
    dst->isAbstract = src->isAbstract;
    dst->lifecycleManagement = src->lifecycleManagement;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_VariableNode */
void UA_VariableNode_init(UA_VariableNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VARIABLE;
    p->valueSource = UA_VALUESOURCE_VARIANT;
    UA_Variant_init(&p->value.variant.value);
    p->value.variant.callback = (UA_ValueCallback){NULL,NULL,NULL};
    p->valueRank = -2; // scalar or array of any dimension
    p->accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    p->userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    p->minimumSamplingInterval = 0.0;
    p->historizing = UA_FALSE;
}

UA_VariableNode * UA_VariableNode_new(void) {
    UA_VariableNode *p = (UA_VariableNode*)UA_malloc(sizeof(UA_VariableNode));
    if(p)
        UA_VariableNode_init(p);
    return p;
}

void UA_VariableNode_deleteMembers(UA_VariableNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    if(p->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&p->value.variant.value);
}

void UA_VariableNode_delete(UA_VariableNode *p) {
    UA_VariableNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_VariableNode_copy(const UA_VariableNode *src, UA_VariableNode *dst) {
    UA_VariableNode_init(dst);
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    dst->valueRank = src->valueRank;
    dst->valueSource = src->valueSource;
    if(src->valueSource == UA_VALUESOURCE_VARIANT) {
        retval = UA_Variant_copy(&src->value.variant.value, &dst->value.variant.value);
        dst->value.variant.callback = src->value.variant.callback;
    } else
        dst->value.dataSource = src->value.dataSource;
    if(retval) {
        UA_VariableNode_deleteMembers(dst);
        return retval;
    }
    dst->accessLevel = src->accessLevel;
    dst->userAccessLevel = src->accessLevel;
    dst->minimumSamplingInterval = src->minimumSamplingInterval;
    dst->historizing = src->historizing;
    return UA_STATUSCODE_GOOD;
}

/* UA_VariableTypeNode */
void UA_VariableTypeNode_init(UA_VariableTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VARIABLETYPE;
    p->valueSource = UA_VALUESOURCE_VARIANT;
    UA_Variant_init(&p->value.variant.value);
    p->value.variant.callback = (UA_ValueCallback){NULL, NULL, NULL};
    p->valueRank = -2; // scalar or array of any dimension
    p->isAbstract = UA_FALSE;
}

UA_VariableTypeNode * UA_VariableTypeNode_new(void) {
    UA_VariableTypeNode *p = (UA_VariableTypeNode*)UA_malloc(sizeof(UA_VariableTypeNode));
    if(p)
        UA_VariableTypeNode_init(p);
    return p;
}

void UA_VariableTypeNode_deleteMembers(UA_VariableTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    if(p->valueSource == UA_VALUESOURCE_VARIANT)
        UA_Variant_deleteMembers(&p->value.variant.value);
}

void UA_VariableTypeNode_delete(UA_VariableTypeNode *p) {
    UA_VariableTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_VariableTypeNode_copy(const UA_VariableTypeNode *src, UA_VariableTypeNode *dst) {
    UA_VariableTypeNode_init(dst);
	UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    dst->valueRank = src->valueRank;
    dst->valueSource = src->valueSource;
    if(src->valueSource == UA_VALUESOURCE_VARIANT){
        UA_Variant_copy(&src->value.variant.value, &dst->value.variant.value);
        dst->value.variant.callback = src->value.variant.callback;
    }else
        dst->value.dataSource = src->value.dataSource;
    if(retval) {
        UA_VariableTypeNode_deleteMembers(dst);
        return retval;
    }
    dst->isAbstract = src->isAbstract;
    return UA_STATUSCODE_GOOD;
}

/* UA_ReferenceTypeNode */
void UA_ReferenceTypeNode_init(UA_ReferenceTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_REFERENCETYPE;
    p->isAbstract = UA_FALSE;
    p->symmetric = UA_FALSE;
    UA_LocalizedText_init(&p->inverseName);
}

UA_ReferenceTypeNode * UA_ReferenceTypeNode_new(void) {
    UA_ReferenceTypeNode *p = (UA_ReferenceTypeNode*)UA_malloc(sizeof(UA_ReferenceTypeNode));
    if(p)
        UA_ReferenceTypeNode_init(p);
    return p;
}

void UA_ReferenceTypeNode_deleteMembers(UA_ReferenceTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
    UA_LocalizedText_deleteMembers(&p->inverseName);
}

void UA_ReferenceTypeNode_delete(UA_ReferenceTypeNode *p) {
    UA_ReferenceTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ReferenceTypeNode_copy(const UA_ReferenceTypeNode *src, UA_ReferenceTypeNode *dst) {
    UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    if(retval)
        return retval;
    retval = UA_LocalizedText_copy(&src->inverseName, &dst->inverseName);
    if(retval) {
        UA_ReferenceTypeNode_deleteMembers(dst);
        return retval;
    }
    dst->isAbstract = src->isAbstract;
    dst->symmetric = src->symmetric;
    return UA_STATUSCODE_GOOD;
}

/* UA_MethodNode */
void UA_MethodNode_init(UA_MethodNode *p) {
    UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_METHOD;
    p->executable = UA_FALSE;
    p->userExecutable = UA_FALSE;
#ifdef ENABLE_METHODCALLS
    p->methodHandle        = NULL;
    p->attachedMethod      = NULL;
#endif
}

UA_MethodNode * UA_MethodNode_new(void) {
    UA_MethodNode *p = (UA_MethodNode*)UA_malloc(sizeof(UA_MethodNode));
    if(p)
        UA_MethodNode_init(p);
    return p;
}

void UA_MethodNode_deleteMembers(UA_MethodNode *p) {
#ifdef ENABLE_METHODCALLS
    p->attachedMethod = NULL;
#endif
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_MethodNode_delete(UA_MethodNode *p) {
    UA_MethodNode_deleteMembers(p);
#ifdef ENABLE_METHODCALLS
    p->methodHandle   = NULL;
    p->attachedMethod = NULL;
#endif
    UA_free(p);
}

UA_StatusCode UA_MethodNode_copy(const UA_MethodNode *src, UA_MethodNode *dst) {
    UA_StatusCode retval = UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    dst->executable = src->executable;
    dst->userExecutable = src->userExecutable;
#ifdef ENABLE_METHODCALLS
    dst->methodHandle  = src->methodHandle;
    dst->attachedMethod = src->attachedMethod;
#endif
    return retval;
}

/* UA_ViewNode */
void UA_ViewNode_init(UA_ViewNode *p) {
    UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_VIEW;
    p->containsNoLoops = UA_FALSE;
    p->eventNotifier = 0;
}

UA_ViewNode * UA_ViewNode_new(void) {
    UA_ViewNode *p = UA_malloc(sizeof(UA_ViewNode));
    if(p)
        UA_ViewNode_init(p);
    return p;
}

void UA_ViewNode_deleteMembers(UA_ViewNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_ViewNode_delete(UA_ViewNode *p) {
    UA_ViewNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_ViewNode_copy(const UA_ViewNode *src, UA_ViewNode *dst) {
    dst->containsNoLoops = src->containsNoLoops;
    dst->eventNotifier = src->eventNotifier;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/* UA_DataTypeNode */
void UA_DataTypeNode_init(UA_DataTypeNode *p) {
	UA_Node_init((UA_Node*)p);
    p->nodeClass = UA_NODECLASS_DATATYPE;
    p->isAbstract = UA_FALSE;
}

UA_DataTypeNode * UA_DataTypeNode_new(void) {
    UA_DataTypeNode *p = UA_malloc(sizeof(UA_DataTypeNode));
    if(p)
        UA_DataTypeNode_init(p);
    return p;
}

void UA_DataTypeNode_deleteMembers(UA_DataTypeNode *p) {
    UA_Node_deleteMembers((UA_Node*)p);
}

void UA_DataTypeNode_delete(UA_DataTypeNode *p) {
    UA_DataTypeNode_deleteMembers(p);
    UA_free(p);
}

UA_StatusCode UA_DataTypeNode_copy(const UA_DataTypeNode *src, UA_DataTypeNode *dst) {
    dst->isAbstract = src->isAbstract;
	return UA_Node_copy((const UA_Node*)src, (UA_Node*)dst);
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_server_worker.c" ***********************************/


#if defined(__APPLE__) || defined(__MACH__)
#include <mach/clock.h>
#include <mach/mach.h>
#endif

/**
 * There are four types of job execution:
 *
 * 1. Normal jobs (dispatched to worker threads if multithreading is activated)
 *
 * 2. Repeated jobs with a repetition interval (dispatched to worker threads)
 *
 * 3. Mainloop jobs are executed (once) from the mainloop and not in the worker threads. The server
 * contains a stack structure where all threads can add mainloop jobs for the next mainloop
 * iteration. This is used e.g. to trigger adding and removing repeated jobs without blocking the
 * mainloop.
 *
 * 4. Delayed jobs are executed once in a worker thread. But only when all normal jobs that were
 * dispatched earlier have been executed. This is achieved by a counter in the worker threads. We
 * compute from the counter if all previous jobs have finished. The delay can be very long, since we
 * try to not interfere too much with normal execution. A use case is to eventually free obsolete
 * structures that _could_ still be accessed from concurrent threads.
 *
 * - Remove the entry from the list
 * - mark it as "dead" with an atomic operation
 * - add a delayed job that frees the memory when all concurrent operations have completed
 * 
 * This approach to concurrently accessible memory is known as epoch based reclamation [1]. According to
 * [2], it performs competitively well on many-core systems. Our version of EBR does however not require
 * a global epoch. Instead, every worker thread has its own epoch counter that we observe for changes.
 * 
 * [1] Fraser, K. 2003. Practical lock freedom. Ph.D. thesis. Computer Laboratory, University of Cambridge.
 * [2] Hart, T. E., McKenney, P. E., Brown, A. D., & Walpole, J. (2007). Performance of memory reclamation
 *     for lockless synchronization. Journal of Parallel and Distributed Computing, 67(12), 1270-1285.
 * 
 * 
 */

#define MAXTIMEOUT 50000 // max timeout in microsec until the next main loop iteration
#define BATCHSIZE 20 // max number of jobs that are dispatched at once to workers

/**
 * server:		UA server context
 * jobs: 		pointer to array of jobs or NULL if jobsSize == -1
 * jobsSize: 	nr. of valid jobs or -1
*/
static void processJobs(UA_Server *server, UA_Job *jobs, UA_Int32 jobsSize) {
    for (UA_Int32 i = 0; i < jobsSize; i++) {
        UA_Job *job = &jobs[i];
        switch(job->type) {
        case UA_JOBTYPE_NOTHING:
            break;
        case UA_JOBTYPE_DETACHCONNECTION:
            UA_Connection_detachSecureChannel(job->job.closeConnection);
            break;
        case UA_JOBTYPE_BINARYMESSAGE_NETWORKLAYER:
            UA_Server_processBinaryMessage(server, job->job.binaryMessage.connection,
                                           &job->job.binaryMessage.message);
            UA_Connection *connection = job->job.binaryMessage.connection;
            connection->releaseRecvBuffer(connection, &job->job.binaryMessage.message);
            break;
        case UA_JOBTYPE_BINARYMESSAGE_ALLOCATED:
            UA_Server_processBinaryMessage(server, job->job.binaryMessage.connection,
                                           &job->job.binaryMessage.message);
            UA_ByteString_deleteMembers(&job->job.binaryMessage.message);
            break;
        case UA_JOBTYPE_METHODCALL:
        case UA_JOBTYPE_METHODCALL_DELAYED:
            job->job.methodCall.method(server, job->job.methodCall.data);
            break;
        default:
            UA_LOG_WARNING(server->logger, UA_LOGCATEGORY_SERVER, "Trying to execute a job of unknown type");
            break;
        }
    }
}

/*******************************/
/* Worker Threads and Dispatch */
/*******************************/

#ifdef UA_MULTITHREADING

struct MainLoopJob {
    struct cds_lfs_node node;
    UA_Job job;
};

/** Entry in the dispatch queue */
struct DispatchJobsList {
    struct cds_wfcq_node node; // node for the queue
    size_t jobsSize;
    UA_Job *jobs;
};

/** Dispatch jobs to workers. Slices the job array up if it contains more than BATCHSIZE items. The jobs
    array is freed in the worker threads. */
static void dispatchJobs(UA_Server *server, UA_Job *jobs, size_t jobsSize) {
    size_t startIndex = jobsSize; // start at the end
    while(jobsSize > 0) {
        size_t size = BATCHSIZE;
        if(size > jobsSize)
            size = jobsSize;
        startIndex = startIndex - size;
        struct DispatchJobsList *wln = UA_malloc(sizeof(struct DispatchJobsList));
        if(startIndex > 0) {
            wln->jobs = UA_malloc(size * sizeof(UA_Job));
            memcpy(wln->jobs, &jobs[startIndex], size * sizeof(UA_Job));
            wln->jobsSize = size;
        } else {
            /* forward the original array */
            wln->jobsSize = size;
            wln->jobs = jobs;
        }
        cds_wfcq_node_init(&wln->node);
        cds_wfcq_enqueue(&server->dispatchQueue_head, &server->dispatchQueue_tail, &wln->node);
        jobsSize -= size;
    }
}

// throwaway struct to bring data into the worker threads
struct workerStartData {
    UA_Server *server;
    UA_UInt32 **workerCounter;
};

/** Waits until jobs arrive in the dispatch queue and processes them. */
static void * workerLoop(struct workerStartData *startInfo) {
    /* Initialized the (thread local) random seed */
    UA_random_seed((uintptr_t)startInfo);

   	rcu_register_thread();
    UA_UInt32 *c = UA_malloc(sizeof(UA_UInt32));
    uatomic_set(c, 0);
    *startInfo->workerCounter = c;
    UA_Server *server = startInfo->server;
    UA_free(startInfo);

    pthread_mutex_t mutex; // required for the condition variable
    pthread_mutex_init(&mutex,0);
    pthread_mutex_lock(&mutex);
    struct timespec to;

    while(*server->running) {
        struct DispatchJobsList *wln = (struct DispatchJobsList*)
            cds_wfcq_dequeue_blocking(&server->dispatchQueue_head, &server->dispatchQueue_tail);
        if(wln) {
            processJobs(server, wln->jobs, wln->jobsSize);
            UA_free(wln->jobs);
            UA_free(wln);
        } else {
            /* sleep until a work arrives (and wakes up all worker threads) */
            #if defined(__APPLE__) || defined(__MACH__) // OS X does not have clock_gettime, use clock_get_time
              clock_serv_t cclock;
              mach_timespec_t mts;
              host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
              clock_get_time(cclock, &mts);
              mach_port_deallocate(mach_task_self(), cclock);
              to.tv_sec = mts.tv_sec;
              to.tv_nsec = mts.tv_nsec;
            #else
              clock_gettime(CLOCK_REALTIME, &to);
            #endif
            to.tv_sec += 2;
            pthread_cond_timedwait(&server->dispatchQueue_condition, &mutex, &to);
        }
        uatomic_inc(c); // increase the workerCounter;
    }
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);

    rcu_barrier(); // wait for all scheduled call_rcu work to complete
   	rcu_unregister_thread();

    /* we need to return _something_ for pthreads */
    return NULL;
}

static void emptyDispatchQueue(UA_Server *server) {
    while(!cds_wfcq_empty(&server->dispatchQueue_head, &server->dispatchQueue_tail)) {
        struct DispatchJobsList *wln = (struct DispatchJobsList*)
            cds_wfcq_dequeue_blocking(&server->dispatchQueue_head, &server->dispatchQueue_tail);
        processJobs(server, wln->jobs, wln->jobsSize);
        UA_free(wln->jobs);
        UA_free(wln);
    }
}

#endif

/*****************/
/* Repeated Jobs */
/*****************/

struct IdentifiedJob {
    UA_Job job;
    UA_Guid id;
};

/**
 * The RepeatedJobs structure contains an array of jobs that are either executed with the same
 * repetition interval. The linked list is sorted, so we can stop traversing when the first element
 * has nextTime > now.
 */
struct RepeatedJobs {
    LIST_ENTRY(RepeatedJobs) pointers; ///> Links to the next list of repeated jobs (with a different) interval
    UA_DateTime nextTime; ///> The next time when the jobs are to be executed
    UA_UInt32 interval; ///> Interval in 100ns resolution
    size_t jobsSize; ///> Number of jobs contained
    struct IdentifiedJob jobs[]; ///> The jobs. This is not a pointer, instead the struct is variable sized.
};

/* throwaway struct for the mainloop callback */
struct AddRepeatedJob {
    struct IdentifiedJob job;
    UA_UInt32 interval;
};

/* internal. call only from the main loop. */
static UA_StatusCode addRepeatedJob(UA_Server *server, struct AddRepeatedJob * UA_RESTRICT arw) {
    struct RepeatedJobs *matchingTw = NULL; // add the item here
    struct RepeatedJobs *lastTw = NULL; // if there is no repeated job, add a new one this entry
    struct RepeatedJobs *tempTw;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;

    /* search for matching entry */
    UA_DateTime firstTime = UA_DateTime_now() + arw->interval;
    tempTw = LIST_FIRST(&server->repeatedJobs);
    while(tempTw) {
        if(arw->interval == tempTw->interval) {
            matchingTw = tempTw;
            break;
        }
        if(tempTw->nextTime > firstTime)
            break;
        lastTw = tempTw;
        tempTw = LIST_NEXT(lastTw, pointers);
    }

    if(matchingTw) {
        /* append to matching entry */
        matchingTw = UA_realloc(matchingTw, sizeof(struct RepeatedJobs) +
                                (sizeof(struct IdentifiedJob) * (matchingTw->jobsSize + 1)));
        if(!matchingTw) {
            retval = UA_STATUSCODE_BADOUTOFMEMORY;
            goto cleanup;
        }
        /* link the reallocated tw into the list */
        LIST_REPLACE(matchingTw, matchingTw, pointers);
    } else {
        /* create a new entry */
        matchingTw = UA_malloc(sizeof(struct RepeatedJobs) + sizeof(struct IdentifiedJob));
        if(!matchingTw) {
            retval = UA_STATUSCODE_BADOUTOFMEMORY;
            goto cleanup;
        }
        matchingTw->jobsSize = 0;
        matchingTw->nextTime = firstTime;
        matchingTw->interval = arw->interval;
        if(lastTw)
            LIST_INSERT_AFTER(lastTw, matchingTw, pointers);
        else
            LIST_INSERT_HEAD(&server->repeatedJobs, matchingTw, pointers);
    }
    matchingTw->jobs[matchingTw->jobsSize] = arw->job;
    matchingTw->jobsSize++;

 cleanup:
#ifdef UA_MULTITHREADING
    UA_free(arw);
#endif
    return retval;
}

UA_StatusCode UA_Server_addRepeatedJob(UA_Server *server, UA_Job job, UA_UInt32 interval, UA_Guid *jobId) {
    /* the interval needs to be at least 5ms */
    if(interval < 5)
        return UA_STATUSCODE_BADINTERNALERROR;
    interval *= 10000; // from ms to 100ns resolution

#ifdef UA_MULTITHREADING
    struct AddRepeatedJob *arw = UA_malloc(sizeof(struct AddRepeatedJob));
    if(!arw)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    arw->interval = interval;
    arw->job.job = job;
    if(jobId) {
        arw->job.id = UA_Guid_random(&server->random_seed);
        *jobId = arw->job.id;
    } else
        UA_Guid_init(&arw->job.id);

    struct MainLoopJob *mlw = UA_malloc(sizeof(struct MainLoopJob));
    if(!mlw) {
        UA_free(arw);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
    mlw->job = (UA_Job) {
        .type = UA_JOBTYPE_METHODCALL,
        .job.methodCall = {.data = arw, .method = (void (*)(UA_Server*, void*))addRepeatedJob}};
    cds_lfs_push(&server->mainLoopJobs, &mlw->node);
#else
    struct AddRepeatedJob arw;
    arw.interval = interval;
    arw.job.job = job;
    if(jobId) {
        arw.job.id = UA_Guid_random(&server->random_seed);
        *jobId = arw.job.id;
    } else
        UA_Guid_init(&arw.job.id);
    addRepeatedJob(server, &arw);
#endif
    return UA_STATUSCODE_GOOD;
}

/* Returns the timeout until the next repeated job in ms */
static UA_UInt16 processRepeatedJobs(UA_Server *server) {
    UA_DateTime current = UA_DateTime_now();
    struct RepeatedJobs *tw = NULL;

    while((tw = LIST_FIRST(&server->repeatedJobs)) != NULL) {
        if(tw->nextTime > current)
            break;

#ifdef UA_MULTITHREADING
        // copy the entry and insert at the new location
        UA_Job *jobsCopy = UA_malloc(sizeof(UA_Job) * tw->jobsSize);
        if(!jobsCopy) {
            UA_LOG_ERROR(server->logger, UA_LOGCATEGORY_SERVER, "Not enough memory to dispatch delayed jobs");
            break;
        }
        for(size_t i=0;i<tw->jobsSize;i++)
            jobsCopy[i] = tw->jobs[i].job;
        dispatchJobs(server, jobsCopy, tw->jobsSize); // frees the job pointer
#else
        for(size_t i=0;i<tw->jobsSize;i++)
            //processJobs may sort the list but dont delete entries
            processJobs(server, &tw->jobs[i].job, 1); // does not free the job ptr
#endif
        tw->nextTime += tw->interval;
        //start iterating the list from the beginning
        struct RepeatedJobs *prevTw = LIST_FIRST(&server->repeatedJobs); // after which tw do we insert?
        while(UA_TRUE) {
            struct RepeatedJobs *n = LIST_NEXT(prevTw, pointers);
            if(!n || n->nextTime > tw->nextTime)
                break;
            prevTw = n;
        }
        if(prevTw != tw) {
            LIST_REMOVE(tw, pointers);
            LIST_INSERT_AFTER(prevTw, tw, pointers);
        }
    }

    // check if the next repeated job is sooner than the usual timeout
    // calc in 32 bit must be ok
    struct RepeatedJobs *first = LIST_FIRST(&server->repeatedJobs);
    UA_UInt32 timeout = MAXTIMEOUT;
    if(first) {
        timeout = (UA_UInt32)((first->nextTime - current) / 10);
        if(timeout > MAXTIMEOUT)
            return MAXTIMEOUT;
    }
    return timeout;
}

/* Call this function only from the main loop! */
static void removeRepeatedJob(UA_Server *server, UA_Guid *jobId) {
    struct RepeatedJobs *tw;
    LIST_FOREACH(tw, &server->repeatedJobs, pointers) {
        for(size_t i = 0; i < tw->jobsSize; i++) {
            if(!UA_Guid_equal(jobId, &tw->jobs[i].id))
                continue;
            if(tw->jobsSize == 1) {
                LIST_REMOVE(tw, pointers);
                UA_free(tw);
            } else {
                tw->jobsSize--;
                tw->jobs[i] = tw->jobs[tw->jobsSize]; // move the last entry to overwrite
            }
            goto finish; // ugly break
        }
    }
 finish:
#ifdef UA_MULTITHREADING
    UA_free(jobId);
#endif
    return;
}

UA_StatusCode UA_Server_removeRepeatedJob(UA_Server *server, UA_Guid jobId) {
#ifdef UA_MULTITHREADING
    UA_Guid *idptr = UA_malloc(sizeof(UA_Guid));
    if(!idptr)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    *idptr = jobId;
    // dispatch to the mainloopjobs stack
    struct MainLoopJob *mlw = UA_malloc(sizeof(struct MainLoopJob));
    mlw->job = (UA_Job) {
        .type = UA_JOBTYPE_METHODCALL,
        .job.methodCall = {.data = idptr, .method = (void (*)(UA_Server*, void*))removeRepeatedJob}};
    cds_lfs_push(&server->mainLoopJobs, &mlw->node);
#else
    removeRepeatedJob(server, &jobId);
#endif
    return UA_STATUSCODE_GOOD;
}

void UA_Server_deleteAllRepeatedJobs(UA_Server *server) {
    struct RepeatedJobs *current, *temp;
    LIST_FOREACH_SAFE(current, &server->repeatedJobs, pointers, temp) {
        LIST_REMOVE(current, pointers);
        UA_free(current);
    }
}

/****************/
/* Delayed Jobs */
/****************/

#ifdef UA_MULTITHREADING

#define DELAYEDJOBSSIZE 100 // Collect delayed jobs until we have DELAYEDWORKSIZE items

struct DelayedJobs {
    struct DelayedJobs *next;
    UA_UInt32 *workerCounters; // initially NULL until the counter are set
    UA_UInt32 jobsCount; // the size of the array is DELAYEDJOBSSIZE, the count may be less
    UA_Job jobs[DELAYEDJOBSSIZE]; // when it runs full, a new delayedJobs entry is created
};

/* Dispatched as an ordinary job when the DelayedJobs list is full */
static void getCounters(UA_Server *server, struct DelayedJobs *delayed) {
    UA_UInt32 *counters = UA_malloc(server->nThreads * sizeof(UA_UInt32));
    for(UA_UInt16 i = 0;i<server->nThreads;i++)
        counters[i] = *server->workerCounters[i];
    delayed->workerCounters = counters;
}

// Call from the main thread only. This is the only function that modifies
// server->delayedWork. processDelayedWorkQueue modifies the "next" (after the
// head).
static void addDelayedJob(UA_Server *server, UA_Job *job) {
    struct DelayedJobs *dj = server->delayedJobs;
    if(!dj || dj->jobsCount >= DELAYEDJOBSSIZE) {
        /* create a new DelayedJobs and add it to the linked list */
        dj = UA_malloc(sizeof(struct DelayedJobs));
        if(!dj) {
            UA_LOG_ERROR(server->logger, UA_LOGCATEGORY_SERVER, "Not enough memory to add a delayed job");
            return;
        }
        dj->jobsCount = 0;
        dj->workerCounters = NULL;
        dj->next = server->delayedJobs;
        server->delayedJobs = dj;

        /* dispatch a method that sets the counter for the full list that comes afterwards */
        if(dj->next) {
            UA_Job *setCounter = UA_malloc(sizeof(UA_Job));
            *setCounter = (UA_Job) {.type = UA_JOBTYPE_METHODCALL, .job.methodCall =
                                    {.method = (void (*)(UA_Server*, void*))getCounters, .data = dj->next}};
            dispatchJobs(server, setCounter, 1);
        }
    }
    dj->jobs[dj->jobsCount] = *job;
    dj->jobsCount++;
}

static void addDelayedJobAsync(UA_Server *server, UA_Job *job) {
    addDelayedJob(server, job);
    UA_free(job);
}

UA_StatusCode UA_Server_addDelayedJob(UA_Server *server, UA_Job job) {
    UA_Job *j = UA_malloc(sizeof(UA_Job));
    if(!j)
        return UA_STATUSCODE_BADOUTOFMEMORY;
    *j = job;
    struct MainLoopJob *mlw = UA_malloc(sizeof(struct MainLoopJob));
    mlw->job = (UA_Job) {.type = UA_JOBTYPE_METHODCALL, .job.methodCall =
                         {.data = j, .method = (void (*)(UA_Server*, void*))addDelayedJobAsync}};
    cds_lfs_push(&server->mainLoopJobs, &mlw->node);
    return UA_STATUSCODE_GOOD;
}

/* Find out which delayed jobs can be executed now */
static void dispatchDelayedJobs(UA_Server *server, void *data /* not used, but needed for the signature*/) {
    /* start at the second */
    struct DelayedJobs *dw = server->delayedJobs, *beforedw = dw;
    if(dw)
        dw = dw->next;

    /* find the first delayedwork where the counters have been set and have moved */
    while(dw) {
        if(!dw->workerCounters) {
            beforedw = dw;
            dw = dw->next;
            continue;
        }
        UA_Boolean allMoved = UA_TRUE;
        for(UA_UInt16 i=0;i<server->nThreads;i++) {
            if(dw->workerCounters[i] == *server->workerCounters[i]) {
                allMoved = UA_FALSE;
                break;
            }
        }
        if(allMoved)
            break;
        beforedw = dw;
        dw = dw->next;
    }

    /* process and free all delayed jobs from here on */
    while(dw) {
        processJobs(server, dw->jobs, dw->jobsCount);
        struct DelayedJobs *next = uatomic_xchg(&beforedw->next, NULL);
        UA_free(dw);
        UA_free(dw->workerCounters);
        dw = next;
    }
}

#endif

/********************/
/* Main Server Loop */
/********************/

#ifdef UA_MULTITHREADING
static void processMainLoopJobs(UA_Server *server) {
    /* no synchronization required if we only use push and pop_all */
    struct cds_lfs_head *head = __cds_lfs_pop_all(&server->mainLoopJobs);
    if(!head)
        return;
    struct MainLoopJob *mlw = (struct MainLoopJob*)&head->node;
    struct MainLoopJob *next;
    do {
        processJobs(server, &mlw->job, 1);
        next = (struct MainLoopJob*)mlw->node.next;
        UA_free(mlw);
    } while((mlw = next));
    //UA_free(head);
}
#endif

UA_StatusCode UA_Server_run_startup(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running) {
UA_StatusCode result = UA_STATUSCODE_GOOD;

#ifdef UA_MULTITHREADING
    /* Prepare the worker threads */
    server->running = running; // the threads need to access the variable
    server->nThreads = nThreads;
    pthread_cond_init(&server->dispatchQueue_condition, 0);
    server->thr = UA_malloc(nThreads * sizeof(pthread_t));
    server->workerCounters = UA_malloc(nThreads * sizeof(UA_UInt32 *));
    for(UA_UInt32 i=0;i<nThreads;i++) {
        struct workerStartData *startData = UA_malloc(sizeof(struct workerStartData));
        startData->server = server;
        startData->workerCounter = &server->workerCounters[i];
        pthread_create(&server->thr[i], NULL, (void* (*)(void*))workerLoop, startData);
    }

    /* try to execute the delayed callbacks every 10 sec */
    UA_Job processDelayed = {.type = UA_JOBTYPE_METHODCALL,
                             .job.methodCall = {.method = dispatchDelayedJobs, .data = NULL} };
    UA_Server_addRepeatedJob(server, processDelayed, 10000, NULL);
#endif

    /* Start the networklayers */
    for(size_t i = 0; i < server->networkLayersSize; i++)
        result |= server->networkLayers[i]->start(server->networkLayers[i], server->logger);

    return result;
}

UA_StatusCode UA_Server_run_mainloop(UA_Server *server, UA_Boolean *running) {
#ifdef UA_MULTITHREADING
    /* Run Work in the main loop */
    processMainLoopJobs(server);
#endif
    /* Process repeated work */
    UA_UInt16 timeout = processRepeatedJobs(server);

    /* Get work from the networklayer */
    for(size_t i = 0; i < server->networkLayersSize; i++) {
        UA_ServerNetworkLayer *nl = server->networkLayers[i];
        UA_Job *jobs;
        UA_Int32 jobsSize;
        if(*running) {
            if(i == server->networkLayersSize-1)
                jobsSize = nl->getJobs(nl, &jobs, timeout);
            else
                jobsSize = nl->getJobs(nl, &jobs, 0);
        } else
            jobsSize = server->networkLayers[i]->stop(nl, &jobs);

#ifdef UA_MULTITHREADING
        /* Filter out delayed work */
        for(UA_Int32 k=0;k<jobsSize;k++) {
            if(jobs[k].type != UA_JOBTYPE_METHODCALL_DELAYED)
                continue;
            addDelayedJob(server, &jobs[k]);
            jobs[k].type = UA_JOBTYPE_NOTHING;
        }

        /* Dispatch work to the worker threads */
        dispatchJobs(server, jobs, jobsSize);

        /* Trigger sleeping worker threads */
        if(jobsSize > 0)
            pthread_cond_broadcast(&server->dispatchQueue_condition);
#else
        processJobs(server, jobs, jobsSize);
        if(jobsSize > 0)
            UA_free(jobs);
#endif
    }
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Server_run_shutdown(UA_Server *server, UA_UInt16 nThreads){
    UA_Job *stopJobs;
    for(size_t i = 0; i < server->networkLayersSize; i++) {
        size_t stopJobsSize = server->networkLayers[i]->stop(server->networkLayers[i], &stopJobs);
        processJobs(server, stopJobs, stopJobsSize);
        UA_free(stopJobs);
    }
#ifdef UA_MULTITHREADING
    /* Wait for all worker threads to finish */
    for(UA_UInt32 i=0;i<nThreads;i++) {
        pthread_join(server->thr[i], NULL);
        UA_free(server->workerCounters[i]);
    }
    UA_free(server->workerCounters);
    UA_free(server->thr);

    /* Manually finish the work still enqueued */
    emptyDispatchQueue(server);

    /* Process the remaining delayed work */
    struct DelayedJobs *dw = server->delayedJobs;
    while(dw) {
        processJobs(server, dw->jobs, dw->jobsCount);
        struct DelayedJobs *next = dw->next;
        UA_free(dw->workerCounters);
        UA_free(dw);
        dw = next;
    }
#endif
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_Server_run(UA_Server *server, UA_UInt16 nThreads, UA_Boolean *running) {
    if(UA_STATUSCODE_GOOD == UA_Server_run_startup(server, nThreads, running)){
        while(*running) {
            UA_Server_run_mainloop(server, running);
        }
    }
    UA_Server_run_shutdown(server, nThreads);
    return UA_STATUSCODE_GOOD;
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_securechannel_manager.c" ***********************************/


UA_StatusCode UA_SecureChannelManager_init(UA_SecureChannelManager *cm,
        size_t maxChannelCount, UA_UInt32 tokenLifetime,
        UA_UInt32 startChannelId, UA_UInt32 startTokenId) {
    LIST_INIT(&cm->channels);
    cm->lastChannelId = startChannelId;
    cm->lastTokenId = startTokenId;
    cm->maxChannelLifetime = tokenLifetime;
    cm->maxChannelCount = maxChannelCount;
    cm->currentChannelCount = 0;
    return UA_STATUSCODE_GOOD;
}

void UA_SecureChannelManager_deleteMembers(UA_SecureChannelManager *cm) {
    channel_list_entry *entry, *temp;
    LIST_FOREACH_SAFE(entry, &cm->channels, pointers, temp)
    {
        LIST_REMOVE(entry, pointers);
        UA_SecureChannel_deleteMembersCleanup(&entry->channel);
        UA_free(entry);
    }
}

/* remove channels that were not renewed or who have no connection attached */
void UA_SecureChannelManager_cleanupTimedOut(UA_SecureChannelManager *cm,
        UA_DateTime now) {
    channel_list_entry *entry, *temp;
    LIST_FOREACH_SAFE(entry, &cm->channels, pointers, temp)
    {
        UA_DateTime timeout = entry->channel.securityToken.createdAt
                + ((UA_DateTime) entry->channel.securityToken.revisedLifetime
                        * 10000);
        if (timeout < now || !entry->channel.connection) {
            LIST_REMOVE(entry, pointers);
            UA_SecureChannel_deleteMembersCleanup(&entry->channel);
#ifndef UA_MULTITHREADING
            cm->currentChannelCount--;
#else
            cm->currentChannelCount = uatomic_add_return(
                    &cm->currentChannelCount, -1);
#endif
            UA_free(entry);
        } else if (entry->channel.nextSecurityToken.tokenId > 0) {
            UA_SecureChannel_revolveTokens(&entry->channel);
        }
    }
}

UA_StatusCode UA_SecureChannelManager_open(UA_SecureChannelManager *cm,
        UA_Connection *conn, const UA_OpenSecureChannelRequest *request,
        UA_OpenSecureChannelResponse *response) {

    if (request->securityMode != UA_MESSAGESECURITYMODE_NONE) {
        return UA_STATUSCODE_BADSECURITYMODEREJECTED;

    }

    channel_list_entry *entry = UA_malloc(sizeof(channel_list_entry));
    if (!entry) {
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
    if (cm->currentChannelCount >= cm->maxChannelCount) {
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }
#ifndef UA_MULTITHREADING
    cm->currentChannelCount++;
#else
    cm->currentChannelCount = uatomic_add_return(&cm->currentChannelCount, 1);
#endif

    UA_SecureChannel_init(&entry->channel);
    response->responseHeader.stringTableSize = 0;
    response->responseHeader.timestamp = UA_DateTime_now();
    response->serverProtocolVersion = 0;

    entry->channel.securityToken.channelId = cm->lastChannelId++;
    entry->channel.securityToken.tokenId = cm->lastTokenId++;
    entry->channel.securityToken.createdAt = UA_DateTime_now();
    entry->channel.securityToken.revisedLifetime =
            (request->requestedLifetime > cm->maxChannelLifetime) ?
                    cm->maxChannelLifetime : request->requestedLifetime;
    /* pragmatic workaround to get clients requesting lifetime of 0 working */
    if (entry->channel.securityToken.revisedLifetime == 0)
        entry->channel.securityToken.revisedLifetime = cm->maxChannelLifetime;

    UA_ByteString_copy(&request->clientNonce, &entry->channel.clientNonce);
    entry->channel.serverAsymAlgSettings.securityPolicyUri = UA_STRING_ALLOC(
            "http://opcfoundation.org/UA/SecurityPolicy#None");

    UA_SecureChannel_generateNonce(&entry->channel.serverNonce);
    UA_ByteString_copy(&entry->channel.serverNonce, &response->serverNonce);
    UA_ChannelSecurityToken_copy(&entry->channel.securityToken,
            &response->securityToken);

    UA_Connection_attachSecureChannel(conn, &entry->channel);
    LIST_INSERT_HEAD(&cm->channels, entry, pointers);
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_SecureChannelManager_renew(UA_SecureChannelManager *cm,
        UA_Connection *conn, const UA_OpenSecureChannelRequest *request,
        UA_OpenSecureChannelResponse *response) {
    UA_SecureChannel *channel = conn->channel;
    if (!channel)
        return UA_STATUSCODE_BADINTERNALERROR;

    /* if no security token is already issued */
    if (channel->nextSecurityToken.tokenId == 0) {
        channel->nextSecurityToken.channelId = channel->securityToken.channelId;
        //FIXME: UaExpert seems not to use the new tokenid
        channel->nextSecurityToken.tokenId = cm->lastTokenId++;
        //channel->nextSecurityToken.tokenId = channel->securityToken.tokenId;
        channel->nextSecurityToken.createdAt = UA_DateTime_now();
        channel->nextSecurityToken.revisedLifetime =
                (request->requestedLifetime > cm->maxChannelLifetime) ?
                        cm->maxChannelLifetime : request->requestedLifetime;

        /* pragmatic workaround to get clients requesting lifetime of 0 working */
        if (channel->nextSecurityToken.revisedLifetime == 0)
            channel->nextSecurityToken.revisedLifetime = cm->maxChannelLifetime;
    }

    if (channel->clientNonce.data)
        UA_ByteString_deleteMembers(&channel->clientNonce);

    UA_ByteString_copy(&request->clientNonce, &channel->clientNonce);
    UA_ByteString_copy(&channel->serverNonce, &response->serverNonce);
    UA_ChannelSecurityToken_copy(&channel->nextSecurityToken,
            &response->securityToken);
    return UA_STATUSCODE_GOOD;
}

UA_SecureChannel *
UA_SecureChannelManager_get(UA_SecureChannelManager *cm, UA_UInt32 channelId) {
    channel_list_entry *entry;
    LIST_FOREACH(entry, &cm->channels, pointers)
    {
        if (entry->channel.securityToken.channelId == channelId)
            return &entry->channel;
    }
    return NULL;
}

UA_StatusCode UA_SecureChannelManager_close(UA_SecureChannelManager *cm,
        UA_UInt32 channelId) {
    channel_list_entry *entry;
    LIST_FOREACH(entry, &cm->channels, pointers)
    {
        if (entry->channel.securityToken.channelId == channelId) {
            LIST_REMOVE(entry, pointers);
            UA_SecureChannel_deleteMembersCleanup(&entry->channel);
            UA_free(entry);
#ifndef UA_MULTITHREADING
            cm->currentChannelCount--;
#else
            cm->currentChannelCount = uatomic_add_return(
                    &cm->currentChannelCount, -1);
#endif
            return UA_STATUSCODE_GOOD;
        }
    }
    return UA_STATUSCODE_BADINTERNALERROR;
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_session_manager.c" ***********************************/


UA_StatusCode
UA_SessionManager_init(UA_SessionManager *sessionManager, UA_UInt32 maxSessionCount,
                       UA_UInt32 maxSessionLifeTime, UA_UInt32 startSessionId) {
    LIST_INIT(&sessionManager->sessions);
    sessionManager->maxSessionCount = maxSessionCount;
    sessionManager->lastSessionId   = startSessionId;
    sessionManager->maxSessionLifeTime  = maxSessionLifeTime;
    sessionManager->currentSessionCount = 0;
    return UA_STATUSCODE_GOOD;
}

void UA_SessionManager_deleteMembers(UA_SessionManager *sessionManager, UA_Server *server) {
    session_list_entry *current, *temp;
    LIST_FOREACH_SAFE(current, &sessionManager->sessions, pointers, temp) {
        LIST_REMOVE(current, pointers);
        UA_Session_deleteMembersCleanup(&current->session, server);
        UA_free(current);
    }
}

void UA_SessionManager_cleanupTimedOut(UA_SessionManager *sessionManager,
                                       UA_Server* server, UA_DateTime now) {
    session_list_entry *sentry, *temp;
    LIST_FOREACH_SAFE(sentry, &sessionManager->sessions, pointers, temp) {
        if(sentry->session.validTill < now) {
            LIST_REMOVE(sentry, pointers);
            UA_Session_deleteMembersCleanup(&sentry->session, server);
            UA_free(sentry);
            sessionManager->currentSessionCount--;
        }
    }
}

UA_Session *
UA_SessionManager_getSession(UA_SessionManager *sessionManager, const UA_NodeId *token) {
    session_list_entry *current = NULL;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.authenticationToken, token)) {
            if(UA_DateTime_now() > current->session.validTill)
                return NULL;
            return &current->session;
        }
    }
    return NULL;
}

/** Creates and adds a session. But it is not yet attached to a secure channel. */
UA_StatusCode
UA_SessionManager_createSession(UA_SessionManager *sessionManager, UA_SecureChannel *channel,
                                const UA_CreateSessionRequest *request, UA_Session **session) {
    if(sessionManager->currentSessionCount >= sessionManager->maxSessionCount)
        return UA_STATUSCODE_BADTOOMANYSESSIONS;

    session_list_entry *newentry = UA_malloc(sizeof(session_list_entry));
    if(!newentry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    sessionManager->currentSessionCount++;
    UA_Session_init(&newentry->session);
    newentry->session.sessionId = UA_NODEID_NUMERIC(1, sessionManager->lastSessionId++);
    UA_UInt32 randSeed = (UA_UInt32)(sessionManager->lastSessionId + UA_DateTime_now());
    newentry->session.authenticationToken = UA_NODEID_GUID(1, UA_Guid_random(&randSeed));

    if(request->requestedSessionTimeout <= sessionManager->maxSessionLifeTime &&
       request->requestedSessionTimeout > 0)
        newentry->session.timeout = (UA_Int64)request->requestedSessionTimeout;
    else
        newentry->session.timeout = sessionManager->maxSessionLifeTime; // todo: remove when the CTT is fixed

    UA_Session_updateLifetime(&newentry->session);
    LIST_INSERT_HEAD(&sessionManager->sessions, newentry, pointers);
    *session = &newentry->session;
    return UA_STATUSCODE_GOOD;
}

UA_StatusCode
UA_SessionManager_removeSession(UA_SessionManager *sessionManager,
                                UA_Server* server, const UA_NodeId *token) {
    session_list_entry *current;
    LIST_FOREACH(current, &sessionManager->sessions, pointers) {
        if(UA_NodeId_equal(&current->session.authenticationToken, token))
            break;
    }

    if(!current)
        return UA_STATUSCODE_BADSESSIONIDINVALID;

    LIST_REMOVE(current, pointers);
    UA_Session_deleteMembersCleanup(&current->session, server);
    UA_free(current);
    sessionManager->currentSessionCount--;
    return UA_STATUSCODE_GOOD;
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_services_discovery.c" ***********************************/


void Service_FindServers(UA_Server *server, const UA_FindServersRequest *request, UA_FindServersResponse *response) {
    response->servers = UA_malloc(sizeof(UA_ApplicationDescription));
    if(!response->servers) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    if(UA_ApplicationDescription_copy(&server->description, response->servers) != UA_STATUSCODE_GOOD) {
        UA_free(response->servers);
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
	response->serversSize = 1;
}

void Service_GetEndpoints(UA_Server *server, const UA_GetEndpointsRequest *request, UA_GetEndpointsResponse *response) {
    /* test if the supported binary profile shall be returned */
#ifdef NO_ALLOCA
	UA_Boolean relevant_endpoints[server->endpointDescriptionsSize];
#else
	UA_Boolean *relevant_endpoints = UA_alloca(sizeof(UA_Boolean)*server->endpointDescriptionsSize);
#endif /*NO_ALLOCA */
    size_t relevant_count = 0;
    for(UA_Int32 j = 0; j < server->endpointDescriptionsSize; j++) {
        relevant_endpoints[j] = UA_FALSE;
        if(request->profileUrisSize <= 0) {
            relevant_endpoints[j] = UA_TRUE;
            relevant_count++;
            continue;
        }
        for(UA_Int32 i = 0; i < request->profileUrisSize; i++) {
            if(UA_String_equal(&request->profileUris[i], &server->endpointDescriptions->transportProfileUri)) {
                relevant_endpoints[j] = UA_TRUE;
                relevant_count++;
                break;
            }
        }
    }

    if(relevant_count == 0) {
        response->endpointsSize = 0;
        return;
    }

    response->endpoints = UA_malloc(sizeof(UA_EndpointDescription) * relevant_count);
    if(!response->endpoints) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    size_t k = 0;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    for(UA_Int32 j = 0; j < server->endpointDescriptionsSize && retval == UA_STATUSCODE_GOOD; j++) {
        if(relevant_endpoints[j] != UA_TRUE)
            continue;
        retval = UA_EndpointDescription_copy(&server->endpointDescriptions[j], &response->endpoints[k]);
        k++;
    }

    if(retval != UA_STATUSCODE_GOOD) {
        response->responseHeader.serviceResult = retval;
        UA_Array_delete(response->endpoints, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], --k);
        return;
    }
    response->endpointsSize = relevant_count;
}


/*********************************** amalgamated original file "C:/open62541/src/server/ua_services_securechannel.c" ***********************************/


void Service_OpenSecureChannel(UA_Server *server, UA_Connection *connection,
                               const UA_OpenSecureChannelRequest *request,
                               UA_OpenSecureChannelResponse *response) {
    // todo: if(request->clientProtocolVersion != protocolVersion)
    if(request->requestType == UA_SECURITYTOKENREQUESTTYPE_ISSUE) {
        response->responseHeader.serviceResult =
            UA_SecureChannelManager_open(&server->secureChannelManager, connection, request, response);

        if(response->responseHeader.serviceResult == UA_STATUSCODE_GOOD)
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Opened SecureChannel %i on Connection %i",
                         response->securityToken.channelId, connection->sockfd);
        else
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Opening SecureChannel on Connection %i failed", connection->sockfd);
    } else {
        response->responseHeader.serviceResult =
            UA_SecureChannelManager_renew(&server->secureChannelManager, connection, request, response);

        if(response->responseHeader.serviceResult == UA_STATUSCODE_GOOD)
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Renewed SecureChannel %i on Connection %i",
                         response->securityToken.channelId, connection->sockfd);
        else
            UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Renewing SecureChannel on Connection %i failed", connection->sockfd);
    }
}

/* The server does not send a CloseSecureChannel response */
void Service_CloseSecureChannel(UA_Server *server, UA_Int32 channelId) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SECURECHANNEL,
                 "Closing SecureChannel %i", channelId);
    UA_SecureChannelManager_close(&server->secureChannelManager, channelId);
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_services_session.c" ***********************************/


void Service_CreateSession(UA_Server *server, UA_SecureChannel *channel,
                           const UA_CreateSessionRequest *request,
                           UA_CreateSessionResponse *response) {
    response->responseHeader.serviceResult =
        UA_Array_copy(server->endpointDescriptions, (void**)&response->serverEndpoints,
                      &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION], server->endpointDescriptionsSize);
    if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD)
        return;
    response->serverEndpointsSize = server->endpointDescriptionsSize;

	UA_Session *newSession;
    response->responseHeader.serviceResult = UA_SessionManager_createSession(&server->sessionManager,
                                                                             channel, request, &newSession);
	if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing CreateSessionRequest on SecureChannel %i failed",
                 channel->securityToken.channelId);
		return;
    }

    //TODO get maxResponseMessageSize internally
    newSession->maxResponseMessageSize = request->maxResponseMessageSize;
    response->sessionId = newSession->sessionId;
    response->revisedSessionTimeout = (UA_Double)newSession->timeout;
    response->authenticationToken = newSession->authenticationToken;
    response->responseHeader.serviceResult = UA_String_copy(&request->sessionName, &newSession->sessionName);
    if(server->endpointDescriptions)
        response->responseHeader.serviceResult |=
            UA_ByteString_copy(&server->endpointDescriptions->serverCertificate, &response->serverCertificate);
    if(response->responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        UA_SessionManager_removeSession(&server->sessionManager, server, &newSession->authenticationToken);
         return;
    }
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing CreateSessionRequest on SecureChannel %i succeeded, created Session (ns=%i,i=%i)",
                 channel->securityToken.channelId, response->sessionId.namespaceIndex,
                 response->sessionId.identifier.numeric);
}

void Service_ActivateSession(UA_Server *server, UA_SecureChannel *channel,
                             const UA_ActivateSessionRequest *request,
                             UA_ActivateSessionResponse *response) {
    // make the channel know about the session
	UA_Session *foundSession =
        UA_SessionManager_getSession(&server->sessionManager,
                                     (const UA_NodeId*)&request->requestHeader.authenticationToken);

	if(foundSession == NULL) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
        UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                     "Processing ActivateSessionRequest on SecureChannel %i, but no session found for the authentication token",
                     channel->securityToken.channelId);
        return;
	} else if(foundSession->validTill < UA_DateTime_now()) {
        UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                     "Processing ActivateSessionRequest on SecureChannel %i, but the session has timed out",
                     channel->securityToken.channelId);
        response->responseHeader.serviceResult = UA_STATUSCODE_BADSESSIONIDINVALID;
        return;
	}
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing ActivateSessionRequest on SecureChannel %i for Session (ns=%i,i=%i)",
                 channel->securityToken.channelId, foundSession->sessionId.namespaceIndex,
                 foundSession->sessionId.identifier.numeric);

    UA_UserIdentityToken token;
    UA_UserIdentityToken_init(&token);
    size_t offset = 0;
    UA_UserIdentityToken_decodeBinary(&request->userIdentityToken.body, &offset, &token);

    UA_UserNameIdentityToken username_token;
    UA_UserNameIdentityToken_init(&username_token);

    UA_String ap = UA_STRING(ANONYMOUS_POLICY);
    UA_String up = UA_STRING(USERNAME_POLICY);
    //(Compatibility notice)
    //Siemens OPC Scout v10 provides an empty policyId, this is not okay
    //For compatibility we will assume that empty policyId == ANONYMOUS_POLICY
    //if(token.policyId.data == NULL) {
    //    /* 1) no policy defined */
    //    response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
    //} else
    //(End Compatibility notice)
    if(server->config.Login_enableAnonymous && (token.policyId.data == NULL || UA_String_equal(&token.policyId, &ap))) {
        /* 2) anonymous logins */
        if(foundSession->channel && foundSession->channel != channel)
            UA_SecureChannel_detachSession(foundSession->channel, foundSession);
        UA_SecureChannel_attachSession(channel, foundSession);
        foundSession->activated = UA_TRUE;
        UA_Session_updateLifetime(foundSession);
    } else if(server->config.Login_enableUsernamePassword && UA_String_equal(&token.policyId, &up)) {
        /* 3) username logins */
        offset = 0;
        UA_UserNameIdentityToken_decodeBinary(&request->userIdentityToken.body, &offset, &username_token);
        if(username_token.encryptionAlgorithm.data != NULL) {
            /* 3.1) we only support encryption */
            response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
        } else  if(username_token.userName.length == -1 && username_token.password.length == -1){
            /* 3.2) empty username and password */
            response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
        } else {
            /* 3.3) ok, trying to match the username */
            UA_UInt32 i = 0;
            for(; i < server->config.Login_loginsCount; ++i) {
                UA_String user = UA_STRING(server->config.Login_usernames[i]);
                UA_String pw = UA_STRING(server->config.Login_passwords[i]);
                if(UA_String_equal(&username_token.userName, &user) &&
                   UA_String_equal(&username_token.password, &pw)) {
                    /* success - activate */
                    if(foundSession->channel && foundSession->channel != channel)
                        UA_SecureChannel_detachSession(foundSession->channel, foundSession);
                    UA_SecureChannel_attachSession(channel, foundSession);
                    foundSession->activated = UA_TRUE;
                    UA_Session_updateLifetime(foundSession);
                    break;
                }
            }
            /* no username/pass matched */
            if(i >= server->config.Login_loginsCount)
                response->responseHeader.serviceResult = UA_STATUSCODE_BADUSERACCESSDENIED;
        }
    } else {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADIDENTITYTOKENINVALID;
    }
    UA_UserIdentityToken_deleteMembers(&token);
    UA_UserNameIdentityToken_deleteMembers(&username_token);
    return;
}

void Service_CloseSession(UA_Server *server, UA_Session *session, const UA_CloseSessionRequest *request,
                          UA_CloseSessionResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing CloseSessionRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);
    response->responseHeader.serviceResult =
        UA_SessionManager_removeSession(&server->sessionManager, server, &session->authenticationToken);
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_services_attribute.c" ***********************************/


/******************/
/* Read Attribute */
/******************/

static size_t readNumber(UA_Byte *buf, UA_Int32 buflen, UA_UInt32 *number) {
    UA_UInt32 n = 0;
    size_t progress = 0;
    /* read numbers until the end or a non-number character appears */
    UA_Byte c;
    while((UA_Int32)progress < buflen) {
        c = buf[progress];
        if('0' > c || '9' < c)
            break;
        n = (n*10) + (c-'0');
        progress++;
    }
    *number = n;
    return progress;
}

static size_t readDimension(UA_Byte *buf, UA_Int32 buflen, struct UA_NumericRangeDimension *dim) {
    size_t progress = readNumber(buf, buflen, &dim->min);
    if(progress == 0)
        return 0;
    if(buflen <= (UA_Int32)progress || buf[progress] != ':') {
        dim->max = dim->min;
        return progress;
    }
    size_t progress2 = readNumber(&buf[progress+1], buflen - (progress + 1), &dim->max);
    if(progress2 == 0)
        return 0;
    return progress + progress2 + 1;
}

#ifndef BUILD_UNIT_TESTS
static
#endif
UA_StatusCode parse_numericrange(const UA_String *str, UA_NumericRange *range) {
    UA_Int32 index = 0;
    size_t dimensionsMax = 0;
    struct UA_NumericRangeDimension *dimensions = NULL;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_Int32 pos = 0;
    do {
        /* alloc dimensions */
        if(index >= (UA_Int32)dimensionsMax) {
            struct UA_NumericRangeDimension *newds = UA_realloc(dimensions, sizeof(struct UA_NumericRangeDimension) * (dimensionsMax + 2));
            if(!newds) {
                retval = UA_STATUSCODE_BADOUTOFMEMORY;
                break;
            }
            dimensions = newds;
            dimensionsMax = dimensionsMax + 2;
        }

        /* read the dimension */
        size_t progress = readDimension(&str->data[pos], str->length - pos, &dimensions[index]);
        if(progress == 0) {
            retval = UA_STATUSCODE_BADINDEXRANGEINVALID;
            break;
        }
        pos += progress;
        index++;

        /* loop into the next dimension */
        if(pos >= str->length)
            break;
    } while(str->data[pos] == ',' && pos++);

    if(retval == UA_STATUSCODE_GOOD && index > 0) {
        range->dimensions = dimensions;
        range->dimensionsSize = index;
    } else
        UA_free(dimensions);

    return retval;
}

#define CHECK_NODECLASS(CLASS)                                  \
    if(!(node->nodeClass & (CLASS))) {                          \
        retval = UA_STATUSCODE_BADATTRIBUTEIDINVALID;           \
        break;                                                  \
    }

static void handleServerTimestamps(UA_TimestampsToReturn timestamps, UA_DataValue* v) {
	if(v && (timestamps == UA_TIMESTAMPSTORETURN_SERVER || timestamps == UA_TIMESTAMPSTORETURN_BOTH)) {
		v->hasServerTimestamp = UA_TRUE;
		v->serverTimestamp = UA_DateTime_now();
	}
}

static void handleSourceTimestamps(UA_TimestampsToReturn timestamps, UA_DataValue* v) {
	if(timestamps == UA_TIMESTAMPSTORETURN_SOURCE || timestamps == UA_TIMESTAMPSTORETURN_BOTH) {
		v->hasSourceTimestamp = UA_TRUE;
		v->sourceTimestamp = UA_DateTime_now();
	}
}

static UA_StatusCode getVariableNodeValue(const UA_VariableNode *vn, const UA_TimestampsToReturn timestamps,
                                          const UA_ReadValueId *id, UA_DataValue *v) {
    UA_NumericRange range;
    UA_NumericRange *rangeptr = NULL;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(id->indexRange.length > 0) {
        retval = parse_numericrange(&id->indexRange, &range);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        rangeptr = &range;
    }

    if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
        if(vn->value.variant.callback.onRead)
            vn->value.variant.callback.onRead(vn->value.variant.callback.handle, vn->nodeId,
                                              &v->value, rangeptr);
        if(rangeptr)
            retval = UA_Variant_copyRange(&vn->value.variant.value, &v->value, range);
        else
            retval = UA_Variant_copy(&vn->value.variant.value, &v->value);
        if(retval == UA_STATUSCODE_GOOD)
            handleSourceTimestamps(timestamps, v);
    } else {
        UA_Boolean sourceTimeStamp = (timestamps == UA_TIMESTAMPSTORETURN_SOURCE ||
                                      timestamps == UA_TIMESTAMPSTORETURN_BOTH);
        retval = vn->value.dataSource.read(vn->value.dataSource.handle, vn->nodeId,
                                           sourceTimeStamp, rangeptr, v);
    }

    if(rangeptr)
        UA_free(range.dimensions);
    return retval;
}

static UA_StatusCode getVariableNodeDataType(const UA_VariableNode *vn, UA_DataValue *v) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
        retval = UA_Variant_setScalarCopy(&v->value, &vn->value.variant.value.type->typeId,
                                          &UA_TYPES[UA_TYPES_NODEID]);
    } else {
        /* Read from the datasource to see the data type */
        UA_DataValue val;
        UA_DataValue_init(&val);
        retval = vn->value.dataSource.read(vn->value.dataSource.handle, vn->nodeId, UA_FALSE, NULL, &val);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        retval = UA_Variant_setScalarCopy(&v->value, &val.value.type->typeId, &UA_TYPES[UA_TYPES_NODEID]);
        UA_DataValue_deleteMembers(&val);
    }
    return retval;
}

static UA_StatusCode getVariableNodeArrayDimensions(const UA_VariableNode *vn, UA_DataValue *v) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(vn->valueSource == UA_VALUESOURCE_VARIANT) {
        retval = UA_Variant_setArrayCopy(&v->value, vn->value.variant.value.arrayDimensions,
                                         vn->value.variant.value.arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);
    } else {
        /* Read the datasource to see the array dimensions */
        UA_DataValue val;
        UA_DataValue_init(&val);
        retval = vn->value.dataSource.read(vn->value.dataSource.handle, vn->nodeId, UA_FALSE, NULL, &val);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        retval = UA_Variant_setArrayCopy(&v->value, val.value.arrayDimensions,
                                         val.value.arrayDimensionsSize, &UA_TYPES[UA_TYPES_INT32]);
        UA_DataValue_deleteMembers(&val);
    }
    return retval;
}

static const UA_String binEncoding = {sizeof("DefaultBinary")-1, (UA_Byte*)"DefaultBinary"};
/* clang complains about unused variables */
// static const UA_String xmlEncoding = {sizeof("DefaultXml")-1, (UA_Byte*)"DefaultXml"};

/** Reads a single attribute from a node in the nodestore. */
void Service_Read_single(UA_Server *server, UA_Session *session, const UA_TimestampsToReturn timestamps,
                         const UA_ReadValueId *id, UA_DataValue *v) {
	if(id->dataEncoding.name.length >= 0 && !UA_String_equal(&binEncoding, &id->dataEncoding.name)) {
           v->hasStatus = UA_TRUE;
           v->status = UA_STATUSCODE_BADDATAENCODINGINVALID;
           return;
	}

	//index range for a non-value
	if(id->indexRange.length >= 0 && id->attributeId != UA_ATTRIBUTEID_VALUE){
		v->hasStatus = UA_TRUE;
		v->status = UA_STATUSCODE_BADINDEXRANGENODATA;
		return;
	}

    UA_Node const *node = UA_NodeStore_get(server->nodestore, &id->nodeId);
    if(!node) {
        v->hasStatus = UA_TRUE;
        v->status = UA_STATUSCODE_BADNODEIDUNKNOWN;
        return;
    }

    /* When setting the value fails in the switch, we get an error code and set hasValue to false */
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    v->hasValue = UA_TRUE;

    switch(id->attributeId) {
    case UA_ATTRIBUTEID_NODEID:
        retval = UA_Variant_setScalarCopy(&v->value, &node->nodeId, &UA_TYPES[UA_TYPES_NODEID]);
        break;
    case UA_ATTRIBUTEID_NODECLASS:
        retval = UA_Variant_setScalarCopy(&v->value, &node->nodeClass, &UA_TYPES[UA_TYPES_INT32]);
        break;
    case UA_ATTRIBUTEID_BROWSENAME:
        retval = UA_Variant_setScalarCopy(&v->value, &node->browseName, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]);
        break;
    case UA_ATTRIBUTEID_DISPLAYNAME:
        retval = UA_Variant_setScalarCopy(&v->value, &node->displayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_DESCRIPTION:
        retval = UA_Variant_setScalarCopy(&v->value, &node->description, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_WRITEMASK:
        retval = UA_Variant_setScalarCopy(&v->value, &node->writeMask, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    case UA_ATTRIBUTEID_USERWRITEMASK:
        retval = UA_Variant_setScalarCopy(&v->value, &node->userWriteMask, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    case UA_ATTRIBUTEID_ISABSTRACT:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE | UA_NODECLASS_OBJECTTYPE |
                        UA_NODECLASS_VARIABLETYPE | UA_NODECLASS_DATATYPE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode*)node)->isAbstract,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_SYMMETRIC:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode*)node)->symmetric,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_INVERSENAME:
        CHECK_NODECLASS(UA_NODECLASS_REFERENCETYPE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_ReferenceTypeNode*)node)->inverseName,
                                          &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        break;
    case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
        CHECK_NODECLASS(UA_NODECLASS_VIEW);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_ViewNode*)node)->containsNoLoops,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_EVENTNOTIFIER:
        CHECK_NODECLASS(UA_NODECLASS_VIEW | UA_NODECLASS_OBJECT);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_ViewNode*)node)->eventNotifier,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;
    case UA_ATTRIBUTEID_VALUE:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        retval = getVariableNodeValue((const UA_VariableNode*)node, timestamps, id, v);
        break;
    case UA_ATTRIBUTEID_DATATYPE:
		CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        retval = getVariableNodeDataType((const UA_VariableNode*)node, v);
        break;
    case UA_ATTRIBUTEID_VALUERANK:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_VariableTypeNode*)node)->valueRank,
                                           &UA_TYPES[UA_TYPES_INT32]);
        break;
    case UA_ATTRIBUTEID_ARRAYDIMENSIONS:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        retval = getVariableNodeArrayDimensions((const UA_VariableNode*)node, v);
        break;
    case UA_ATTRIBUTEID_ACCESSLEVEL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode*)node)->accessLevel,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;
    case UA_ATTRIBUTEID_USERACCESSLEVEL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode*)node)->userAccessLevel,
                                          &UA_TYPES[UA_TYPES_BYTE]);
        break;
    case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode*)node)->minimumSamplingInterval,
                                          &UA_TYPES[UA_TYPES_DOUBLE]);
        break;
    case UA_ATTRIBUTEID_HISTORIZING:
        CHECK_NODECLASS(UA_NODECLASS_VARIABLE);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_VariableNode*)node)->historizing,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_EXECUTABLE:
        CHECK_NODECLASS(UA_NODECLASS_METHOD);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_MethodNode*)node)->executable,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    case UA_ATTRIBUTEID_USEREXECUTABLE:
        CHECK_NODECLASS(UA_NODECLASS_METHOD);
        retval = UA_Variant_setScalarCopy(&v->value, &((const UA_MethodNode*)node)->userExecutable,
                                          &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    default:
        retval = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
        break;
    }

    UA_NodeStore_release(node);

    if(retval != UA_STATUSCODE_GOOD) {
        v->hasValue = UA_FALSE;
        v->hasStatus = UA_TRUE;
        v->status = retval;
    }

    // Todo: what if the timestamp from the datasource are already present?
    handleServerTimestamps(timestamps, v);
}

void Service_Read(UA_Server *server, UA_Session *session, const UA_ReadRequest *request,
                  UA_ReadResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing ReadRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);
    if(request->nodesToReadSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    if(request->timestampsToReturn > 3){
    	response->responseHeader.serviceResult = UA_STATUSCODE_BADTIMESTAMPSTORETURNINVALID;
    	return;
    }

    size_t size = request->nodesToReadSize;
    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_DATAVALUE], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    response->resultsSize = size;
    if(request->maxAge < 0) {
    	response->responseHeader.serviceResult = UA_STATUSCODE_BADMAXAGEINVALID;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
    memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0;j<server->externalNamespacesSize;j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToRead[i].nodeId.namespaceIndex != server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->readNodes(ens->ensHandle, &request->requestHeader, request->nodesToRead,
                       indices, indexSize, response->results, UA_FALSE, response->diagnosticInfos);
    }
#endif

    for(size_t i = 0;i < size;i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            Service_Read_single(server, session, request->timestampsToReturn,
                                &request->nodesToRead[i], &response->results[i]);
    }

#ifdef EXTENSION_STATELESS
    if(session==&anonymousSession){
		/* expiry header */
		UA_ExtensionObject additionalHeader;
		UA_ExtensionObject_init(&additionalHeader);
		additionalHeader.typeId = UA_TYPES[UA_TYPES_VARIANT].typeId;
		additionalHeader.encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;

		UA_Variant variant;
		UA_Variant_init(&variant);

		UA_DateTime* expireArray = NULL;
		expireArray = UA_Array_new(&UA_TYPES[UA_TYPES_DATETIME], request->nodesToReadSize);
		variant.data = expireArray;

		/*expires in 20 seconds*/
		for(UA_Int32 i = 0;i < response->resultsSize;i++) {
			expireArray[i] = UA_DateTime_now() + 20 * 100 * 1000 * 1000;
		}
		UA_Variant_setArray(&variant, expireArray, request->nodesToReadSize, &UA_TYPES[UA_TYPES_DATETIME]);

		size_t offset = 0;
		UA_ByteString str;
        UA_ByteString_newMembers(&str, 65536);
		UA_Variant_encodeBinary(&variant, &str, &offset);

        UA_Array_delete(expireArray, &UA_TYPES[UA_TYPES_DATETIME], request->nodesToReadSize);

		additionalHeader.body = str;
		additionalHeader.body.length = offset;
		response->responseHeader.additionalHeader = additionalHeader;
    }
#endif
}

/*******************/
/* Write Attribute */
/*******************/

UA_StatusCode UA_Server_editNode(UA_Server *server, UA_Session *session, const UA_NodeId *nodeId,
                                 UA_EditNodeCallback callback, const void *data) {
    UA_StatusCode retval;
    do {
        const UA_Node *node = UA_NodeStore_get(server->nodestore, nodeId);
        if(!node)
            return UA_STATUSCODE_BADNODEIDUNKNOWN;
#ifndef UA_MULTITHREADING
        retval = callback(server, session, (UA_Node*)(uintptr_t)node, data);
        UA_NodeStore_release(node);
        return retval;
#else
        UA_Node *copy = UA_Node_copyAnyNodeClass(node);
        UA_NodeStore_release(node);
        if(!copy)
            return UA_STATUSCODE_BADOUTOFMEMORY;
        retval = callback(server, session, copy, data);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Node_deleteAnyNodeClass(copy);
            return retval;
        }
        retval = UA_NodeStore_replace(server->nodestore, node, copy, NULL);
        if(retval != UA_STATUSCODE_GOOD)
            UA_Node_deleteAnyNodeClass(copy);
#endif
    } while(retval != UA_STATUSCODE_GOOD);
    return UA_STATUSCODE_GOOD;
}

#define CHECK_DATATYPE(EXP_DT)                                          \
    if(!wvalue->value.hasValue ||                                       \
       &UA_TYPES[UA_TYPES_##EXP_DT] != wvalue->value.value.type ||      \
       !UA_Variant_isScalar(&wvalue->value.value)) {                    \
        retval = UA_STATUSCODE_BADTYPEMISMATCH;                         \
        break;                                                          \
    }

#define CHECK_NODECLASS_WRITE(CLASS)                                    \
    if((node->nodeClass & (CLASS)) == 0) {                              \
        retval = UA_STATUSCODE_BADNODECLASSINVALID;                     \
        break;                                                          \
    }

static UA_StatusCode
Service_Write_single_ValueDataSource(UA_Server *server, UA_Session *session, const UA_VariableNode *node,
                                     UA_WriteValue *wvalue) {
    UA_assert(wvalue->attributeId == UA_ATTRIBUTEID_VALUE);
    UA_assert(node->nodeClass == UA_NODECLASS_VARIABLE || node->nodeClass == UA_NODECLASS_VARIABLETYPE);
    UA_assert(node->valueSource == UA_VALUESOURCE_DATASOURCE);

    UA_StatusCode retval;
    if(wvalue->indexRange.length <= 0) {
        retval = node->value.dataSource.write(node->value.dataSource.handle, node->nodeId,
                                              &wvalue->value.value, NULL);
    } else {
        UA_NumericRange range;
        retval = parse_numericrange(&wvalue->indexRange, &range);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        retval = node->value.dataSource.write(node->value.dataSource.handle, node->nodeId,
                                              &wvalue->value.value, &range);
        UA_free(range.dimensions);
    }
    return retval;
}

/* In the multithreaded case, node is a copy */
static UA_StatusCode
MoveValueIntoNode(UA_Server *server, UA_Session *session, UA_VariableNode *node, UA_WriteValue *wvalue) {
    UA_assert(wvalue->attributeId == UA_ATTRIBUTEID_VALUE);
    UA_assert(node->nodeClass == UA_NODECLASS_VARIABLE || node->nodeClass == UA_NODECLASS_VARIABLETYPE);
    UA_assert(node->valueSource == UA_VALUESOURCE_VARIANT);

    /* Parse the range */
    UA_NumericRange range;
    UA_NumericRange *rangeptr = NULL;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(wvalue->indexRange.length > 0) {
        retval = parse_numericrange(&wvalue->indexRange, &range);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
        rangeptr = &range;
    }

    /* The nodeid on the wire may be != the nodeid in the node: opaque types, enums and bytestrings.
       nodeV contains the correct type definition. */
    UA_Variant *newV = &wvalue->value.value;
    UA_Variant *oldV = &node->value.variant.value;
    UA_Variant cast_v;
    if(!UA_NodeId_equal(&oldV->type->typeId, &newV->type->typeId)) {
        cast_v = wvalue->value.value;
        newV = &cast_v;
        if(oldV->type->namespaceZero && newV->type->namespaceZero &&
           oldV->type->typeIndex == newV->type->typeIndex) {
            /* An enum was sent as an int32, or an opaque type as a bytestring. This is
               detected with the typeIndex indicated the "true" datatype. */
            newV->type = oldV->type;
        } else if(oldV->type == &UA_TYPES[UA_TYPES_BYTE] && !UA_Variant_isScalar(oldV) &&
                  newV->type == &UA_TYPES[UA_TYPES_BYTESTRING] && UA_Variant_isScalar(newV)) {
            /* a string is written to a byte array */
            UA_ByteString *str = (UA_ByteString*) newV->data;
            newV->arrayLength = str->length;
            newV->data = str->data;
            newV->type = &UA_TYPES[UA_TYPES_BYTE];
        } else {
            if(rangeptr)
                UA_free(range.dimensions);
            return UA_STATUSCODE_BADTYPEMISMATCH;
        }
    }
    
    if(!rangeptr) {
        // TODO: Avoid copying the whole node and then delete the old value for multithreading
        UA_Variant_deleteMembers(&node->value.variant.value);
        node->value.variant.value = *newV;
        UA_Variant_init(&wvalue->value.value);
    } else {
        retval = UA_Variant_setRangeCopy(&node->value.variant.value, newV->data, newV->arrayLength, range);
    }
    if(node->value.variant.callback.onWrite)
        node->value.variant.callback.onWrite(node->value.variant.callback.handle, node->nodeId,
                                             &node->value.variant.value, rangeptr);
    if(rangeptr)
        UA_free(range.dimensions);
    return retval;
}

static UA_StatusCode
MoveAttributeIntoNode(UA_Server *server, UA_Session *session, UA_Node *node, UA_WriteValue *wvalue) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    void *value = wvalue->value.value.data;
	switch(wvalue->attributeId) {
    case UA_ATTRIBUTEID_NODEID:
    case UA_ATTRIBUTEID_NODECLASS:
    case UA_ATTRIBUTEID_DATATYPE:
		retval = UA_STATUSCODE_BADWRITENOTSUPPORTED;
		break;
	case UA_ATTRIBUTEID_BROWSENAME:
		CHECK_DATATYPE(QUALIFIEDNAME);
		UA_QualifiedName_deleteMembers(&node->browseName);
        node->browseName = *(UA_QualifiedName*)value;
        UA_QualifiedName_init((UA_QualifiedName*)value);
		break;
	case UA_ATTRIBUTEID_DISPLAYNAME:
		CHECK_DATATYPE(LOCALIZEDTEXT);
		UA_LocalizedText_deleteMembers(&node->displayName);
        node->displayName = *(UA_LocalizedText*)value;
		UA_LocalizedText_init((UA_LocalizedText*)value);
		break;
	case UA_ATTRIBUTEID_DESCRIPTION:
		CHECK_DATATYPE(LOCALIZEDTEXT);
		UA_LocalizedText_deleteMembers(&node->description);
        node->description = *(UA_LocalizedText*)value;
		UA_LocalizedText_init((UA_LocalizedText*)value);
		break;
	case UA_ATTRIBUTEID_WRITEMASK:
		CHECK_DATATYPE(UINT32);
		node->writeMask = *(UA_UInt32*)value;
		break;
	case UA_ATTRIBUTEID_USERWRITEMASK:
		CHECK_DATATYPE(UINT32);
		node->userWriteMask = *(UA_UInt32*)value;
		break;    
	case UA_ATTRIBUTEID_ISABSTRACT:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_REFERENCETYPE |
                              UA_NODECLASS_VARIABLETYPE | UA_NODECLASS_DATATYPE);
		CHECK_DATATYPE(BOOLEAN);
		((UA_ObjectTypeNode*)node)->isAbstract = *(UA_Boolean*)value;
		break;
	case UA_ATTRIBUTEID_SYMMETRIC:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_REFERENCETYPE);
		CHECK_DATATYPE(BOOLEAN);
		((UA_ReferenceTypeNode*)node)->symmetric = *(UA_Boolean*)value;
		break;
	case UA_ATTRIBUTEID_INVERSENAME:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_REFERENCETYPE);
		CHECK_DATATYPE(LOCALIZEDTEXT);
        UA_ReferenceTypeNode *n = (UA_ReferenceTypeNode*)node;
		UA_LocalizedText_deleteMembers(&n->inverseName);
        n->inverseName = *(UA_LocalizedText*)value;
		UA_LocalizedText_init((UA_LocalizedText*)value);
		break;
	case UA_ATTRIBUTEID_CONTAINSNOLOOPS:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VIEW);
		CHECK_DATATYPE(BOOLEAN);
        ((UA_ViewNode*)node)->containsNoLoops = *(UA_Boolean*)value;
		break;
	case UA_ATTRIBUTEID_EVENTNOTIFIER:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VIEW | UA_NODECLASS_OBJECT);
		CHECK_DATATYPE(BYTE);
        ((UA_ViewNode*)node)->eventNotifier = *(UA_Byte*)value;
		break;
	case UA_ATTRIBUTEID_VALUE:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLETYPE);
        retval = MoveValueIntoNode(server, session, (UA_VariableNode*)node, wvalue);
		break;
	case UA_ATTRIBUTEID_ACCESSLEVEL:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VARIABLE);
		CHECK_DATATYPE(BYTE);
		((UA_VariableNode*)node)->accessLevel = *(UA_Byte*)value;
		break;
	case UA_ATTRIBUTEID_USERACCESSLEVEL:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VARIABLE);
		CHECK_DATATYPE(BYTE);
		((UA_VariableNode*)node)->userAccessLevel = *(UA_Byte*)value;
		break;
	case UA_ATTRIBUTEID_MINIMUMSAMPLINGINTERVAL:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VARIABLE);
		CHECK_DATATYPE(DOUBLE);
		((UA_VariableNode*)node)->minimumSamplingInterval = *(UA_Double*)value;
		break;
	case UA_ATTRIBUTEID_HISTORIZING:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_VARIABLE);
		CHECK_DATATYPE(BOOLEAN);
		((UA_VariableNode*)node)->historizing = *(UA_Boolean*)value;
		break;
	case UA_ATTRIBUTEID_EXECUTABLE:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_METHOD);
		CHECK_DATATYPE(BOOLEAN);
		((UA_MethodNode*)node)->executable = *(UA_Boolean*)value;
		break;
	case UA_ATTRIBUTEID_USEREXECUTABLE:
		CHECK_NODECLASS_WRITE(UA_NODECLASS_METHOD);
		CHECK_DATATYPE(BOOLEAN);
		((UA_MethodNode*)node)->userExecutable = *(UA_Boolean*)value;
		break;
	default:
		retval = UA_STATUSCODE_BADATTRIBUTEIDINVALID;
		break;
	}
    return retval;
}

UA_StatusCode Service_Write_single(UA_Server *server, UA_Session *session, UA_WriteValue *wvalue) {
    if(!wvalue->value.hasValue || !wvalue->value.value.data)
        return UA_STATUSCODE_BADNODATA; // TODO: is this the right return code?
    if(wvalue->attributeId == UA_ATTRIBUTEID_VALUE) {
        const UA_Node *orig = UA_NodeStore_get(server->nodestore, &wvalue->nodeId);
        if(!orig)
            return UA_STATUSCODE_BADNODEIDUNKNOWN;
        if(orig->nodeClass & (UA_NODECLASS_VARIABLE | UA_NODECLASS_VARIABLE) &&
           ((const UA_VariableNode*)orig)->valueSource == UA_VALUESOURCE_DATASOURCE) {
            UA_StatusCode retval = Service_Write_single_ValueDataSource(server, session, (const UA_VariableNode*)orig, wvalue);
            UA_NodeStore_release(orig);
            return retval;
        }
        UA_NodeStore_release(orig);
    }
    return UA_Server_editNode(server, session, &wvalue->nodeId,
                              (UA_EditNodeCallback)MoveAttributeIntoNode, wvalue);
}

void Service_Write(UA_Server *server, UA_Session *session, const UA_WriteRequest *request,
                   UA_WriteResponse *response) {
    UA_assert(server != NULL && session != NULL && request != NULL && response != NULL);
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing WriteRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);

    if(request->nodesToWriteSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_STATUSCODE], request->nodesToWriteSize);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
    UA_Boolean isExternal[request->nodesToWriteSize];
    UA_UInt32 indices[request->nodesToWriteSize];
    memset(isExternal, UA_FALSE, sizeof(UA_Boolean)*request->nodesToWriteSize);
    for(size_t j = 0; j < server->externalNamespacesSize; j++) {
        UA_UInt32 indexSize = 0;
        for(UA_Int32 i = 0; i < request->nodesToWriteSize; i++) {
            if(request->nodesToWrite[i].nodeId.namespaceIndex !=
               server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->writeNodes(ens->ensHandle, &request->requestHeader, request->nodesToWrite,
                        indices, indexSize, response->results, response->diagnosticInfos);
    }
#endif
    
    response->resultsSize = request->nodesToWriteSize;
    for(UA_Int32 i = 0;i < request->nodesToWriteSize;i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
		  response->results[i] = Service_Write_single(server, session, &request->nodesToWrite[i]);
    }
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_services_nodemanagement.c" ***********************************/


/************/
/* Add Node */
/************/

void UA_Server_addExistingNode(UA_Server *server, UA_Session *session, UA_Node *node,
                               const UA_NodeId *parentNodeId, const UA_NodeId *referenceTypeId,
                               UA_AddNodesResult *result) {
    if(node->nodeId.namespaceIndex >= server->namespacesSize) {
        result->statusCode = UA_STATUSCODE_BADNODEIDINVALID;
        return;
    }

    const UA_Node *parent = UA_NodeStore_get(server->nodestore, parentNodeId);
    if(!parent) {
        result->statusCode = UA_STATUSCODE_BADPARENTNODEIDINVALID;
        return;
    }

    const UA_ReferenceTypeNode *referenceType =
        (const UA_ReferenceTypeNode *)UA_NodeStore_get(server->nodestore, referenceTypeId);
    if(!referenceType) {
        result->statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
        goto ret;
    }

    if(referenceType->nodeClass != UA_NODECLASS_REFERENCETYPE) {
        result->statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
        goto ret2;
    }

    if(referenceType->isAbstract == UA_TRUE) {
        result->statusCode = UA_STATUSCODE_BADREFERENCENOTALLOWED;
        goto ret2;
    }

    // todo: test if the referencetype is hierarchical
    // todo: namespace index is assumed to be valid
    const UA_Node *managed = NULL;
    UA_NodeId tempNodeid = node->nodeId;
    tempNodeid.namespaceIndex = 0;
    if(UA_NodeId_isNull(&tempNodeid)) {
        if(UA_NodeStore_insert(server->nodestore, node, &managed) != UA_STATUSCODE_GOOD) {
            result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
            goto ret2;
        }
        result->addedNodeId = managed->nodeId; // cannot fail as unique nodeids are numeric
    } else {
        if(UA_NodeId_copy(&node->nodeId, &result->addedNodeId) != UA_STATUSCODE_GOOD) {
            result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
            goto ret2;
        }

        if(UA_NodeStore_insert(server->nodestore, node, &managed) != UA_STATUSCODE_GOOD) {
            result->statusCode = UA_STATUSCODE_BADNODEIDEXISTS;
            UA_NodeId_deleteMembers(&result->addedNodeId);
            goto ret2;
        }
    }
    
    // reference back to the parent
    UA_AddReferencesItem item;
    UA_AddReferencesItem_init(&item);
    item.sourceNodeId = managed->nodeId;
    item.referenceTypeId = referenceType->nodeId;
    item.isForward = UA_FALSE;
    item.targetNodeId.nodeId = parent->nodeId;
    Service_AddReferences_single(server, session, &item);

    // todo: error handling. remove new node from nodestore
    UA_NodeStore_release(managed);
    
 ret2:
    UA_NodeStore_release((const UA_Node*)referenceType);
 ret:
    UA_NodeStore_release(parent);
}

static UA_StatusCode
instantiateVariableNode(UA_Server *server, UA_Session *session,
                        const UA_NodeId *nodeId, const UA_NodeId *typeId);
static UA_StatusCode
instantiateObjectNode(UA_Server *server, UA_Session *session,
                      const UA_NodeId *nodeId, const UA_NodeId *typeId);

/* copy an existing variable under the given parent. then instantiate the
   variable for all hastypedefinitions of the original version. */
static UA_StatusCode
copyExistingVariable(UA_Server *server, UA_Session *session, const UA_NodeId *variable,
                     const UA_NodeId *referenceType, const UA_NodeId *parent) {
    const UA_VariableNode *node = (const UA_VariableNode*)UA_NodeStore_get(server->nodestore, variable);
    if(!node)
        return UA_STATUSCODE_BADNODEIDINVALID;
    if(node->nodeClass != UA_NODECLASS_VARIABLE) {
        UA_NodeStore_release((const UA_Node*)node);
        return UA_STATUSCODE_BADNODECLASSINVALID;
    }
    
    // copy the variable attributes
    UA_VariableAttributes attr;
    UA_VariableAttributes_init(&attr);
    UA_LocalizedText_copy(&node->displayName, &attr.displayName);
    UA_LocalizedText_copy(&node->description, &attr.description);
    attr.writeMask = node->writeMask;
    attr.userWriteMask = node->userWriteMask;
    // todo: handle data sources!!!!
    UA_Variant_copy(&node->value.variant.value, &attr.value);
    // datatype is taken from the value
    // valuerank is taken from the value
    // array dimensions are taken from the value
    attr.accessLevel = node->accessLevel;
    attr.userAccessLevel = node->userAccessLevel;
    attr.minimumSamplingInterval = node->minimumSamplingInterval;
    attr.historizing = node->historizing;

    UA_AddNodesItem item;
    UA_AddNodesItem_init(&item);
    UA_NodeId_copy(parent, &item.parentNodeId.nodeId);
    UA_NodeId_copy(referenceType, &item.referenceTypeId);
    UA_QualifiedName_copy(&node->browseName, &item.browseName);
    item.nodeClass = UA_NODECLASS_VARIABLE;
    // don't add a typedefinition here.

    // add the new variable
    UA_AddNodesResult res;
    UA_AddNodesResult_init(&res);
    Service_AddNodes_single(server, session, &item, (UA_NodeAttributes*)&attr, &res);
    UA_VariableAttributes_deleteMembers(&attr);
    UA_AddNodesItem_deleteMembers(&item);

    // now instantiate the variable for all hastypedefinition references
    for(UA_Int32 i = 0; i < node->referencesSize; i++) {
        UA_ReferenceNode *rn = &node->references[i];
        if(rn->isInverse)
            continue;
        const UA_NodeId hasTypeDef = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
        if(!UA_NodeId_equal(&rn->referenceTypeId, &hasTypeDef))
            continue;
        instantiateVariableNode(server, session, &res.addedNodeId, &rn->targetId.nodeId);
    }

    UA_AddNodesResult_deleteMembers(&res);
    UA_NodeStore_release((const UA_Node*)node);
    return UA_STATUSCODE_GOOD;
}

/* copy an existing object under the given parent. then instantiate the
   variable for all hastypedefinitions of the original version. */
static UA_StatusCode
copyExistingObject(UA_Server *server, UA_Session *session, const UA_NodeId *variable,
                   const UA_NodeId *referenceType, const UA_NodeId *parent) {
    const UA_ObjectNode *node = (const UA_ObjectNode*)UA_NodeStore_get(server->nodestore, variable);
    if(!node)
        return UA_STATUSCODE_BADNODEIDINVALID;
    if(node->nodeClass != UA_NODECLASS_OBJECT) {
        UA_NodeStore_release((const UA_Node*)node);
        return UA_STATUSCODE_BADNODECLASSINVALID;
    }
    // copy the variable attributes
    UA_ObjectAttributes attr;
    UA_ObjectAttributes_init(&attr);
    UA_LocalizedText_copy(&node->displayName, &attr.displayName);
    UA_LocalizedText_copy(&node->description, &attr.description);
    attr.writeMask = node->writeMask;
    attr.userWriteMask = node->userWriteMask;
    attr.eventNotifier = node->eventNotifier;

    UA_AddNodesItem item;
    UA_AddNodesItem_init(&item);
    UA_NodeId_copy(parent, &item.parentNodeId.nodeId);
    UA_NodeId_copy(referenceType, &item.referenceTypeId);
    UA_QualifiedName_copy(&node->browseName, &item.browseName);
    item.nodeClass = UA_NODECLASS_OBJECT;
    // don't add a typedefinition here.

    // add the new object
    UA_AddNodesResult res;
    UA_AddNodesResult_init(&res);
    Service_AddNodes_single(server, session, &item, (UA_NodeAttributes*)&attr, &res);
    UA_ObjectAttributes_deleteMembers(&attr);
    UA_AddNodesItem_deleteMembers(&item);

    // now instantiate the object for all hastypedefinition references
    for(UA_Int32 i = 0; i < node->referencesSize; i++) {
        UA_ReferenceNode *rn = &node->references[i];
        if(rn->isInverse)
            continue;
        const UA_NodeId hasTypeDef = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
        if(!UA_NodeId_equal(&rn->referenceTypeId, &hasTypeDef))
            continue;
        instantiateObjectNode(server, session, &res.addedNodeId, &rn->targetId.nodeId);
    }

    UA_AddNodesResult_deleteMembers(&res);
    UA_NodeStore_release((const UA_Node*)node);
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
setObjectInstanceHandle(UA_Server *server, UA_Session *session, UA_ObjectNode* node, void *handle) {
    if(node->nodeClass != UA_NODECLASS_OBJECT)
        return UA_STATUSCODE_BADNODECLASSINVALID;
    node->instanceHandle = handle;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
instantiateObjectNode(UA_Server *server, UA_Session *session,
                      const UA_NodeId *nodeId, const UA_NodeId *typeId) {
    const UA_ObjectTypeNode *type = (const UA_ObjectTypeNode*)UA_NodeStore_get(server->nodestore, typeId);
    if(!type)
        return UA_STATUSCODE_BADNODEIDINVALID;
    if(type->nodeClass != UA_NODECLASS_OBJECTTYPE) {
        UA_NodeStore_release((const UA_Node*)type);
        return UA_STATUSCODE_BADNODECLASSINVALID;
    }

    /* Add all the child nodes */
    UA_BrowseDescription browseChildren;
    UA_BrowseDescription_init(&browseChildren);
    browseChildren.nodeId = *typeId;
    browseChildren.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_AGGREGATES);
    browseChildren.includeSubtypes = UA_TRUE;
    browseChildren.browseDirection = UA_BROWSEDIRECTION_FORWARD;
    browseChildren.nodeClassMask = UA_NODECLASS_OBJECT | UA_NODECLASS_VARIABLE | UA_NODECLASS_METHOD;
    browseChildren.resultMask = UA_BROWSERESULTMASK_REFERENCETYPEID | UA_BROWSERESULTMASK_NODECLASS;

    UA_BrowseResult browseResult;
    UA_BrowseResult_init(&browseResult);
    // todo: continuation points if there are too many results
    Service_Browse_single(server, session, NULL, &browseChildren, 100, &browseResult);

    for(UA_Int32 i = 0; i < browseResult.referencesSize; i++) {
        UA_ReferenceDescription *rd = &browseResult.references[i];
        if(rd->nodeClass == UA_NODECLASS_METHOD) {
            /* add a reference to the method in the objecttype */
            UA_AddReferencesItem item;
            UA_AddReferencesItem_init(&item);
            item.sourceNodeId = *nodeId;
            item.referenceTypeId = rd->referenceTypeId;
            item.isForward = UA_TRUE;
            item.targetNodeId = rd->nodeId;
            item.targetNodeClass = UA_NODECLASS_METHOD;
            Service_AddReferences_single(server, session, &item);
        } else if(rd->nodeClass == UA_NODECLASS_VARIABLE)
            copyExistingVariable(server, session, &rd->nodeId.nodeId, &rd->referenceTypeId, nodeId);
        else if(rd->nodeClass == UA_NODECLASS_OBJECT)
            copyExistingObject(server, session, &rd->nodeId.nodeId, &rd->referenceTypeId, nodeId);
    }

    /* add a hastypedefinition reference */
    UA_AddReferencesItem addref;
    UA_AddReferencesItem_init(&addref);
    addref.sourceNodeId = *nodeId;
    addref.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
    addref.isForward = UA_TRUE;
    addref.targetNodeId.nodeId = *typeId;
    addref.targetNodeClass = UA_NODECLASS_OBJECTTYPE;
    Service_AddReferences_single(server, session, &addref);

    /* call the constructor */
    const UA_ObjectLifecycleManagement *olm = &type->lifecycleManagement;
    if(olm->constructor)
        UA_Server_editNode(server, session, nodeId,
                           (UA_EditNodeCallback)setObjectInstanceHandle, olm->constructor(*nodeId));
    UA_NodeStore_release((const UA_Node*)type);
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
instantiateVariableNode(UA_Server *server, UA_Session *session, const UA_NodeId *nodeId,
                        const UA_NodeId *typeId) {
    const UA_ObjectTypeNode *type = (const UA_ObjectTypeNode*)UA_NodeStore_get(server->nodestore, typeId);
    if(!type)
        return UA_STATUSCODE_BADNODEIDINVALID;
    if(type->nodeClass != UA_NODECLASS_VARIABLETYPE) {
        UA_NodeStore_release((const UA_Node*)type);
        return UA_STATUSCODE_BADNODECLASSINVALID;
    }

    /* get the references to child properties */
    UA_BrowseDescription browseChildren;
    UA_BrowseDescription_init(&browseChildren);
    browseChildren.nodeId = *typeId;
    browseChildren.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
    browseChildren.includeSubtypes = UA_TRUE;
    browseChildren.browseDirection = UA_BROWSEDIRECTION_FORWARD;
    browseChildren.nodeClassMask = UA_NODECLASS_VARIABLE;
    browseChildren.resultMask = UA_BROWSERESULTMASK_REFERENCETYPEID | UA_BROWSERESULTMASK_NODECLASS;

    UA_BrowseResult browseResult;
    UA_BrowseResult_init(&browseResult);
    // todo: continuation points if there are too many results
    Service_Browse_single(server, session, NULL, &browseChildren, 100, &browseResult);

    /* add the child properties */
    for(UA_Int32 i = 0; i < browseResult.referencesSize; i++) {
        UA_ReferenceDescription *rd = &browseResult.references[i];
        copyExistingVariable(server, session, &rd->nodeId.nodeId, &rd->referenceTypeId, nodeId);
    }
    UA_NodeStore_release((const UA_Node*)type);

    /* add a hastypedefinition reference */
    UA_AddReferencesItem addref;
    UA_AddReferencesItem_init(&addref);
    addref.sourceNodeId = *nodeId;
    addref.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);
    addref.isForward = UA_TRUE;
    addref.targetNodeId.nodeId = *typeId;
    addref.targetNodeClass = UA_NODECLASS_OBJECTTYPE;
    Service_AddReferences_single(server, session, &addref);

    return UA_STATUSCODE_GOOD;
}

static void moveStandardAttributes(UA_Node *node, UA_AddNodesItem *item, UA_NodeAttributes *attr) {
    node->nodeId = item->requestedNewNodeId.nodeId;
    UA_NodeId_init(&item->requestedNewNodeId.nodeId);
    node->browseName = item->browseName;
    UA_QualifiedName_init(&item->browseName);
    node->displayName = attr->displayName;
    UA_LocalizedText_init(&attr->displayName);
    node->description = attr->description;
    UA_LocalizedText_init(&attr->description);
    node->writeMask = attr->writeMask;
    node->userWriteMask = attr->userWriteMask;
}

static UA_Node *
variableNodeFromAttributes(UA_AddNodesItem *item, UA_VariableAttributes *attr) {
    UA_VariableNode *vnode = UA_VariableNode_new();
    if(!vnode)
        return NULL;
    moveStandardAttributes((UA_Node*)vnode, item, (UA_NodeAttributes*)attr);
    // todo: test if the type / valueRank / value attributes are consistent
    vnode->accessLevel = attr->accessLevel;
    vnode->userAccessLevel = attr->userAccessLevel;
    vnode->historizing = attr->historizing;
    vnode->minimumSamplingInterval = attr->minimumSamplingInterval;
    vnode->valueRank = attr->valueRank;
    vnode->value.variant.value = attr->value;
    UA_Variant_init(&attr->value);
    return (UA_Node*)vnode;
}

static UA_Node *
objectNodeFromAttributes(UA_AddNodesItem *item, UA_ObjectAttributes *attr) {
    UA_ObjectNode *onode = UA_ObjectNode_new();
    if(!onode)
        return NULL;
    moveStandardAttributes((UA_Node*)onode, item, (UA_NodeAttributes*)attr);
    onode->eventNotifier = attr->eventNotifier;
    return (UA_Node*)onode;
}

static UA_Node *
referenceTypeNodeFromAttributes(UA_AddNodesItem *item, UA_ReferenceTypeAttributes *attr) {
    UA_ReferenceTypeNode *rtnode = UA_ReferenceTypeNode_new();
    if(!rtnode)
        return NULL;
    moveStandardAttributes((UA_Node*)rtnode, item, (UA_NodeAttributes*)attr);
    rtnode->isAbstract = attr->isAbstract;
    rtnode->symmetric = attr->symmetric;
    rtnode->inverseName = attr->inverseName;
    UA_LocalizedText_init(&attr->inverseName);
    return (UA_Node*)rtnode;
}

static UA_Node *
objectTypeNodeFromAttributes(UA_AddNodesItem *item, UA_ObjectTypeAttributes *attr) {
    UA_ObjectTypeNode *otnode = UA_ObjectTypeNode_new();
    if(!otnode)
        return NULL;
    moveStandardAttributes((UA_Node*)otnode, item, (UA_NodeAttributes*)attr);
    otnode->isAbstract = attr->isAbstract;
    return (UA_Node*)otnode;
}

static UA_Node *
variableTypeNodeFromAttributes(UA_AddNodesItem *item, UA_VariableTypeAttributes *attr) {
    UA_VariableTypeNode *vtnode = UA_VariableTypeNode_new();
    if(!vtnode)
        return NULL;
    moveStandardAttributes((UA_Node*)vtnode, item, (UA_NodeAttributes*)attr);
    vtnode->value.variant.value = attr->value;
    UA_Variant_init(&attr->value);
    // datatype is taken from the value
    vtnode->valueRank = attr->valueRank;
    // array dimensions are taken from the value
    vtnode->isAbstract = attr->isAbstract;
    return (UA_Node*)vtnode;
}

static UA_Node *
viewNodeFromAttributes(UA_AddNodesItem *item, UA_ViewAttributes *attr) {
    UA_ViewNode *vnode = UA_ViewNode_new();
    if(!vnode)
        return NULL;
    moveStandardAttributes((UA_Node*)vnode, item, (UA_NodeAttributes*)attr);
    vnode->containsNoLoops = attr->containsNoLoops;
    vnode->eventNotifier = attr->eventNotifier;
    return (UA_Node*)vnode;
}

static UA_Node *
dataTypeNodeFromAttributes(UA_AddNodesItem *item, UA_DataTypeAttributes *attr) {
    UA_DataTypeNode *dtnode = UA_DataTypeNode_new();
    if(!dtnode)
        return NULL;
    moveStandardAttributes((UA_Node*)dtnode, item, (UA_NodeAttributes*)attr);
    dtnode->isAbstract = attr->isAbstract;
    return (UA_Node*)dtnode;
}

void Service_AddNodes_single(UA_Server *server, UA_Session *session, UA_AddNodesItem *item,
                             UA_NodeAttributes *attr, UA_AddNodesResult *result) {
    /* create the node */
    UA_Node *node;
    switch(item->nodeClass) {
    case UA_NODECLASS_OBJECT:
        node = objectNodeFromAttributes(item, (UA_ObjectAttributes*)attr);
        break;
    case UA_NODECLASS_VARIABLE:
        node = variableNodeFromAttributes(item, (UA_VariableAttributes*)attr);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        node = objectTypeNodeFromAttributes(item, (UA_ObjectTypeAttributes*)attr);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        node = variableTypeNodeFromAttributes(item, (UA_VariableTypeAttributes*)attr);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        node = referenceTypeNodeFromAttributes(item, (UA_ReferenceTypeAttributes*)attr);
        break;
    case UA_NODECLASS_DATATYPE:
        node = dataTypeNodeFromAttributes(item, (UA_DataTypeAttributes*)attr);
        break;
    case UA_NODECLASS_VIEW:
        node = viewNodeFromAttributes(item, (UA_ViewAttributes*)attr);
        break;
    case UA_NODECLASS_METHOD:
    case UA_NODECLASS_UNSPECIFIED:
    default:
        result->statusCode = UA_STATUSCODE_BADNODECLASSINVALID;
        return;
    }

    if(!node) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

    /* add it to the server */
    UA_Server_addExistingNode(server, session, node, &item->parentNodeId.nodeId,
                              &item->referenceTypeId, result);
    if(result->statusCode != UA_STATUSCODE_GOOD) {
        UA_Node_deleteAnyNodeClass(node);
        return;
    }

    /* instantiate if it has a type */
    if(!UA_NodeId_isNull(&item->typeDefinition.nodeId)) {
        if(item->nodeClass == UA_NODECLASS_OBJECT)
            result->statusCode = instantiateObjectNode(server, session, &result->addedNodeId,
                                                       &item->typeDefinition.nodeId);
        else if(item->nodeClass == UA_NODECLASS_VARIABLE)
            result->statusCode = instantiateVariableNode(server, session, &result->addedNodeId,
                                                         &item->typeDefinition.nodeId);
    }

    /* if instantiation failed, remove the node */
    if(result->statusCode != UA_STATUSCODE_GOOD)
        Service_DeleteNodes_single(server, session, &result->addedNodeId, UA_TRUE);
}

static void Service_AddNodes_single_unparsed(UA_Server *server, UA_Session *session, UA_AddNodesItem *item,
                                             UA_AddNodesResult *result) {
    /* adding nodes to ns0 is not allowed over the wire */
    if(item->requestedNewNodeId.nodeId.namespaceIndex == 0) {
        result->statusCode = UA_STATUSCODE_BADNODEIDREJECTED;
        return;
    }

    const UA_DataType *attributeType;
    switch (item->nodeClass) {
    case UA_NODECLASS_OBJECT:
        attributeType = &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES];
        break;
    case UA_NODECLASS_OBJECTTYPE:
        attributeType = &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES];
        break;
    case UA_NODECLASS_REFERENCETYPE:
        attributeType = &UA_TYPES[UA_TYPES_REFERENCETYPEATTRIBUTES];
        break;
    case UA_NODECLASS_VARIABLE:
        attributeType = &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES];
        break;
    case UA_NODECLASS_VIEW:
        attributeType = &UA_TYPES[UA_TYPES_VIEWATTRIBUTES];
        break;
    case UA_NODECLASS_DATATYPE:
        attributeType = &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES];
        break;
    default:
        result->statusCode = UA_STATUSCODE_BADNOTIMPLEMENTED;
        return;
    }

    if(!UA_NodeId_equal(&item->nodeAttributes.typeId, &attributeType->typeId)) {
        result->statusCode = UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
        return;
    }

    UA_NodeAttributes *attr = UA_alloca(attributeType->memSize);
    UA_init(attr, attributeType);
    size_t pos = 0;
    result->statusCode = UA_decodeBinary(&item->nodeAttributes.body, &pos, attr, attributeType);
    if(result->statusCode != UA_STATUSCODE_GOOD)
        return;

    Service_AddNodes_single(server, session, item, attr, result);
    UA_deleteMembers(attr, attributeType);
}

void Service_AddNodes(UA_Server *server, UA_Session *session, const UA_AddNodesRequest *request,
                      UA_AddNodesResponse *response) {
    if(request->nodesToAddSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
    size_t size = request->nodesToAddSize;

    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_ADDNODESRESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    
#ifdef UA_EXTERNAL_NAMESPACES
#ifdef _MSVC_VER
    UA_Boolean *isExternal = UA_alloca(size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32)*size);
#else
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#endif
    memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0; j <server->externalNamespacesSize; j++) {
        size_t indexSize = 0;
        for(size_t i = 0;i < size;i++) {
            if(request->nodesToAdd[i].requestedNewNodeId.nodeId.namespaceIndex !=
               server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->addNodes(ens->ensHandle, &request->requestHeader, request->nodesToAdd,
                      indices, indexSize, response->results, response->diagnosticInfos);
    }
#endif
    
    response->resultsSize = size;
    for(size_t i = 0;i < size;i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            Service_AddNodes_single_unparsed(server, session, &request->nodesToAdd[i], &response->results[i]);
    }
}

/**************************************************/
/* Add Special Nodes (not possible over the wire) */
/**************************************************/

UA_StatusCode UA_EXPORT
UA_Server_addDataSourceVariableNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
                                    const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
                                    const UA_QualifiedName browseName, const UA_NodeId typeDefinition,
                                    const UA_VariableAttributes attr, const UA_DataSource dataSource, UA_NodeId *outNewNodeId) {
    UA_AddNodesResult result;
    UA_AddNodesResult_init(&result);

    UA_AddNodesItem item;
    UA_AddNodesItem_init(&item);
    result.statusCode = UA_QualifiedName_copy(&browseName, &item.browseName);
    item.nodeClass = UA_NODECLASS_VARIABLE;
    result.statusCode |= UA_NodeId_copy(&parentNodeId, &item.parentNodeId.nodeId);
    result.statusCode |= UA_NodeId_copy(&referenceTypeId, &item.referenceTypeId);
    result.statusCode |= UA_NodeId_copy(&requestedNewNodeId, &item.requestedNewNodeId.nodeId);
    result.statusCode |= UA_NodeId_copy(&typeDefinition, &item.typeDefinition.nodeId);
    
    UA_VariableAttributes attrCopy;
    result.statusCode |= UA_VariableAttributes_copy(&attr, &attrCopy);
    if(result.statusCode != UA_STATUSCODE_GOOD) {
        UA_AddNodesItem_deleteMembers(&item);
        UA_VariableAttributes_deleteMembers(&attrCopy);
        return result.statusCode;
    }

    UA_VariableNode *node = UA_VariableNode_new();
    if(!node) {
        UA_AddNodesItem_deleteMembers(&item);
        UA_VariableAttributes_deleteMembers(&attrCopy);
        return UA_STATUSCODE_BADOUTOFMEMORY;
    }

    moveStandardAttributes((UA_Node*)node, &item, (UA_NodeAttributes*)&attrCopy);
    node->valueSource = UA_VALUESOURCE_DATASOURCE;
    node->value.dataSource = dataSource;
    node->accessLevel = attr.accessLevel;
    node->userAccessLevel = attr.userAccessLevel;
    node->historizing = attr.historizing;
    node->minimumSamplingInterval = attr.minimumSamplingInterval;
    node->valueRank = attr.valueRank;

    UA_Server_addExistingNode(server, &adminSession, (UA_Node*)node, &item.parentNodeId.nodeId,
                              &item.referenceTypeId, &result);
    UA_AddNodesItem_deleteMembers(&item);
    UA_VariableAttributes_deleteMembers(&attrCopy);

    if(result.statusCode != UA_STATUSCODE_GOOD)
        UA_VariableNode_delete(node);

    if(outNewNodeId && result.statusCode == UA_STATUSCODE_GOOD)
        *outNewNodeId = result.addedNodeId;
    else
        UA_AddNodesResult_deleteMembers(&result);
    return result.statusCode;
}

#ifdef ENABLE_METHODCALLS
UA_StatusCode UA_EXPORT
UA_Server_addMethodNode(UA_Server *server, const UA_NodeId requestedNewNodeId,
                        const UA_NodeId parentNodeId, const UA_NodeId referenceTypeId,
                        const UA_QualifiedName browseName, const UA_MethodAttributes attr,
                        UA_MethodCallback method, void *handle,
                        UA_Int32 inputArgumentsSize, const UA_Argument* inputArguments, 
                        UA_Int32 outputArgumentsSize, const UA_Argument* outputArguments,
                        UA_NodeId *outNewNodeId) {
    UA_AddNodesResult result;
    UA_AddNodesResult_init(&result);
    
    UA_AddNodesItem item;
    UA_AddNodesItem_init(&item);
    result.statusCode = UA_QualifiedName_copy(&browseName, &item.browseName);
    item.nodeClass = UA_NODECLASS_METHOD;
    result.statusCode |= UA_NodeId_copy(&parentNodeId, &item.parentNodeId.nodeId);
    result.statusCode |= UA_NodeId_copy(&referenceTypeId, &item.referenceTypeId);
    result.statusCode |= UA_NodeId_copy(&requestedNewNodeId, &item.requestedNewNodeId.nodeId);
    
    UA_MethodAttributes attrCopy;
    result.statusCode |= UA_MethodAttributes_copy(&attr, &attrCopy);
    if(result.statusCode != UA_STATUSCODE_GOOD) {
        UA_AddNodesItem_deleteMembers(&item);
        UA_MethodAttributes_deleteMembers(&attrCopy);
        return result.statusCode;
    }

    UA_MethodNode *node = UA_MethodNode_new();
    if(!node) {
        result.statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        UA_AddNodesItem_deleteMembers(&item);
        UA_MethodAttributes_deleteMembers(&attrCopy);
        return result.statusCode;
    }
    
    moveStandardAttributes((UA_Node*)node, &item, (UA_NodeAttributes*)&attrCopy);
    node->executable = attrCopy.executable;
    node->userExecutable = attrCopy.executable;
    node->attachedMethod = method;
    node->methodHandle = handle;
    UA_AddNodesItem_deleteMembers(&item);
    UA_MethodAttributes_deleteMembers(&attrCopy);

    UA_Server_addExistingNode(server, &adminSession, (UA_Node*)node, &item.parentNodeId.nodeId,
                              &item.referenceTypeId, &result);
    if(result.statusCode != UA_STATUSCODE_GOOD)
        UA_MethodNode_delete(node);
    
    if(result.statusCode == UA_STATUSCODE_GOOD && method != NULL) {
        UA_ExpandedNodeId parent;
        UA_ExpandedNodeId_init(&parent);
        parent.nodeId = result.addedNodeId;
    
        /* create InputArguments */
        UA_VariableNode *inputArgumentsVariableNode = UA_VariableNode_new();
        inputArgumentsVariableNode->nodeId.namespaceIndex = result.addedNodeId.namespaceIndex;
        inputArgumentsVariableNode->browseName = UA_QUALIFIEDNAME_ALLOC(0,"InputArguments");
        inputArgumentsVariableNode->displayName = UA_LOCALIZEDTEXT_ALLOC("en_US", "InputArguments");
        inputArgumentsVariableNode->description = UA_LOCALIZEDTEXT_ALLOC("en_US", "InputArguments");
        inputArgumentsVariableNode->valueRank = 1;
        UA_Variant_setArrayCopy(&inputArgumentsVariableNode->value.variant.value, inputArguments,
                                inputArgumentsSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
        UA_AddNodesResult inputAddRes;
        const UA_NodeId hasproperty = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
        UA_Server_addExistingNode(server, &adminSession, (UA_Node*)inputArgumentsVariableNode,
                                  &parent.nodeId, &hasproperty, &inputAddRes);
        // todo: check if adding succeeded
        UA_AddNodesResult_deleteMembers(&inputAddRes);

        /* create OutputArguments */
        UA_VariableNode *outputArgumentsVariableNode  = UA_VariableNode_new();
        outputArgumentsVariableNode->nodeId.namespaceIndex = result.addedNodeId.namespaceIndex;
        outputArgumentsVariableNode->browseName  = UA_QUALIFIEDNAME_ALLOC(0,"OutputArguments");
        outputArgumentsVariableNode->displayName = UA_LOCALIZEDTEXT_ALLOC("en_US", "OutputArguments");
        outputArgumentsVariableNode->description = UA_LOCALIZEDTEXT_ALLOC("en_US", "OutputArguments");
        outputArgumentsVariableNode->valueRank = 1;
        UA_Variant_setArrayCopy(&outputArgumentsVariableNode->value.variant.value, outputArguments,
                                outputArgumentsSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
        UA_AddNodesResult outputAddRes;
        UA_Server_addExistingNode(server, &adminSession, (UA_Node*)outputArgumentsVariableNode,
                                  &parent.nodeId, &hasproperty, &outputAddRes);
        // todo: check if adding succeeded
        UA_AddNodesResult_deleteMembers(&outputAddRes);
    }

    if(outNewNodeId && result.statusCode == UA_STATUSCODE_GOOD)
        *outNewNodeId = result.addedNodeId;
    else
        UA_AddNodesResult_deleteMembers(&result);
    return result.statusCode;
}
#endif

/******************/
/* Add References */
/******************/

/* Adds a one-way reference to the local nodestore */
static UA_StatusCode
addOneWayReference(UA_Server *server, UA_Session *session, UA_Node *node, const UA_AddReferencesItem *item) {
	size_t i = node->referencesSize;
	if(node->referencesSize < 0)
		i = 0;
    size_t refssize = (i+1) | 3; // so the realloc is not necessary every time
	UA_ReferenceNode *new_refs = UA_realloc(node->references, sizeof(UA_ReferenceNode) * refssize);
	if(!new_refs)
		return UA_STATUSCODE_BADOUTOFMEMORY;
    node->references = new_refs;
    UA_ReferenceNode_init(&new_refs[i]);
    UA_StatusCode retval = UA_NodeId_copy(&item->referenceTypeId, &new_refs[i].referenceTypeId);
    retval |= UA_ExpandedNodeId_copy(&item->targetNodeId, &new_refs[i].targetId);
    new_refs[i].isInverse = !item->isForward;
    if(retval == UA_STATUSCODE_GOOD) 
        node->referencesSize = i+1;
    else
        UA_ReferenceNode_deleteMembers(&new_refs[i]);
	return retval;
}

UA_StatusCode Service_AddReferences_single(UA_Server *server, UA_Session *session,
                                           const UA_AddReferencesItem *item) {
    if(item->targetServerUri.length > 0)
        return UA_STATUSCODE_BADNOTIMPLEMENTED; // currently no expandednodeids are allowed

    /* cast away the const to loop the call through UA_Server_editNode */
    UA_StatusCode retval = UA_Server_editNode(server, session, &item->sourceNodeId,
                                              (UA_EditNodeCallback)addOneWayReference,
                                              item);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_AddReferencesItem secondItem;
    secondItem = *item;
    secondItem.targetNodeId.nodeId = item->sourceNodeId;
    secondItem.sourceNodeId = item->targetNodeId.nodeId;
    secondItem.isForward = !item->isForward;
    retval = UA_Server_editNode(server, session, &secondItem.sourceNodeId,
                                (UA_EditNodeCallback)addOneWayReference, &secondItem);

    // todo: remove reference if the second direction failed
    return retval;
} 

void Service_AddReferences(UA_Server *server, UA_Session *session, const UA_AddReferencesRequest *request,
                           UA_AddReferencesResponse *response) {
	if(request->referencesToAddSize <= 0) {
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
		return;
	}
    size_t size = request->referencesToAddSize;
	
    if(!(response->results = UA_malloc(sizeof(UA_StatusCode) * size))) {
		response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
		return;
	}
	response->resultsSize = size;

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
	for(size_t j = 0; j < server->externalNamespacesSize; j++) {
		size_t indicesSize = 0;
		for(size_t i = 0;i < size;i++) {
			if(request->referencesToAdd[i].sourceNodeId.namespaceIndex
               != server->externalNamespaces[j].index)
				continue;
			isExternal[i] = UA_TRUE;
			indices[indicesSize] = i;
			indicesSize++;
		}
		if (indicesSize == 0)
			continue;
		UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
		ens->addReferences(ens->ensHandle, &request->requestHeader, request->referencesToAdd,
                           indices, indicesSize, response->results, response->diagnosticInfos);
	}
#endif

	response->resultsSize = size;
	for(UA_Int32 i = 0; i < response->resultsSize; i++) {
#ifdef UA_EXTERNAL_NAMESPACES
		if(!isExternal[i])
#endif
            Service_AddReferences_single(server, session, &request->referencesToAdd[i]);
	}
}

/****************/
/* Delete Nodes */
/****************/

// TODO: Check consistency constraints, remove the references.

UA_StatusCode Service_DeleteNodes_single(UA_Server *server, UA_Session *session, const UA_NodeId *nodeId,
                                         UA_Boolean deleteReferences) {
    const UA_Node *node = UA_NodeStore_get(server->nodestore, nodeId);
    if(!node)
        return UA_STATUSCODE_BADNODEIDINVALID;
    if(deleteReferences == UA_TRUE) {
        UA_DeleteReferencesItem delItem;
        UA_DeleteReferencesItem_init(&delItem);
        delItem.deleteBidirectional = UA_FALSE;
        delItem.targetNodeId.nodeId = *nodeId;
        for(int i = 0; i < node->referencesSize; i++) {
            delItem.sourceNodeId = node->references[i].targetId.nodeId;
            delItem.isForward = node->references[i].isInverse;
            Service_DeleteReferences_single(server, session, &delItem);
        }
    }

    /* destroy an object before removing it */
    if(node->nodeClass == UA_NODECLASS_OBJECT) {
        /* find the object type(s) */
        UA_BrowseDescription bd;
        UA_BrowseDescription_init(&bd);
        bd.browseDirection = UA_BROWSEDIRECTION_INVERSE;
        bd.nodeId = *nodeId;
        bd.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE);
        bd.includeSubtypes = UA_TRUE;
        bd.nodeClassMask = UA_NODECLASS_OBJECTTYPE;
        
        /* browse type definitions with admin rights */
        UA_BrowseResult result;
        UA_BrowseResult_init(&result);
        Service_Browse_single(server, &adminSession, NULL, &bd, UA_UINT32_MAX, &result);
        for(int i = 0; i < result.referencesSize; i++) {
            /* call the destructor */
            UA_ReferenceDescription *rd = &result.references[i];
            const UA_ObjectTypeNode *type = (const UA_ObjectTypeNode*)UA_NodeStore_get(server->nodestore, &rd->nodeId.nodeId);
            if(!type)
                continue;
            if(type->nodeClass != UA_NODECLASS_OBJECTTYPE || !type->lifecycleManagement.destructor) {
                UA_NodeStore_release((const UA_Node*)type);
                continue;
            }
            /* if there are several types with lifecycle management, call all the destructors */
            type->lifecycleManagement.destructor(*nodeId, ((const UA_ObjectNode*)node)->instanceHandle);
            UA_NodeStore_release((const UA_Node*)type);
        }
        UA_BrowseResult_deleteMembers(&result);
    }
    
    UA_NodeStore_release(node);
    return UA_NodeStore_remove(server->nodestore, nodeId);
}

void Service_DeleteNodes(UA_Server *server, UA_Session *session, const UA_DeleteNodesRequest *request,
                         UA_DeleteNodesResponse *response) {
    if(request->nodesToDeleteSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
    response->results = UA_malloc(sizeof(UA_StatusCode) * request->nodesToDeleteSize);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;;
        return;
    }
    response->resultsSize = request->nodesToDeleteSize;
    for(int i=0; i<request->nodesToDeleteSize; i++) {
        UA_DeleteNodesItem *item = &request->nodesToDelete[i];
        response->results[i] = Service_DeleteNodes_single(server, session, &item->nodeId,
                                                          item->deleteTargetReferences);
    }
}

/*********************/
/* Delete References */
/*********************/

static UA_StatusCode
deleteOneWayReference(UA_Server *server, UA_Session *session, UA_Node *node,
                      const UA_DeleteReferencesItem *item) {
    UA_Boolean edited = UA_FALSE;
    for(UA_Int32 i = node->referencesSize - 1; i >= 0; i--) {
        if(!UA_NodeId_equal(&item->targetNodeId.nodeId, &node->references[i].targetId.nodeId))
            continue;
        if(!UA_NodeId_equal(&item->referenceTypeId, &node->references[i].referenceTypeId))
            continue;
        if(item->isForward == node->references[i].isInverse)
            continue;
        /* move the last entry to override the current position */
        UA_ReferenceNode_deleteMembers(&node->references[i]);
        node->references[i] = node->references[node->referencesSize-1];
        node->referencesSize--;
        edited = UA_TRUE;
    }
    if(!edited)
        return UA_STATUSCODE_UNCERTAINREFERENCENOTDELETED;
    /* we removed the last reference */
    if(node->referencesSize <= 0 && node->references)
        UA_free(node->references);
    return UA_STATUSCODE_GOOD;;
}

UA_StatusCode
Service_DeleteReferences_single(UA_Server *server, UA_Session *session, const UA_DeleteReferencesItem *item) {
    UA_StatusCode retval = UA_Server_editNode(server, session, &item->sourceNodeId,
                                       (UA_EditNodeCallback)deleteOneWayReference, item);
    if(!item->deleteBidirectional || item->targetNodeId.serverIndex != 0)
        return retval;
    UA_DeleteReferencesItem secondItem;
    UA_DeleteReferencesItem_init(&secondItem);
    secondItem.isForward = !item->isForward;
    secondItem.sourceNodeId = item->targetNodeId.nodeId;
    secondItem.targetNodeId.nodeId = item->sourceNodeId;
    return UA_Server_editNode(server, session, &secondItem.sourceNodeId,
                              (UA_EditNodeCallback)deleteOneWayReference, &secondItem);
}

void Service_DeleteReferences(UA_Server *server, UA_Session *session,
                              const UA_DeleteReferencesRequest *request,
                              UA_DeleteReferencesResponse *response) {
    if(request->referencesToDeleteSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
    response->results = UA_malloc(sizeof(UA_StatusCode) * request->referencesToDeleteSize);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;;
        return;
    }
    response->resultsSize = request->referencesToDeleteSize;
    for(int i=0; i<request->referencesToDeleteSize; i++)
        response->results[i] = Service_DeleteReferences_single(server, session,
                                                               &request->referencesToDelete[i]);
}

/*********************************** amalgamated original file "C:/open62541/src/server/ua_services_view.c" ***********************************/


static UA_StatusCode
fillReferenceDescription(UA_NodeStore *ns, const UA_Node *curr, UA_ReferenceNode *ref,
                         UA_UInt32 mask, UA_ReferenceDescription *descr) {
    UA_ReferenceDescription_init(descr);
    UA_StatusCode retval = UA_NodeId_copy(&curr->nodeId, &descr->nodeId.nodeId);
    descr->nodeId.serverIndex = 0;
    descr->nodeId.namespaceUri.length = -1;

    if(mask & UA_BROWSERESULTMASK_REFERENCETYPEID)
        retval |= UA_NodeId_copy(&ref->referenceTypeId, &descr->referenceTypeId);
    if(mask & UA_BROWSERESULTMASK_ISFORWARD)
        descr->isForward = !ref->isInverse;
    if(mask & UA_BROWSERESULTMASK_NODECLASS)
        retval |= UA_NodeClass_copy(&curr->nodeClass, &descr->nodeClass);
    if(mask & UA_BROWSERESULTMASK_BROWSENAME)
        retval |= UA_QualifiedName_copy(&curr->browseName, &descr->browseName);
    if(mask & UA_BROWSERESULTMASK_DISPLAYNAME)
        retval |= UA_LocalizedText_copy(&curr->displayName, &descr->displayName);
    if(mask & UA_BROWSERESULTMASK_TYPEDEFINITION){
        if(curr->nodeClass == UA_NODECLASS_OBJECT || curr->nodeClass == UA_NODECLASS_VARIABLE) {
            for(UA_Int32 i = 0; i < curr->referencesSize; i++) {
                UA_ReferenceNode *refnode = &curr->references[i];
                if(refnode->referenceTypeId.identifier.numeric != UA_NS0ID_HASTYPEDEFINITION)
                    continue;
                retval |= UA_ExpandedNodeId_copy(&refnode->targetId, &descr->typeDefinition);
                break;
            }
        }
    }

    if(retval)
        UA_ReferenceDescription_deleteMembers(descr);
    return retval;
}

#ifdef UA_EXTERNAL_NAMESPACES
static const UA_Node * returnRelevantNodeExternal(UA_ExternalNodeStore *ens, const UA_BrowseDescription *descr,
                                                  const UA_ReferenceNode *reference) {
    /*	prepare a read request in the external nodestore	*/
    UA_ReadValueId *readValueIds = UA_Array_new(&UA_TYPES[UA_TYPES_READVALUEID], 6);
    UA_UInt32 *indices = UA_Array_new(&UA_TYPES[UA_TYPES_UINT32], 6);
    UA_UInt32 indicesSize = 6;
    UA_DataValue *readNodesResults = UA_Array_new(&UA_TYPES[UA_TYPES_DATAVALUE], 6);
    UA_DiagnosticInfo *diagnosticInfos = UA_Array_new(&UA_TYPES[UA_TYPES_DIAGNOSTICINFO], 6);
    for(UA_UInt32 i = 0; i < 6; i++) {
        readValueIds[i].nodeId = reference->targetId.nodeId;
        indices[i] = i;
    }
    readValueIds[0].attributeId = UA_ATTRIBUTEID_NODECLASS;
    readValueIds[1].attributeId = UA_ATTRIBUTEID_BROWSENAME;
    readValueIds[2].attributeId = UA_ATTRIBUTEID_DISPLAYNAME;
    readValueIds[3].attributeId = UA_ATTRIBUTEID_DESCRIPTION;
    readValueIds[4].attributeId = UA_ATTRIBUTEID_WRITEMASK;
    readValueIds[5].attributeId = UA_ATTRIBUTEID_USERWRITEMASK;

    ens->readNodes(ens->ensHandle, NULL, readValueIds, indices,
                   indicesSize, readNodesResults, UA_FALSE, diagnosticInfos);

    /* create and fill a dummy nodeStructure */
    UA_Node *node = (UA_Node*) UA_ObjectNode_new();
    UA_NodeId_copy(&(reference->targetId.nodeId), &(node->nodeId));
    if(readNodesResults[0].status == UA_STATUSCODE_GOOD)
        UA_NodeClass_copy((UA_NodeClass*)readNodesResults[0].value.data, &(node->nodeClass));
    if(readNodesResults[1].status == UA_STATUSCODE_GOOD)
        UA_QualifiedName_copy((UA_QualifiedName*)readNodesResults[1].value.data, &(node->browseName));
    if(readNodesResults[2].status == UA_STATUSCODE_GOOD)
        UA_LocalizedText_copy((UA_LocalizedText*)readNodesResults[2].value.data, &(node->displayName));
    if(readNodesResults[3].status == UA_STATUSCODE_GOOD)
        UA_LocalizedText_copy((UA_LocalizedText*)readNodesResults[3].value.data, &(node->description));
    if(readNodesResults[4].status == UA_STATUSCODE_GOOD)
        UA_UInt32_copy((UA_UInt32*)readNodesResults[4].value.data, &(node->writeMask));
    if(readNodesResults[5].status == UA_STATUSCODE_GOOD)
        UA_UInt32_copy((UA_UInt32*)readNodesResults[5].value.data, &(node->userWriteMask));
    UA_Array_delete(readValueIds, &UA_TYPES[UA_TYPES_READVALUEID], 6);
    UA_Array_delete(indices, &UA_TYPES[UA_TYPES_UINT32], 6);
    UA_Array_delete(readNodesResults, &UA_TYPES[UA_TYPES_DATAVALUE], 6);
    UA_Array_delete(diagnosticInfos, &UA_TYPES[UA_TYPES_DIAGNOSTICINFO], 6);
    if(node && descr->nodeClassMask != 0 && (node->nodeClass & descr->nodeClassMask) == 0) {
        UA_ObjectNode_delete((UA_ObjectNode*)node);
        return NULL;
    }
    return node;
}
#endif

/* Tests if the node is relevant to the browse request and shall be returned. If
   so, it is retrieved from the Nodestore. If not, null is returned. */
static const UA_Node *
returnRelevantNode(UA_Server *server, const UA_BrowseDescription *descr, UA_Boolean return_all,
                   const UA_ReferenceNode *reference, const UA_NodeId *relevant, size_t relevant_count,
                   UA_Boolean *isExternal) {
    /* reference in the right direction? */
    if(reference->isInverse && descr->browseDirection == UA_BROWSEDIRECTION_FORWARD)
        return NULL;
    if(!reference->isInverse && descr->browseDirection == UA_BROWSEDIRECTION_INVERSE)
        return NULL;

    /* is the reference part of the hierarchy of references we look for? */
    if(!return_all) {
        UA_Boolean is_relevant = UA_FALSE;
        for(size_t i = 0; i < relevant_count; i++) {
            if(UA_NodeId_equal(&reference->referenceTypeId, &relevant[i])) {
                is_relevant = UA_TRUE;
                break;
            }
        }
        if(!is_relevant)
            return NULL;
    }

#ifdef UA_EXTERNAL_NAMESPACES
    /* return the node from an external namespace*/
	for(size_t nsIndex = 0; nsIndex < server->externalNamespacesSize; nsIndex++) {
		if(reference->targetId.nodeId.namespaceIndex != server->externalNamespaces[nsIndex].index)
			continue;
        *isExternal = UA_TRUE;
        return returnRelevantNodeExternal(&server->externalNamespaces[nsIndex].externalNodeStore,
                                          descr, reference);
    }
#endif

    /* return from the internal nodestore */
    const UA_Node *node = UA_NodeStore_get(server->nodestore, &reference->targetId.nodeId);
    if(node && descr->nodeClassMask != 0 && (node->nodeClass & descr->nodeClassMask) == 0) {
    	UA_NodeStore_release(node);
        return NULL;
    }
    *isExternal = UA_FALSE;
    return node;
}

/**
 * We find all subtypes by a single iteration over the array. We start with an array with a single
 * root nodeid at the beginning. When we find relevant references, we add the nodeids to the back of
 * the array and increase the size. Since the hierarchy is not cyclic, we can safely progress in the
 * array to process the newly found referencetype nodeids (emulated recursion).
 */
static UA_StatusCode
findSubTypes(UA_NodeStore *ns, const UA_NodeId *root, UA_NodeId **reftypes, size_t *reftypes_count) {
    const UA_Node *node = UA_NodeStore_get(ns, root);
    if(!node)
        return UA_STATUSCODE_BADNOMATCH;
    if(node->nodeClass != UA_NODECLASS_REFERENCETYPE)  {
        UA_NodeStore_release(node);
        return UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
    }
    UA_NodeStore_release(node);

    size_t results_size = 20; // probably too big, but saves mallocs
    UA_NodeId *results = UA_malloc(sizeof(UA_NodeId) * results_size);
    if(!results)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    UA_StatusCode retval = UA_NodeId_copy(root, &results[0]);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_free(results);
        return retval;
    }
        
    size_t index = 0; // where are we currently in the array?
    size_t last = 0; // where is the last element in the array?
    do {
        node = UA_NodeStore_get(ns, &results[index]);
        if(!node || node->nodeClass != UA_NODECLASS_REFERENCETYPE)
            continue;
        for(UA_Int32 i = 0; i < node->referencesSize; i++) {
            if(node->references[i].referenceTypeId.identifier.numeric != UA_NS0ID_HASSUBTYPE ||
               node->references[i].isInverse == UA_TRUE)
                continue;

            if(++last >= results_size) { // is the array big enough?
                UA_NodeId *new_results = UA_realloc(results, sizeof(UA_NodeId) * results_size * 2);
                if(!new_results) {
                    retval = UA_STATUSCODE_BADOUTOFMEMORY;
                    break;
                }
                results = new_results;
                results_size *= 2;
            }

            retval = UA_NodeId_copy(&node->references[i].targetId.nodeId, &results[last]);
            if(retval != UA_STATUSCODE_GOOD) {
                last--; // for array_delete
                break;
            }
        }
        UA_NodeStore_release(node);
    } while(++index <= last && retval == UA_STATUSCODE_GOOD);

    if(retval) {
        UA_Array_delete(results, &UA_TYPES[UA_TYPES_NODEID], last);
        return retval;
    }

    *reftypes = results;
    *reftypes_count = last + 1;
    return UA_STATUSCODE_GOOD;
}

static void removeCp(struct ContinuationPointEntry *cp, UA_Session* session) {
    session->availableContinuationPoints++;
    UA_ByteString_deleteMembers(&cp->identifier);
    UA_BrowseDescription_deleteMembers(&cp->browseDescription);
    LIST_REMOVE(cp, pointers);
    UA_free(cp);
}

/**
 * Results for a single browsedescription. This is the inner loop for both Browse and BrowseNext
 * @param session Session to save continuationpoints
 * @param ns The nodstore where the to-be-browsed node can be found
 * @param cp If cp is not null, we continue from here
 *           If cp is null, we can add a new continuation point if possible and necessary.
 * @param descr If no cp is set, we take the browsedescription from there
 * @param maxrefs The maximum number of references the client has requested
 * @param result The entry in the request
 */
void
Service_Browse_single(UA_Server *server, UA_Session *session, struct ContinuationPointEntry *cp,
                      const UA_BrowseDescription *descr, UA_UInt32 maxrefs, UA_BrowseResult *result) {
    UA_UInt32 continuationIndex = 0;
    size_t referencesCount = 0;
    UA_Int32 referencesIndex = 0;
    
    /* set the browsedescription if a cp is given */
    if(cp) {
        descr = &cp->browseDescription;
        maxrefs = cp->maxReferences;
        continuationIndex = cp->continuationIndex;
    }

    /* is the browsedirection valid? */
    if(descr->browseDirection != UA_BROWSEDIRECTION_BOTH &&
       descr->browseDirection != UA_BROWSEDIRECTION_FORWARD &&
       descr->browseDirection != UA_BROWSEDIRECTION_INVERSE) {
        result->statusCode = UA_STATUSCODE_BADBROWSEDIRECTIONINVALID;
        return;
    }
    
    /* get the references that match the browsedescription */
    size_t relevant_refs_size = 0;
    UA_NodeId *relevant_refs = NULL;
    UA_Boolean all_refs = UA_NodeId_isNull(&descr->referenceTypeId);
    if(!all_refs) {
        if(descr->includeSubtypes) {
            result->statusCode = findSubTypes(server->nodestore, &descr->referenceTypeId,
                                              &relevant_refs, &relevant_refs_size);
            if(result->statusCode != UA_STATUSCODE_GOOD)
                return;
        } else {
            const UA_Node *rootRef = UA_NodeStore_get(server->nodestore, &descr->referenceTypeId);
            if(!rootRef) {
                result->statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
                return;
            }
            if(rootRef->nodeClass != UA_NODECLASS_REFERENCETYPE) {
                UA_NodeStore_release(rootRef);
                result->statusCode = UA_STATUSCODE_BADREFERENCETYPEIDINVALID;
                return;
            }
            UA_NodeStore_release(rootRef);
            relevant_refs = (UA_NodeId*)(uintptr_t)&descr->referenceTypeId;
            relevant_refs_size = 1;
        }
    }

    /* get the node */
    const UA_Node *node = UA_NodeStore_get(server->nodestore, &descr->nodeId);
    if(!node) {
        result->statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
        if(!all_refs && descr->includeSubtypes)
            UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
        return;
    }

    /* if the node has no references, just return */
    if(node->referencesSize <= 0) {
        result->referencesSize = 0;
        UA_NodeStore_release(node);
        if(!all_refs && descr->includeSubtypes)
            UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
        return;
    }

    /* how many references can we return at most? */
    UA_UInt32 real_maxrefs = maxrefs;
    if(real_maxrefs == 0)
        real_maxrefs = node->referencesSize;
    if(node->referencesSize <= 0)
        real_maxrefs = 0;
    else if(real_maxrefs > (UA_UInt32)node->referencesSize)
        real_maxrefs = node->referencesSize;
    result->references = UA_malloc(sizeof(UA_ReferenceDescription) * real_maxrefs);
    if(!result->references) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        goto cleanup;
    }

    /* loop over the node's references */
    size_t skipped = 0;
    UA_Boolean isExternal = UA_FALSE;
    for(; referencesIndex < node->referencesSize && referencesCount < real_maxrefs; referencesIndex++) {
    	isExternal = UA_FALSE;
    	const UA_Node *current =
            returnRelevantNode(server, descr, all_refs, &node->references[referencesIndex],
                               relevant_refs, relevant_refs_size, &isExternal);
        if(!current)
            continue;
        if(skipped < continuationIndex) {
#ifdef UA_EXTERNAL_NAMESPACES
        	if(isExternal == UA_TRUE)
        		/* relevant_node returns a node malloced with UA_ObjectNode_new
                   if it is external (there is no UA_Node_new function) */
        		UA_ObjectNode_delete((UA_ObjectNode*)(uintptr_t)current);
        	else
        		UA_NodeStore_release(current);
#else
        	UA_NodeStore_release(current);
#endif
            skipped++;
            continue;
        }
        UA_StatusCode retval =
            fillReferenceDescription(server->nodestore, current, &node->references[referencesIndex],
                                     descr->resultMask, &result->references[referencesCount]);
#ifdef UA_EXTERNAL_NAMESPACES
        if(isExternal == UA_TRUE)
        	UA_ObjectNode_delete((UA_ObjectNode*)(uintptr_t)current);
        else
        	UA_NodeStore_release(current);
#else
        UA_NodeStore_release(current);
#endif
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Array_delete(result->references, &UA_TYPES[UA_TYPES_REFERENCEDESCRIPTION], referencesCount);
            result->references = NULL;
            result->referencesSize = 0;
            result->statusCode = UA_STATUSCODE_UNCERTAINNOTALLNODESAVAILABLE;
            goto cleanup;
        }
        referencesCount++;
    }

    result->referencesSize = referencesCount;
    if(referencesCount == 0) {
        UA_free(result->references);
        result->references = NULL;
    }

    cleanup:
    UA_NodeStore_release(node);
    if(!all_refs && descr->includeSubtypes)
        UA_Array_delete(relevant_refs, &UA_TYPES[UA_TYPES_NODEID], relevant_refs_size);
    if(result->statusCode != UA_STATUSCODE_GOOD)
        return;

    /* create, update, delete continuation points */
    if(cp) {
        if(referencesIndex == node->referencesSize) {
            /* all done, remove a finished continuationPoint */
            removeCp(cp, session);
        } else {
            /* update the cp and return the cp identifier */
            cp->continuationIndex += referencesCount;
            UA_ByteString_copy(&cp->identifier, &result->continuationPoint);
        }
    } else if(maxrefs != 0 && referencesCount >= maxrefs) {
        /* create a cp */
        if(session->availableContinuationPoints <= 0 ||
           !(cp = UA_malloc(sizeof(struct ContinuationPointEntry)))) {
            result->statusCode = UA_STATUSCODE_BADNOCONTINUATIONPOINTS;
            return;
        }
        UA_BrowseDescription_copy(descr, &cp->browseDescription);
        cp->maxReferences = maxrefs;
        cp->continuationIndex = referencesCount;
        UA_Guid *ident = UA_Guid_new();
        UA_UInt32 seed = (uintptr_t)cp;
        *ident = UA_Guid_random(&seed);
        cp->identifier.data = (UA_Byte*)ident;
        cp->identifier.length = sizeof(UA_Guid);
        UA_ByteString_copy(&cp->identifier, &result->continuationPoint);

        /* store the cp */
        LIST_INSERT_HEAD(&session->continuationPoints, cp, pointers);
        session->availableContinuationPoints--;
    }
}

void Service_Browse(UA_Server *server, UA_Session *session, const UA_BrowseRequest *request,
                    UA_BrowseResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing BrowseRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);
    if(!UA_NodeId_isNull(&request->view.viewId)) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADVIEWIDUNKNOWN;
        return;
    }
    
    if(request->nodesToBrowseSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    size_t size = request->nodesToBrowseSize;
    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    response->resultsSize = size;
    
#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0; j < server->externalNamespacesSize; j++) {
        size_t indexSize = 0;
        for(size_t i = 0; i < size; i++) {
            if(request->nodesToBrowse[i].nodeId.namespaceIndex != server->externalNamespaces[j].index)
                continue;
            isExternal[i] = UA_TRUE;
            indices[indexSize] = i;
            indexSize++;
        }
        if(indexSize == 0)
            continue;
        UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
        ens->browseNodes(ens->ensHandle, &request->requestHeader, request->nodesToBrowse, indices, indexSize,
                         request->requestedMaxReferencesPerNode, response->results, response->diagnosticInfos);
    }
#endif

    for(size_t i = 0; i < size; i++) {
#ifdef UA_EXTERNAL_NAMESPACES
        if(!isExternal[i])
#endif
            Service_Browse_single(server, session, NULL, &request->nodesToBrowse[i],
                                  request->requestedMaxReferencesPerNode, &response->results[i]);
    }
}

void Service_BrowseNext(UA_Server *server, UA_Session *session, const UA_BrowseNextRequest *request,
                        UA_BrowseNextResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing BrowseNextRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);
   if(request->continuationPointsSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
   }
   size_t size = request->continuationPointsSize;
   if(!request->releaseContinuationPoints) {
       /* continue with the cp */
       response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
       if(!response->results) {
           response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
           return;
       }
       response->resultsSize = size;
       for(size_t i = 0; i < size; i++) {
           struct ContinuationPointEntry *cp, *temp;
           LIST_FOREACH_SAFE(cp, &session->continuationPoints, pointers, temp) {
               if(UA_ByteString_equal(&cp->identifier, &request->continuationPoints[i])) {
                   Service_Browse_single(server, session, cp, NULL, 0, &response->results[i]);
                   break;
               }
           }
           if(!cp)
               response->results[i].statusCode = UA_STATUSCODE_BADCONTINUATIONPOINTINVALID;
       }
   } else {
       /* remove the cp */
       response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSERESULT], size);
       if(!response->results) {
           response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
           return;
       }
       response->resultsSize = size;
       for(size_t i = 0; i < size; i++) {
           struct ContinuationPointEntry *cp, *temp;
           LIST_FOREACH_SAFE(cp, &session->continuationPoints, pointers, temp) {
               if(!UA_ByteString_equal(&cp->identifier, &request->continuationPoints[i])) {
                   removeCp(cp, session);
                   break;
               }
           }
           if(!cp)
               response->results[i].statusCode = UA_STATUSCODE_BADCONTINUATIONPOINTINVALID;
       }
   }
}

/***********************/
/* TranslateBrowsePath */
/***********************/

static UA_StatusCode
walkBrowsePath(UA_Server *server, UA_Session *session, const UA_Node *node, const UA_RelativePath *path,
               UA_Int32 pathindex, UA_BrowsePathTarget **targets, UA_Int32 *targets_size,
               UA_Int32 *target_count) {
    const UA_RelativePathElement *elem = &path->elements[pathindex];
    if(elem->targetName.name.length == -1)
        return UA_STATUSCODE_BADBROWSENAMEINVALID;

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_NodeId *reftypes = NULL;
    size_t reftypes_count = 1; // all_refs or no subtypes => 1
    UA_Boolean all_refs = UA_FALSE;
    if(UA_NodeId_isNull(&elem->referenceTypeId))
        all_refs = UA_TRUE;
    else if(!elem->includeSubtypes)
        reftypes = (UA_NodeId*)(uintptr_t)&elem->referenceTypeId; // ptr magic due to const cast
    else {
        retval = findSubTypes(server->nodestore, &elem->referenceTypeId, &reftypes, &reftypes_count);
        if(retval != UA_STATUSCODE_GOOD)
            return retval;
    }

    for(UA_Int32 i = 0; i < node->referencesSize && retval == UA_STATUSCODE_GOOD; i++) {
        UA_Boolean match = all_refs;
        for(size_t j = 0; j < reftypes_count && !match; j++) {
            if(node->references[i].isInverse == elem->isInverse &&
               UA_NodeId_equal(&node->references[i].referenceTypeId, &reftypes[j]))
                match = UA_TRUE;
        }
        if(!match)
            continue;

        // get the node, todo: expandednodeid
        const UA_Node *next = UA_NodeStore_get(server->nodestore, &node->references[i].targetId.nodeId);
        if(!next)
            continue;

        // test the browsename
        if(elem->targetName.namespaceIndex != next->browseName.namespaceIndex ||
           !UA_String_equal(&elem->targetName.name, &next->browseName.name)) {
            UA_NodeStore_release(next);
            continue;
        }

        if(pathindex + 1 < path->elementsSize) {
            // recursion if the path is longer
            retval = walkBrowsePath(server, session, next, path, pathindex + 1,
                                    targets, targets_size, target_count);
            UA_NodeStore_release(next);
        } else {
            // add the browsetarget
            if(*target_count >= *targets_size) {
                UA_BrowsePathTarget *newtargets;
                newtargets = UA_realloc(targets, sizeof(UA_BrowsePathTarget) * (*targets_size) * 2);
                if(!newtargets) {
                    retval = UA_STATUSCODE_BADOUTOFMEMORY;
                    UA_NodeStore_release(next);
                    break;
                }
                *targets = newtargets;
                *targets_size *= 2;
            }

            UA_BrowsePathTarget *res = *targets;
            UA_ExpandedNodeId_init(&res[*target_count].targetId);
            retval = UA_NodeId_copy(&next->nodeId, &res[*target_count].targetId.nodeId);
            UA_NodeStore_release(next);
            if(retval != UA_STATUSCODE_GOOD)
                break;
            res[*target_count].remainingPathIndex = UA_UINT32_MAX;
            *target_count += 1;
        }
    }

    if(!all_refs && elem->includeSubtypes)
        UA_Array_delete(reftypes, &UA_TYPES[UA_TYPES_NODEID], (UA_Int32)reftypes_count);
    return retval;
}

void Service_TranslateBrowsePathsToNodeIds_single(UA_Server *server, UA_Session *session,
                                                  const UA_BrowsePath *path, UA_BrowsePathResult *result) {
    if(path->relativePath.elementsSize <= 0) {
        result->statusCode = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }
        
    UA_Int32 arraySize = 10;
    result->targets = UA_malloc(sizeof(UA_BrowsePathTarget) * arraySize);
    if(!result->targets) {
        result->statusCode = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }
    result->targetsSize = 0;
    const UA_Node *firstNode = UA_NodeStore_get(server->nodestore, &path->startingNode);
    if(!firstNode) {
        result->statusCode = UA_STATUSCODE_BADNODEIDUNKNOWN;
        UA_free(result->targets);
        result->targets = NULL;
        return;
    }
    result->statusCode = walkBrowsePath(server, session, firstNode, &path->relativePath, 0,
                                        &result->targets, &arraySize, &result->targetsSize);
    UA_NodeStore_release(firstNode);
    if(result->targetsSize == 0 && result->statusCode == UA_STATUSCODE_GOOD)
        result->statusCode = UA_STATUSCODE_BADNOMATCH;
    if(result->statusCode != UA_STATUSCODE_GOOD) {
        UA_Array_delete(result->targets, &UA_TYPES[UA_TYPES_BROWSEPATHTARGET], result->targetsSize);
        result->targets = NULL;
        result->targetsSize = -1;
    }
}

void Service_TranslateBrowsePathsToNodeIds(UA_Server *server, UA_Session *session,
                                           const UA_TranslateBrowsePathsToNodeIdsRequest *request,
                                           UA_TranslateBrowsePathsToNodeIdsResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing TranslateBrowsePathsToNodeIdsRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);
	if(request->browsePathsSize <= 0) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
        return;
    }

    size_t size = request->browsePathsSize;
    response->results = UA_Array_new(&UA_TYPES[UA_TYPES_BROWSEPATHRESULT], size);
    if(!response->results) {
        response->responseHeader.serviceResult = UA_STATUSCODE_BADOUTOFMEMORY;
        return;
    }

#ifdef UA_EXTERNAL_NAMESPACES
#ifdef NO_ALLOCA
    UA_Boolean isExternal[size];
    UA_UInt32 indices[size];
#else
    UA_Boolean *isExternal = UA_alloca(sizeof(UA_Boolean) * size);
    UA_UInt32 *indices = UA_alloca(sizeof(UA_UInt32) * size);
#endif /*NO_ALLOCA */
    memset(isExternal, UA_FALSE, sizeof(UA_Boolean) * size);
    for(size_t j = 0; j < server->externalNamespacesSize; j++) {
    	size_t indexSize = 0;
    	for(size_t i = 0;i < size;i++) {
    		if(request->browsePaths[i].startingNode.namespaceIndex != server->externalNamespaces[j].index)
    			continue;
    		isExternal[i] = UA_TRUE;
    		indices[indexSize] = i;
    		indexSize++;
    	}
    	if(indexSize == 0)
    		continue;
    	UA_ExternalNodeStore *ens = &server->externalNamespaces[j].externalNodeStore;
    	ens->translateBrowsePathsToNodeIds(ens->ensHandle, &request->requestHeader, request->browsePaths,
    			indices, indexSize, response->results, response->diagnosticInfos);
    }
#endif

    response->resultsSize = size;
    for(size_t i = 0; i < size; i++) {
#ifdef UA_EXTERNAL_NAMESPACES
    	if(!isExternal[i])
#endif
    		Service_TranslateBrowsePathsToNodeIds_single(server, session, &request->browsePaths[i],
                                                         &response->results[i]);
    }
}

void Service_RegisterNodes(UA_Server *server, UA_Session *session, const UA_RegisterNodesRequest *request,
                           UA_RegisterNodesResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing RegisterNodesRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);

	//TODO: hang the nodeids to the session if really needed
	response->responseHeader.timestamp = UA_DateTime_now();
    if(request->nodesToRegisterSize <= 0)
        response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
    else {
        response->responseHeader.serviceResult =
            UA_Array_copy(request->nodesToRegister, (void**)&response->registeredNodeIds,
                          &UA_TYPES[UA_TYPES_NODEID], request->nodesToRegisterSize);
        if(response->responseHeader.serviceResult == UA_STATUSCODE_GOOD)
            response->registeredNodeIdsSize = request->nodesToRegisterSize;
    }
}

void Service_UnregisterNodes(UA_Server *server, UA_Session *session, const UA_UnregisterNodesRequest *request,
                             UA_UnregisterNodesResponse *response) {
    UA_LOG_DEBUG(server->logger, UA_LOGCATEGORY_SESSION,
                 "Processing UnRegisterNodesRequest for Session (ns=%i,i=%i)",
                 session->sessionId.namespaceIndex, session->sessionId.identifier.numeric);

	//TODO: remove the nodeids from the session if really needed
	response->responseHeader.timestamp = UA_DateTime_now();
	if(request->nodesToUnregisterSize==0)
		response->responseHeader.serviceResult = UA_STATUSCODE_BADNOTHINGTODO;
}

/*********************************** amalgamated original file "C:/open62541/src/client/ua_client.c" ***********************************/


const UA_EXPORT UA_ClientConfig UA_ClientConfig_standard =
    { .timeout = 500 /* ms receive timout */, .secureChannelLifeTime = 30000,
      .timeToRenewSecureChannel = 2000,
      {.protocolVersion = 0, .sendBufferSize = 65536, .recvBufferSize  = 65536,
       .maxMessageSize = 65536, .maxChunkCount = 1}};

/*********************/
/* Create and Delete */
/*********************/

static void UA_Client_init(UA_Client* client, UA_ClientConfig config,
                           UA_Logger logger) {
    client->state = UA_CLIENTSTATE_READY;
    UA_Connection_init(&client->connection);
    UA_SecureChannel_init(&client->channel);
    client->channel.connection = &client->connection;
    UA_String_init(&client->endpointUrl);
    client->requestId = 0;

    UA_NodeId_init(&client->authenticationToken);
    client->requestHandle = 0;

    client->logger = logger;
    client->config = config;
    client->scExpiresAt = 0;

#ifdef ENABLE_SUBSCRIPTIONS
    client->monitoredItemHandles = 0;
    LIST_INIT(&client->pendingNotificationsAcks);
    LIST_INIT(&client->subscriptions);
#endif
}

UA_Client * UA_Client_new(UA_ClientConfig config, UA_Logger logger) {
    UA_Client *client = UA_calloc(1, sizeof(UA_Client));
    if(!client)
        return NULL;

    UA_Client_init(client, config, logger);
    return client;
}

static void UA_Client_deleteMembers(UA_Client* client){
    UA_Client_disconnect(client);
    UA_Connection_deleteMembers(&client->connection);
    UA_SecureChannel_deleteMembersCleanup(&client->channel);
    if(client->endpointUrl.data)
        UA_String_deleteMembers(&client->endpointUrl);
    UA_UserTokenPolicy_deleteMembers(&client->token);
}

void UA_Client_reset(UA_Client* client){
    UA_Client_deleteMembers(client);
    UA_Client_init(client, client->config, client->logger);
}

void UA_Client_delete(UA_Client* client){
    if(client->state != UA_CLIENTSTATE_READY)
        UA_Client_deleteMembers(client);
    UA_free(client);
}

/*************************/
/* Manage the Connection */
/*************************/

static UA_StatusCode HelAckHandshake(UA_Client *c) {
    UA_TcpMessageHeader messageHeader;
    messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_HELF;

    UA_TcpHelloMessage hello;
    UA_String_copy(&c->endpointUrl, &hello.endpointUrl); /* must be less than 4096 bytes */

    UA_Connection *conn = &c->connection;
    hello.maxChunkCount = conn->localConf.maxChunkCount;
    hello.maxMessageSize = conn->localConf.maxMessageSize;
    hello.protocolVersion = conn->localConf.protocolVersion;
    hello.receiveBufferSize = conn->localConf.recvBufferSize;
    hello.sendBufferSize = conn->localConf.sendBufferSize;

    UA_ByteString message;
    UA_StatusCode retval;
    retval = c->connection.getSendBuffer(&c->connection, c->connection.remoteConf.recvBufferSize, &message);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    size_t offset = 8;
    retval |= UA_TcpHelloMessage_encodeBinary(&hello, &message, &offset);
    messageHeader.messageSize = offset;
    offset = 0;
    retval |= UA_TcpMessageHeader_encodeBinary(&messageHeader, &message, &offset);
    UA_TcpHelloMessage_deleteMembers(&hello);
    if(retval != UA_STATUSCODE_GOOD) {
        c->connection.releaseSendBuffer(&c->connection, &message);
        return retval;
    }

    message.length = messageHeader.messageSize;
    retval = c->connection.send(&c->connection, &message);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_DEBUG(c->logger, UA_LOGCATEGORY_NETWORK, "Sending HEL failed");
        return retval;
    }
    UA_LOG_DEBUG(c->logger, UA_LOGCATEGORY_NETWORK, "Sent HEL message");

    UA_ByteString reply;
    UA_ByteString_init(&reply);
    do {
        retval = c->connection.recv(&c->connection, &reply, c->config.timeout);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_DEBUG(c->logger, UA_LOGCATEGORY_NETWORK, "Receiving ACK message failed");
            return retval;
        }
    } while(!reply.data);

    offset = 0;
    UA_TcpMessageHeader_decodeBinary(&reply, &offset, &messageHeader);
    UA_TcpAcknowledgeMessage ackMessage;
    retval = UA_TcpAcknowledgeMessage_decodeBinary(&reply, &offset, &ackMessage);
    UA_ByteString_deleteMembers(&reply);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_DEBUG(c->logger, UA_LOGCATEGORY_NETWORK, "Decoding ACK message failed");
        return retval;
    }

    UA_LOG_DEBUG(c->logger, UA_LOGCATEGORY_NETWORK, "Received ACK message");
    conn->remoteConf.maxChunkCount = ackMessage.maxChunkCount;
    conn->remoteConf.maxMessageSize = ackMessage.maxMessageSize;
    conn->remoteConf.protocolVersion = ackMessage.protocolVersion;
    conn->remoteConf.recvBufferSize = ackMessage.receiveBufferSize;
    conn->remoteConf.sendBufferSize = ackMessage.sendBufferSize;
    conn->state = UA_CONNECTION_ESTABLISHED;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode SecureChannelHandshake(UA_Client *client, UA_Boolean renew) {
    /* Check if sc is still valid */
    if(renew && client->scExpiresAt - UA_DateTime_now() > client->config.timeToRenewSecureChannel * 10000)
        return UA_STATUSCODE_GOOD;

    UA_SecureConversationMessageHeader messageHeader;
    messageHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_OPNF;
    messageHeader.secureChannelId = 0;

    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = ++client->channel.sequenceNumber;
    seqHeader.requestId = ++client->requestId;

    UA_AsymmetricAlgorithmSecurityHeader asymHeader;
    UA_AsymmetricAlgorithmSecurityHeader_init(&asymHeader);
    asymHeader.securityPolicyUri = UA_STRING_ALLOC("http://opcfoundation.org/UA/SecurityPolicy#None");

    /* id of opensecurechannelrequest */
    UA_NodeId requestType = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELREQUEST + UA_ENCODINGOFFSET_BINARY);

    UA_OpenSecureChannelRequest opnSecRq;
    UA_OpenSecureChannelRequest_init(&opnSecRq);
    opnSecRq.requestHeader.timestamp = UA_DateTime_now();
    opnSecRq.requestHeader.authenticationToken = client->authenticationToken;
    opnSecRq.requestedLifetime = client->config.secureChannelLifeTime;
    if(renew) {
        opnSecRq.requestType = UA_SECURITYTOKENREQUESTTYPE_RENEW;
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL, "Requesting to renew the SecureChannel");
    } else {
        opnSecRq.requestType = UA_SECURITYTOKENREQUESTTYPE_ISSUE;
        UA_ByteString_init(&client->channel.clientNonce);
        UA_ByteString_copy(&client->channel.clientNonce, &opnSecRq.clientNonce);
        opnSecRq.securityMode = UA_MESSAGESECURITYMODE_NONE;
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL, "Requesting to open a SecureChannel");
    }

    UA_ByteString message;
    UA_Connection *c = &client->connection;
    UA_StatusCode retval = c->getSendBuffer(c, c->remoteConf.recvBufferSize, &message);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_OpenSecureChannelRequest_deleteMembers(&opnSecRq);
        return retval;
    }

    size_t offset = 12;
    retval = UA_AsymmetricAlgorithmSecurityHeader_encodeBinary(&asymHeader, &message, &offset);
    retval |= UA_SequenceHeader_encodeBinary(&seqHeader, &message, &offset);
    retval |= UA_NodeId_encodeBinary(&requestType, &message, &offset);
    retval |= UA_OpenSecureChannelRequest_encodeBinary(&opnSecRq, &message, &offset);
    messageHeader.messageHeader.messageSize = offset;
    offset = 0;
    retval |= UA_SecureConversationMessageHeader_encodeBinary(&messageHeader, &message, &offset);

    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    UA_OpenSecureChannelRequest_deleteMembers(&opnSecRq);
    if(retval != UA_STATUSCODE_GOOD) {
        client->connection.releaseSendBuffer(&client->connection, &message);
        return retval;
    }

    message.length = messageHeader.messageHeader.messageSize;
    retval = client->connection.send(&client->connection, &message);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    UA_ByteString reply;
    UA_ByteString_init(&reply);
    do {
        retval = client->connection.recv(&client->connection, &reply, client->config.timeout);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL,
                         "Receiving OpenSecureChannelResponse failed");
            return retval;
        }
    } while(!reply.data);

    offset = 0;
    UA_SecureConversationMessageHeader_decodeBinary(&reply, &offset, &messageHeader);
    UA_AsymmetricAlgorithmSecurityHeader_decodeBinary(&reply, &offset, &asymHeader);
    UA_SequenceHeader_decodeBinary(&reply, &offset, &seqHeader);
    UA_NodeId_decodeBinary(&reply, &offset, &requestType);
    UA_NodeId expectedRequest = UA_NODEID_NUMERIC(0, UA_NS0ID_OPENSECURECHANNELRESPONSE +
                                                  UA_ENCODINGOFFSET_BINARY);
    if(!UA_NodeId_equal(&requestType, &expectedRequest)) {
        UA_ByteString_deleteMembers(&reply);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_NodeId_deleteMembers(&requestType);
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_CLIENT,
                     "Reply answers the wrong request. Expected OpenSecureChannelResponse.");
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    UA_OpenSecureChannelResponse response;
    UA_OpenSecureChannelResponse_init(&response);
    retval = UA_OpenSecureChannelResponse_decodeBinary(&reply, &offset, &response);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL,
                     "Decoding OpenSecureChannelResponse failed");
        UA_ByteString_deleteMembers(&reply);
        UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
        UA_OpenSecureChannelResponse_init(&response);
        response.responseHeader.serviceResult = retval;
        return retval;
    }
    client->scExpiresAt = UA_DateTime_now() + response.securityToken.revisedLifetime * 10000;
    UA_ByteString_deleteMembers(&reply);
    retval = response.responseHeader.serviceResult;

    if(retval != UA_STATUSCODE_GOOD)
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL,
                     "SecureChannel could not be opened / renewed");
    else if(!renew) {
        UA_ChannelSecurityToken_copy(&response.securityToken, &client->channel.securityToken);
        /* if the handshake is repeated, replace the old nonce */
        UA_ByteString_deleteMembers(&client->channel.serverNonce);
        UA_ByteString_copy(&response.serverNonce, &client->channel.serverNonce);
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL, "SecureChannel opened");
    } else
        UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_SECURECHANNEL, "SecureChannel renewed");

    UA_OpenSecureChannelResponse_deleteMembers(&response);
    UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(&asymHeader);
    return retval;
}

static UA_StatusCode ActivateSession(UA_Client *client) {
    UA_ActivateSessionRequest request;
    UA_ActivateSessionRequest_init(&request);

    request.requestHeader.requestHandle = 2; //TODO: is it a magic number?
    request.requestHeader.authenticationToken = client->authenticationToken;
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;

    UA_AnonymousIdentityToken identityToken;
    UA_AnonymousIdentityToken_init(&identityToken);
    UA_String_copy(&client->token.policyId, &identityToken.policyId);

    //manual ExtensionObject encoding of the identityToken
    request.userIdentityToken.encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;
    request.userIdentityToken.typeId = UA_TYPES[UA_TYPES_ANONYMOUSIDENTITYTOKEN].typeId;
    request.userIdentityToken.typeId.identifier.numeric+=UA_ENCODINGOFFSET_BINARY;
    
    if (identityToken.policyId.length >= 0)
        UA_ByteString_newMembers(&request.userIdentityToken.body, identityToken.policyId.length+4);
    else {
        identityToken.policyId.length = -1;
        UA_ByteString_newMembers(&request.userIdentityToken.body, 4);
    }
    
    size_t offset = 0;
    UA_ByteString_encodeBinary(&identityToken.policyId,&request.userIdentityToken.body,&offset);

    UA_ActivateSessionResponse response;
    __UA_Client_Service(client, &request, &UA_TYPES[UA_TYPES_ACTIVATESESSIONREQUEST],
                        &response, &UA_TYPES[UA_TYPES_ACTIVATESESSIONRESPONSE]);

    UA_AnonymousIdentityToken_deleteMembers(&identityToken);
    UA_ActivateSessionRequest_deleteMembers(&request);
    UA_ActivateSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode EndpointsHandshake(UA_Client *client) {
    UA_GetEndpointsRequest request;
    UA_GetEndpointsRequest_init(&request);
    request.requestHeader.authenticationToken = client->authenticationToken;
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.endpointUrl = client->endpointUrl;
    UA_String profileUri = UA_STRING("http://opcfoundation.org/UA-Profile/Transport/uatcp-uasc-uabinary");
    request.profileUris = &profileUri;
    request.profileUrisSize = 1;

    UA_GetEndpointsResponse response;
    UA_GetEndpointsResponse_init(&response);
    __UA_Client_Service(client, &request, &UA_TYPES[UA_TYPES_GETENDPOINTSREQUEST],
                        &response, &UA_TYPES[UA_TYPES_GETENDPOINTSRESPONSE]);

    UA_Boolean endpointFound = UA_FALSE;
    UA_Boolean tokenFound = UA_FALSE;
    UA_String securityNone = UA_STRING("http://opcfoundation.org/UA/SecurityPolicy#None");
    for(UA_Int32 i = 0; i < response.endpointsSize; i++) {
        UA_EndpointDescription* endpoint = &response.endpoints[i];
        /* look out for an endpoint without security */
        if(!UA_String_equal(&endpoint->securityPolicyUri, &securityNone))
            continue;
        endpointFound = UA_TRUE;
        /* endpoint with no security found */
        /* look for a user token policy with an anonymous token */
        for(UA_Int32 j=0; j<endpoint->userIdentityTokensSize; ++j) {
            UA_UserTokenPolicy* userToken = &endpoint->userIdentityTokens[j];
            if(userToken->tokenType != UA_USERTOKENTYPE_ANONYMOUS)
                continue;
            tokenFound = UA_TRUE;
            UA_UserTokenPolicy_copy(userToken, &client->token);
            break;
        }
    }

    UA_GetEndpointsResponse_deleteMembers(&response);
    if(!endpointFound) {
        UA_LOG_ERROR(client->logger, UA_LOGCATEGORY_CLIENT, "No suitable endpoint found");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    if(!tokenFound) {
        UA_LOG_ERROR(client->logger, UA_LOGCATEGORY_CLIENT, "No anonymous token found");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    return response.responseHeader.serviceResult;
}

static UA_StatusCode SessionHandshake(UA_Client *client) {
    UA_CreateSessionRequest request;
    UA_CreateSessionRequest_init(&request);

    // todo: is this needed for all requests?
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    UA_ByteString_copy(&client->channel.clientNonce, &request.clientNonce);
    request.requestedSessionTimeout = 1200000;
    request.maxResponseMessageSize = UA_INT32_MAX;

    UA_CreateSessionResponse response;
    UA_CreateSessionResponse_init(&response);
    __UA_Client_Service(client, &request, &UA_TYPES[UA_TYPES_CREATESESSIONREQUEST],
                        &response, &UA_TYPES[UA_TYPES_CREATESESSIONRESPONSE]);

    UA_NodeId_copy(&response.authenticationToken, &client->authenticationToken);

    UA_CreateSessionRequest_deleteMembers(&request);
    UA_CreateSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode CloseSession(UA_Client *client) {
    UA_CloseSessionRequest request;
    UA_CloseSessionRequest_init(&request);

    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.deleteSubscriptions = UA_TRUE;
    UA_NodeId_copy(&client->authenticationToken, &request.requestHeader.authenticationToken);
    UA_CloseSessionResponse response;
    __UA_Client_Service(client, &request, &UA_TYPES[UA_TYPES_CLOSESESSIONREQUEST],
                        &response, &UA_TYPES[UA_TYPES_CLOSESESSIONRESPONSE]);

    UA_CloseSessionRequest_deleteMembers(&request);
    UA_CloseSessionResponse_deleteMembers(&response);
    return response.responseHeader.serviceResult; // not deleted
}

static UA_StatusCode CloseSecureChannel(UA_Client *client) {
    UA_SecureChannel *channel = &client->channel;
    UA_CloseSecureChannelRequest request;
    UA_CloseSecureChannelRequest_init(&request);
    request.requestHeader.requestHandle = 1; //TODO: magic number?
    request.requestHeader.timestamp = UA_DateTime_now();
    request.requestHeader.timeoutHint = 10000;
    request.requestHeader.authenticationToken = client->authenticationToken;

    UA_SecureConversationMessageHeader msgHeader;
    msgHeader.messageHeader.messageTypeAndFinal = UA_MESSAGETYPEANDFINAL_CLOF;
    msgHeader.secureChannelId = client->channel.securityToken.channelId;

    UA_SymmetricAlgorithmSecurityHeader symHeader;
    symHeader.tokenId = channel->securityToken.tokenId;
    
    UA_SequenceHeader seqHeader;
    seqHeader.sequenceNumber = ++channel->sequenceNumber;
    seqHeader.requestId = ++client->requestId;

    UA_NodeId typeId = UA_NODEID_NUMERIC(0, UA_NS0ID_CLOSESECURECHANNELREQUEST + UA_ENCODINGOFFSET_BINARY);

    UA_ByteString message;
    UA_Connection *c = &client->connection;
    UA_StatusCode retval = c->getSendBuffer(c, c->remoteConf.recvBufferSize, &message);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;

    size_t offset = 12;
    retval |= UA_SymmetricAlgorithmSecurityHeader_encodeBinary(&symHeader, &message, &offset);
    retval |= UA_SequenceHeader_encodeBinary(&seqHeader, &message, &offset);
    retval |= UA_NodeId_encodeBinary(&typeId, &message, &offset);
    retval |= UA_encodeBinary(&request, &UA_TYPES[UA_TYPES_CLOSESECURECHANNELREQUEST], &message, &offset);

    msgHeader.messageHeader.messageSize = offset;
    offset = 0;
    retval |= UA_SecureConversationMessageHeader_encodeBinary(&msgHeader, &message, &offset);

    if(retval != UA_STATUSCODE_GOOD) {
        client->connection.releaseSendBuffer(&client->connection, &message);
        return retval;
    }
        
    message.length = msgHeader.messageHeader.messageSize;
    retval = client->connection.send(&client->connection, &message);
    return retval;
}

UA_StatusCode UA_Client_connect(UA_Client *client, UA_ConnectClientConnection connectFunc,
                                char *endpointUrl) {
    if(client->state == UA_CLIENTSTATE_CONNECTED)
        return UA_STATUSCODE_GOOD;
    if(client->state == UA_CLIENTSTATE_ERRORED) {
        UA_Client_reset(client);
    }

    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    client->connection = connectFunc(UA_ConnectionConfig_standard, endpointUrl, client->logger);
    if(client->connection.state != UA_CONNECTION_OPENING) {
        retval = UA_STATUSCODE_BADCONNECTIONCLOSED;
        goto cleanup;
    }

    client->endpointUrl = UA_STRING_ALLOC(endpointUrl);
    if(client->endpointUrl.length < 0) {
        retval = UA_STATUSCODE_BADOUTOFMEMORY;
        goto cleanup;
    }

    client->connection.localConf = client->config.localConnectionConfig;
    retval = HelAckHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = SecureChannelHandshake(client, UA_FALSE);
    if(retval == UA_STATUSCODE_GOOD)
        retval = EndpointsHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = SessionHandshake(client);
    if(retval == UA_STATUSCODE_GOOD)
        retval = ActivateSession(client);
    if(retval == UA_STATUSCODE_GOOD) {
        client->connection.state = UA_CONNECTION_ESTABLISHED;
        client->state = UA_CLIENTSTATE_CONNECTED;
    } else {
        goto cleanup;
    }
    return retval;

    cleanup:
    client->state = UA_CLIENTSTATE_ERRORED;
    UA_Client_reset(client);
    return retval;
}

UA_StatusCode UA_Client_disconnect(UA_Client *client) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    if(client->channel.connection->state == UA_CONNECTION_ESTABLISHED){
        retval = CloseSession(client);
        if(retval == UA_STATUSCODE_GOOD)
            retval = CloseSecureChannel(client);
    }
    return retval;
}

UA_StatusCode UA_Client_manuallyRenewSecureChannel(UA_Client *client) {
    return SecureChannelHandshake(client, UA_TRUE);
}

/****************/
/* Raw Services */
/****************/

void __UA_Client_Service(UA_Client *client, const void *r, const UA_DataType *requestType,
                         void *response, const UA_DataType *responseType) {
    /* Requests always begin witih a RequestHeader, therefore we can cast. */
    UA_RequestHeader *request = (void*)(uintptr_t)r;
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_init(response, responseType);
    UA_ResponseHeader *respHeader = (UA_ResponseHeader*)response;
    
    /* make sure we have a valid session */
    retval = UA_Client_manuallyRenewSecureChannel(client);
    if(retval != UA_STATUSCODE_GOOD) {
        respHeader->serviceResult = retval;
        client->state = UA_CLIENTSTATE_ERRORED;
        return;
    }

    /* handling request parameters */
    UA_NodeId_copy(&client->authenticationToken, &request->authenticationToken);
    request->timestamp = UA_DateTime_now();
    request->requestHandle = ++client->requestHandle;

    /* Send the request */
    UA_UInt32 requestId = ++client->requestId;
    UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_CLIENT,
                 "Sending a request of type %i", requestType->typeId.identifier.numeric);
    retval = UA_SecureChannel_sendBinaryMessage(&client->channel, requestId, request, requestType);
    if(retval) {
        if(retval == UA_STATUSCODE_BADENCODINGLIMITSEXCEEDED)
            respHeader->serviceResult = UA_STATUSCODE_BADREQUESTTOOLARGE;
        else
            respHeader->serviceResult = retval;
        client->state = UA_CLIENTSTATE_ERRORED;
        return;
    }

    /* Retrieve the response */
    // Todo: push this into the generic securechannel implementation for client and server
    UA_ByteString reply;
    UA_ByteString_init(&reply);
    do {
        retval = client->connection.recv(&client->connection, &reply, client->config.timeout);
        if(retval != UA_STATUSCODE_GOOD) {
            respHeader->serviceResult = retval;
            client->state = UA_CLIENTSTATE_ERRORED;
            return;
        }
    } while(!reply.data);

    size_t offset = 0;
    UA_SecureConversationMessageHeader msgHeader;
    retval |= UA_SecureConversationMessageHeader_decodeBinary(&reply, &offset, &msgHeader);
    UA_SymmetricAlgorithmSecurityHeader symHeader;
    retval |= UA_SymmetricAlgorithmSecurityHeader_decodeBinary(&reply, &offset, &symHeader);
    UA_SequenceHeader seqHeader;
    retval |= UA_SequenceHeader_decodeBinary(&reply, &offset, &seqHeader);
    UA_NodeId responseId;
    retval |= UA_NodeId_decodeBinary(&reply, &offset, &responseId);
    UA_NodeId expectedNodeId = UA_NODEID_NUMERIC(0, responseType->typeId.identifier.numeric +
                                                 UA_ENCODINGOFFSET_BINARY);

    if(retval != UA_STATUSCODE_GOOD) {
        goto finish;
    }

    /* Todo: we need to demux responses since a publish responses may come at any time */
    if(!UA_NodeId_equal(&responseId, &expectedNodeId) || seqHeader.requestId != requestId) {
        if(responseId.identifier.numeric != UA_NS0ID_SERVICEFAULT + UA_ENCODINGOFFSET_BINARY) {
            UA_LOG_ERROR(client->logger, UA_LOGCATEGORY_CLIENT,
                         "Reply answers the wrong request. Expected ns=%i,i=%i. But retrieved ns=%i,i=%i",
                         expectedNodeId.namespaceIndex, expectedNodeId.identifier.numeric,
                         responseId.namespaceIndex, responseId.identifier.numeric);
            respHeader->serviceResult = UA_STATUSCODE_BADINTERNALERROR;
        } else
            retval = UA_decodeBinary(&reply, &offset, respHeader, &UA_TYPES[UA_TYPES_SERVICEFAULT]);
        goto finish;
    } 
    
    retval = UA_decodeBinary(&reply, &offset, response, responseType);
    if(retval == UA_STATUSCODE_BADENCODINGLIMITSEXCEEDED)
        retval = UA_STATUSCODE_BADRESPONSETOOLARGE;

 finish:
    UA_SymmetricAlgorithmSecurityHeader_deleteMembers(&symHeader);
    UA_ByteString_deleteMembers(&reply);
    if(retval != UA_STATUSCODE_GOOD){
        UA_LOG_INFO(client->logger, UA_LOGCATEGORY_CLIENT, "Error receiving the response");
        client->state = UA_CLIENTSTATE_ERRORED;
        respHeader->serviceResult = retval;
    }
    UA_LOG_DEBUG(client->logger, UA_LOGCATEGORY_CLIENT,
                 "Received a response of type %i", responseId.identifier.numeric);
}

/*********************************** amalgamated original file "C:/open62541/src/client/ua_client_highlevel.c" ***********************************/


UA_StatusCode UA_Client_NamespaceGetIndex(UA_Client *client, UA_String *namespaceUri, UA_UInt16 *namespaceIndex){
	UA_ReadRequest ReadRequest;
	UA_ReadResponse ReadResponse;
	UA_StatusCode retval = UA_STATUSCODE_BADUNEXPECTEDERROR;

	UA_ReadRequest_init(&ReadRequest);
    UA_ReadValueId id;
	id.attributeId = UA_ATTRIBUTEID_VALUE;
	id.nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_NAMESPACEARRAY);
	ReadRequest.nodesToRead = &id;
	ReadRequest.nodesToReadSize = 1;

	ReadResponse = UA_Client_Service_read(client, ReadRequest);

    if(ReadResponse.responseHeader.serviceResult != UA_STATUSCODE_GOOD){
        retval = ReadResponse.responseHeader.serviceResult;
        goto cleanup;
    }

    if(ReadResponse.resultsSize != 1 || !ReadResponse.results[0].hasValue){
        retval = UA_STATUSCODE_BADNODEATTRIBUTESINVALID;
        goto cleanup;
    }

    if(ReadResponse.results[0].value.type != &UA_TYPES[UA_TYPES_STRING]){
        retval = UA_STATUSCODE_BADTYPEMISMATCH;
        goto cleanup;
    }

    retval = UA_STATUSCODE_BADNOTFOUND;
    for(UA_UInt16 iterator = 0; iterator < ReadResponse.results[0].value.arrayLength; iterator++){
        if(UA_String_equal(namespaceUri, &((UA_String*)ReadResponse.results[0].value.data)[iterator] )){
            *namespaceIndex = iterator;
            retval = UA_STATUSCODE_GOOD;
            break;
        }
    }

cleanup:
    UA_ReadResponse_deleteMembers(&ReadResponse);

	return retval;
}


/*******************/
/* Node Management */
/*******************/

UA_StatusCode UA_EXPORT
UA_Client_addReference(UA_Client *client, const UA_NodeId sourceNodeId, const UA_NodeId referenceTypeId,
                       UA_Boolean isForward, const UA_String targetServerUri,
                       const UA_ExpandedNodeId targetNodeId, UA_NodeClass targetNodeClass) {
    UA_AddReferencesItem item;
    UA_AddReferencesItem_init(&item);
    item.sourceNodeId = sourceNodeId;
    item.referenceTypeId = referenceTypeId;
    item.isForward = isForward;
    item.targetServerUri = targetServerUri;
    item.targetNodeId = targetNodeId;
    item.targetNodeClass = targetNodeClass;
    UA_AddReferencesRequest request;
    UA_AddReferencesRequest_init(&request);
    request.referencesToAdd = &item;
    request.referencesToAddSize = 1;
    UA_AddReferencesResponse response = UA_Client_Service_addReferences(client, request);
    UA_StatusCode retval = response.responseHeader.serviceResult;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_AddReferencesResponse_deleteMembers(&response);
        return retval;
    }
    if(response.resultsSize != 1) {
        UA_AddReferencesResponse_deleteMembers(&response);
        return UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
    retval = response.results[0];
    UA_AddReferencesResponse_deleteMembers(&response);
    return retval;
}

UA_StatusCode UA_EXPORT
UA_Client_deleteReference(UA_Client *client, const UA_NodeId sourceNodeId, const UA_NodeId referenceTypeId,
                          UA_Boolean isForward, const UA_ExpandedNodeId targetNodeId,
                          UA_Boolean deleteBidirectional) {
    UA_DeleteReferencesItem item;
    UA_DeleteReferencesItem_init(&item);
    item.sourceNodeId = sourceNodeId;
    item.referenceTypeId = referenceTypeId;
    item.isForward = isForward;
    item.targetNodeId = targetNodeId;
    item.deleteBidirectional = deleteBidirectional;
    UA_DeleteReferencesRequest request;
    UA_DeleteReferencesRequest_init(&request);
    request.referencesToDelete = &item;
    request.referencesToDeleteSize = 1;
    UA_DeleteReferencesResponse response = UA_Client_Service_deleteReferences(client, request);
    UA_StatusCode retval = response.responseHeader.serviceResult;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_DeleteReferencesResponse_deleteMembers(&response);
        return retval;
    }
    if(response.resultsSize != 1) {
        UA_DeleteReferencesResponse_deleteMembers(&response);
        return UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
    retval = response.results[0];
    UA_DeleteReferencesResponse_deleteMembers(&response);
    return retval;
}

UA_StatusCode UA_Client_deleteNode(UA_Client *client, const UA_NodeId nodeId,
                                   UA_Boolean deleteTargetReferences) {
    UA_DeleteNodesItem item;
    UA_DeleteNodesItem_init(&item);
    item.nodeId = nodeId;
    item.deleteTargetReferences = deleteTargetReferences;
    UA_DeleteNodesRequest request;
    UA_DeleteNodesRequest_init(&request);
    request.nodesToDelete = &item;
    request.nodesToDeleteSize = 1;
    UA_DeleteNodesResponse response = UA_Client_Service_deleteNodes(client, request);
    UA_StatusCode retval = response.responseHeader.serviceResult;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_DeleteNodesResponse_deleteMembers(&response);
        return retval;
    }
    if(response.resultsSize != 1) {
        UA_DeleteNodesResponse_deleteMembers(&response);
        return UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
    retval = response.results[0];
    UA_DeleteNodesResponse_deleteMembers(&response);
    return retval;
}

UA_StatusCode __UA_Client_addNode(UA_Client *client, const UA_NodeClass nodeClass,
                                  const UA_NodeId requestedNewNodeId, const UA_NodeId parentNodeId,
                                  const UA_NodeId referenceTypeId, const UA_QualifiedName browseName,
                                  const UA_NodeId typeDefinition, const UA_NodeAttributes *attr,
                                  const UA_DataType *attributeType, UA_NodeId *outNewNodeId) {
    UA_StatusCode retval = UA_STATUSCODE_GOOD;
    UA_AddNodesRequest request;
    UA_AddNodesRequest_init(&request);
    UA_AddNodesItem item;
    UA_AddNodesItem_init(&item);
    item.parentNodeId.nodeId = parentNodeId;
    item.referenceTypeId = referenceTypeId;
    item.requestedNewNodeId.nodeId = requestedNewNodeId;
    item.browseName = browseName;
    item.nodeClass = nodeClass;
    item.typeDefinition.nodeId = typeDefinition;
    size_t attributes_length = UA_calcSizeBinary(attr, attributeType);
    item.nodeAttributes.typeId = attributeType->typeId;
    item.nodeAttributes.encoding = UA_EXTENSIONOBJECT_ENCODINGMASK_BODYISBYTESTRING;
    retval = UA_ByteString_newMembers(&item.nodeAttributes.body, attributes_length);
    if(retval != UA_STATUSCODE_GOOD)
        return retval;
    size_t offset = 0;
    retval = UA_encodeBinary(attr, attributeType, &item.nodeAttributes.body, &offset);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_ByteString_deleteMembers(&item.nodeAttributes.body);
        return retval;
    }
    request.nodesToAdd = &item;
    request.nodesToAddSize = 1;
    UA_AddNodesResponse response = UA_Client_Service_addNodes(client, request);
    UA_ByteString_deleteMembers(&item.nodeAttributes.body);
    if(response.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        retval = response.responseHeader.serviceResult;
        UA_AddNodesResponse_deleteMembers(&response);
        return retval;
    }
    if(response.resultsSize != 1) {
        UA_AddNodesResponse_deleteMembers(&response);
        return UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
    if(outNewNodeId && response.results[0].statusCode) {
        *outNewNodeId = response.results[0].addedNodeId;
        UA_NodeId_init(&response.results[0].addedNodeId);
    }
    return response.results[0].statusCode;
}

/********/
/* Call */
/********/

UA_StatusCode
UA_Client_call(UA_Client *client, const UA_NodeId objectId, const UA_NodeId methodId,
               UA_Int32 inputSize, const UA_Variant *input, UA_Int32 *outputSize,
               UA_Variant **output) {
    UA_CallRequest request;
    UA_CallRequest_init(&request);
    UA_CallMethodRequest item;
    UA_CallMethodRequest_init(&item);
    item.methodId = methodId;
    item.objectId = objectId;
    item.inputArguments = (void*)(uintptr_t)input; // cast const...
    item.inputArgumentsSize = inputSize;
    request.methodsToCall = &item;
    request.methodsToCallSize = 1;
    UA_CallResponse response = UA_Client_Service_call(client, request);
    UA_StatusCode retval = response.responseHeader.serviceResult;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_CallResponse_deleteMembers(&response);
        return retval;
    }
    if(response.resultsSize != 1) {
        UA_CallResponse_deleteMembers(&response);
        return UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
    retval = response.results[0].statusCode;
    if(retval == UA_STATUSCODE_GOOD) {
        *output = response.results[0].outputArguments;
        *outputSize = response.results[0].outputArgumentsSize;
        response.results[0].outputArguments = NULL;
        response.results[0].outputArgumentsSize = -1;
    }
    UA_CallResponse_deleteMembers(&response);
    return retval;
}

/**************/
/* Attributes */
/**************/

UA_StatusCode 
__UA_Client_readAttribute(UA_Client *client, UA_NodeId nodeId, UA_AttributeId attributeId,
                          void *out, const UA_DataType *outDataType) {
    UA_ReadValueId item;
    UA_ReadValueId_init(&item);
    item.nodeId = nodeId;
    item.attributeId = attributeId;
    UA_ReadRequest request;
    UA_ReadRequest_init(&request);
    request.nodesToRead = &item;
    request.nodesToReadSize = 1;
    UA_ReadResponse response = UA_Client_Service_read(client, request);
    UA_StatusCode retval = response.responseHeader.serviceResult;
    if(retval == UA_STATUSCODE_GOOD && response.resultsSize != 1)
        retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_ReadResponse_deleteMembers(&response);
        return retval;
    }
    UA_DataValue *res = response.results;
    if(res->hasStatus != UA_STATUSCODE_GOOD)
        retval = res->hasStatus;
    else if(!res->hasValue || !UA_Variant_isScalar(&res->value))
        retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
    if(retval != UA_STATUSCODE_GOOD) {
        UA_ReadResponse_deleteMembers(&response);
        return retval;
    }
    if(attributeId == UA_ATTRIBUTEID_VALUE) {
        memcpy(out, &res->value, sizeof(UA_Variant));
        UA_Variant_init(&res->value);
    }
    else if(res->value.type != outDataType) {
        retval = UA_STATUSCODE_BADUNEXPECTEDERROR;
    } else {
        memcpy(out, res->value.data, res->value.type->memSize);
        UA_free(res->value.data);
        res->value.data = NULL;
    }
    UA_ReadResponse_deleteMembers(&response);
    return retval;
}

/*********************************** amalgamated original file "C:/open62541/examples/networklayer_tcp.c" ***********************************/

 /*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */


#include <stdlib.h> // malloc, free
#include <stdio.h> // snprintf
#include <string.h> // memset
#include <errno.h>

#ifdef _WIN32
# include <malloc.h>
# include <winsock2.h>
# include <ws2tcpip.h>
# define CLOSESOCKET(S) closesocket(S)
#else
# include <fcntl.h>
# include <sys/select.h>
# include <netinet/in.h>
#ifndef __CYGWIN__
    # include <netinet/tcp.h>
#endif
# include <sys/ioctl.h>
# include <netdb.h> //gethostbyname for the client
# include <unistd.h> // read, write, close
# include <arpa/inet.h>
#ifdef __QNX__
#include <sys/socket.h>
#endif
# define CLOSESOCKET(S) close(S)
#endif

#ifdef UA_MULTITHREADING
# include <urcu/uatomic.h>
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

/****************************/
/* Generic Socket Functions */
/****************************/

static void
socket_close(UA_Connection *connection) {
    connection->state = UA_CONNECTION_CLOSED;
    shutdown(connection->sockfd,2);
    CLOSESOCKET(connection->sockfd);
}

static UA_StatusCode
socket_write(UA_Connection *connection, UA_ByteString *buf) {
    size_t nWritten = 0;
    while(buf->length > 0 && nWritten < (size_t)buf->length) {
        UA_Int32 n = 0;
        do {
#ifdef _WIN32
            n = send((SOCKET)connection->sockfd, (const char*)buf->data, (size_t)buf->length, 0);
            const int last_error = WSAGetLastError();
            if(n < 0 && last_error != WSAEINTR && last_error != WSAEWOULDBLOCK) {
                connection->close(connection);
                socket_close(connection);
                UA_ByteString_deleteMembers(buf);
                return UA_STATUSCODE_BADCONNECTIONCLOSED;
            }
#else
            n = send(connection->sockfd, (const char*)buf->data, (size_t)buf->length, MSG_NOSIGNAL);
            if(n == -1L && errno != EINTR && errno != EAGAIN) {
                connection->close(connection);
                socket_close(connection);
                UA_ByteString_deleteMembers(buf);
                return UA_STATUSCODE_BADCONNECTIONCLOSED;
            }
#endif
        } while (n == -1L);
        nWritten += n;
    }
    UA_ByteString_deleteMembers(buf);
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
socket_recv(UA_Connection *connection, UA_ByteString *response, UA_UInt32 timeout) {
    response->data = malloc(connection->localConf.recvBufferSize);
    if(!response->data) {
        response->length = -1;
        return UA_STATUSCODE_BADOUTOFMEMORY; /* not enough memory retry */
    }

    if(timeout > 0) {
        /* currently, only the client uses timeouts */
        struct timeval tmptv = {0, timeout * 1000};
        if(0 != setsockopt(connection->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tmptv, sizeof(struct timeval))) {
            UA_ByteString_deleteMembers(response);
            socket_close(connection);
            return UA_STATUSCODE_BADCONNECTIONCLOSED;
        }
    }

    int ret = recv(connection->sockfd, (char*)response->data, connection->localConf.recvBufferSize, 0);
	if(ret == 0) {
        /* server has closed the connection */
        UA_ByteString_deleteMembers(response);
        socket_close(connection);
        return UA_STATUSCODE_BADCONNECTIONCLOSED;
	} else if(ret < 0) {
        UA_ByteString_deleteMembers(response);
#ifdef _WIN32
        const int last_error = WSAGetLastError();
        if(last_error == WSAEINTR || last_error == WSAEWOULDBLOCK)
#else
		if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
#endif
            return UA_STATUSCODE_BADINTERNALERROR; /* retry */
        else {
            socket_close(connection);
            return UA_STATUSCODE_BADCONNECTIONCLOSED;
        }
    }
    response->length = ret;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode socket_set_nonblocking(UA_Int32 sockfd) {
#ifdef _WIN32
    u_long iMode = 1;
    if(ioctlsocket(sockfd, FIONBIO, &iMode) != NO_ERROR)
        return UA_STATUSCODE_BADINTERNALERROR;
#else
    int opts = fcntl(sockfd, F_GETFL);
    if(opts < 0 || fcntl(sockfd, F_SETFL, opts|O_NONBLOCK) < 0)
        return UA_STATUSCODE_BADINTERNALERROR;
#endif
    return UA_STATUSCODE_GOOD;
}

static void FreeConnectionCallback(UA_Server *server, void *ptr) {
    UA_Connection_deleteMembers((UA_Connection*)ptr);
    free(ptr);
 }

/***************************/
/* Server NetworkLayer TCP */
/***************************/

/**
 * For the multithreaded mode, assume a single thread that periodically "gets work" from the network
 * layer. In addition, several worker threads are asynchronously calling into the callbacks of the
 * UA_Connection that holds a single connection.
 *
 * Creating a connection: When "GetWork" encounters a new connection, it creates a UA_Connection
 * with the socket information. This is added to the mappings array that links sockets to
 * UA_Connection structs.
 *
 * Reading data: In "GetWork", we listen on the sockets in the mappings array. If data arrives (or
 * the connection closes), a WorkItem is created that carries the work and a pointer to the
 * connection.
 *
 * Closing a connection: Closing can happen in two ways. Either it is triggered by the server in an
 * asynchronous callback. Or the connection is close by the client and this is detected in
 * "GetWork". The server needs to do some internal cleanups (close attached securechannels, etc.).
 * So even when a closed connection is detected in "GetWork", we trigger the server to close the
 * connection (with a WorkItem) and continue from the callback.
 *
 * - Server calls close-callback: We close the socket, set the connection-state to closed and add
 *   the connection to a linked list from which it is deleted later. The connection cannot be freed
 *   right away since other threads might still be using it.
 *
 * - GetWork: We remove the connection from the mappings array. In the non-multithreaded case, the
 *   connection is freed. For multithreading, we return a workitem that is delayed, i.e. that is
 *   called only after all workitems created before are finished in all threads. This workitems
 *   contains a callback that goes through the linked list of connections to be freed.
 *
 */

#define MAXBACKLOG 100

typedef struct {
    UA_ServerNetworkLayer layer;
    
    /* config */
    UA_UInt32 port;
    UA_ConnectionConfig conf; /* todo: rename to localconf. */

    /* open sockets and connections */
    fd_set fdset;
    UA_Int32 serversockfd;
    UA_Int32 highestfd;
    size_t mappingsSize;
    struct ConnectionMapping {
        UA_Connection *connection;
        UA_Int32 sockfd;
    } *mappings;
} ServerNetworkLayerTCP;

static UA_StatusCode
ServerNetworkLayerGetSendBuffer(UA_Connection *connection, size_t length, UA_ByteString *buf) {
    if(length > connection->remoteConf.recvBufferSize)
        return UA_STATUSCODE_BADCOMMUNICATIONERROR;
    return UA_ByteString_newMembers(buf, length);
}

static void
ServerNetworkLayerReleaseSendBuffer(UA_Connection *connection, UA_ByteString *buf) {
    UA_ByteString_deleteMembers(buf);
}

static void
ServerNetworkLayerReleaseRecvBuffer(UA_Connection *connection, UA_ByteString *buf) {
    UA_ByteString_deleteMembers(buf);
}

/* after every select, we need to reset the sockets we want to listen on */
static void setFDSet(ServerNetworkLayerTCP *layer) {
    FD_ZERO(&layer->fdset);
    FD_SET((UA_UInt32)layer->serversockfd, &layer->fdset);
    layer->highestfd = layer->serversockfd;
    for(size_t i = 0; i < layer->mappingsSize; i++) {
        FD_SET((UA_UInt32)layer->mappings[i].sockfd, &layer->fdset);
        if(layer->mappings[i].sockfd > layer->highestfd)
            layer->highestfd = layer->mappings[i].sockfd;
    }
}

/* callback triggered from the server */
static void ServerNetworkLayerTCP_closeConnection(UA_Connection *connection) {
#ifdef UA_MULTITHREADING
    if(uatomic_xchg(&connection->state, UA_CONNECTION_CLOSED) == UA_CONNECTION_CLOSED)
        return;
#else
    if(connection->state == UA_CONNECTION_CLOSED)
        return;
    connection->state = UA_CONNECTION_CLOSED;
#endif
    ServerNetworkLayerTCP *layer = connection->handle;
    UA_LOG_INFO(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "Closing the Connection %i",
                connection->sockfd);
    /* only "shutdown" here. this triggers the select, where the socket is
       "closed" in the mainloop */
    shutdown(connection->sockfd, 2);
}

/* call only from the single networking thread */
static UA_StatusCode ServerNetworkLayerTCP_add(ServerNetworkLayerTCP *layer, UA_Int32 newsockfd) {
    UA_Connection *c = malloc(sizeof(UA_Connection));
    if(!c)
        return UA_STATUSCODE_BADINTERNALERROR;

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    getsockname(newsockfd, (struct sockaddr*)&addr, &addrlen);
    UA_LOG_INFO(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "New Connection %i over TCP from %s:%d",
                newsockfd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    UA_Connection_init(c);
    c->sockfd = newsockfd;
    c->handle = layer;
    c->localConf = layer->conf;
    c->send = socket_write;
    c->close = ServerNetworkLayerTCP_closeConnection;
    c->getSendBuffer = ServerNetworkLayerGetSendBuffer;
    c->releaseSendBuffer = ServerNetworkLayerReleaseSendBuffer;
    c->releaseRecvBuffer = ServerNetworkLayerReleaseRecvBuffer;
    c->state = UA_CONNECTION_OPENING;
    struct ConnectionMapping *nm;
    nm = realloc(layer->mappings, sizeof(struct ConnectionMapping)*(layer->mappingsSize+1));
    if(!nm) {
        UA_LOG_ERROR(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "No memory for a new Connection");
        free(c);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    layer->mappings = nm;
    layer->mappings[layer->mappingsSize] = (struct ConnectionMapping){c, newsockfd};
    layer->mappingsSize++;
    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode ServerNetworkLayerTCP_start(ServerNetworkLayerTCP *layer, UA_Logger logger) {
    layer->layer.logger = logger;
#ifdef _WIN32
    if((layer->serversockfd = socket(PF_INET, SOCK_STREAM,0)) == (UA_Int32)INVALID_SOCKET) {
        UA_LOG_WARNING(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "Error opening socket, code: %d",
                       WSAGetLastError());
        return UA_STATUSCODE_BADINTERNALERROR;
    }
#else
    if((layer->serversockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        UA_LOG_WARNING(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "Error opening socket");
        return UA_STATUSCODE_BADINTERNALERROR;
    }
#endif
    const struct sockaddr_in serv_addr =
        {.sin_family = AF_INET, .sin_addr.s_addr = INADDR_ANY,
         .sin_port = htons(layer->port), .sin_zero = {0}};
    int optval = 1;
    if(setsockopt(layer->serversockfd, SOL_SOCKET,
                  SO_REUSEADDR, (const char *)&optval,
                  sizeof(optval)) == -1) {
        UA_LOG_WARNING(layer->layer.logger, UA_LOGCATEGORY_NETWORK,
                       "Error during setting of socket options");
        CLOSESOCKET(layer->serversockfd);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    if(bind(layer->serversockfd, (const struct sockaddr *)&serv_addr,
            sizeof(serv_addr)) < 0) {
        UA_LOG_WARNING(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "Error during socket binding");
        CLOSESOCKET(layer->serversockfd);
        return UA_STATUSCODE_BADINTERNALERROR;
    }
    socket_set_nonblocking(layer->serversockfd);
    listen(layer->serversockfd, MAXBACKLOG);
    UA_LOG_INFO(layer->layer.logger, UA_LOGCATEGORY_NETWORK, "TCP network layer listening on %.*s",
                layer->layer.discoveryUrl.length, layer->layer.discoveryUrl.data);
    return UA_STATUSCODE_GOOD;
}

static size_t
ServerNetworkLayerTCP_getJobs(ServerNetworkLayerTCP *layer, UA_Job **jobs, UA_UInt16 timeout) {
    setFDSet(layer);
    struct timeval tmptv = {0, timeout};
    UA_Int32 resultsize;
    resultsize = select(layer->highestfd+1, &layer->fdset, NULL, NULL, &tmptv);
    if(resultsize < 0) {
        *jobs = NULL;
        return 0;
    }

    /* accept new connections (can only be a single one) */
    if(FD_ISSET(layer->serversockfd, &layer->fdset)) {
        resultsize--;
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int newsockfd = accept(layer->serversockfd, (struct sockaddr *) &cli_addr, &cli_len);
        int i = 1;
        setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&i, sizeof(i));
        if(newsockfd >= 0) {
            socket_set_nonblocking(newsockfd);
            ServerNetworkLayerTCP_add(layer, newsockfd);
        }
    }

    /* alloc enough space for a cleanup-connection and free-connection job per resulted socket */
    if(resultsize == 0)
        return 0;
    UA_Job *js = malloc(sizeof(UA_Job) * resultsize * 2);
    if(!js)
        return 0;

    /* read from established sockets */
    size_t j = 0;
    UA_ByteString buf = UA_BYTESTRING_NULL;
    for(size_t i = 0; i < layer->mappingsSize && j < (size_t)resultsize; i++) {
        if(!(FD_ISSET(layer->mappings[i].sockfd, &layer->fdset)))
            continue;
        UA_StatusCode retval = socket_recv(layer->mappings[i].connection, &buf, 0);
        if(retval == UA_STATUSCODE_GOOD) {
            js[j] = UA_Connection_completeMessages(layer->mappings[i].connection, buf);
            j++;
        } else if (retval == UA_STATUSCODE_BADCONNECTIONCLOSED) {
            UA_Connection *c = layer->mappings[i].connection;
            /* the socket was closed from remote */
            js[j].type = UA_JOBTYPE_DETACHCONNECTION;
            js[j].job.closeConnection = layer->mappings[i].connection;
            layer->mappings[i] = layer->mappings[layer->mappingsSize-1];
            layer->mappingsSize--;
            j++;
            js[j].type = UA_JOBTYPE_METHODCALL_DELAYED;
            js[j].job.methodCall.method = FreeConnectionCallback;
            js[j].job.methodCall.data = c;
            j++;
        }
    }

    if(j == 0) {
    	free(js);
    	js = NULL;
    }

    *jobs = js;
    return j;
}

static size_t
ServerNetworkLayerTCP_stop(ServerNetworkLayerTCP *layer, UA_Job **jobs) {
    UA_LOG_INFO(layer->layer.logger, UA_LOGCATEGORY_NETWORK,
                "Shutting down the TCP network layer with %d open connection(s)", layer->mappingsSize);
    shutdown(layer->serversockfd,2);
    CLOSESOCKET(layer->serversockfd);
    UA_Job *items = malloc(sizeof(UA_Job) * layer->mappingsSize * 2);
    if(!items)
        return 0;
    for(size_t i = 0; i < layer->mappingsSize; i++) {
        socket_close(layer->mappings[i].connection);
        items[i*2].type = UA_JOBTYPE_DETACHCONNECTION;
        items[i*2].job.closeConnection = layer->mappings[i].connection;
        items[(i*2)+1].type = UA_JOBTYPE_METHODCALL_DELAYED;
        items[(i*2)+1].job.methodCall.method = FreeConnectionCallback;
        items[(i*2)+1].job.methodCall.data = layer->mappings[i].connection;
    }
#ifdef _WIN32
    WSACleanup();
#endif
    *jobs = items;
    return layer->mappingsSize*2;
}

/* run only when the server is stopped */
static void ServerNetworkLayerTCP_deleteMembers(ServerNetworkLayerTCP *layer) {
    free(layer->mappings);
}

UA_ServerNetworkLayer * ServerNetworkLayerTCP_new(UA_ConnectionConfig conf, UA_UInt32 port) {
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
#endif
    ServerNetworkLayerTCP *layer = malloc(sizeof(ServerNetworkLayerTCP));
    if(!layer)
        return NULL;
    memset(layer, 0, sizeof(ServerNetworkLayerTCP));
    layer->conf = conf;
    layer->mappingsSize = 0;
    layer->mappings = NULL;
    layer->port = port;
    char hostname[256];
    if(gethostname(hostname, 255) == 0) {
        char discoveryUrl[256];
        UA_String str;
#ifndef _MSC_VER
        str.length = snprintf(discoveryUrl, 255, "opc.tcp://%s:%d", hostname, port);
#else
        str.length = _snprintf_s(discoveryUrl, 255, _TRUNCATE, "opc.tcp://%s:%d", hostname, port);
#endif
        str.data = (UA_Byte*)discoveryUrl;
        UA_String_copy(&str, &layer->layer.discoveryUrl);
    }

    layer->layer.start = (UA_StatusCode(*)(UA_ServerNetworkLayer*,UA_Logger))ServerNetworkLayerTCP_start;
    layer->layer.getJobs = (size_t(*)(UA_ServerNetworkLayer*,UA_Job**,UA_UInt16))ServerNetworkLayerTCP_getJobs;
    layer->layer.stop = (size_t(*)(UA_ServerNetworkLayer*, UA_Job**))ServerNetworkLayerTCP_stop;
    layer->layer.deleteMembers = (void(*)(UA_ServerNetworkLayer*))ServerNetworkLayerTCP_deleteMembers;
    return &layer->layer;
}

/***************************/
/* Client NetworkLayer TCP */
/***************************/

static UA_StatusCode
ClientNetworkLayerGetBuffer(UA_Connection *connection, size_t length, UA_ByteString *buf) {
    if(length > connection->remoteConf.recvBufferSize)
        return UA_STATUSCODE_BADCOMMUNICATIONERROR;
    if(connection->state == UA_CONNECTION_CLOSED)
        return UA_STATUSCODE_BADCONNECTIONCLOSED;
    return UA_ByteString_newMembers(buf, connection->remoteConf.recvBufferSize);
}

static void
ClientNetworkLayerReleaseBuffer(UA_Connection *connection, UA_ByteString *buf) {
    UA_ByteString_deleteMembers(buf);
}

static void
ClientNetworkLayerClose(UA_Connection *connection) {
#ifdef UA_MULTITHREADING
    if(uatomic_xchg(&connection->state, UA_CONNECTION_CLOSED) == UA_CONNECTION_CLOSED)
        return;
#else
    if(connection->state == UA_CONNECTION_CLOSED)
        return;
    connection->state = UA_CONNECTION_CLOSED;
#endif
    socket_close(connection);
}

/* we have no networklayer. instead, attach the reusable buffer to the handle */
UA_Connection
ClientNetworkLayerTCP_connect(UA_ConnectionConfig localConf, char *endpointUrl, UA_Logger logger) {
    UA_Connection connection;
    UA_Connection_init(&connection);
    connection.localConf = localConf;

    size_t urlLength = strlen(endpointUrl);
    if(urlLength < 11 || urlLength >= 512) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "Server url size invalid");
        return connection;
    }
    if(strncmp(endpointUrl, "opc.tcp://", 10) != 0) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "Server url does not begin with opc.tcp://");
        return connection;
    }

    UA_UInt16 portpos = 9;
    UA_UInt16 port;
    for(port = 0; portpos < urlLength-1; portpos++) {
        if(endpointUrl[portpos] == ':') {
            char *endPtr = NULL;
            unsigned long int tempulong = strtoul(&endpointUrl[portpos+1], &endPtr, 10);
            if (ERANGE != errno &&
                tempulong < UINT16_MAX &&
                endPtr != &endpointUrl[portpos+1])
            {
                port = (UA_UInt16)tempulong;
            }
            break;
        }
    }
    if(port == 0) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "Port invalid");
        return connection;
    }

    char hostname[512];
    for(int i=10; i < portpos; i++)
        hostname[i-10] = endpointUrl[i];
    hostname[portpos-10] = 0;
#ifdef _WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    WSAStartup(wVersionRequested, &wsaData);
    if((connection.sockfd = socket(PF_INET, SOCK_STREAM,0)) == (UA_Int32)INVALID_SOCKET) {
#else
    if((connection.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
#endif
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "Could not create socket");
        return connection;
    }
    struct hostent *server = gethostbyname(hostname);
    if(!server) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "DNS lookup of %s failed", hostname);
        return connection;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memcpy((char *)&server_addr.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    connection.state = UA_CONNECTION_OPENING;
    if(connect(connection.sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        ClientNetworkLayerClose(&connection);
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "Connection failed");
        return connection;
    }

#ifdef SO_NOSIGPIPE
    int val = 1;
    if(setsockopt(connection.sockfd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&val, sizeof(val)) < 0) {
        UA_LOG_WARNING((*logger), UA_LOGCATEGORY_NETWORK, "Couldn't set SO_NOSIGPIPE");
        return connection;
    }
#endif

    //socket_set_nonblocking(connection.sockfd);
    connection.send = socket_write;
    connection.recv = socket_recv;
    connection.close = ClientNetworkLayerClose;
    connection.getSendBuffer = ClientNetworkLayerGetBuffer;
    connection.releaseSendBuffer = ClientNetworkLayerReleaseBuffer;
    connection.releaseRecvBuffer = ClientNetworkLayerReleaseBuffer;
    return connection;
}

/*********************************** amalgamated original file "C:/open62541/examples/logger_stdout.c" ***********************************/

/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

#include <stdio.h>
#include <stdarg.h>

const char *LogLevelNames[6] = {"trace", "debug", "info", "warning", "error", "fatal"};
const char *LogCategoryNames[6] = {"network", "channel", "session", "server", "client", "userland"};

#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif

void Logger_Stdout(UA_LogLevel level, UA_LogCategory category, const char *msg, ...) {
	UA_String time;
	UA_DateTime_toString(UA_DateTime_now(), &time);
    printf("[%.23s] %s/%s\t", time.data, LogLevelNames[level], LogCategoryNames[category]);
	UA_ByteString_deleteMembers(&time);
    va_list ap;
    va_start(ap, msg);
    vprintf(msg, ap);
    va_end(ap);
    printf("\n");
}

#if ((__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4 || defined(__clang__))
#pragma GCC diagnostic pop
#endif

/*********************************** amalgamated original file "C:/open62541/deps/pcg_basic.c" ***********************************/

/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *       http://www.pcg-random.org
 */

/*
 * This code is derived from the full C implementation, which is in turn
 * derived from the canonical C++ PCG implementation. The C++ version
 * has many additional features and is preferable if you can use C++ in
 * your project.
 */


// state for global RNGs

static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

// pcg32_srandom(initstate, initseq)
// pcg32_srandom_r(rng, initstate, initseq):
//     Seed the rng.  Specified in two parts, state initializer and a
//     sequence selection constant (a.k.a. stream id)

void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg32_random_r(rng);
    rng->state += initstate;
    pcg32_random_r(rng);
}

void pcg32_srandom(uint64_t seed, uint64_t seq)
{
    pcg32_srandom_r(&pcg32_global, seed, seq);
}

// pcg32_random()
// pcg32_random_r(rng)
//     Generate a uniformly distributed 32-bit random number

uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t pcg32_random()
{
    return pcg32_random_r(&pcg32_global);
}


// pcg32_boundedrand(bound):
// pcg32_boundedrand_r(rng, bound):
//     Generate a uniformly distributed number, r, where 0 <= r < bound

uint32_t pcg32_boundedrand_r(pcg32_random_t* rng, uint32_t bound)
{
    // To avoid bias, we need to make the range of the RNG a multiple of
    // bound, which we do by dropping output less than a threshold.
    // A naive scheme to calculate the threshold would be to do
    //
    //     uint32_t threshold = 0x100000000ull % bound;
    //
    // but 64-bit div/mod is slower than 32-bit div/mod (especially on
    // 32-bit platforms).  In essence, we do
    //
    //     uint32_t threshold = (0x100000000ull-bound) % bound;
    //
    // because this version will calculate the same modulus, but the LHS
    // value is less than 2^32.

    uint32_t threshold = -bound % bound;

    // Uniformity guarantees that this loop will terminate.  In practice, it
    // should usually terminate quickly; on average (assuming all bounds are
    // equally likely), 82.25% of the time, we can expect it to require just
    // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
    // (i.e., 2147483649), which invalidates almost 50% of the range.  In 
    // practice, bounds are typically small and only a tiny amount of the range
    // is eliminated.
    for (;;) {
        uint32_t r = pcg32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}


uint32_t pcg32_boundedrand(uint32_t bound)
{
    return pcg32_boundedrand_r(&pcg32_global, bound);
}


/*********************************** amalgamated original file "C:/open62541/src/server/ua_nodestore.c" ***********************************/


/* It could happen that we want to delete a node even though a function higher
   in the call-chain still has a reference. So we count references and delete
   once the count falls to zero. That means we copy every node to a new place
   where it is right behind the refcount integer.

   Since we copy nodes on the heap, we make the alloc for the nodeEntry bigger
   to accommodate for the different nodeclasses (the nodeEntry may have an
   overlength "tail"). */
#define ALIVE_BIT (1 << 15) /* Alive bit in the refcount */
struct nodeEntry {
    UA_UInt16 refcount;
    UA_Node node; // could be const, but then we cannot free it without compilers warnings
};

struct UA_NodeStore {
    struct nodeEntry **entries;
    UA_UInt32          size;
    UA_UInt32          count;
    UA_UInt32          sizePrimeIndex;
};


/* The size of the hash-map is always a prime number. They are chosen to be
   close to the next power of 2. So the size ca. doubles with each prime. */
static hash_t const primes[] = {
    7,         13,         31,         61,         127,         251,
    509,       1021,       2039,       4093,       8191,        16381,
    32749,     65521,      131071,     262139,     524287,      1048573,
    2097143,   4194301,    8388593,    16777213,   33554393,    67108859,
    134217689, 268435399,  536870909,  1073741789, 2147483647,  4294967291
};

static UA_Int16 higher_prime_index(hash_t n) {
    UA_UInt16 low  = 0;
    UA_UInt16 high = sizeof(primes) / sizeof(hash_t);
    while(low != high) {
        UA_UInt16 mid = low + (high - low) / 2;
        if(n > primes[mid])
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

/* Returns UA_TRUE if an entry was found under the nodeid. Otherwise, returns
   false and sets slot to a pointer to the next free slot. */
static UA_Boolean containsNodeId(const UA_NodeStore *ns, const UA_NodeId *nodeid, struct nodeEntry ***entry) {
    hash_t         h     = hash(nodeid);
    UA_UInt32      size  = ns->size;
    hash_t         index = mod(h, size);
    struct nodeEntry **e = &ns->entries[index];

    if(*e == NULL) {
        *entry = e;
        return UA_FALSE;
    }

    if(UA_NodeId_equal(&(*e)->node.nodeId, nodeid)) {
        *entry = e;
        return UA_TRUE;
    }

    hash_t hash2 = mod2(h, size);
    for(;;) {
        index += hash2;
        if(index >= size)
            index -= size;

        e = &ns->entries[index];

        if(*e == NULL) {
            *entry = e;
            return UA_FALSE;
        }

        if(UA_NodeId_equal(&(*e)->node.nodeId, nodeid)) {
            *entry = e;
            return UA_TRUE;
        }
    }

    /* NOTREACHED */
    return UA_TRUE;
}

/* The following function changes size of memory allocated for the entries and
   repeatedly inserts the table elements. The occupancy of the table after the
   call will be about 50%. */
static UA_StatusCode expand(UA_NodeStore *ns) {
    UA_Int32 osize = ns->size;
    UA_Int32 count = ns->count;
    /* Resize only when table after removal of unused elements is either too full or too empty.  */
    if(count * 2 < osize && (count * 8 > osize || osize <= 32))
        return UA_STATUSCODE_GOOD;


    UA_UInt32 nindex = higher_prime_index(count * 2);
    UA_Int32 nsize = primes[nindex];
    struct nodeEntry **nentries;
    if(!(nentries = UA_malloc(sizeof(struct nodeEntry *) * nsize)))
        return UA_STATUSCODE_BADOUTOFMEMORY;

    memset(nentries, 0, nsize * sizeof(struct nodeEntry *));
    struct nodeEntry **oentries = ns->entries;
    ns->entries = nentries;
    ns->size    = nsize;
    ns->sizePrimeIndex = nindex;

    // recompute the position of every entry and insert the pointer
    for(UA_Int32 i=0, j=0;i<osize && j<count;i++) {
        if(!oentries[i])
            continue;
        struct nodeEntry **e;
        containsNodeId(ns, &(*oentries[i]).node.nodeId, &e);  /* We know this returns an empty entry here */
        *e = oentries[i];
        j++;
    }

    UA_free(oentries);
    return UA_STATUSCODE_GOOD;
}

/* Marks the entry dead and deletes if necessary. */
static void deleteEntry(struct nodeEntry *entry) {
    if(entry->refcount > 0)
        return;
    switch(entry->node.nodeClass) {
    case UA_NODECLASS_OBJECT:
        UA_ObjectNode_deleteMembers((UA_ObjectNode*)&entry->node);
        break;
    case UA_NODECLASS_VARIABLE:
        UA_VariableNode_deleteMembers((UA_VariableNode*)&entry->node);
        break;
    case UA_NODECLASS_METHOD:
        UA_MethodNode_deleteMembers((UA_MethodNode *)&entry->node);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        UA_ObjectTypeNode_deleteMembers((UA_ObjectTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        UA_VariableTypeNode_deleteMembers((UA_VariableTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        UA_ReferenceTypeNode_deleteMembers((UA_ReferenceTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_DATATYPE:
        UA_DataTypeNode_deleteMembers((UA_DataTypeNode*)&entry->node);
        break;
    case UA_NODECLASS_VIEW:
        UA_ViewNode_deleteMembers((UA_ViewNode*)&entry->node);
        break;
    default:
        UA_assert(UA_FALSE);
        break;
    }
    UA_free(entry);
}

/** Copies the node into the entry. Then free the original node (but not its content). */
static struct nodeEntry * nodeEntryFromNode(UA_Node *node) {
    size_t nodesize = 0;
    switch(node->nodeClass) {
    case UA_NODECLASS_OBJECT:
        nodesize = sizeof(UA_ObjectNode);
        break;
    case UA_NODECLASS_VARIABLE:
        nodesize = sizeof(UA_VariableNode);
        break;
    case UA_NODECLASS_METHOD:
        nodesize = sizeof(UA_MethodNode);
        break;
    case UA_NODECLASS_OBJECTTYPE:
        nodesize = sizeof(UA_ObjectTypeNode);
        break;
    case UA_NODECLASS_VARIABLETYPE:
        nodesize = sizeof(UA_VariableTypeNode);
        break;
    case UA_NODECLASS_REFERENCETYPE:
        nodesize = sizeof(UA_ReferenceTypeNode);
        break;
    case UA_NODECLASS_DATATYPE:
        nodesize = sizeof(UA_DataTypeNode);
        break;
    case UA_NODECLASS_VIEW:
        nodesize = sizeof(UA_ViewNode);
        break;
    default:
        UA_assert(UA_FALSE);
    }

    struct nodeEntry *newEntry;
    if(!(newEntry = UA_malloc(sizeof(struct nodeEntry) - sizeof(UA_Node) + nodesize)))
        return NULL;

    memcpy(&newEntry->node, node, nodesize);
    UA_free(node);
    return newEntry;
}

/**********************/
/* Exported functions */
/**********************/

UA_NodeStore * UA_NodeStore_new(void) {
    UA_NodeStore *ns;
    if(!(ns = UA_malloc(sizeof(UA_NodeStore))))
        return NULL;

    ns->sizePrimeIndex = higher_prime_index(32);
    ns->size = primes[ns->sizePrimeIndex];
    ns->count = 0;
    if(!(ns->entries = UA_malloc(sizeof(struct nodeEntry *) * ns->size))) {
        UA_free(ns);
        return NULL;
    }
    memset(ns->entries, 0, ns->size * sizeof(struct nodeEntry *));
    return ns;
}

void UA_NodeStore_delete(UA_NodeStore *ns) {
    UA_UInt32 size = ns->size;
    struct nodeEntry **entries = ns->entries;
    for(UA_UInt32 i = 0;i < size;i++) {
        if(entries[i] != NULL) {
            entries[i]->refcount &= ~ALIVE_BIT; // mark dead
            deleteEntry(entries[i]);
            entries[i] = NULL;
            ns->count--;
        }
    }

    UA_free(ns->entries);
    UA_free(ns);
}

UA_StatusCode UA_NodeStore_insert(UA_NodeStore *ns, UA_Node *node, const UA_Node **inserted) {
    if(ns->size * 3 <= ns->count * 4) {
        if(expand(ns) != UA_STATUSCODE_GOOD)
            return UA_STATUSCODE_BADINTERNALERROR;
    }
    // get a free slot
    struct nodeEntry **slot;
    //FIXME: a bit dirty workaround of preserving namespace
    //namespace index is assumed to be valid
    UA_NodeId tempNodeid;
    UA_NodeId_copy(&node->nodeId, &tempNodeid);
    tempNodeid.namespaceIndex = 0;
    if(UA_NodeId_isNull(&tempNodeid)) {
        // find a unique nodeid that is not taken
        node->nodeId.identifierType = UA_NODEIDTYPE_NUMERIC;

        if(node->nodeId.namespaceIndex==0) //original request for ns=0 should yield ns=1
            node->nodeId.namespaceIndex=1;

        UA_Int32 identifier = ns->count+1; // start value
        UA_Int32 size = ns->size;
        hash_t increase = mod2(identifier, size);
        while(UA_TRUE) {
            node->nodeId.identifier.numeric = identifier;
            if(!containsNodeId(ns, &node->nodeId, &slot))
                break;
            identifier += increase;
            if(identifier >= size)
                identifier -= size;
        }
    } else {
        UA_NodeId_deleteMembers(&tempNodeid);
        if(containsNodeId(ns, &node->nodeId, &slot))
            return UA_STATUSCODE_BADNODEIDEXISTS;
    }

    struct nodeEntry *entry = nodeEntryFromNode(node);
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    *slot = entry;
    ns->count++;

    if(inserted) {
        entry->refcount = ALIVE_BIT + 1;
        *inserted = &entry->node;
    } else {
        entry->refcount = ALIVE_BIT;
    }

    return UA_STATUSCODE_GOOD;
}

UA_StatusCode UA_NodeStore_replace(UA_NodeStore *ns, const UA_Node *oldNode, UA_Node *node,
                                   const UA_Node **inserted) {
    struct nodeEntry **slot;
    const UA_NodeId *nodeId = &node->nodeId;
    if(!containsNodeId(ns, nodeId, &slot))
        return UA_STATUSCODE_BADNODEIDUNKNOWN;

    // you try to replace an obsolete node (without threading this can't happen
    // if the user doesn't do it deliberately in his code)
    if(&(*slot)->node != oldNode)
        return UA_STATUSCODE_BADINTERNALERROR;

    struct nodeEntry *entry = nodeEntryFromNode(node);
    if(!entry)
        return UA_STATUSCODE_BADOUTOFMEMORY;

    (*slot)->refcount &= ~ALIVE_BIT; // mark dead
    *slot = entry;

    if(inserted) {
        entry->refcount = ALIVE_BIT + 1;
        *inserted = &entry->node;
    } else {
        entry->refcount = ALIVE_BIT;
    }
    return UA_STATUSCODE_GOOD;
}

const UA_Node * UA_NodeStore_get(const UA_NodeStore *ns, const UA_NodeId *nodeid) {
    struct nodeEntry **slot;
    if(!containsNodeId(ns, nodeid, &slot))
        return NULL;
    (*slot)->refcount++;
    return &(*slot)->node;
}

UA_StatusCode UA_NodeStore_remove(UA_NodeStore *ns, const UA_NodeId *nodeid) {
    struct nodeEntry **slot;
    if(!containsNodeId(ns, nodeid, &slot))
        return UA_STATUSCODE_BADNODEIDUNKNOWN;

    // Check before if deleting the node makes the UA_NodeStore inconsistent.
    (*slot)->refcount &= ~ALIVE_BIT; // mark dead
    deleteEntry(*slot);
    *slot = NULL;
    ns->count--;

    /* Downsize the hashmap if it is very empty */
    if(ns->count * 8 < ns->size && ns->size > 32)
        expand(ns); // this can fail. we just continue with the bigger hashmap.

    return UA_STATUSCODE_GOOD;
}

void UA_NodeStore_iterate(const UA_NodeStore *ns, UA_NodeStore_nodeVisitor visitor) {
    for(UA_UInt32 i = 0;i < ns->size;i++) {
        if(ns->entries[i] != NULL)
            visitor(&ns->entries[i]->node);
    }
}

void UA_NodeStore_release(const UA_Node *managed) {
    /* We know what we are doing here and remove a compiler warning. Nobody has
       a reference to the const pointer, so we can free it. */
    struct nodeEntry *entry = (struct nodeEntry *) ((uintptr_t)managed - offsetof(struct nodeEntry, node));
    entry->refcount--;
    deleteEntry(entry);
}
