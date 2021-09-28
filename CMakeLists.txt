cmake_minimum_required(VERSION 3.10)
project(sdl2_minesweeper VERSION 0.0.1 LANGUAGES CXX)

# list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

file(GLOB GAME_SRC src/*.cpp src/states/*.cpp)

# add_library(PROJECT_OPTIONS INTERFACE)
# include(cmake/Sanitizers.cmake)
# enable_sanitizers(PROJECT_OPTIONS)

set(SDL2_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2_image/include)
set(SDL2_IMAGE_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/SDL2_image.lib)
set(SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.16/include)
set(SDL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.16/lib/x64/SDL2.lib)

add_executable(${PROJECT_NAME} ${GAME_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS} ${SDL2_IMAGE_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PRIVATE PROJECT_OPTIONS)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

macro(copy_dll_to_bin dir_path)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${dir_path}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endmacro()

if (WIN32) # Copy .dll to build folder
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.16/lib/x64/SDL2.dll")
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/SDL2_image.dll")
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/libpng16-16.dll")
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/libjpeg-9.dll")
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/libtiff-5.dll")
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/libwebp-7.dll")
    copy_dll_to_bin("${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/zlib1.dll")
endif()

file(GLOB ASSETS_DATA ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(COPY ${ASSETS_DATA}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror -fconcepts>
)
