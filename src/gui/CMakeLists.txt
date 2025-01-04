configure_file(game_version.cpp.in "${CMAKE_BINARY_DIR}/src/gui/game_version.cpp")

file(GLOB GAME_GUI_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${GAME_GUI_SRC}
    ${CMAKE_BINARY_DIR}/src/gui/game_version.cpp
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        SDL2::SDL2 SDL2_image::SDL2_image ${PROJECT_NAME}-lib
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)
