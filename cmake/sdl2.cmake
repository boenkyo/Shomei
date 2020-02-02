message("-- External Project: SDL2")

include(FetchContent)

FetchContent_Declare(
    sdl2
    HG_REPOSITORY https://hg.libsdl.org/SDL
    HG_TAG        release-2.0.10
)

FetchContent_MakeAvailable(sdl2)