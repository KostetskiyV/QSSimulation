#include <stdio.h>
#include <stddef.h>
#include "creative_mode.h"
#include "printers.h"
#include "error_codes.h"

int proccess(struct array_queue_t *aqueue, struct list_queue_t *lqueue)
{
    void *back;
    int inp, rc = OK;
    int arr_nums[1000], list_nums[1000];
    size_t arr_index = 0, list_index = 0;
    
    while (rc == OK)
    {
        printf("Наберите 1, чтобы добавить в массив\n");
        printf("Наберите 2, чтобы извлечь из массива\n");
        printf("Наберите 3, чтобы добавить в список\n");
        printf("Наберите 4, чтобы извлечь из списка\n");
        printf("Наберите 0, чтобы завершить\n");
        if (scanf("%d", &inp) != 1)
            return ERR_IO;
        
        switch (inp)
        {
            case 1:
                printf("Введите добавляемое число:\n");
                if (scanf("%d", &(arr_nums[arr_index++])) != 1)
                    return ERR_IO;
                rc = aqueue_push(aqueue, (void*) &(arr_nums[arr_index-1]));
                if (rc == OK)
                    print_aqueue(aqueue, print_int);
                break;
            case 2:
                back = aqueue_pop(aqueue);
                if (!back)
                    rc = ERR_MEM;
                else
                {
                    printf("poped: %d\n", *((int*) back));
                    print_aqueue(aqueue, print_int);
                    arr_index--;
                }
                break;
            case 3:
                printf("Введите добавляемое число:\n");
                if (scanf("%d", &(list_nums[list_index++])) != 1)
                    return ERR_IO;
                rc = lqueue_push(lqueue, (void*) &(list_nums[list_index-1]));
                if (rc == OK)
                    print_list(lqueue->pout, print_int);
                break;
            case 4:
                back = lqueue_pop(lqueue);
                if (!back)
                    rc = ERR_MEM;
                else
                {
                    printf("poped: %d\n", *((int*) back));
                    print_list(lqueue->pout, print_int);
                    list_index--;
                }
                break;
            case 0:
                return OK;
            default:
                rc = ERR_MODE;
        }
    }
    return rc;
}
