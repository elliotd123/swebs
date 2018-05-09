cmake_minimum_required (VERSION 3.1)
project (swebs)
file(GLOB swebs_SRC
	"src/*.h"
	"src/*.cpp"
)
add_executable(swebs ${swebs_SRC})
set (CMAKE_CXX_STANDARD 11)

install(TARGETS swebs DESTINATION bin)
