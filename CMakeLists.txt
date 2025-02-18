cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Utiliser pkg-config pour trouver GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

# Ajouter les répertoires d'inclusion et de bibliothèque de GLFW
include_directories(${GLFW3_INCLUDE_DIRS})
link_directories(${GLFW3_LIBRARY_DIRS})

# Afficher les informations de GLFW
message(STATUS "Found GLFW3: ${GLFW3_INCLUDE_DIRS} ${GLFW3_LIBRARIES}")

# Définir le nom de l'exécutable
set(NAME "my-exec")

# Ajouter les fichiers sources
file(GLOB SOURCE
    "src/*.h"
    "src/*.cpp"
    "src/shaders/*.vs"
    "src/shaders/*.fs"
    "src/objects/*"
)

# Crée l'exécutable
add_executable(${NAME} ${SOURCE})

# Ajouter GLFW aux bibliothèques à lier
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

# Lier les bibliothèques au target
target_link_libraries(${NAME} ${LIBS})

# Crée les bibliothèques statiques nécessaires (par exemple STB_IMAGE, GLAD, IMGUI)
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set(LIBS ${LIBS} GLAD)

add_library(IMGUI
    "includes/imgui/imgui.cpp"
    "includes/imgui/imgui_demo.cpp"
    "includes/imgui/imgui_draw.cpp"
    "includes/imgui/imgui_widgets.cpp"
    "includes/imgui/imgui_tables.cpp"
    "includes/backends/imgui_impl_glfw.cpp"
    "includes/backends/imgui_impl_opengl3.cpp"
)
set(LIBS ${LIBS} IMGUI)

# Lier les bibliothèques supplémentaires au projet
target_link_libraries(${NAME} ${LIBS})

# Spécifier le répertoire de sortie
if(WIN32)
  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
elseif(UNIX AND NOT APPLE)
  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
elseif(APPLE)
  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
  set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")
endif(WIN32)

# Copier les shaders et DLLs dans le répertoire de sortie
file(GLOB SHADERS "src/*.vs" "src/*.fs")
file(GLOB DLLS "dlls/*.dll")

foreach(SHADER ${SHADERS})
    if(WIN32)
        add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
        add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
    elseif(UNIX AND NOT APPLE)
        file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${chapter})
    elseif(APPLE)
        get_filename_component(SHADERNAME ${SHADER} NAME)
        add_custom_command(TARGET ${NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${SHADER} ${CMAKE_SOURCE_DIR}/bin/${chapter}/${SHADERNAME} DEPENDS ${SHADER})
    endif(WIN32)
endforeach(SHADER)

include_directories(${CMAKE_SOURCE_DIR}/includes)