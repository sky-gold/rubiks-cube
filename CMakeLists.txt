cmake_minimum_required(VERSION 3.14)
project(rubiks_cube_solver VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories for source and test files
add_subdirectory(src)
add_subdirectory(tests)

include(CTest)

# Add custom targets
add_custom_target(build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
    COMMENT "Building the project"
)

add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS build
    COMMENT "Running tests"
)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/src/rubiks_cube_solver
    DEPENDS build
    COMMENT "Running the project"
)