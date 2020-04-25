#include "projectile.hpp"
#include <iostream>

Projectile::Projectile( ProjectileParams params ) {
  
}

bool Projectile::init() {
  bool loaded = GameObject::loadModelFromGltf( "projectile.glb", "shaderBasic.glsl" );
  return loaded;
}

void Projectile::update( float dt ) {
  
  if( position_.z > 65.0f )
    active_ = false;
    
  if( !active_ )
    return;
  
  
  GameObject::update( dt );
  
}

void Projectile::render( glm::mat4 viewProjectionMatrix ) {
  if( !active_ )
    return;
  
  GameObject::render( viewProjectionMatrix );
}

void Projectile::activate( ProjectileParams params ) {
  
  position_ = params.startingPostion;
  glm::vec4 velocity1 = params.destinationPosition - position_;
  glm::vec3 velocity = glm::normalize( glm::vec3( velocity1.x, velocity1.y, velocity1.z ) );
  
  velocity_ = glm::vec4( velocity, 1.0f );
  velocity_ *= params.speed;
  
  active_ = true;
}

Projectile::~Projectile() {
  
}

