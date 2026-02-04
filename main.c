#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "queueing_system.h"
#include "array_queue.h"
#include "list_queue.h"
#include "printers.h"
#include "creative_mode.h"
#include "error_codes.h"

#define CHANGE_PARAMS_MODE 1
#define SOLVE_WITH_ARRAY_MODE 2
#define SOLVE_WITH_LIST_MODE 3
#define COUNT_DIFF_MODE 4
#define SWITCH_ADRESS_PRINT_MODE 5
#define CREATIVE_MODE 6
#define END_MODE 0

unsigned long int calc_elapsed_time(const struct timespec *beg,
                         const struct timespec *end) {
    return ((end->tv_sec - beg->tv_sec) * 1000 * 1000 * 1000 +
        (end->tv_nsec - beg->tv_nsec));
}

void print_menu()
{
    printf("Наберите 1, чтобы сменить параметры времени заявок\n");
    printf("Наберите 2, чтобы расчитать систему очерьдью-массивом\n");
    printf("Наберите 3, чтобы расчитать систему очерьдью-списком\n");
    printf("Наберите 4, чтобы расчитать погрешность\n");
    printf("Наберите 5, чтобы %s показ адресов элементов очереди\n", get_adress_print() ? "отключить" : "включить");
    printf("Наберите 6, чтобы войти в креативный режим\n");
    printf("Наберите 0, чтобы завершить\n");
}


int main(void)
{
    srand(time(NULL));
    int mode, rc = OK;
    struct timespec start, end;

    struct array_queue_t aqueue;
    aqueue_init(&aqueue);
    struct list_queue_t lqueue;
    lqueue_init(&lqueue);
    struct result_t measures = { -1, 0, 0, 0, 0 };
    
    while (rc == OK)
    {
        print_menu();
        if (scanf("%d", &mode) != 1)
            return ERR_IO;
        if (mode == END_MODE)
            break;
        
        switch (mode)
        {
            case CHANGE_PARAMS_MODE:
                rc = change_time_config();
                measures.time_modeling = -1;
                break;
            case SOLVE_WITH_ARRAY_MODE:
                clock_gettime(CLOCK_MONOTONIC_RAW, &start);
                rc = count_with_rand(&measures, &aqueue,
                        (pop_func_t) aqueue_pop, (push_func_t) aqueue_push,
                        (push_type2_func_t) aqueue_push_index4);
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                
                if (rc == OK)
                {
                    print_results(&measures);
                    printf("Memory cost: %lu Bytes\n", sizeof(aqueue) + aqueue.length * sizeof(bool));
                    printf("Time: %lu ns\n\n", calc_elapsed_time(&start, &end));
                }
                break;
            case SOLVE_WITH_LIST_MODE:
                clock_gettime(CLOCK_MONOTONIC_RAW, &start);
                rc = count_with_rand(&measures, &lqueue,
                        (pop_func_t) lqueue_pop, (push_func_t) lqueue_push,
                        (push_type2_func_t) lqueue_push_index4);
                clock_gettime(CLOCK_MONOTONIC_RAW, &end);
                
                if (rc == OK)
                {
                    print_results(&measures);
                    printf("Memory cost: %lu Bytes\n", sizeof(lqueue) + lqueue.length * (sizeof(struct list_node_t) + sizeof(bool)));
                    printf("Time: %lu ns\n\n", calc_elapsed_time(&start, &end));
                }
                break;
            case COUNT_DIFF_MODE:
                if (measures.time_modeling < 0)
                    printf("Для начала произведите замер\n");
                else
                {
                    struct result_t theory;
                    rc = count_average(&theory, &lqueue,
                        (pop_func_t) lqueue_pop, (push_func_t) lqueue_push,
                        (push_type2_func_t) lqueue_push_index4);
                    if (rc == OK)
                        print_results_difference(&theory, &measures);
                }
                break;
            case SWITCH_ADRESS_PRINT_MODE:
                set_adress_print(!get_adress_print());
                break;
            case CREATIVE_MODE:
                return proccess(&aqueue, &lqueue);
            default:
                rc = ERR_MODE;
        }
        aqueue_free_content(&aqueue);
        lqueue_free(&lqueue);
    }

    if (rc == ERR_IO)
        printf("Неверный формат\n");
    else if (rc == ERR_RANGE)
        printf("Введённые параметры некорректны\n");
    else if (rc == ERR_MODE)
        printf("Неверный пункт меню\n");
    else if (rc == ERR_MEM)
        printf("Out of memory!\n");
    else if (rc == ERR_QUEUE_OVERFLOW)
        printf("Очередь переполнена\n");

    return rc;
}