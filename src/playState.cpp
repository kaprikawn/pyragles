#include "playState.hpp"
#include <iostream>

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber ) {
  
  camera_ = camera;
  viewProjectionMatrix_ = camera_ -> viewProjectionMatrix();
  
  box_ = std::make_unique<GameObject>();
  box_ -> init( "numbox.glb", camera_ );
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  

}

void PlayState::render() {
  
  box_ -> render( viewProjectionMatrix_ );

}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
