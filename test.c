#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "countdownlatch.h"

static pthread_mutex_t test_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t test_cond = PTHREAD_COND_INITIALIZER;

static cdl_context_t* g_ctx = NULL;

static unsigned long long currentTimeInMilliseconds()
{
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void* test_thread(void* param)
{
	// usleep(1000*1000);
	int idx = *(int*)param;
	while(1)
	{
		pthread_mutex_lock(&test_mutex);
		pthread_cond_wait(&test_cond, &test_mutex);
		pthread_mutex_unlock(&test_mutex);
		printf("thread %d got signal\n", idx);
		cdl_countdown(g_ctx);
		printf("thread %d done\n", idx);
	}
}

#define THREAD_COUNT 10

int main(int argc, char** argv)
{
	pthread_t th_test[THREAD_COUNT];
	int i;
	int th_index[THREAD_COUNT];

	g_ctx = cdl_create();

	for(i=0; i<THREAD_COUNT; i++)
	{
		th_index[i] = i;
		pthread_create(&th_test[i], NULL, test_thread, &th_index[i]);
	}

	while(1)
	{
		usleep(10*1000);
		cdl_init(g_ctx, THREAD_COUNT);
		pthread_cond_broadcast(&test_cond);
		cdl_wait(g_ctx);
		printf("all done %llu\n", currentTimeInMilliseconds());
	}
	return 0;
}
