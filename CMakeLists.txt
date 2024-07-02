cmake_minimum_required(VERSION 3.28)
project(sdl2_minesweeper VERSION 0.0.1 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)

file(GLOB GAME_CORE_SRC src/core/*.cpp src/core/states/*.cpp)
file(GLOB GAME_GUI_SRC src/gui/*.cpp)

# add_library(PROJECT_OPTIONS INTERFACE)
# include(cmake/Sanitizers.cmake)
# enable_sanitizers(PROJECT_OPTIONS)

add_executable(${PROJECT_NAME} ${GAME_CORE_SRC} ${GAME_GUI_SRC} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4 /WX>
)

add_library(${PROJECT_NAME}-lib ${GAME_CORE_SRC})
target_include_directories(${PROJECT_NAME}-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2_image::SDL2_image ${PROJECT_NAME}-lib)

file(GLOB ASSETS_DATA ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(COPY ${ASSETS_DATA}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

if(BUILD_TESTING)
     add_subdirectory(tests)
endif()
