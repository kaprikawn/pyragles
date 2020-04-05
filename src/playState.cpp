#include "playState.hpp"
#include <iostream>
#include "inputHandler.hpp"
#include "collision.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( int levelNumber ) {
  
  viewProjectionMatrix_ = Camera::Instance() -> viewProjectionMatrix();
  bool loadSuccessful;
  
  
  floor_  = std::make_unique<Floor>();
  hud_    = std::make_unique<Hud>();
  
  std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
  loadSuccessful = enemy -> init( "enemyPod.glb" );
  if( !loadSuccessful )
    return false;
  
  enemies_.push_back( std::move( enemy ) );
  
  ship_ = std::make_unique<Ship>();
  loadSuccessful = ship_ -> init( "ship.glb" );
  if( !loadSuccessful )
    return false;
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
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
  
  ship_ -> update( dt );
  hud_ -> update( dt, bombCount_ );
  
  // check for collisions
  for( unsigned int e = 0; e < enemies_.size(); e++ ) {
    
    std::vector<glm::vec3>  collider = enemies_[ e ] -> collider();
    
    Collision myCollision( ship_ -> collider(), enemies_[ e ] -> collider() );
    std::cout << "are colliding is " << myCollision.areColliding() << std::endl;
  }
}

void PlayState::render() {
  
  if( enemies_.size() > 0 ) {
    for( unsigned int i = 0; i < enemies_.size(); i++ ) {
      enemies_[ i ] -> render( viewProjectionMatrix_ );
    }
  }
  
  ship_ -> render( viewProjectionMatrix_ );
  
  floor_ -> render( viewProjectionMatrix_ );
  hud_ -> render();
}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
