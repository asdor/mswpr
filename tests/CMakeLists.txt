find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

file(GLOB TEST_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_utils/*.cpp"
)

add_executable(${PROJECT_NAME}-test ${TEST_SRC})

target_include_directories(${PROJECT_NAME}-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}-test
    PRIVATE
        gtest::gtest ${PROJECT_NAME}-core
)

gtest_discover_tests(${PROJECT_NAME}-test NO_PRETTY_VALUES)
