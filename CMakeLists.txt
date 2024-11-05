#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2023.09
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2023-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(yaya_logtime_project)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")

add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(using)

add_subdirectory(libs/c_lib_memory/core)
add_subdirectory(libs/c_lib_systime/core)
