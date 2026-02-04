#ifndef PRINTERS_H__
#define PRINTERS_H__

#include <stdbool.h>

typedef void (*printer_t)(void*);

void set_adress_print(bool new_val);
bool get_adress_print();
void print_adress(void *adress);
void print_int(void *num);

#endif
