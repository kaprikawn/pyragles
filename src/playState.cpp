#include "playState.hpp"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber ) {
  
  camera_ = camera;
  viewProjectionMatrix_ = camera_ -> viewProjectionMatrix();
  
  // ship_ = std::make_unique<Ship>( inputHandler );
  // bool loadSuccessful = ship_ -> init( "ship.glb" );
  // if( !loadSuccessful )
  //   return false;
  
  // floor_ = std::make_unique<Floor>();
  
  particles_ = std::make_unique<Particles>();
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  // ship_ -> update( dt );
  // floor_ -> update( dt );
  particles_ -> update( dt );

}

void PlayState::render() {
  
  // ship_ -> render( viewProjectionMatrix_ );
  // floor_ -> render( viewProjectionMatrix_ );
  
  particles_ -> render( viewProjectionMatrix_ );

}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
