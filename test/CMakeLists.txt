#Author                 : Seityagiya Terlekchi
#Contacts               : seityaya@ukr.net
#Creation Date          : 2022.12
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2022-2022 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_STANDARD 11)

set(VERSION 0.1)
set(PROJECT_NAME void_test)
project(${PROJECT_NAME})

add_executable(
    ${PROJECT_NAME}
    main.c
    )

add_subdirectory(${CMAKE_SOURCE_DIR}/../lib/ yaya_void)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/)
target_link_libraries(${PROJECT_NAME} yaya_void)
