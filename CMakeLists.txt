# minimum requirement
cmake_minimum_required(VERSION 3.6)

# solution
project(nonogramSolver)

# include gtest settings
include("cmake/gtest.cmake")

SET(TARGET_NAME solver)
SET(RUNNER_NAME solverRunner)
SET(TEST_NAME solverTest)

# build TARGET_NAME from source code
add_library(${TARGET_NAME} STATIC solver.cpp solver.hpp)
cxx_test(${TEST_NAME} solverTest.cpp)
add_executable(${RUNNER_NAME} solverRunner.cpp)
target_link_libraries(${RUNNER_NAME} ${TARGET_NAME})
target_link_libraries(${TEST_NAME} ${TARGET_NAME})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_BINARY_DIR}/third-party/include)

