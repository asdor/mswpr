option(USE_CPPLINT "Use cpplint cmake feature based on clang-format" ON)
option(CPPLINT_INPLACE "Correct source code in-place" ON)

if(NOT USE_CPPLINT)
    return()
endif()

find_program(CLANG_FORMAT clang-format NO_CMAKE_ENVIRONMENT_PATH DOC "clang-format executable")
if(NOT CLANG_FORMAT)
    message(SEND_ERROR "clang-format not found")
endif()

set(CMAKE_CXX_CPPLINT ${CMAKE_COMMAND} "-DEXE=${CLANG_FORMAT}" "-DSEARCH_PATHS=${CMAKE_SOURCE_DIR}" -DINPLACE=${CPPLINT_INPLACE} -P ${CMAKE_CURRENT_LIST_DIR}/cpplint.cmake)
set(CMAKE_C_CPPLINT ${CMAKE_CXX_CPPLINT})

# set(CMAKE_CXX_COMPILER_LAUNCHER ${CMAKE_COMMAND} "-DEXE=${CLANG_FORMAT}" -DINPLACE=${CPPLINT_INPLACE} -P ${CMAKE_CURRENT_LIST_DIR}/cpplint.cmake)
# set(CMAKE_C_COMPILER_LAUNCHER ${CMAKE_CXX_COMPILER_LAUNCHER})
