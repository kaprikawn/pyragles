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
  ProjectileManager::updateColliers();
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

void ProjectileManager::updateColliers() {
  colliders_.clear();
  projectilePositions_.clear();
  for( unsigned int i = 0; i < projectiles_.size(); i++ ) {
    if( !projectiles_[ i ] -> active() ) { continue; }
    colliders_.push_back( projectiles_[ i ] -> collider() );
    projectilePositions_.push_back( i );
  }
}

void ProjectileManager::registerCollision( unsigned int colliderIndex ) {
  //std::cout << "registered collision at index " << colliderIndex << std::endl;
  unsigned int projectilesPosition = projectilePositions_[ colliderIndex ];
  projectiles_[ projectilesPosition ] -> registerCollision();
}

ProjectileManager::~ProjectileManager() {
  
}

