#include "projectileManager.hpp"
#include <iostream>

ProjectileManager::ProjectileManager() {
  
  for( unsigned int i = 0; i < 20; i++ ) {
    ProjectileParams myProjectileParams;
    std::unique_ptr<Projectile> myProjectile = std::make_unique<Projectile>( myProjectileParams );
    myProjectile -> init();
    projectiles_.push_back( std::move( myProjectile ) );
  }
}

void ProjectileManager::update( float dt ) {
  for( unsigned int i = 0; i < projectiles_.size(); i++ ) {
    projectiles_[ i ] -> update( dt );
  }
}

void ProjectileManager::render( glm::mat4 viewProjectionMatrix ) {
  for( unsigned int i = 0; i < projectiles_.size(); i++ ) {
    projectiles_[ i ] -> render( viewProjectionMatrix );
  }
}

void ProjectileManager::spawnProjectile( ProjectileParams projectileParams ) {
  for( unsigned int i = 0; i < projectiles_.size(); i++ ) {
    if( !projectiles_[ i ] -> active() ) {
      projectiles_[ i ] -> activate( projectileParams );
      break;
    }
  }
}


ProjectileManager::~ProjectileManager() {
  
}

