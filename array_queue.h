#ifndef ARRAY_QUEUE_H__
#define ARRAY_QUEUE_H__

#include <stddef.h>
#include "printers.h"

#define ARRAY_LEN 10000

struct array_queue_t
{
    size_t pin;
    size_t pout;
    size_t length;
    void *content[ARRAY_LEN];
};

void aqueue_init(struct array_queue_t *queue);
void aqueue_free_content(struct array_queue_t *queue);
int aqueue_push(struct array_queue_t *queue, void *new_el);
void *aqueue_pop(struct array_queue_t *queue);

int aqueue_push_by_index(struct array_queue_t *queue, size_t index, void *new_el);
int aqueue_push_index4(struct array_queue_t *queue, void *new_el);
void print_aqueue(struct array_queue_t *queue, printer_t print_val);

#endif
