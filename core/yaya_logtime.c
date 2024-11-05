// Author                 : Seityagiya Terlekchi
// Contacts               : terlekchiseityaya@gmail.com
// Creation Date          : 2023.09
// License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright © 2023-2024 Seityagiya Terlekchi. All rights reserved.

#include "stdlib.h"
#include "string.h"
#include "time.h"

#include "yaya_logtime.h"
#include "yaya_memory.h"

typedef struct logtime_node_t {
    char*    name;
    intmax_t deep;

    intmax_t time_beg;
    intmax_t time_end;
    intmax_t time_sum;
    intmax_t time_min;
    intmax_t time_max;
    intmax_t time_cnt;

    bool                    bar_flag;
    bool                    loop_flag;
    size_t                  loop_count;
    size_t                  node_count;
    struct logtime_node_t** node;
    struct logtime_node_t*  prev;
} logtime_node_t;

typedef struct private_logger_time_t {
    logtime_sett_t* sett;
    logtime_node_t* last;
    logtime_node_t* head;
} private_logger_time_t;

logtime_sett_t logtime_setting_std = {.tab_size = 4, .get_time_tic = logtime_time_tic, .get_time_sec = logtime_time_sec, .format_string = ""};

bool logtime_init(logtime_t** logger_time, logtime_sett_t* logger_time_setting) {
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

    if (logger_time_setting != NULL) {
        logtime->sett = logger_time_setting;
    } else {
        logtime->sett = &logtime_setting_std;
    }
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

intmax_t logtime_time_tic(void) { return clock(); }

double logtime_time_sec(intmax_t tic) { return (double)(tic) / (double)(CLOCKS_PER_SEC); }

bool logtime_beg(logtime_t* logger_time, char* name) {
    private_logger_time_t* logtime = logger_time->ptr;
    intmax_t               time    = logtime->sett->get_time_tic();
    logtime_node_t*        node    = NULL;

    if (logtime->last->loop_flag == false) {
        if (!memory_req((void**)(&logtime->last->node), logtime->last->node_count + 1, sizeof(logtime_node_t*))) {
            return false;
        }

        if (!memory_req((void**)(&logtime->last->node[logtime->last->node_count]), 1, sizeof(logtime_node_t))) {
            return false;
        }

        node           = logtime->last->node[logtime->last->node_count];
        node->name     = name;
        node->deep     = logtime->last->deep + 1;
        node->prev     = logtime->last;
        node->time_min = INTMAX_MAX;
        node->time_max = INTMAX_MIN;
        node->time_sum = 0;
        node->time_cnt = 0;

        logtime->last->node_count++;
    } else {
        node = logtime->last->node[logtime->last->loop_count];
        logtime->last->loop_count++;
    }

    logtime->last = node;

    node->time_end  = 0;
    node->time_beg  = logtime->sett->get_time_tic();
    node->time_sum -= node->time_beg - time;

    return true;
}

void logtime_end(logtime_t* logger_time) {
    private_logger_time_t* logtime = logger_time->ptr;
    intmax_t               time    = logtime->sett->get_time_tic();
    logtime_node_t*        node    = logtime->last;

    node->time_end = time;

    intmax_t time_diff = logtime->last->time_end - logtime->last->time_beg;

    node->time_sum += time_diff;

    if (node->bar_flag == false) {
        node->time_min = time_diff < node->time_min ? time_diff : node->time_min;
        node->time_max = time_diff > node->time_max ? time_diff : node->time_max;
        node->time_cnt++;
    }
    logtime->last = node->prev;

    intmax_t time_correction  = logtime->sett->get_time_tic();
    node->time_sum           -= time_correction - time;
}

void recursive_set_flag(logtime_node_t* head) {
    for (size_t i = 0; i < head->node_count; i++) {
        head->node[i]->loop_flag  = true;
        head->node[i]->loop_count = 0;
        recursive_set_flag(head->node[i]);
    }
}

void logtime_bar(logtime_t* logger_time) {
    private_logger_time_t* logtime = logger_time->ptr;
    intmax_t               time    = logtime->sett->get_time_tic();
    logtime_node_t*        node    = logtime->last;

    node->time_end = time;

    intmax_t time_diff = logtime->last->time_end - logtime->last->time_beg;

    node->time_min  = time_diff < node->time_min ? time_diff : node->time_min;
    node->time_max  = time_diff > node->time_max ? time_diff : node->time_max;
    node->time_sum += time_diff;
    node->time_cnt++;

    node->bar_flag   = true;
    node->loop_flag  = true;
    node->loop_count = 0;

    recursive_set_flag(node);

    intmax_t time_correction  = logtime->sett->get_time_tic();
    node->time_sum           -= time_correction - time;
}

static bool recursive_out_file(private_logger_time_t* logtime, logtime_node_t* head, FILE* out) {
    if (head != NULL) {
        for (size_t i = 0; i < head->node_count; i++) {
            logtime_node_t* node = head->node[i];
            if (0 > fprintf(out,
                            "%10ld; " "%f; " "%10ld; " "%10ld; " "%10ld; " "%10ld; " "%s" "%*s%s \n",
                            node->time_sum,
                            logtime->sett->get_time_sec(node->time_sum),
                            node->time_min,
                            node->time_max,
                            node->time_cnt,
                            node->time_sum / node->time_cnt,
                            node->node_count == 0 ? ". " : "> ",
                            (int)((node->deep - 1) * logtime->sett->tab_size),
                            "",
                            node->name)) {
                return false;
            }

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
        fprintf(out, "  time tic;      sec;        min;        max;        cnt;    avg cnt; ^ text:\n");
        if (!recursive_out_file(logtime, logtime->head, out)) {
            return false;
        }
    } else {
        return false;
    }

    return true;
}
