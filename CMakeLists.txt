cmake_minimum_required(VERSION 3.0.0)
project(euclid)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

file(GLOB SOURCES
	"${PROJECT_SOURCE_DIR}/src/*.c"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.hpp"
	"${PROJECT_SOURCE_DIR}/ext/src/*.c"
	"${PROJECT_SOURCE_DIR}/ext/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/ext/inc/*.h"
	"${PROJECT_SOURCE_DIR}/ext/inc/*.hpp"
)

add_executable(euclid ${SOURCES})

target_include_directories(euclid PRIVATE src ext/inc)
target_link_directories(euclid PRIVATE ext/lib)
target_link_libraries(euclid PRIVATE libglfw3.a)
target_link_libraries(euclid PRIVATE OpenGL::GL)
