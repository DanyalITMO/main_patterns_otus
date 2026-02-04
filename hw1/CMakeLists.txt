project(homework1)

enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}_tests tests.cpp)

target_link_libraries(
        ${PROJECT_NAME}_tests
        GTest::gtest_main
        GTest::gmock
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
