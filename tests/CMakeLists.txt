find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

file(GLOB TEST_SRC *.cpp)

add_executable(${PROJECT_NAME}-test ${TEST_SRC})

target_link_libraries(${PROJECT_NAME}-test PUBLIC gtest::gtest ${PROJECT_NAME}-lib)

gtest_discover_tests(${PROJECT_NAME}-test)
