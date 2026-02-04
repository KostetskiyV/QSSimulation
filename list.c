#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "error_codes.h"

struct list_node_t *append(struct list_node_t *pend, void *value)
{
    if (!pend)
    {
        pend = malloc(sizeof(struct list_node_t));
        if (!pend)
            return NULL;
        pend->value = value;
        pend->next = NULL;
        return pend;
    }

    pend->next = malloc(sizeof(struct list_node_t));
    if (!pend->next)
        return NULL;

    pend->next->value = value;
    pend->next->next = NULL;
    return pend;
}

struct list_node_t *insert(struct list_node_t *head, size_t index, void *value)
{
    struct list_node_t *cur = head, *new = malloc(sizeof(struct list_node_t));
    if (!new)
        return NULL;
    new->value = value;
    
    for (size_t i = 1; i < index; i++)
        cur = cur->next;

    new->next = cur->next;
    cur->next = new;

    return head;
}

struct list_node_t *free_list(struct list_node_t *head)
{
    struct list_node_t *prev = head;
    while (prev)
    {
        head = prev->next;
        free(prev->value);
        free(prev);
        prev = head;
    }
    return head;
}

void print_list(const struct list_node_t *head, printer_t print_val)
{
    const struct list_node_t *cur = head;
    printf("[");
    while (cur)
    {
        print_val(cur->value);
        cur = cur->next;
    }
    printf("]\n");
}
