project(CreateDevice LANGUAGES C)

add_executable(CreateDevice src/main.c)

target_include_directories(CreateDevice PUBLIC "../../api/src")
target_link_libraries(CreateDevice PUBLIC "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/Api.lib")
