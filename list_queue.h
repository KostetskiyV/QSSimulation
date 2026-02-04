#ifndef LIST_QUEUE_H__
#define LIST_QUEUE_H__

#include <stddef.h>
#include "list.h"

struct list_queue_t
{
    size_t length;
    struct list_node_t *pin;
    struct list_node_t *pout;
};

void lqueue_init(struct list_queue_t *queue);
void lqueue_free(struct list_queue_t *queue);
int lqueue_push(struct list_queue_t *queue, void *new_el);
void *lqueue_pop(struct list_queue_t *queue);

int lqueue_push_index4(struct list_queue_t *queue, void *new_el);
void print_lqueue(struct list_queue_t *queue, printer_t print_val);

#endif
