project(Challenges)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(WDK REQUIRED)

include(FetchContent)
FetchContent_Declare(kCaveHook
        GIT_REPOSITORY https://github.com/Umbre11as/kCaveHook.git
        GIT_TAG        c # Version for C language
)
FetchContent_MakeAvailable(kCaveHook)

add_subdirectory(create-device)
