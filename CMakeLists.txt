# This is a clang-tidy check for eliminating <qthing.h> includes.
# Copyright (C) 2024  Mm2PL
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(UseProperQtHeaders LANGUAGES C CXX)
add_compile_options(-Wall -Wextra)
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG COMPONENTS ClangTidy)

add_library(UseProperQtHeaders MODULE "")
target_compile_options(UseProperQtHeaders PRIVATE -fno-rtti)
target_include_directories(UseProperQtHeaders
    PRIVATE
        ${CLANG_INCLUDE_DIRS}
        ${LLVM_INCLUDE_DIRS}
)
target_sources(UseProperQtHeaders PRIVATE 
    UseProperQtHeaders.cpp
    UseProperQtHeaders.hpp
)
