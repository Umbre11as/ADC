project(Gui LANGUAGES CXX)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
list(APPEND IMGUI_SOURCES
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Gui ${IMGUI_SOURCES} ${SOURCES})

target_include_directories(Gui PRIVATE ${imgui_SOURCE_DIR})
target_link_libraries(Gui PRIVATE glfw opengl32)
