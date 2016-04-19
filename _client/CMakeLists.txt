cmake_minimum_required(VERSION 3.0)
project(strategy_game_client)

file(GLOB_RECURSE sources 
	../sources/client/*.cpp 
	../sources/client/*.h 
	../sources/game/*.cpp 
	../sources/game/*.h
	${CMAKE_SOURCE_DIR}/dependency/json/*.*
)

include_directories(
	${CMAKE_SOURCE_DIR}/dependency/irrlicht/include
	${CMAKE_SOURCE_DIR}/dependency/boost
	${CMAKE_SOURCE_DIR}/dependency/json
	${CMAKE_SOURCE_DIR}/sources/client
	${CMAKE_SOURCE_DIR}/sources/game
)

link_directories(
	${CMAKE_SOURCE_DIR}/dependency/irrlicht/lib
)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Od /ZI")

add_executable(game_client ${sources} ${data})

target_link_libraries(game_client PUBLIC debug Irrlicht_d optimized Irrlicht)

target_compile_definitions(game_client PUBLIC _IRR_STATIC_LIB_ NO_IRR_COMPILE_WITH_DIRECT3D_9_ CLIENT)