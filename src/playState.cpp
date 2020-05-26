#include "playState.hpp"
#include <iostream>
#include "rendering/renderable.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( int levelNumber ) {
  
  shipPhysics_.setVelocity( 1.0f, 0.0f, 0.0f );
  ship_.addComponent( &shipPhysics_ );
  
  if( ! ship_.initalize() ) {
    return false;
  }
  
  return true;
}

void PlayState::update( GLfloat dt ) {

  ship_.update( dt );
  
}

void PlayState::render() {
  
}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
