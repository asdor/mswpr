cmake_minimum_required(VERSION 3.10)
project(sdl2_minesweeper-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

file(GLOB TEST_SRC *.cpp)

add_executable(${PROJECT_NAME} ${TEST_SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::gtest sdl2_minesweeper-lib)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
