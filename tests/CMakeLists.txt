cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Collect C++ source files recursively
file(GLOB_RECURSE CXX_FILES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
add_executable(unit_tests ${CXX_FILES})

# Link GoogleTest libraries
target_link_libraries(unit_tests
    PRIVATE
    gtest_main
    ${PROJECT_NAME}_lib  # Link to the main project library
)

# Include directories (including where GoogleTest is built)
target_link_libraries(unit_tests PRIVATE gtest_main gmock_main)
target_include_directories(unit_tests PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/_deps/googletest-src/googletest/include
)

# Add include directories for tests to find headers
target_include_directories(unit_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Enable testing and discover tests
include(GoogleTest)
gtest_discover_tests(unit_tests)
