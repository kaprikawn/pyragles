CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project( pyragles )

add_executable( a.out inputHandler.cpp game.cpp main.cpp )

include( FindPkgConfig )

pkg_search_module( SDL2 REQUIRED sdl2 )
pkg_search_module( GLESv2 REQUIRED glesv2 )

include_directories( ${SDL2_INCLUDE_DIRS} ${GLESv2_INCLUDE_DIRS} )
target_link_libraries( a.out ${SDL2_LIBRARIES} ${GLESv2_LIBRARIES} )

set_property( TARGET a.out PROPERTY CXX_STANDARD 11 )

if ( CMAKE_COMPILER_IS_GNUCC )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
endif ( CMAKE_COMPILER_IS_GNUCC )

