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
    
  // ProjectileParams params;
  // params.active = true;
  // params.startingPostion.z = -20.0f;
  
  // bullet_ = std::make_unique<Projectile>( params );
  // bullet_ -> init();
  
  projectiles_ = std::make_unique<ProjectileManager>();
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  floor_ -> update( dt );
  
  if( bombCount_ > 0 ) {
    if( InputHandler::Instance() -> justPressed( BOMB ) ) {
      bombCount_--;
    }
  }
  
  // spawn projectiles
  if( ship_ -> spawnProjectile() ) {
    ProjectileParams params;
    params.startingPostion      = ship_ -> position();
    params.destinationPosition  = ship_ -> targetPosition();
    params.speed                = 30.0f;
    projectiles_ -> spawnProjectile( params );
  }
  
  projectiles_ -> update( dt );
  
  if( enemies_.size() > 0 ) {
    for( unsigned int i = 0; i < enemies_.size(); i++ ) {
      enemies_[ i ] -> update( dt );
    }
  }
  
  ship_ -> update( dt );
  hud_ -> update( dt, bombCount_ );
  
  // check for collisions
  for( unsigned int e = 0; e < enemies_.size(); e++ ) {
    
    std::vector<glm::vec4> collider = enemies_[ e ] -> collider();
    
    Collision myCollision( ship_ -> collider(), enemies_[ e ] -> collider() );
    //std::cout << "are colliding is " << myCollision.areColliding() << std::endl;
  }
  
  // bullet_ -> update( dt );
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
  projectiles_ -> render( viewProjectionMatrix_ );
  // bullet_ -> render( viewProjectionMatrix_ );
}

int PlayState::nextLevel() {
  return nextLevel_;
}

bool PlayState::onExit() {
  
  return true;
}
