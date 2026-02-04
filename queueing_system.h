#ifndef QUEUEING_SYSTEM_H__
#define QUEUEING_SYSTEM_H__

#include <stddef.h>
#include "time_config.h"

typedef void* (*pop_func_t)(void *queue);
typedef int (*push_func_t)(void *queue, const void *new_el);
typedef int (*push_type2_func_t)(void *queue, const void *new_el);

struct result_t
{
    double time_modeling;
    double time_sleep;
    size_t requests_in1;
    size_t requests_out1;
    size_t requests_processed2;
};

int change_time_config();

int count_average(struct result_t *res, void *queue,
    pop_func_t pop, push_func_t push, push_type2_func_t push_type2);

int count_with_rand(struct result_t *res, void *queue,
    pop_func_t pop, push_func_t push, push_type2_func_t push_type2);

    void print_medium_results(struct result_t *res, size_t queue_length, size_t sum_length, size_t length_n);
void print_results(struct result_t *res);
void print_results_difference(struct result_t *theory, struct result_t *real);

#endif
