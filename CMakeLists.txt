cmake_minimum_required(VERSION 3.5.0)
set(PROJECT_NAME "Shomei")
project(${PROJECT_NAME} C CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
message("-- CONFIGURATION TYPES: ${CMAKE_CONFIGURATION_TYPES}")

include("cmake/compiler_features.cmake")
include("cmake/compiler_warnings.cmake")
include("cmake/cotire.cmake")
include("cmake/glfw.cmake")
include("cmake/sdl2.cmake")
include("cmake/glm.cmake")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")
set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("thirdparty")
add_subdirectory("app")