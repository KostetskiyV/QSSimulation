#include <stdlib.h>
#include "request.h"

bool *create_request(int request_type)
{
    bool *request = malloc(sizeof(bool));
    if (!request)
        return NULL;
    *request = (request_type == 1);
    return request;
}

void free_request(bool *request)
{
    free(request);
}

double get_rand_request_time(double min, double max)
{
    return ((double) rand() / RAND_MAX * (max - min) + min);
}
