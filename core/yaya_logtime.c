// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2023.09
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2023-2024 Seityagiya Terlekchi. All rights reserved.

#include "stdlib.h"

#include "yaya_logtime.h"
#include "yaya_memory.h"
#include "yaya_systime.h"

typedef struct logtime_node_t {
    char*    name;
    intmax_t deep;

    time_system_t time_beg;
    time_system_t time_end;
    time_system_t time_sum;
    time_system_t time_min;
    time_system_t time_max;
    intmax_t      time_cnt;

    bool                    bar_flag;
    bool                    loop_flag;
    size_t                  loop_count;
    size_t                  node_count;
    struct logtime_node_t** node;
    struct logtime_node_t*  prev;
} logtime_node_t;

typedef struct {
    logtime_sett_t* sett;
    logtime_node_t* last;
    logtime_node_t* head;
} private_logger_time_t;

bool logtime_init(logtime_t** logger_time, logtime_sett_t* logger_time_setting) {
    if (logger_time_setting == NULL) {
        return false;
    }
    if (logger_time == NULL) {
        return false;
    }
    if (!memory_req((void**)(logger_time), 1, sizeof(logtime_t))) {
        return false;
    }
    if (!memory_req((void**)(&(*logger_time)->ptr), 1, sizeof(private_logger_time_t))) {
        return false;
    }

    private_logger_time_t* logtime = (*logger_time)->ptr;

    if (!memory_req((void**)(&(logtime->head)), 1, sizeof(logtime_node_t))) {
        return false;
    }

    logtime->last = logtime->head;

    logtime->sett = logger_time_setting;

    return true;
}

static bool recursive_free(logtime_node_t* head) {
    if (head != NULL) {
        for (size_t i = 0; i < head->node_count; i++) {
            logtime_node_t* node = head->node[i];
            recursive_free(node);
            if (!memory_ret((void**)(&node))) {
                return false;
            }
        }
        if (!memory_ret((void**)(&head->node))) {
            return false;
        }
    }
    return true;
}

bool logtime_free(logtime_t** logger_time) {
    private_logger_time_t* logtime = (*logger_time)->ptr;

    if (!recursive_free(logtime->head)) {
        return false;
    }
    if (!memory_ret((void**)(&logtime->head))) {
        return false;
    }
    if (!memory_ret((void**)(&logtime))) {
        return false;
    }
    if (!memory_ret((void**)(logger_time))) {
        return false;
    }
    return true;
}

bool logtime_beg(logtime_t* logger_time, char* name) {
    private_logger_time_t* logtime = logger_time->ptr;

    //    time_system_t time_func_beg;
    time_system_t time_func_end;

    //    time_func_beg.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);
    //    time_func_beg.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_NONE);
    //    time_func_beg.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_NONE);

    time_fragment_t time_nul = time_build(0, 0, 0, 0);

    logtime_node_t* node = NULL;

    if (logtime->last->loop_flag == false) {
        if (!memory_req((void**)(&logtime->last->node), logtime->last->node_count + 1, sizeof(logtime_node_t*))) {
            return false;
        }
        if (!memory_req((void**)(&logtime->last->node[logtime->last->node_count]), 1, sizeof(logtime_node_t))) {
            return false;
        }

        node = logtime->last->node[logtime->last->node_count];

        node->name = name;
        node->deep = logtime->last->deep + 1;
        node->prev = logtime->last;

        time_fragment_t time_min = time_build(YAYA_TIME_LIMIT_SECOND_MAX, 0, 0, 0);
        time_fragment_t time_max = time_build(YAYA_TIME_LIMIT_SECOND_MIN, 0, 0, 0);

        node->time_min = time_system_build(time_min, time_min, time_min);
        node->time_max = time_system_build(time_max, time_max, time_max);
        node->time_sum = time_system_build(time_nul, time_nul, time_nul);
        node->time_cnt = 0;

        logtime->last->node_count++;
    } else {
        node = logtime->last->node[logtime->last->loop_count];
        logtime->last->loop_count++;
    }

    logtime->last = node;

    node->time_end = time_system_build(time_nul, time_nul, time_nul);

    time_func_end.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_NONE);
    time_func_end.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_NONE);
    time_func_end.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);

    node->time_beg = time_func_end;

    //    if(node->prev != NULL) {
    //        node->prev->time_sum = time_system_dif(node->prev->time_sum, time_system_dif(time_func_end, time_func_beg));
    //    }

    return true;
}

bool logtime_end(logtime_t* logger_time) {
    private_logger_time_t* logtime = logger_time->ptr;
    logtime_node_t*        node    = logtime->last;

    time_system_t time_func_beg;
    //    time_system_t time_func_end;

    time_func_beg.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);
    time_func_beg.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_NONE);
    time_func_beg.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_NONE);

    node->time_end = time_func_beg;

    if (node->bar_flag == false) {
        node->time_min = time_system_min(node->time_min, time_system_dif(node->time_end, node->time_beg));
        node->time_max = time_system_max(node->time_max, time_system_dif(node->time_end, node->time_beg));
        node->time_cnt++;
    }
    logtime->last = logtime->last->prev;

    node->time_sum = time_system_sum(node->time_sum, time_system_dif(node->time_end, node->time_beg));

    //    time_func_end.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_NONE);
    //    time_func_end.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_NONE);
    //    time_func_end.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);

    //    if(node->prev != NULL) {
    //        node->prev->time_sum = time_system_dif(node->prev->time_sum, time_system_dif(time_func_end, time_func_beg));
    //    }

    return true;
}

static void recursive_set_flag(logtime_node_t* head) {
    for (size_t i = 0; i < head->node_count; i++) {
        head->node[i]->loop_flag  = true;
        head->node[i]->loop_count = 0;
        recursive_set_flag(head->node[i]);
    }
}

bool logtime_bar(logtime_t* logger_time) {
    private_logger_time_t* logtime = logger_time->ptr;
    logtime_node_t*        node    = logtime->last;

    time_system_t time_func_beg;
    //    time_system_t time_func_end;

    time_func_beg.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);
    time_func_beg.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_NONE);
    time_func_beg.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_NONE);

    node->time_end = time_func_beg;

    time_system_t time_diff = time_system_dif(node->time_end, node->time_beg);

    node->time_min = time_system_min(time_diff, node->time_min);
    node->time_max = time_system_max(time_diff, node->time_max);
    node->time_cnt++;

    node->bar_flag   = true;
    node->loop_flag  = true;
    node->loop_count = 0;

    recursive_set_flag(node);

    //    time_func_end.sys  = time_get(YAYA_TIME_FRAGMENT_TYPE_SYS, YAYA_TIME_CLOCK_TYPE_NONE);
    //    time_func_end.user = time_get(YAYA_TIME_FRAGMENT_TYPE_USER, YAYA_TIME_CLOCK_TYPE_NONE);
    //    time_func_end.real = time_get(YAYA_TIME_FRAGMENT_TYPE_REAL, YAYA_TIME_CLOCK_TYPE_REALTIME);

    //    if (node->prev != NULL) {
    //        node->prev->time_sum = time_system_dif(node->prev->time_sum, time_system_dif(time_func_end, time_func_beg));
    //    }

    return true;
}

static bool recursive_out_file(private_logger_time_t* logtime, logtime_node_t* head, FILE* out) {
    if (head != NULL) {
        for (size_t i = 0; i < head->node_count; i++) {
            logtime_node_t* node = head->node[i];
            // clang-format off
            fprintf(out, "|%15.9f; " "%15.9f; " "%15.9f; |",
                         time_convflt(node->time_sum.real),
                         time_convflt(node->time_sum.user),
                         time_convflt(node->time_sum.sys));

            if (node->loop_flag) {
                fprintf(out, "|%15.9f; " "%15.9f; " "%15.9f; |"
                             "|%15.9f; " "%15.9f; " "%15.9f; |",
                             time_convflt(node->time_min.real),
                             time_convflt(node->time_min.user),
                             time_convflt(node->time_min.sys),
                             time_convflt(node->time_max.real),
                             time_convflt(node->time_max.user),
                             time_convflt(node->time_max.sys));
            } else {
                fprintf(out, "|%15s; %15s; %15s; |"
                             "|%15s; %15s; %15s; |",
                             "-", "-", "-",
                             "-", "-", "-");
            }

            if (node->loop_flag) {
                fprintf(out, "|%15.9f; " "%15.9f; " "%15.9f; |",
                             time_convflt(node->time_sum.real) / node->time_cnt,
                             time_convflt(node->time_sum.user) / node->time_cnt,
                             time_convflt(node->time_sum.sys)  / node->time_cnt);
            } else {
                fprintf(out, "|%15s; %15s; %15s; |",
                             "-", "-", "-");
            }

            fprintf(out, "%10ld; ", node->time_cnt);

            fprintf(out, "%s; %*s%s \n", node->node_count == 0 ? ". " : "> ", (int)((node->deep - 1) * logtime->sett->tab_size), "", node->name);
            // clang-format on

            if (!recursive_out_file(logtime, node, out)) {
                return false;
            }
        }
    }
    return true;
}

bool logtime_out(logtime_t* logger_time, FILE* out) {
    private_logger_time_t* logtime = logger_time->ptr;
    if (logtime->head == logtime->last) {
        // clang-format off
        fprintf(
            out,
            "|  time_sum.real;   time_sum.user;    time_sum.sys; |"
            "|  time_min.real;   time_min.user;    time_min.sys; |"
            "|  time_max.real;   time_max.user;    time_max.sys; |"
            "|  time_avg.real;   time_avg.user;    time_avg.sys; |"
            "  cnt loop; "
            "^ ; text;\n");
        // clang-format on
        if (!recursive_out_file(logtime, logtime->head, out)) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}
