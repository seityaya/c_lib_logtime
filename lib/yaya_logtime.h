//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.09
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2023-2023 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_LOGGER_TIME_H
#define YAYA_LOGGER_TIME_H

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

typedef struct logger_time_t {
    void *ptr;
} logger_time_t;

typedef struct logger_time_setting_t {
    intmax_t tab_size;
    intmax_t (*get_time_tic)();
    double   (*get_time_sec)(intmax_t);
    char*    format_string;
} logger_time_setting_t;

intmax_t logtime_time_tic();
intmax_t logtime_time_sec(intmax_t tic);

bool logtime_init(logger_time_t** logger_time, logger_time_setting_t* logger_time_setting);
bool logtime_free(logger_time_t** logger_time);

bool logtime_beg(logger_time_t* logger_time, char* name);
void logtime_end(logger_time_t* logger_time);
void logtime_bar(logger_time_t* logger_time);

bool logtime_str(logger_time_t* logger_time, char* out, size_t len);
bool logtime_out(logger_time_t* logger_time, FILE* out);

#endif /*YAYA_LOGGER_TIME_H*/
