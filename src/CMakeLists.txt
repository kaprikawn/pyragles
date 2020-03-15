add_executable( ${PROJECT_NAME}
  inputHandler.cpp camera.cpp glCallLog.cpp stb_image.cpp
  texture.cpp indexBuffer.cpp vertexBuffer.cpp shaderCache.cpp shader.cpp
  gameObject.cpp
  gameStateMachine.cpp playState.cpp
  model.cpp
  ship.cpp floor.cpp exhaust.cpp
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

target_link_libraries( ${PROJECT_NAME}
  ${SDL2_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
  ${GLESv2_LIBRARIES}
)

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/src/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
                  )
                  