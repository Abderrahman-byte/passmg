cmake_minimum_required(VERSION 3.11)

project("Password Manager" VERSION 2.0)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(
    ${PROJECT_SOURCE_DIR}/include/libpassmg/config.hpp.in 
    ${PROJECT_BINARY_DIR}/include/libpassmg/config.hpp 
    @ONLY
)

enable_testing()

add_subdirectory(./lib/)
add_subdirectory(./tests/)
add_subdirectory(./src/)
