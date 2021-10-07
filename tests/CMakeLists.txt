cmake_minimum_required(VERSION 3.10)
project(sdl2_minesweeper-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

file(GLOB TEST_SRC *.cpp)

add_executable(${PROJECT_NAME} ${TEST_SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::gtest sdl2_minesweeper-lib)

include(GoogleTest)
#gtest_add_tests(${PROJECT_NAME} SOURCES test_minefield.cpp WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

#gtest_discover_tests(${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} TEST_PREFIX Mine)
