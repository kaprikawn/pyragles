#include "playState.hpp"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, int levelNumber ) {
  
  viewProjectionMatrix_ = Camera::Instance() -> viewProjectionMatrix();
  
  ship_ = std::make_unique<Ship>( inputHandler );
  bool loadSuccessful = ship_ -> init( "ship.glb" );
  if( !loadSuccessful )
    return false;
  
  floor_ = std::make_unique<Floor>();
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  ship_ -> update( dt );
  floor_ -> update( dt );
  

}

void PlayState::render() {
  
  ship_ -> render( viewProjectionMatrix_ );
  floor_ -> render( viewProjectionMatrix_ );
  
}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
