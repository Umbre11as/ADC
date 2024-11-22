project(Api LANGUAGES C)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(Api STATIC ${SOURCES})
