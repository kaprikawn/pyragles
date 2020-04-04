#include "playState.hpp"
#include <iostream>
#include "inputHandler.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( int levelNumber ) {
  
  viewProjectionMatrix_ = Camera::Instance() -> viewProjectionMatrix();
  bool loadSuccessful;
  
  ship_ = std::make_unique<Ship>();
  loadSuccessful = ship_ -> init( "ship.glb" );
  if( !loadSuccessful )
    return false;
  
  floor_  = std::make_unique<Floor>();
  hud_    = std::make_unique<Hud>();
  
  std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
  loadSuccessful = enemy -> init( "enemyPod.glb" );
  
  enemies_.push_back( std::move( enemy ) );
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  ship_ -> update( dt );
  floor_ -> update( dt );
  
  if( bombCount_ > 0 ) {
    if( InputHandler::Instance() -> justPressed( BOMB ) ) {
      bombCount_--;
    }
  }
  
  if( enemies_.size() > 0 ) {
    for( unsigned int i = 0; i < enemies_.size(); i++ ) {
      enemies_[ i ] -> update( dt );
    }
  }
  
  hud_ -> update( dt, bombCount_ );
}

void PlayState::render() {
  
  ship_ -> render( viewProjectionMatrix_ );
  
  if( enemies_.size() > 0 ) {
    for( unsigned int i = 0; i < enemies_.size(); i++ ) {
      enemies_[ i ] -> render( viewProjectionMatrix_ );
    }
  }
  
  floor_ -> render( viewProjectionMatrix_ );
  hud_ -> render();
}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
