/*
 * < list 구현을 위한 것들 >
 */

#ifndef __JH_QUEUE__
#define __JH_QUEUE__
/* brief    : 
 * details  : 
 * author   : 김중현
 * date	    : 2018-12-24
 * param
 * return
 */

/*	    { head }					     {tail}
 *  { head }.[first] <-> [node1] <-> [node2] <-> [node3] <-> [node4]
 *		^						^
 *		|						|			
 *		-------------------------------------------------
 */

/*
 *  < node >
 *  : 모든 리스트를 사용할 때, 아래의 node를 사용하여 구현한다.
 *  일반적인 리스트에대한 interface를 제공하기 위해서이다.
 */
typedef struct node {
    struct node *next;
    struct node *prev;
} jh_node_t;

/*
 *  < head >
 *  : 모든 리스트를 사용할 때, 아래의 head를 사용하여 구현한다.
 *  일반적인 리스트에대한 interface를 제공하기 위해서이다.
 *  그리고 해당 리스트에대한 lock등의 부차적인 처리는 해당 head를 
 *  포함하는 곳에서 선언한다.
 */
typedef struct head {
    jh_node_t first;
} jh_head_t;

/******************************/
/******* Functional API *******/
/******************************/

/* Init Head */
static inline void jh_init_head ( jh_head_t *head )
{
    head->first.next = head->first.prev = &head->first;
}

/* Init Node */
static inline void jh_init_node ( jh_node_t *node )
{
    node->next = node->prev = NULL;
}

/* Insert Node to Prev pos */
static inline void jh_insert_node_prev ( jh_node_t *node, jh_node_t *new_node )
{
    jh_node_t *prev_node = NULL;

    prev_node = node->prev;

    new_node->next = node;
    new_node->prev = prev_node;

    prev_node->next = new_node;
    node->prev = new_node;
}

/* Insert Node to After pos */
static inline void jh_insert_node_after ( jh_node_t *node, jh_node_t *new_node )
{
    jh_insert_node_prev ( node->next, new_node );
}

/* Insert Node to First pos */
static inline void jh_insert_node_head ( jh_head_t *head, jh_node_t *new_node )
{
    jh_node_t *first = &head->first;

    jh_insert_node_after ( first, new_node );
}

/* Insert Node to Tail pos */
static inline void jh_insert_node_tail ( jh_head_t *head, jh_node_t *new_node )
{
    jh_node_t *first = &head->first;

    jh_insert_node_prev ( first, new_node );
}

/* detach node */
static inline void jh_detach_node ( jh_node_t *detach_node )
{
    jh_node_t *prev_node = detach_node->prev;
    jh_node_t *next_node = detach_node->next;

    prev_node->next = next_node;
    next_node->prev = prev_node;

    jh_init_node ( detach_node );
}

/* Pop Node */
static inline void jh_pop_node ( jh_head_t *head, jh_node_t *node, jh_node_t **return_node )
{
    jh_node_t *first = &head->first;

    /* Head is Init state */
    if ( node == first )
    {
	*return_node = NULL;
	return;
    }

    jh_detach_node ( node );

    *return_node = node;
}

/* Pop tail Node */
static inline void jh_pop_tail ( jh_head_t *head, jh_node_t **return_node )
{
    jh_node_t *first = &head->first;
    jh_node_t *tail_node = first->prev;

    jh_pop_node ( head, tail_node, return_node );
}

/* Pop first Node */
static inline void jh_pop_first ( jh_head_t *head, jh_node_t **return_node )
{
    jh_node_t *first = &head->first;
    jh_node_t *first_node = first->next;

    jh_pop_node ( head, first_node, return_node );
}

/*
 * type {
 *	jh_node_t node;
 * }
 * 
 */

/* offset of  */
#define offsetof(type, member) __builtin_offsetof(type, member)

/* container of */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

/* Get List Head Node */
#define JH_FIRST(head) ({   \
    typeof(head.first.next) __ptr = (typeof(head.first.next))(head.first.next);\
    __ptr;})

/* Get List Tail Node */
#define JH_TAIL(head) ({    \
    typeof(head.first.prev) __ptr = (typeof(head.first.prev))(head.first.prev);	\
    __ptr;})

/* Get List Init Node */
#define JH_INIT_NODE(head) ({	\
	typeof(head.first) *_ptr = (typeof(head.first) *)&head.first;	\
	_ptr;})

/* For loop on List */
/*
 *  From [List Head] -> to [List Tail]
 *  'Safe'는 side effect에 대하여...
 */
#define JH_FOR_LIST_SAFE(type, member, container, head)		\
    for (							\
        typeof(((type *)0)->member) *_jh_node = JH_FIRST(head);	\
        ((_jh_node) && (JH_INIT_NODE(head) != _jh_node) && ( container = container_of(_jh_node, type, member) ));  \
        _jh_node = _jh_node->next				\
    )

/***********************/
#endif
