if(MSVC)
    message(FATAL_ERROR "Code coverage is not supported on MSVC")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "Code coverage results with an optimised (non-Debug) build may be misleading")
endif()

function(setup_target_for_coverage_lcov target)
    find_program(GCOV_CMD gcov REQUIRED)
    message(STATUS "gcov executable found: ${GCOV_CMD}")

    find_program(LCOV_CMD lcov REQUIRED)
    message(STATUS "lcov executable found: ${LCOV_CMD}")

    find_program(GENHTML_CMD genhtml REQUIRED)
    message(STATUS "genhtml executable found: ${GENHTML_CMD}")

    set(coverage_info_dir ${CMAKE_BINARY_DIR}/coverage_report_lcov)
    set(coverage_info_baseline ${coverage_info_dir}/baseline.info)
    set(coverage_info_test ${coverage_info_dir}/test.info)
    set(coverage_info_total ${coverage_info_dir}/total.info)
    set(coverage_info_html_dir ${coverage_info_dir}/html)

    target_compile_options(${target} PUBLIC -g -O0 --coverage)
    target_link_libraries(${target} PUBLIC --coverage)

    set(LCOV_CLEAN_CMD
        ${LCOV_CMD}
        --zerocounters
        --directory ${CMAKE_BINARY_DIR}
    )
    set(LCOV_BASELINE_CMD
        ${LCOV_CMD}
        --capture
        --initial
        --directory ${CMAKE_BINARY_DIR}
        --base-directory ${CMAKE_SOURCE_DIR}
        --output-file ${coverage_info_baseline}
        --include ${CMAKE_SOURCE_DIR}/include/*
        --include ${CMAKE_SOURCE_DIR}/src/*
        --exclude $ENV{CONAN_HOME}/*
        --exclude /usr/*
        --exclude ${CMAKE_SOURCE_DIR}/tests/*
    )
    set (LCOV_CAPTURE_CMD
        ${LCOV_CMD}
        --capture
        --directory ${CMAKE_BINARY_DIR}
        --base-directory ${CMAKE_SOURCE_DIR}
        --output-file ${coverage_info_test}
        --include ${CMAKE_SOURCE_DIR}/include/*
        --include ${CMAKE_SOURCE_DIR}/src/*
        --exclude $ENV{CONAN_HOME}/*
        --exclude /usr/*
        --exclude ${CMAKE_SOURCE_DIR}/tests/*
    )
    set(LCOV_MERGE_TRACEFILES_CMD
        ${LCOV_CMD}
        --add-tracefile ${coverage_info_baseline}
        --add-tracefile ${coverage_info_test}
        --output-file ${coverage_info_total}
    )
    set(LCOV_LIST_TRACEFILE_CONTENT_CMD
        ${LCOV_CMD}
        --list ${coverage_info_total}
    )
    set(LCOV_GEN_HTML_CMD
        ${GENHTML_CMD}
        --title "Total coverage"
        --demangle-cpp
        --output-directory ${coverage_info_html_dir}
        ${coverage_info_total}
    )

    add_custom_target(clean-coverage
        COMMAND ${LCOV_CLEAN_CMD}
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${coverage_info_dir}
        COMMENT "Cleaning code coverage data"
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${coverage_info_baseline}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${coverage_info_dir}
        COMMAND ${LCOV_BASELINE_CMD}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Collecting initial code coverage"
        VERBATIM
    )
    add_custom_target(capture-coverage-initial DEPENDS ${coverage_info_baseline})
    add_dependencies(capture-coverage-initial clean-coverage)

    add_custom_target(coverage-run-tests
        COMMAND ${CMAKE_CTEST_COMMAND}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running unit tests for collecting code coverage data"
        VERBATIM
    )
    add_dependencies(coverage-run-tests capture-coverage-initial)

    add_custom_command(
        OUTPUT ${coverage_info_test}
        COMMAND ${LCOV_CAPTURE_CMD}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Collecting code coverage"
        VERBATIM
    )
    add_custom_target(capture-coverage DEPENDS ${coverage_info_test})
    add_dependencies(capture-coverage coverage-run-tests)

    add_custom_command(
        OUTPUT ${coverage_info_total}
        COMMAND ${LCOV_MERGE_TRACEFILES_CMD}
        COMMAND ${LCOV_LIST_TRACEFILE_CONTENT_CMD}
        COMMENT "Merging code coverage data"
        VERBATIM
    )
    add_custom_target(merge-coverage DEPENDS ${coverage_info_total})
    add_dependencies(merge-coverage capture-coverage)

    add_custom_command(
        OUTPUT ${coverage_info_html_dir}
        COMMAND ${LCOV_GEN_HTML_CMD}
        COMMENT "Generating html coverage report"
        VERBATIM
    )
    add_custom_target(coverage-html-report DEPENDS ${coverage_info_html_dir})
    add_dependencies(coverage-html-report merge-coverage)

endfunction()
