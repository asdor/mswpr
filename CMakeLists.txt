cmake_minimum_required(VERSION 3.10)
project(sdl2_minesweeper VERSION 0.0.1 LANGUAGES CXX)

# list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP NO_OUTPUT_DIRS CMAKE_TARGETS
                BUILD missing)

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

file(GLOB GAME_CORE_SRC src/core/*.cpp src/core/states/*.cpp)
file(GLOB GAME_GUI_SRC src/gui/*.cpp)

# add_library(PROJECT_OPTIONS INTERFACE)
# include(cmake/Sanitizers.cmake)
# enable_sanitizers(PROJECT_OPTIONS)

add_executable(${PROJECT_NAME} ${GAME_CORE_SRC} ${GAME_GUI_SRC} main.cpp)

set(SDL2_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2_image/include)
set(SDL2_IMAGE_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/x64/SDL2_image.lib)
set(SDL2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.16/include)
set(SDL2_LIBRARIES ${CMAKE_SOURCE_DIR}/lib/SDL2-2.0.16/lib/x64/SDL2.lib)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

message("Conan libs: ${CONAN_LIBS}")

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror -fconcepts>
)

add_library(${PROJECT_NAME}-lib ${GAME_CORE_SRC})
target_include_directories(${PROJECT_NAME}-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${PROJECT_NAME}-lib)

file(GLOB ASSETS_DATA ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(COPY ${ASSETS_DATA}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_subdirectory(tests)

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
