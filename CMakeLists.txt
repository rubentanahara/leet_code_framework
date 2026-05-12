cmake_minimum_required(VERSION 3.20)
project(leetcode-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/Sanitizers.cmake)

include(FetchContent)
# Pull just the single header — avoids doctest's older CMakeLists (incompatible
# with CMake 4.x) and keeps the build's moving parts minimal.
FetchContent_Declare(
    doctest_header
    URL https://raw.githubusercontent.com/doctest/doctest/v2.4.11/doctest/doctest.h
    URL_HASH SHA256=44faa038e9c3f9728efbda143748d01124ea0a27f4bf78f35a15d8fab2e039fb
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(doctest_header)
set(_doctest_include ${CMAKE_BINARY_DIR}/_deps/doctest_header-include)
file(MAKE_DIRECTORY ${_doctest_include}/doctest)
file(COPY ${doctest_header_SOURCE_DIR}/doctest.h DESTINATION ${_doctest_include}/doctest)

add_library(doctest INTERFACE)
# SYSTEM: doctest is third-party; suppress warnings originating from its headers
# (e.g. clang's -Wc2y-extensions on doctest's use of __COUNTER__).
target_include_directories(doctest SYSTEM INTERFACE ${_doctest_include})

add_library(lc_common INTERFACE)
target_include_directories(lc_common INTERFACE ${CMAKE_SOURCE_DIR}/common)
target_compile_features(lc_common INTERFACE cxx_std_20)

add_library(lc_warnings INTERFACE)
target_compile_options(lc_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wnon-virtual-dtor
)

enable_testing()

file(GLOB PROBLEM_DIRS CONFIGURE_DEPENDS
    RELATIVE ${CMAKE_SOURCE_DIR}/problems
    ${CMAKE_SOURCE_DIR}/problems/*
)

set(PROBLEM_COUNT 0)
foreach(dir ${PROBLEM_DIRS})
    set(full_path ${CMAKE_SOURCE_DIR}/problems/${dir})
    if(IS_DIRECTORY ${full_path} AND EXISTS ${full_path}/test.cpp)
        add_executable(${dir} ${full_path}/test.cpp)
        target_include_directories(${dir} PRIVATE ${full_path})
        target_link_libraries(${dir} PRIVATE doctest lc_common lc_warnings lc_sanitizers)
        add_test(NAME ${dir} COMMAND ${dir})
        math(EXPR PROBLEM_COUNT "${PROBLEM_COUNT} + 1")
    endif()
endforeach()

message(STATUS "leetcode-cpp: discovered ${PROBLEM_COUNT} problem(s)")
