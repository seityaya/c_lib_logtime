#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2023.09
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2023-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(
    yaya_logtime
    VERSION      0.1.0.0
    LANGUAGES    C
    HOMEPAGE_URL "https://github.com/seityaya/c_logtime"
    DESCRIPTION  "Tree view time logger"
)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST
    yaya_logtime.c
)

set(INC_LIST
    yaya_logtime.h
)

add_library(${PROJECT_NAME} ${SRC_LIST} ${INC_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC yaya_memory)
