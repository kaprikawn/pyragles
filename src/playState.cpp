#include <iostream>
#include <vector>
#include "playState.hpp"
#include "game.hpp"

const std::string PlayState::s_playID = "PLAY";

void PlayState::addGlObject( GlObject* glObject ) {
  //sprite -> setSpriteID( nextSpriteID_ );
  //nextSpriteID_++;
  
  glObjects_.push_back( glObject );
  
}

bool PlayState::onEnter() {
  
  hero_ = new Hero();
  PlayState::addGlObject( hero_ );
  
  levelStart_ = SDL_GetTicks();
  
  return true;
}

void PlayState::gameLogic( float dt ) {
  for( unsigned int i = 0; i < glObjects_.size(); i++ ) {
    glObjects_[i] -> update( dt );
  }
}

void PlayState::update( float dt ) {
   
   PlayState::gameLogic( dt );
}

void PlayState::render() {
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}

bool PlayState::onExit() {
  delete hero_;
  return true;
}
