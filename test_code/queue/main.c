#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define MAX_LEN 10

typedef struct work {
    jh_node_t jh_node;
    int val;
} work_t;

typedef struct work_head {
    jh_head_t jh_head;
    int len;
} work_head_t;

int main () {
    int i;
    work_head_t work_head;
    work_t *work = NULL;
    
    work_head.len = 0;
    jh_init_head ( &work_head.jh_head );

    for ( i = 0; i < MAX_LEN; i++ )
    {
	work = ( work_t * ) malloc ( sizeof ( work_t ) );
	if ( !work ) {
	    printf ("malloc err!\n");
	    return 1;
	}
	work->val = i;
	jh_init_node ( &work->jh_node );
	printf ("[alloc node] val:%d\n", i);

	jh_insert_node_head ( &work_head.jh_head, &work->jh_node );
	printf ("[insert node] id:%d\n", i);

	work_head.len++;
    }

    i = 0;
    jh_node_t *first_node = &work_head.jh_head.first;
    JH_FOR_LIST_SAFE(work_t, jh_node, work, first_node) 
    {
    
	printf ("[for loop] id:%d, val:%d", i, work->val);
    }

    return 0;
}
