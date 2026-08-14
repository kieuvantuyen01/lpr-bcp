cmake_minimum_required(VERSION 3.16)
project(lpr_bcp LANGUAGES CXX)

include(CTest)

add_executable(lpr-bcp LPR_BCP.cpp)
target_compile_features(lpr-bcp PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(lpr-bcp PRIVATE /W4)
else()
    target_compile_options(lpr-bcp PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_test(
        NAME lpr-smoke
        COMMAND
            ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/smoke_test.py
            $<TARGET_FILE:lpr-bcp>
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/data/triangle.col
            ${CMAKE_CURRENT_BINARY_DIR}/test-output
    )
endif()
