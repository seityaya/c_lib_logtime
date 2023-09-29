#Author                 : Seityagiya Terlekchi
#Contacts               : seityaya@ukr.net
#Creation Date          : 2023.09
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2023-2023 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 11)

set(VERSION 0.1)
set(PROJECT_NAME logtime_test)
project(${PROJECT_NAME})

add_executable(
    ${PROJECT_NAME}
    main.c
    )

if (NOT TARGET yaya_logger_time)
    add_subdirectory(${CMAKE_SOURCE_DIR}/../../yaya_logtime/lib/ yaya_lib/yaya_logtime)
    target_include_directories(${PROJECT_NAME} PUBLIC ../../yaya_logtime/lib/)
    target_link_libraries(${PROJECT_NAME} yaya_logtime)
endif()

