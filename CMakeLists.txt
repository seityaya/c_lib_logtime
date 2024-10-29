#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2023.09
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2023-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

set(PROJECT_NAME yaya_logtime)

project(${PROJECT_NAME})

add_subdirectory(core)
add_subdirectory(libs/c_memory/)
add_subdirectory(test)
add_subdirectory(using)
