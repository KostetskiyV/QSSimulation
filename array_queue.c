#include <stdio.h>
#include <stdlib.h>
#include "array_queue.h"
#include "error_codes.h"

int aqueue_push_index4(struct array_queue_t *queue, void *new_el)
{
    if (queue->length <= 4)
        return aqueue_push(queue, new_el);
    return aqueue_push_by_index(queue, 4, new_el);
}

void step_forward(size_t *pindex)
{
    if (*pindex == ARRAY_LEN - 1)
        *pindex = 0;
    else
        (*pindex)++;
}

size_t step_backside(size_t pindex)
{
    if (pindex == 0)
        return ARRAY_LEN - 1;
    else
        return pindex - 1;
}

void aqueue_init(struct array_queue_t *queue)
{
    queue->pin = 0;
    queue->pout = 0;
    queue->length = 0;
}

void aqueue_free_content(struct array_queue_t *queue)
{
    void *cur = aqueue_pop(queue);
    while (cur)
    {
        free(cur);
        cur = aqueue_pop(queue);
    }
}

int aqueue_push(struct array_queue_t *queue, void *new_el)
{
    if (queue->pin == queue->pout && queue->length > 0)
        return ERR_QUEUE_OVERFLOW;
    queue->content[queue->pin] = new_el;
    step_forward(&(queue->pin));
    queue->length++;

    if (get_adress_print())
        print_aqueue(queue, print_adress);
    return OK;
}

int aqueue_push_by_index(struct array_queue_t *queue, size_t index, void *new_el)
{
    if (queue->pin == queue->pout && queue->length > 0)
        return ERR_QUEUE_OVERFLOW;

    size_t i, n;
    for (i = queue->pin, n = 0; n < queue->length - index; n++, i = step_backside(i))
        queue->content[i] = queue->content[step_backside(i)];
    queue->content[i] = new_el;
    queue->length++;
    step_forward(&queue->pin);

    if (get_adress_print())
        print_aqueue(queue, print_adress);
    return OK;
}

void *aqueue_pop(struct array_queue_t *queue)
{
    if (queue->pin == queue->pout && queue->length == 0)
        return NULL;
    void *extracted = queue->content[queue->pout];
    step_forward(&(queue->pout));
    queue->length--;

    if (get_adress_print())
        print_aqueue(queue, print_adress);
    return extracted;
}

void print_aqueue(struct array_queue_t *queue, printer_t print_val)
{
    printf("[");
    for (size_t i = queue->pout, n = 0; n < queue->length; n++, step_forward(&i))
        print_val(queue->content[i]);
    printf("]\n");
}
