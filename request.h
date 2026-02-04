#ifndef REQUEST_H__
#define REQUEST_H__

#include <stdbool.h>

bool *create_request(int request_type);
void free_request(bool *request);
double get_rand_request_time(double min, double max);

#endif
