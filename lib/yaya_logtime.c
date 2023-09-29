//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.09
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2023-2023 Seityagiya Terlekchi. All rights reserved.

#include "stdlib.h"

#include "string.h"
#include "time.h"

#include "yaya_memory.h"
#include "yaya_bool.h"

#include "yaya_logtime.h"

typedef struct logger_time_node_t {
    char*    name;
    intmax_t deep;
    intmax_t time_beg;
    intmax_t time_end;
    intmax_t time_sum;
    intmax_t time_min;
    intmax_t time_max;
    intmax_t time_cnt;

    bool_t                      bar_flag;
    bool_t                      loop_flag;
    size_t                      loop_count;
    size_t                      node_count;
    struct logger_time_node_t** node;
    struct logger_time_node_t*  prev;
} logger_time_node_t;

typedef struct private_logger_time_t {
    logger_time_setting_t* sett;
    logger_time_node_t*    last;
    logger_time_node_t*    head;
} private_logger_time_t;

bool logtime_init(logger_time_t** logger_time, logger_time_setting_t* logger_time_setting){
    if(!memory_new((void**)logger_time, *logger_time, 1, sizeof(logger_time_t))) {
        return false;
    }
    if(!memory_new((void**)(&(*logger_time)->ptr), (*logger_time)->ptr, 1, sizeof(private_logger_time_t))) {
        return false;
    }
    private_logger_time_t* logtime = (*logger_time)->ptr;

    if(!memory_new((void**)(&(logtime->head)), logtime->head, 1, sizeof(logger_time_node_t))) {
        return false;
    }
    logtime->sett       = logger_time_setting;
    logtime->last       = logtime->head;

    return true;
}

bool logtime_free(logger_time_t** logger_time){
    bool recursive_free(logger_time_node_t* head) {
        if(head != NULL) {
            for(int i = 0; i < head->node_count; i++) {
                logger_time_node_t* node = head->node[i];
                recursive_free(node);
                if(!memory_del(&node)){
                    return false;
                }
            }
            if(!memory_del(&head->node)){
                return false;
            }
        }
    }
    private_logger_time_t* logtime = (*logger_time)->ptr;

    if(!recursive_free(logtime->head)){
        return false;
    }
    if(!memory_del(&logtime->head)){
        return false;
    }
    if(!memory_del(&logtime)){
        return false;
    }
    if(!memory_del(&(*logger_time))){
        return false;
    }
    return true;
}

intmax_t logtime_time_tic(){
    return clock();
}

intmax_t logtime_time_sec(intmax_t tic){
    return (double)(tic) / (double)(CLOCKS_PER_SEC);
}

bool logtime_beg(logger_time_t* logger_time, char* name) {
    private_logger_time_t* logtime = logger_time->ptr;
    intmax_t               time    = logtime->sett->get_time_tic();
    logger_time_node_t*    node    = NULL;

    if(logtime->last->loop_flag == false) {
        size_t index = logtime->last->node_count;

        if(!memory_new(&logtime->last->node,        logtime->last->node,        index + 1, sizeof(logger_time_node_t*))){
            return false;
        }
        if(!memory_new(&logtime->last->node[index], logtime->last->node[index],         1, sizeof(logger_time_node_t))){
            return false;
        }
        logtime->last->node_count++;

        node       = logtime->last->node[index];
        node->name = name;
        node->deep = logtime->last->deep + 1;
        node->prev = logtime->last;
    } else {
        size_t index = logtime->last->loop_count;
        logtime->last->loop_count++;
        node = logtime->last->node[index];
    }

    logtime->last = node;

    node->time_end  = 0;
    node->time_beg = logtime->sett->get_time_tic();
    node->time_sum -= node->time_beg - time;

    return true;
}

void logtime_end(logger_time_t* logger_time) {
    private_logger_time_t* logtime = logger_time->ptr;
    intmax_t               time    = logtime->sett->get_time_tic();
    logger_time_node_t*    node    = logtime->last;

    node->time_end  = time;
    node->time_sum += logtime->last->time_end - logtime->last->time_beg;

    if(node->bar_flag == false){
        node->time_cnt++;
    }
    logtime->last = node->prev;

    intmax_t time_correction = logtime->sett->get_time_tic();
    node->time_sum -= time_correction - time;
}

void logtime_bar(logger_time_t* logger_time) {
    private_logger_time_t* logtime = logger_time->ptr;
    intmax_t               time    = logtime->sett->get_time_tic();
    logger_time_node_t*    node    = logtime->last;

    void recursive_set_flag(logger_time_node_t* head) {
        for(int i = 0; i < head->node_count; i++) {
            head->node[i]->loop_flag = true;
            head->node[i]->loop_count = 0;
            recursive_set_flag(head->node[i]);
        }
    }
    node->time_end  = time;

    node->time_sum += logtime->last->time_end - logtime->last->time_beg;
    node->time_cnt++;

    node->bar_flag   = true;
    node->loop_flag  = true;
    node->loop_count = 0;

    recursive_set_flag(node);

    intmax_t time_correction = logtime->sett->get_time_tic();
    node->time_sum -= time_correction - time;
}

bool logtime_str(logger_time_t* logger_time, char* out, size_t len) {
    private_logger_time_t* logtime = logger_time->ptr;

    return true;
}

bool logtime_out(logger_time_t* logger_time, FILE* out) {
    bool recursive_out_file(private_logger_time_t* logtime, logger_time_node_t* head, FILE* out) {
        if(head != NULL) {
            for(int i = 0; i < head->node_count; i++) {
                logger_time_node_t* node = head->node[i];
                if(0 > fprintf(out, "time: %10ld tic; %f sec ; %10d cnt; %10ld avg | %s text: %*s%s \n",
                        node->time_sum,
                        logtime->sett->get_time_sec(node->time_sum),
                        node->time_cnt,
                        node->time_sum / node->time_cnt,
                        node->node_count == 0 ? "* " : "| ",
                        (node->deep - 1) * logtime->sett->tab_size, "", node->name)){
                    return false;
                }

                if(0 > recursive_out_file(logtime, node, out)){
                    return false;
                }
            }
        }
        return true;
    }

    private_logger_time_t* logtime = logger_time->ptr;
    if(logtime->head == logtime->last){
        if(!recursive_out_file(logtime, logtime->head, out)){
            return false;
        }
    }else{
        return false;
    }

    return true;
}
