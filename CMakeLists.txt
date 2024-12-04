cmake_minimum_required(VERSION 3.28)
include("${CMAKE_SOURCE_DIR}/cmake/global-flags.cmake")
project(ADC)

include(FetchContent)

add_subdirectory(api)
add_subdirectory(cli)
add_subdirectory(gui)
add_subdirectory(challenges)
