cmake_minimum_required(VERSION 3.0)
project(strategy_game_server)

file(GLOB_RECURSE sources ../sources/server/*.cpp ../sources/server/*.h ../sources/game/*.cpp ../sources/game/*.h)

include_directories(
	${CMAKE_SOURCE_DIR}/dependency/irrlicht/include
	${CMAKE_SOURCE_DIR}/dependency/boost
	${CMAKE_SOURCE_DIR}/sources/server
	${CMAKE_SOURCE_DIR}/sources/game
)

link_directories(
	${CMAKE_SOURCE_DIR}/dependency/irrlicht/lib
)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

add_executable(game_server ${sources} ${data})

target_link_libraries(game_server PUBLIC debug Irrlicht_d optimized Irrlicht)

target_compile_definitions(game_server PUBLIC _IRR_STATIC_LIB_ NO_IRR_COMPILE_WITH_DIRECT3D_9_ SERVER)