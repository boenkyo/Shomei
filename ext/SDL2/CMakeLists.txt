add_library(sdl2 "lib/x64/SDL2.lib")
target_include_directories(sdl2 PUBLIC "include/")
set_target_properties(sdl2 PROPERTIES LINKER_LANGUAGE C)