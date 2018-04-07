#include "playState.hpp"
#include <iostream>
#include <vector>
#include "game.hpp"
#include "shader.hpp"

const std::string PlayState::s_playID = "PLAY";

void PlayState::addGlObject( std::shared_ptr<GlObject> glObject ) {
  
  glObjects_.push_back( glObject );
  
}

bool PlayState::onEnter() {
  
  std::unique_ptr<Shader> shader = std::make_unique<Shader>();
  GLuint programID = shader -> init();
  
  if( programID == 0 )
    return false;
    
  glUseProgram( programID );
  
  target_ = std::make_shared<Target>( TARGET, programID );
  hero_   = std::make_shared<Hero>( SHIP, programID, target_ );
  
  PlayState::addGlObject( target_ );
  PlayState::addGlObject( hero_ );
  
  levelStart_ = SDL_GetTicks();
  
  glClearColor( 0.0f, 0.0f, 0.4f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  glViewport( 0, 0, windowWidth, windowHeight );
  
  return true;
}

void PlayState::gameLogic( float dt ) {
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  for( unsigned int i = 0; i < glObjects_.size(); i++ ) {
    glObjects_[i] -> update( dt );
  }
  
}

void PlayState::update( float dt ) {
   
   PlayState::gameLogic( dt );
   
}

void PlayState::render() {
  
  for( unsigned int i = 0; i < glObjects_.size(); i++ ) {
    glObjects_[i] -> render();
  }
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}

bool PlayState::onExit() {
  //delete hero_;
  //delete target_;
  return true;
}
