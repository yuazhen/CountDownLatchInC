#ifndef _COUNT_DOWN_LATCH_H_
#define _COUNT_DOWN_LATCH_H_

#include <pthread.h>
#include <semaphore.h>

typedef struct _cdl_context_t
{
	int cdl_count;
	pthread_mutex_t cdl_mutex;
	pthread_cond_t cdl_cond;
	sem_t cdl_sem;
} cdl_context_t;

cdl_context_t* cdl_create();
void cdl_countdown(cdl_context_t* ctx);
int cdl_getcount(cdl_context_t* ctx);
void cdl_wait(cdl_context_t* ctx);
void cdl_init(cdl_context_t* ctx, int cnt);

#endif
