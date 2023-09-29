#Author                 : Seityagiya Terlekchi
#Contacts               : seityaya@ukr.net
#Creation Date          : 2023.09
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2023-2023 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 11)
add_definitions(-std=c11)

set(VERSION 0.1)
set(PROJECT_NAME yaya_logtime)

project(${PROJECT_NAME})

set(SRC_LIST yaya_logtime.c)
set(INC_LIST yaya_logtime.h)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST} ${INC_LIST})

if (NOT TARGET yaya_memory)
    add_subdirectory(${CMAKE_SOURCE_DIR}/../../yaya_memory/lib/ yaya_lib/yaya_memory)
    target_include_directories(${PROJECT_NAME} PUBLIC ../../yaya_memory/lib/)
    target_link_libraries(${PROJECT_NAME} yaya_memory)
endif()

if (NOT TARGET yaya_define)
    add_subdirectory(${CMAKE_SOURCE_DIR}/../../yaya_define/lib/ yaya_lib/yaya_define)
    target_include_directories(${PROJECT_NAME} PUBLIC ../../yaya_define/lib/)
    target_link_libraries(${PROJECT_NAME} yaya_define)
endif()
