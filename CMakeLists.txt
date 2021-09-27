cmake_minimum_required(VERSION 3.16)
project(sdl2_minesweeper VERSION 0.0.1 LANGUAGES CXX)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB GAME_SRC src/*.cpp src/states/*.cpp)

add_library(PROJECT_OPTIONS INTERFACE)
include(cmake/Sanitizers.cmake)
enable_sanitizers(PROJECT_OPTIONS)

add_executable(${PROJECT_NAME} ${GAME_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE PROJECT_OPTIONS)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror -fconcepts>
)
