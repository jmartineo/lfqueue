#ifndef QUEUE_MOD_H
#define QUEUE_MOD_H

#ifdef WFQUEUE
#include "wfqueue.h"

#elif LCRQ
#include "lcrq.h"

#elif CCQUEUE
#include "ccqueue.h"

#elif MSQUEUE
#include "msqueue_mod.h"

#elif FAAQ
#include "align.h"

typedef struct {
  volatile long P DOUBLE_CACHE_ALIGNED;
  volatile long C DOUBLE_CACHE_ALIGNED;
} queue_t DOUBLE_CACHE_ALIGNED;

typedef int handle_t;

#elif DELAY

typedef int queue_t;
typedef int handle_t;

#elif NCQ
#include "ncq_mod.h"

#elif SCQ
#include "scq_mod.h"

#elif SCQD
#include "scqd_mod.h"

#elif SCQ2
#include "scq2_mod.h"

#elif WCQ
#include "wcq.h"

#else
#error "Please specify a queue implementation."

#endif

void queue_init(queue_t * q, int nprocs);
void queue_register(queue_t * q, handle_t * th, int id);
void enqueue(queue_t * q, handle_t * th, void * v);
void * dequeue(queue_t * q, handle_t * th);
void queue_free(queue_t * q, handle_t * h);

#endif /* end of include guard: QUEUE_H */
