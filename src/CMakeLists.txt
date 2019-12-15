add_executable( exe
  inputHandler.cpp camera.cpp audio.cpp shader.cpp renderer.cpp mesh.cpp meshLoader.cpp movement.cpp
  gameStateMachine.cpp playState.cpp
  collision.cpp gltf.cpp
  physicsObject.cpp ship.cpp scenary.cpp target.cpp floor.cpp projectile.cpp enemy.cpp
  game.cpp main.cpp
)

include( FindPkgConfig )

pkg_search_module( SDL2 REQUIRED sdl2 )
pkg_search_module( SDL2_MIXER REQUIRED SDL2_mixer )
pkg_search_module( GLESv2 REQUIRED glesv2 )

include_directories(
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS}
  ${GLESv2_INCLUDE_DIRS}
)

target_link_libraries( exe
  ${SDL2_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
  ${GLESv2_LIBRARIES}
)

