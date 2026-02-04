#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "queueing_system.h"
#include "request.h"
#include "error_codes.h"

#define REQUESTS_OUT 1000
#define PRINT_INF_BORDER 100
#define EPS 1e-7

struct time_config_t time_config = { T1_MIN, T1_MAX, T2_MIN, T2_MAX, T3_MIN, T3_MAX };

int cmp_doubles(double l, double r)
{
    if (fabs(l-r) <= EPS)
        return 0;
    return (l > r) ? 1 : -1;
}

int add_request(int type, void *queue, push_func_t push, push_type2_func_t push_type2)
{
    int rc = OK;
    bool *request = create_request(type);
    if (!request)
        return ERR_MEM;
    if (type == 1)
        rc = push(queue, (void*) request);
    else
        rc = push_type2(queue, (void*) request);
    if (rc != OK)
        free_request(request);
    return rc;
}

void result_init(struct result_t *res)
{
    res->time_modeling = 0;
    res->time_sleep = 0;
    res->requests_in1 = 0;
    res->requests_out1 = 0;
    res->requests_processed2 = 0;
}


int change_time_config()
{
    printf("Введите минимальное и максимальное время добавления заявки первого типа:\n");
    if (scanf("%lf%lf", &(time_config.t1_add_time_min), &(time_config.t1_add_time_max)) != 2)
        return ERR_IO;
    if (time_config.t1_add_time_min < 0 || time_config.t1_add_time_max < time_config.t1_add_time_min)
        return ERR_RANGE;
    
    printf("Введите минимальное и максимальное время обработки заявки первого типа:\n");
    if (scanf("%lf%lf", &(time_config.t1_handle_time_min), &(time_config.t1_handle_time_max)) != 2)
        return ERR_IO;
    if (time_config.t1_handle_time_min < 0 || time_config.t1_handle_time_max < time_config.t1_handle_time_min)
        return ERR_RANGE;

    printf("Введите минимальное и максимальное время обработки заявки второго типа:\n");
    if (scanf("%lf%lf", &(time_config.t2_handle_time_min), &(time_config.t2_handle_time_max)) != 2)
        return ERR_IO;
    if (time_config.t2_handle_time_min < 0 || time_config.t2_handle_time_max < time_config.t2_handle_time_min)
        return ERR_RANGE;
    
    return OK;
}

int count_average(struct result_t *res, void *queue,
    pop_func_t pop, push_func_t push, push_type2_func_t push_type2)
{
    int rc = OK;
    double t1_add_time_avg = (time_config.t1_add_time_max + time_config.t1_add_time_min) / 2;
    double t1_handle_time_avg = (time_config.t1_handle_time_max + time_config.t1_handle_time_min) / 2;
    double t2_handle_time_avg = (time_config.t2_handle_time_max + time_config.t2_handle_time_min) / 2;

    result_init(res);
    double t1 = t1_add_time_avg, t2 = -1, t3 = t2_handle_time_avg;

    while (res->requests_out1 < REQUESTS_OUT)
    {
        if (t2 > 0)
        {
            if (cmp_doubles(t1, t2) == 0)
            {
                res->time_modeling += t1;
                res->requests_in1++;
                res->requests_out1++;
                t1 = t1_add_time_avg;
                t2 = -1;

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
            }
            else if (t2 < t1)
            {
                res->time_modeling += t2;
                res->requests_out1++;
                t1 -= t2;
                t2 = -1;
            }
            else
            {
                res->time_modeling += t1;
                res->requests_in1++;
                t2 -= t1;
                t1 = t1_add_time_avg;

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
            }
        }
        else
        {
            if (cmp_doubles(t1, t3) == 0)
            {
                res->time_modeling += t1;
                res->requests_in1++;
                res->requests_processed2++;
                t1 = t1_add_time_avg;
                t3 = -1;

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                rc = add_request(2, queue, push, push_type2);
                if (rc != OK)
                    return rc;
            }
            else if (t3 < t1)
            {
                res->time_modeling += t3;
                res->requests_processed2++;
                t1 -= t3;
                t3 = -1;

                rc = add_request(2, queue, push, push_type2);
                if (rc != OK)
                    return rc;
            }
            else
            {
                res->time_modeling += t1;
                res->requests_in1++;
                t3 -= t1;
                t1 = t1_add_time_avg;

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
            }
        }

        if (cmp_doubles(t2, -1) == 0 && cmp_doubles(t3, -1) == 0)
        {
            bool *request = pop(queue);

            if (*request)
                t2 = t1_handle_time_avg;
            else
                t3 = t2_handle_time_avg;
            free_request(request);
        }
    }

    return OK;
}

int count_with_rand(struct result_t *res, void *queue,
    pop_func_t pop, push_func_t push, push_type2_func_t push_type2)
{
    int rc = OK;
    size_t queue_length = 0, sum_length = 0, length_n = 0;
    result_init(res);
    double t1 = get_rand_request_time(time_config.t1_add_time_min, time_config.t1_add_time_max),
        t2 = get_rand_request_time(time_config.t1_handle_time_min, time_config.t1_handle_time_max),
        t3 = get_rand_request_time(time_config.t2_handle_time_min, time_config.t2_handle_time_max);

    while (res->requests_out1 < REQUESTS_OUT)
    {
        if (t2 > 0)
        {
            if (cmp_doubles(t1, t2) == 0)
            {
                res->time_modeling += t1;
                res->requests_in1++;
                res->requests_out1++;
                t1 = get_rand_request_time(time_config.t1_add_time_min, time_config.t1_add_time_max);
                t2 = -1;

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                queue_length++;
            }
            else if (t2 < t1)
            {
                res->time_modeling += t2;
                res->requests_out1++;
                t1 -= t2;
                t2 = -1;
            }
            else
            {
                res->time_modeling += t1;
                res->requests_in1++;
                t2 -= t1;
                t1 = get_rand_request_time(time_config.t1_add_time_min, time_config.t1_add_time_max);

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                queue_length++;
            }
        }
        else
        {
            if (cmp_doubles(t1, t3) == 0)
            {
                res->time_modeling += t1;
                res->requests_in1++;
                res->requests_processed2++;
                t1 = get_rand_request_time(time_config.t1_add_time_min, time_config.t1_add_time_max);
                t3 = -1;

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                rc = add_request(2, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                queue_length++;
                queue_length++;
            }
            else if (t3 < t1)
            {
                res->time_modeling += t3;
                res->requests_processed2++;
                t1 -= t3;
                t3 = -1;

                rc = add_request(2, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                queue_length++;
            }
            else
            {
                res->time_modeling += t1;
                res->requests_in1++;
                t3 -= t1;
                t1 = get_rand_request_time(time_config.t1_add_time_min, time_config.t1_add_time_max);

                rc = add_request(1, queue, push, push_type2);
                if (rc != OK)
                    return rc;
                queue_length++;
            }
        }

        if (cmp_doubles(t2, -1) == 0 && cmp_doubles(t3, -1) == 0)
        {
            bool *request = pop(queue);

            if (*request)
                t2 = get_rand_request_time(time_config.t1_handle_time_min, time_config.t1_handle_time_max);
            else
                t3 = get_rand_request_time(time_config.t2_handle_time_min, time_config.t2_handle_time_max);
            free_request(request);
            queue_length--;
        }

        sum_length += queue_length;
        length_n++;

        print_medium_results(res, queue_length, sum_length, length_n);
    }

    return OK;
}

void print_medium_results(struct result_t *res, size_t queue_length, size_t sum_length, size_t length_n)
{
    if (res->requests_out1 % PRINT_INF_BORDER == 0)
    {
        printf("Current length: %lu\n", queue_length);
        printf("Average length: %lf\n", (double) sum_length / length_n);
        printf("Requests in: %lu\n", res->requests_in1 + res->requests_processed2 + 1);
        printf("Requests out: %lu\n", res->requests_out1);
        printf("Average time in queue: %lf\n", res->time_modeling / sum_length * sum_length / length_n);
    }
}

void print_results(struct result_t *res)
{
    printf("Время моделирования: %lf\n", res->time_modeling);
    printf("Время простоя: %lf\n", res->time_sleep);
    printf("Количество вошедших заявок первого типа: %lu\n", res->requests_in1 + 1);
    printf("Количество вышедших заявок первого типа: %lu\n", res->requests_out1);
    printf("Количество обработанных заявок второго типа: %lu\n", res->requests_processed2);
}

void print_results_difference(struct result_t *theory, struct result_t *real)
{
    printf("Разница времени моделирования: %lf%%\n",
        (real->time_modeling - theory->time_modeling) / real->time_modeling);
    
    printf("Разница времени простоя: %lf%%\n", 0.0);
    
    printf("Разница количества вошедших заявок первого типа: %lf%%\n",
        ((double) real->requests_in1 - (double) theory->requests_in1) / real->requests_in1);
    
    printf("Разница количества вышедших заявок первого типа: %lf%%\n",
        ((double) real->requests_out1 - (double) theory->requests_out1) / real->requests_out1);
    
    printf("Разница количества обработанных заявок второго типа: %lf%%\n",
        ((double) real->requests_processed2 - (double) theory->requests_processed2) / real->requests_processed2);
}
