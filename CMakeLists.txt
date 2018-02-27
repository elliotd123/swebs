cmake_minimum_required (VERSION 2.6)
project (swebs)
file(GLOB swebs_SRC
	"src/*.h"
	"src/*.cpp"
)
add_executable(swebs ${swebs_SRC})

install(TARGETS swebs DESTINATION bin)
