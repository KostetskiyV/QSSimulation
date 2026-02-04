#include <stdlib.h>
#include "list_queue.h"
#include "printers.h"
#include "error_codes.h"

int lqueue_push_index4(struct list_queue_t *queue, void *new_el)
{
    if (queue->length <= 4)
        return lqueue_push(queue, new_el);
    queue->pout = insert(queue->pout, 4, new_el);
    queue->length++;

    if (get_adress_print())
        print_list(queue->pout, print_adress);
    return (queue->pout) ? OK : ERR_MEM;
}

void lqueue_init(struct list_queue_t *queue)
{
    queue->length = 0;
    queue->pin = NULL;
    queue->pout = NULL;
}

void lqueue_free(struct list_queue_t *queue)
{
    (void) free_list(queue->pout);
    queue->length = 0;
    queue->pin = NULL;
    queue->pout = NULL;
}

int lqueue_push(struct list_queue_t *queue, void *new_el)
{
    queue->pin = append(queue->pin, new_el);
    if (!queue->pin)
        return ERR_MEM;
    if (queue->length++ == 0)
        queue->pout = queue->pin;
    else
        queue->pin = queue->pin->next;

    if (get_adress_print())
        print_list(queue->pout, print_adress);
    return OK;
}

void *lqueue_pop(struct list_queue_t *queue)
{
    if (queue->length == 0)
        return NULL;
    struct list_node_t *extracted = queue->pout;
    void *value = extracted->value;
    queue->pout = extracted->next;
    if (--queue->length == 0)
        queue->pin = NULL;
    free(extracted);

    if (get_adress_print())
        print_list(queue->pout, print_adress);
    return value;
}
