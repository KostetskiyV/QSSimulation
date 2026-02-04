#ifndef TIME_CONFIG_H__
#define TIME_CONFIG_H__

#define T1_MIN 0.0
#define T1_MAX 5.0

#define T2_MIN 0.0
#define T2_MAX 4.0

#define T3_MIN 0.0
#define T3_MAX 4.0

struct time_config_t
{
    double t1_add_time_min;
    double t1_add_time_max;
    double t1_handle_time_min;
    double t1_handle_time_max;
    double t2_handle_time_min;
    double t2_handle_time_max;
};

#endif
