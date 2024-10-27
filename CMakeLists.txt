cmake_minimum_required(VERSION 3.0.0)
project(euclid)

set(CMAKE_CXX_STANDARD 20)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/*.hpp
	${PROJECT_SOURCE_DIR}/ext/src/*.c
	${PROJECT_SOURCE_DIR}/ext/src/*.cpp
	${PROJECT_SOURCE_DIR}/ext/inc/*.h
	${PROJECT_SOURCE_DIR}/ext/inc/*.hpp
)

add_executable(euclid ${SOURCES})

target_include_directories(euclid PRIVATE src ext/inc)
target_link_directories(euclid PRIVATE ext/lib)
if(WIN32)
	target_link_libraries(euclid PRIVATE libglfw3.a)
elseif(UNIX)
	target_link_libraries(euclid PRIVATE glfw)
endif()
target_link_libraries(euclid PRIVATE OpenGL::GL)

add_compile_definitions(GLFW_INCLUDE_NONE)
