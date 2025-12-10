cmake_minimum_required(VERSION 3.16)

project(Soundhouse VERSION 1.0 LANGUAGES C CXX)

# -----------------------------
# C++ standard
# -----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Source files
# -----------------------------
file(GLOB_RECURSE SOUNDHOUSE_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOUNDHOUSE_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# GLFW
add_subdirectory(external/glfw)

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# IMGUI
add_library(imgui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC external/imgui external/glfw/include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link all libraries to your executable
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw imgui ${SDL2_LIBRARIES})

# -----------------------------
# Platform-specific OpenGL
# -----------------------------
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL dl pthread)
endif()

# -----------------------------
# Extra include dirs
# -----------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
    external/glfw/include
    external/imgui
    external/glad/include
)
