#include <iostream>
#include <vector>
#include "playState.hpp"

const std::string PlayState::s_playID = "PLAY";

void PlayState::addGlObject( GlObject* glObject ) {
  //sprite -> setSpriteID( nextSpriteID_ );
  //nextSpriteID_++;
  
  glObjects_.push_back( glObject );
  
}

bool PlayState::onEnter() {
  
  //std::unique_ptr<GlWindow> glWindow_ ( new GlWindow );
  
  //glWindow_ = std::make_unique<GlWindow>();
  
  //glWindow_ -> init();
  
  hero_ = new Hero();
  PlayState::addGlObject( hero_ );
  
  levelStart_ = SDL_GetTicks();
  
  return true;
}

void PlayState::gameLogic( float dt ) {

}

void PlayState::update( float dt ) {
   
   //glWindow_ -> update( dt );
}

void PlayState::render() {
  //glWindow_ -> render();
}

bool PlayState::onExit() {
  delete hero_;
  return true;
}
