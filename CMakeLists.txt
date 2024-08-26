cmake_minimum_required(VERSION 3.28)
project(sdl2_minesweeper VERSION 0.2.0 LANGUAGES CXX)

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

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2 SDL2_image::SDL2_image ${PROJECT_NAME}-lib)

file(GLOB ASSETS_DATA ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(COPY ${ASSETS_DATA}
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets)

if(BUILD_TESTING)
     add_subdirectory(tests)
endif()

include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}-deps
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}-deps
    PRE_EXCLUDE_REGEXES
        [[api-ms-win-.*]]
        [[ext-ms-.*]]
        [[hvsifiletrust\.dll]]
        [[kernel32\.dll]]
        [[libc\.so\..*]] [[libgcc_s\.so\..*]] [[libm\.so\..*]] [[libstdc\+\+\.so\..*]]
    POST_EXCLUDE_REGEXES
        [[.*/system32/.*\.dll]]
        [[^/lib.*]]
        [[^/usr/lib.*]]
    DIRECTORIES ${CONAN_RUNTIME_LIB_DIRS}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY assets DESTINATION ${CMAKE_INSTALL_BINDIR})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR TGZ)
else()
    set(CPACK_GENERATOR ZIP)
endif()

include(CPack)
