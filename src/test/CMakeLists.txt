cmake_minimum_required(VERSION 3.10)
project(sdl2_minesweeper-tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

file(GLOB TEST_SRC *.cpp)

add_executable(${PROJECT_NAME} ${TEST_SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC CONAN_PKG::gtest sdl2_minesweeper-lib ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

include(GoogleTest)
#gtest_add_tests(${PROJECT_NAME} SOURCES test_minefield.cpp WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

#gtest_discover_tests(${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR} TEST_PREFIX Mine)
