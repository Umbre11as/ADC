cmake_minimum_required(VERSION 3.28)
include("${CMAKE_SOURCE_DIR}/cmake/global-flags.cmake")
project(ADC LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(ADC ${SOURCES})
