CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project( pyragles )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ( CMAKE_COMPILER_IS_GNUCC )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
endif ( CMAKE_COMPILER_IS_GNUCC )

add_executable( exe shader.cpp position.cpp glBuffer.cpp glObject.cpp target.cpp hero.cpp shape.cpp camera.cpp gameStateMachine.cpp playState.cpp inputHandler.cpp game.cpp main.cpp )

include( FindPkgConfig )

pkg_search_module( SDL2 REQUIRED sdl2 )
pkg_search_module( GLESv2 REQUIRED glesv2 )

include_directories( ${SDL2_INCLUDE_DIRS} ${GLESv2_INCLUDE_DIRS} )
target_link_libraries( exe ${SDL2_LIBRARIES} ${GLESv2_LIBRARIES} )

