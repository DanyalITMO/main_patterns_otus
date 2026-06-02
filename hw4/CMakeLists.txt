cmake_minimum_required(VERSION 3.14)
project(homework4)
set(CMAKE_CXX_STANDARD 20)

enable_testing()

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME}_tests tests.cpp angle.cpp)

target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
        ${PROJECT_NAME}_tests
        GTest::gtest_main
        GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
