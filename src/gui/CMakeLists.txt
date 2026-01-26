configure_file(game_version.cpp.in "${CMAKE_BINARY_DIR}/src/gui/game_version.cpp")

file(GLOB GAME_GUI_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE WIN32
    ${GAME_GUI_SRC}
    ${CMAKE_BINARY_DIR}/src/gui/game_version.cpp
)

if (MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL2::SDL2 SDL2_image::SDL2_image spdlog::spdlog ${PROJECT_NAME}-core
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets/ship
    ${CMAKE_BINARY_DIR}/assets
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(MSWPR_ICON_FILE "${CMAKE_SOURCE_DIR}/assets/do_not_ship/icons/mswpr.ico")
    configure_file(${CMAKE_SOURCE_DIR}/assets/do_not_ship/icons/app.rc.in "${CMAKE_BINARY_DIR}/assets/icons/app.rc")

    target_sources(${PROJECT_NAME}
        PRIVATE
            ${CMAKE_BINARY_DIR}/assets/icons/app.rc
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MSWPR_ICON_FILE "${CMAKE_SOURCE_DIR}/assets/ship/mswpr.png")
    set(MSWPR_MANIFEST_FILE "${CMAKE_SOURCE_DIR}/assets/ship/mswpr.desktop")

    install(
        FILES ${MSWPR_ICON_FILE}
        DESTINATION share/pixmaps
    )
    install(
        FILES ${MSWPR_MANIFEST_FILE}
        DESTINATION share/applications
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MSWPR_ICON_FILE "${CMAKE_SOURCE_DIR}/assets/ship/mswpr.icns")
    set(MSWPR_MANIFEST_FILE "${CMAKE_SOURCE_DIR}/assets/ship/Info.plist.in")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

    target_sources(${PROJECT_NAME} PRIVATE ${MSWPR_ICON_FILE})
    set_source_files_properties(${MSWPR_ICON_FILE}
        PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            INSTALL_RPATH @executable_path/../Frameworks
            MACOSX_BUNDLE_ICON_FILE "mswpr.icns"
            MACOSX_BUNDLE_INFO_PLIST ${MSWPR_MANIFEST_FILE}
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${MSWPR_VERSION}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${MSWPR_VERSION}"
            MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_COMPANY_NAMESPACE}.${PROJECT_NAME}"
            MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
    )
endif()
