// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2023.09
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2023-2024 Seityagiya Terlekchi. All rights reserved.

#ifndef YAYA_LOGGER_TIME_H
#define YAYA_LOGGER_TIME_H

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"

typedef struct logtime_t {
    void* ptr;
} logtime_t;

typedef struct logtime_sett_t {
    intmax_t tab_size;
    char*    format_string;
} logtime_sett_t;

bool logtime_init(logtime_t** logger_time, logtime_sett_t* logger_time_setting);
bool logtime_free(logtime_t** logger_time);

bool logtime_beg(logtime_t* logger_time, char* name);
bool logtime_end(logtime_t* logger_time);
bool logtime_bar(logtime_t* logger_time);
bool logtime_out(logtime_t* logger_time, FILE* out);

#endif /*YAYA_LOGGER_TIME_H*/
