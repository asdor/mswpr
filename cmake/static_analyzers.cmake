macro(enable_clang_tidy)
    find_program(CLANG_TIDY_CMD clang-tidy REQUIRED)
    message(STATUS "clang-tidy executable found: ${CLANG_TIDY_CMD}")

    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set(CLANG_TIDY_OPTIONS
        ${CLANG_TIDY_CMD}
        -extra-arg=-Wno-unknown-warning-option
        -extra-arg=-Wno-ignored-optimization-argument
        -extra-arg=-Wno-unused-command-line-argument
        -p
    )
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
endmacro()
