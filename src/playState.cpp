#include "playState.hpp"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber ) {
  
  camera_ = camera;
  viewProjectionMatrix_ = camera_ -> viewProjectionMatrix();
  
  ship_ = std::make_unique<Ship>();
  bool loadSuccessful = ship_ -> init( "ship.glb", camera_ );
  if( !loadSuccessful )
    return false;
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  ship_ -> update( dt );

}

void PlayState::render() {
  
  ship_ -> render( viewProjectionMatrix_ );

}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
