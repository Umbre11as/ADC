project(Cli LANGUAGES C)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(Cli ${SOURCES})
