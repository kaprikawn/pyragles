#include "playState.hpp"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( int levelNumber ) {
  
  viewProjectionMatrix_ = Camera::Instance() -> viewProjectionMatrix();
  
  ship_ = std::make_unique<Ship>();
  bool loadSuccessful = ship_ -> init( "ship.glb" );
  if( !loadSuccessful )
    return false;
  
  floor_  = std::make_unique<Floor>();
  hud_    = std::make_unique<Hud>();
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  ship_ -> update( dt );
  floor_ -> update( dt );
  
  hud_ -> update();
}

void PlayState::render() {
  
  //ship_ -> render( viewProjectionMatrix_ );
  //floor_ -> render( viewProjectionMatrix_ );
  
  hud_ -> render();
}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
