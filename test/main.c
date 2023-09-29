//Author                 : Seityagiya Terlekchi
//Contacts               : seityaya@ukr.net
//Creation Date          : 2023.09
//License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
//SPDX-License-Identifier: LGPL-2.1-or-later
//Copyright © 2023-2023 Seityagiya Terlekchi. All rights reserved.

#include "yaya_logtime.h"

#include "time.h"

#define ticstop(tic) do { clock_t now = clock(); while(clock() <= now + (tic)){ } } while (0)

logger_time_t* logger_time = NULL;

int fibonacci(int num) {
    int res = 0;
    if(num <= 2){
        res = num;
    } else {
        res = (fibonacci(num - 1) + fibonacci(num - 2));
    }

    logtime_bar(logger_time);
    return res;
}
int factorial(int num) {
    int res = 0;
    if(num == 0) {
        res = 1;
    }else{
        res = num * factorial(num - 1);
    }

    logtime_bar(logger_time);
    return res;
}
int test_lin(){
    logtime_beg(logger_time, "Lin 0");
    {
        ticstop(100);
        logtime_beg(logger_time, "Lin 1");
        {
            ticstop(100);
        }
        logtime_end(logger_time);

        ticstop(100);
        logtime_beg(logger_time, "Lin 2");
        {
            ticstop(100);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "Lin 3");
        {
            logtime_beg(logger_time, "Lin 3.1");
            {
                logtime_beg(logger_time, "Lin 3.1.1");
                {
                    ticstop(100);
                }
                logtime_end(logger_time);
            }
            logtime_end(logger_time);

            logtime_beg(logger_time, "Lin 3.2");
            {
                logtime_beg(logger_time, "Lin 3.2.1");
                {
                    logtime_beg(logger_time, "Lin 3.2.1.1");
                    {
                        ticstop(100);
                    }
                    logtime_end(logger_time);
                }
                logtime_end(logger_time);
            }
            logtime_end(logger_time);

            logtime_beg(logger_time, "Lin 3.3 ");
            {
                logtime_beg(logger_time, "Lin 3.3.1");
                {
                    logtime_beg(logger_time, "Lin 3.3.1.1");
                    {
                        logtime_beg(logger_time, "Lin 3.3.1.1.1");
                        {
                            ticstop(100);
                        }
                        logtime_end(logger_time);
                        logtime_beg(logger_time, "Lin 3.3.1.1.2");
                        {
                            ticstop(100);
                        }
                        logtime_end(logger_time);
                        logtime_beg(logger_time, "Lin 3.3.1.1.3");
                        {
                            ticstop(100);
                        }
                        logtime_end(logger_time);
                    }
                    logtime_end(logger_time);
                }
                logtime_end(logger_time);
            }
            logtime_end(logger_time);
        }
        logtime_end(logger_time);
    }
    logtime_end(logger_time);
    return 0;
}
int test_cyc(){
    logtime_beg(logger_time, "For 1");
    for(int i = 0; i < 10; i++) {
        ticstop(20);
        logtime_beg(logger_time, "For 1.1");
        for(int j = 0; j < 10; j++) {
            ticstop(100);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "For 1.2");
        for(int j = 0; j < 10; j++) {
            ticstop(100);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "For 1.3");
        for(int j = 0; j < 10; j++) {
            ticstop(1000);
            logtime_beg(logger_time, "For 1.3.1");
            for(int k = 0; k < 10; k++) {
                ticstop(100);
                logtime_bar(logger_time);
            }
            logtime_end(logger_time);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);
        logtime_bar(logger_time);
    }
    logtime_end(logger_time);
    return 0;
}
int test_cyc_two(){
    logtime_beg(logger_time, "For 1");
    for(int i = 0; i < 10; i++)
    {
        logtime_beg(logger_time, "For 1.1");
        for(int j = 0; j < 10; j++) {
            ticstop(100);
            logtime_bar(logger_time);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "For 1.2");
        for(int j = 0; j < 10; j++) {

            ticstop(100);
            logtime_bar(logger_time);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "For 1.3");
        for(int j = 0; j < 10; j++) {
            ticstop(1000);
            logtime_beg(logger_time, "For 1.3.1");
            for(int k = 0; k < 10; k++) {
                ticstop(100);
                logtime_bar(logger_time);
                logtime_bar(logger_time);
            }
            logtime_end(logger_time);
            logtime_bar(logger_time);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);
        logtime_bar(logger_time);
        logtime_bar(logger_time);
    }
    logtime_end(logger_time);
    return 0;
}
int test_lin_cyc(){
    logtime_beg(logger_time, "For 1");
    for(int i = 0; i < 10; i++) {
        logtime_beg(logger_time, "Lin 1.1");
        {
            ticstop(100);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "For 1.2");
        for(int j = 0; j < 10; j++) {
            ticstop(100);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "Lin 1.3");
        {
            ticstop(100);
        }
        logtime_end(logger_time);


        logtime_beg(logger_time, "For 1.4");
        for(int j = 0; j < 10; j++) {
            ticstop(1000);
            logtime_beg(logger_time, "Lin 1.4.1");
            {
                ticstop(100);
            }
            logtime_end(logger_time);
            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "Lin 1.5");
        {
            ticstop(100);
        }
        logtime_end(logger_time);

        logtime_beg(logger_time, "For 1.6");
        for(int j = 0; j < 10; j++) {
            ticstop(1000);
            logtime_beg(logger_time, "Lin 1.6.1");
            {
                logtime_beg(logger_time, "Lin 1.6.1.1");
                {
                    logtime_beg(logger_time, "Lin 1.6.1.1.1");
                    {
                        ticstop(100);
                    }
                    logtime_end(logger_time);
                }
                logtime_end(logger_time);

                logtime_beg(logger_time, "Lin 1.6.2.1");
                {
                    ticstop(100);
                }
                logtime_end(logger_time);
            }
            logtime_end(logger_time);

            logtime_bar(logger_time);
        }
        logtime_end(logger_time);

        logtime_bar(logger_time);
    }
    logtime_end(logger_time);
    return 0;
}
int test_rec(){
    logtime_beg(logger_time, "Factorial from 9");
    factorial(9);
    logtime_end(logger_time);

    logtime_beg(logger_time, "Factorial loop 10");
    for(int i = 0; i < 10; i++) {
        logtime_beg(logger_time, "Factorial from 9");
        factorial(9);
        logtime_end(logger_time);
        logtime_bar(logger_time);
    }
    logtime_end(logger_time);


    logtime_beg(logger_time, "Fibonacci from 9");
    fibonacci(9);
    logtime_end(logger_time);

    logtime_beg(logger_time, "Fibonacci loop 10");
    for(int i = 0; i < 10; i++) {
        logtime_beg(logger_time, "Fibonacci from 9");
        fibonacci(9);
        logtime_end(logger_time);
        logtime_bar(logger_time);
    }
    logtime_end(logger_time);
}

int main() {
    logger_time_setting_t logger_time_setting = {.tab_size = 4,
                                                 .get_time_tic = logtime_time_tic,
                                                 .get_time_sec = logtime_time_sec,
                                                 .format_string = "" };

    logtime_init(&logger_time, &logger_time_setting);

    test_lin();
    test_cyc();
    test_cyc_two();
    test_lin_cyc();
    test_cyc();
    test_rec();

    logtime_out(logger_time, stdout);
    logtime_free(&logger_time);

    return 0;
}
