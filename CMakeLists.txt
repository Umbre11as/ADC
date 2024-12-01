cmake_minimum_required(VERSION 3.28)
include("${CMAKE_SOURCE_DIR}/cmake/global-flags.cmake")
project(ADC LANGUAGES C)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(ADC ${SOURCES})

add_subdirectory(api)
add_subdirectory(Cli)
add_subdirectory(challenges/create-device)
