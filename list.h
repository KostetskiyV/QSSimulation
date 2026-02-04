#ifndef LIST_H__
#define LIST_H__

#include <stddef.h>
#include "printers.h"

struct list_node_t
{
    void *value;
    struct list_node_t *next;
};

struct list_node_t *append(struct list_node_t *head, void *value);
struct list_node_t *insert(struct list_node_t *head, size_t index, void *value);
struct list_node_t *free_list(struct list_node_t *head);
void print_list(const struct list_node_t *head, printer_t print_val);

#endif
