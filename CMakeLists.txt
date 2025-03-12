cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
set(OpenGL_GL_PREFERENCE GLVND)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
include_directories(${GLFW3_INCLUDE_DIRS})
link_directories(${GLFW3_LIBRARY_DIRS})
message(STATUS "Found GLFW3: ${GLFW3_INCLUDE_DIRS} ${GLFW3_LIBRARIES}")

include_directories(${CMAKE_SOURCE_DIR}/includes)

file(GLOB_RECURSE SOURCE_FILES
    "src/*.h"
    "src/*.cpp"
    "src/*.c"
    "src/objects/*.h"
    "src/objects/*.cpp"
    "src/core/*.h"
    "src/core/*.cpp"
    "src/manager/*.h"
    "src/manager/*.cpp"
    "src/utils/*.h"
    "src/utils/*.cpp"
)

file(GLOB MAIN_FILE "src/main.cpp")
list(REMOVE_ITEM SOURCE_FILES ${MAIN_FILE})

set(EXECUTABLE_NAME "my-exec")
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES} ${MAIN_FILE})

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
    set(LIBS ${GLFW3_LIBRARIES} X11 Xrandr Xinerama Xi Xcursor GL dl pthread freetype)
elseif(WIN32)
    set(LIBS glfw3 opengl32 assimp freetype irrKlang)
elseif(APPLE)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)
    set(LIBS ${GLFW3_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES})
endif()

target_link_libraries(${EXECUTABLE_NAME} ${LIBS})

add_library(STB_IMAGE "src/stb_image.cpp")
add_library(GLAD "src/glad.c")
add_library(IMGUI
    "includes/imgui/imgui.cpp"
    "includes/imgui/imgui_demo.cpp"
    "includes/imgui/imgui_draw.cpp"
    "includes/imgui/imgui_widgets.cpp"
    "includes/imgui/imgui_tables.cpp"
    "includes/backends/imgui_impl_glfw.cpp"
    "includes/backends/imgui_impl_opengl3.cpp"
)

target_link_libraries(${EXECUTABLE_NAME} STB_IMAGE GLAD IMGUI)

set(BINARY_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR})

file(GLOB SHADERS "shaders/*.vs" "shaders/*.fs")
file(GLOB DLLS "dlls/*.dll")

foreach(SHADER ${SHADERS})
    add_custom_command(TARGET ${EXECUTABLE_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} ${BINARY_OUTPUT_DIR})
endforeach()

foreach(DLL ${DLLS})
    add_custom_command(TARGET ${EXECUTABLE_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLL} ${BINARY_OUTPUT_DIR})
endforeach()


file(GLOB_RECURSE TEST_FILES "tests/*.cpp" "tests/*.h")

set(TEST_EXECUTABLE "my-tests")
add_executable(${TEST_EXECUTABLE} ${TEST_FILES} ${SOURCE_FILES})

target_link_libraries(${TEST_EXECUTABLE} ${LIBS} STB_IMAGE GLAD IMGUI)

set_target_properties(${TEST_EXECUTABLE} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT_DIR}/tests)

enable_testing()
add_test(NAME RunTests COMMAND ${TEST_EXECUTABLE})
