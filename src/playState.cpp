#include <iostream>
#include <vector>
#include "playState.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter() {
  
  //std::unique_ptr<GlWindow> glWindow_ ( new GlWindow );
  
  glWindow_ = std::make_unique<GlWindow>();
  
  glWindow_ -> init();
  
  levelStart_ = SDL_GetTicks();
  
  return true;
}

void PlayState::gameLogic( float dt ) {

}

void PlayState::update( float dt ) {
   
   glWindow_ -> update( dt );
}

void PlayState::render() {
  glWindow_ -> render();
}

bool PlayState::onExit() {
  
  return true;
}
