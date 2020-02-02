message("-- External Project: glm")
include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY  https://github.com/g-truc/glm.git
	GIT_TAG         0.9.9.7
)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glm)