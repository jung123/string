#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>

#define THREAD_NUM 3
#define CHECK_TIME_INTERVAL 1
#define CHECK_TIME_NUM 10
#define NANO_SEC 1000000000

struct timespec thread_time[THREAD_NUM][CHECK_TIME_NUM];

void *thread_main ( void *arg )
{
    long thread_id = ( long )arg;
    int i = 0;
    int ret = 0;
    int test = 0;
    cpu_set_t mask;
    cpu_set_t mask_tmp;

    printf ("[%lu] Start Processing !\n", thread_id );

    /* set CPU affinity */
    CPU_ZERO (&mask);
    CPU_ZERO (&mask_tmp);
    CPU_SET ( (int)thread_id, &mask );
    ret = sched_setaffinity ( 0, sizeof ( mask ), &mask );
    if ( ret ) 
    {
	printf ("[%lu] error on set affinity!\n", thread_id);
	return NULL;
    }
    sched_getaffinity ( 0, sizeof ( cpu_set_t ), &mask_tmp );
    test = CPU_ISSET ( (int)thread_id, &mask_tmp );
    if ( test ) {
	printf ("[%lu] cpu affinity: %lu, cpu count:%d\n", thread_id, thread_id, CPU_COUNT(&mask));
    }

    /* start check time */
    for ( i = 0; i < CHECK_TIME_NUM; i++ )
    {
	sleep ( CHECK_TIME_INTERVAL );
	while ( clock_gettime ( CLOCK_MONOTONIC, &thread_time[thread_id][i] ) ) {}
    }

    /* End Check time */
    printf ("[%lu] End Processing !\n", thread_id);
    return NULL;
}

int main ()
{
    pthread_t threadid[THREAD_NUM] = { 0 };
    int i = 0, j = 0;
    time_t tv_sec, prev_tv_sec;
    long tv_nsec, prev_tv_nsec;
    unsigned long long output_time = 0;

    /* create thread */
    printf ("[main] Create Thread !\n");
    for ( i = 0; i < THREAD_NUM; i++ ) 
    {
	pthread_create ( &threadid[i], NULL, thread_main, ( void * )((long)i) );
    }

    /* join thread */
    printf ("[main] Join Thread !\n");
    for ( i =0; i < THREAD_NUM; i++ )
    {
	pthread_join ( threadid[i], NULL );
    }

    /* printing  */
    prev_tv_sec = prev_tv_nsec = 0;
    for ( i = 0; i < THREAD_NUM; i++ )
    {
	printf ("-------------------------------\n");
	printf ("[main] Checking Thread[%d] Start\n", i);
	for ( j = 0; j < CHECK_TIME_NUM; j++ )
	{
	    tv_sec = thread_time[i][j].tv_sec;
	    tv_nsec = thread_time[i][j].tv_nsec;
	    output_time = tv_sec + ( tv_nsec * NANO_SEC );
	    printf ("[main] Thread(%d) [%d]: %llu\n", i, j, output_time);
	}
	printf ("[main] Checking Thread[%d] End\n", i);

	prev_tv_sec = thread_time[i][0].tv_sec;
	prev_tv_nsec = thread_time[i][0].tv_nsec;
	for ( j = 1; j < CHECK_TIME_NUM; j++ )
	{
	    tv_sec = thread_time[i][j].tv_sec;
	    tv_nsec = thread_time[i][j].tv_nsec;
	    output_time = ( tv_sec + ( tv_nsec * NANO_SEC ) ) - ( prev_tv_sec + ( prev_tv_nsec * NANO_SEC ) );
	    printf ("[main] Thread(%d) differ Time[%d]: %llu\n", i, j, output_time);
	    prev_tv_sec = thread_time[i][j].tv_sec;
	    prev_tv_nsec = thread_time[i][j].tv_nsec;
	}
    }

    return 0;
}
