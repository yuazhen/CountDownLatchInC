#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "countdownlatch.h"

cdl_context_t* cdl_create()
{
	int err;
	cdl_context_t* ctx = (cdl_context_t*)malloc(sizeof(cdl_context_t));
	memset(ctx, 0, sizeof(cdl_context_t));
	ctx->cdl_count = 0;
	err = pthread_mutex_init(&ctx->cdl_mutex, NULL);
	if(err)
		printf("pthread_mutex_init error %d\n", err);
	err = pthread_cond_init(&ctx->cdl_cond, NULL);
	if(err)
		printf("pthread_cond_init error %d\n", err);
	sem_init(&ctx->cdl_sem, 0, 0);
	return ctx;
}

void cdl_countdown(cdl_context_t* ctx)
{
	pthread_mutex_lock(&ctx->cdl_mutex);
	ctx->cdl_count -= 1;
	if(ctx->cdl_count <= 0)
	{
		sem_post(&ctx->cdl_sem);
	}
	pthread_mutex_unlock(&ctx->cdl_mutex);
}

int cdl_getcount(cdl_context_t* ctx)
{
	return ctx->cdl_count;
}

void cdl_wait(cdl_context_t* ctx)
{
	while((sem_wait(&ctx->cdl_sem) != 0 ) && (errno==EINTR));
}

void cdl_init(cdl_context_t* ctx, int cnt)
{
	pthread_mutex_lock(&ctx->cdl_mutex);
	ctx->cdl_count = cnt;
	pthread_mutex_unlock(&ctx->cdl_mutex);
}

