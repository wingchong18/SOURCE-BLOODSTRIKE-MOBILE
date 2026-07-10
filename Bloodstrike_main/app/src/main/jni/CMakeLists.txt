cmake_minimum_required(VERSION 3.10.2)
project(StarcoolKernel)

# Set standar C++ ke versi 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Folder Include
include_directories(
    .
    Android_draw
    Android_touch
    Android_vulkan
    ImGui
    ImGui/backends
    ImGui/font
    native_surface
    My_Utils
    My_Utils/res
    Oxorany
    Font
)

add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -fexceptions -Wno-format-security -fno-stack-protector")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")

file(GLOB SRC_MAIN "*.cpp" "*.c" "*.cc")
file(GLOB SRC_DRAW "Android_draw/*.cpp" "Android_draw/*.c" "Android_draw/*.cc")
file(GLOB SRC_OXORANY "Oxorany/*.cpp" "Oxorany/*.c" "Oxorany/*.cc")
file(GLOB SRC_TOUCH "Android_touch/*.cpp" "Android_touch/*.c" "Android_touch/*.cc")
file(GLOB SRC_UTILS "My_Utils/*.cpp" "My_Utils/*.c" "My_Utils/*.cc" "My_Utils/res/*.c")
file(GLOB SRC_IMGUI "ImGui/*.cpp" "ImGui/*.c" "ImGui/*.cc")
file(GLOB SRC_IMGUI_BACKENDS "ImGui/backends/*.cpp" "ImGui/backends/*.c" "ImGui/backends/*.cc")
file(GLOB SRC_VULKAN "Android_vulkan/*.cpp" "Android_vulkan/*.c" "Android_vulkan/*.cc")

set(SRC_FILES
    ${SRC_MAIN}
    ${SRC_DRAW}
    ${SRC_OXORANY}
    ${SRC_TOUCH}
    ${SRC_UTILS}
    ${SRC_IMGUI}
    ${SRC_IMGUI_BACKENDS}
    ${SRC_VULKAN}
)

add_executable(Starcool.sh ${SRC_FILES})

target_link_libraries(Starcool.sh
    log
    z
    android
    GLESv2
    EGL
)
