set(target_name "Shomei")

set(src_files
    "src/matrix_base.hpp"
    "src/vertex_buffer.hpp"
    "src/main.cpp")

add_executable(${target_name} ${src_files})

source_group("src" FILES "src/main.cpp")

target_link_libraries(${target_name} "SDL2-static" "SDL2main" "glm_static")

set_target_properties(${target_name} PROPERTIES FOLDER ${PROJECT_NAME})
set_target_properties(${target_name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )
