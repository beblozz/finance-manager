cmake_minimum_required(VERSION 3.16)

project(FinanceManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(finance
    src/main.cpp
    src/date.cpp
    src/transaction.cpp
    src/finance_manager.cpp
    src/report.cpp
    src/csv_io.cpp
)

target_include_directories(finance PRIVATE include)

if(MSVC)
    target_compile_options(finance PRIVATE /W4 /utf-8)
else()
    target_compile_options(finance PRIVATE -Wall -Wextra -Wpedantic)
endif()
