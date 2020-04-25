#include "projectile.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592653589793238462643383279

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

void Projectile::calculateRotation() {
  
  GLfloat targetX = destinationPosition_.x;
  GLfloat targetY = destinationPosition_.y;
  GLfloat targetZ = destinationPosition_.z;
  
  GLfloat positionX = startingPosition_.x;
  GLfloat positionY = startingPosition_.y;
  GLfloat positionZ = startingPosition_.z;
  
  GLfloat yAngle = ( atan( ( targetX - positionX ) / ( targetZ ) ) * 180 / PI ); 
  
  GLfloat xAngle = ( atan( ( targetY - positionY ) / ( positionZ - targetZ ) ) * 180 / PI );
  
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( xAngle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_  , glm::radians( yAngle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void Projectile::activate( ProjectileParams params ) {
  
  position_             = params.startingPostion;
  startingPosition_     = position_;
  destinationPosition_  = params.destinationPosition;
  glm::vec4 velocity1   = destinationPosition_ - startingPosition_;
  glm::vec3 velocity    = glm::normalize( glm::vec3( velocity1.x, velocity1.y, velocity1.z ) );
  
  velocity_ = glm::vec4( velocity, 1.0f );
  velocity_ *= params.speed;
  
  Projectile::calculateRotation();
  
  active_ = true;
}

Projectile::~Projectile() {
  
}

