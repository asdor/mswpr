file(GLOB GAME_CORE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/states/*.cpp"
)

find_package(spdlog REQUIRED)

add_library(${PROJECT_NAME}-core ${GAME_CORE_SRC})
target_include_directories(${PROJECT_NAME}-core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}-core
    PUBLIC
        spdlog::spdlog
)

target_compile_options(${PROJECT_NAME}-core PUBLIC
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic -Werror>
    $<$<CXX_COMPILER_ID:MSVC>:
    /W4 /WX>
)
