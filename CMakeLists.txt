cmake_minimum_required(VERSION 3.28)
project(mswpr VERSION 0.3.0 LANGUAGES CXX)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(MSWPR_VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MSWPR_ENABLE_COVERAGE "Enable coverage generation" OFF)
option(MSWPR_ENABLE_CLANG_TIDY "Enable clang-tidy checks" OFF)

if (MSWPR_ENABLE_CLANG_TIDY)
    include(cmake/static_analyzers.cmake)
    enable_clang_tidy()
endif()

add_subdirectory(src/core)
add_subdirectory(src/gui)

include(CTest)
if(BUILD_TESTING)
     add_subdirectory(tests)
endif()

if(MSWPR_ENABLE_COVERAGE)
    include(cmake/CodeCoverage.cmake)
    setup_target_for_coverage_lcov(${PROJECT_NAME}-core)
endif()

# add_library(PROJECT_OPTIONS INTERFACE)
# include(cmake/Sanitizers.cmake)
# enable_sanitizers(PROJECT_OPTIONS)

include(GNUInstallDirs)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}-deps
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    RUNTIME_DEPENDENCY_SET ${PROJECT_NAME}-deps
    PRE_EXCLUDE_REGEXES
        [[api-ms-win-.*]]
        [[ext-ms-.*]]
        [[hvsifiletrust\.dll]]
        [[vcruntime.*\.dll]]
        [[msvcp.*\.dll]]
        [[setupapi\.dll]]
        [[version\.dll]]
        [[winmm\.dll]]
        [[.*32\.dll]]
        [[libc\.so\..*]] [[libgcc_s\.so\..*]] [[libm\.so\..*]] [[libstdc\+\+\.so\..*]]
    POST_EXCLUDE_REGEXES
        [[.*/system32/.*\.dll]]
        [[^/lib.*]]
        [[^/usr/lib.*]]
    DIRECTORIES ${CONAN_RUNTIME_LIB_DIRS}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    FRAMEWORK DESTINATION ${PROJECT_NAME}.app/Contents/Frameworks
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/assets/ship/
    DESTINATION ${CMAKE_INSTALL_BINDIR}/assets
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR TGZ)
else()
    set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
