project(CreateDevice LANGUAGES C)

add_link_options("/ENTRY:DriverEntry")

wdk_add_driver(CreateDevice src/main.c)

target_link_libraries(CreateDevice kCaveHook)
