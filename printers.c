#include <stdio.h>
#include "printers.h"

bool adress_print = false;

void set_adress_print(bool new_val)
{
    adress_print = new_val;
}

bool get_adress_print()
{
    return adress_print;
}


void print_adress(void *adress)
{
    printf("%p, ", adress);
}

void print_int(void *num)
{
    printf("%d, ", *((int*) num));
}
