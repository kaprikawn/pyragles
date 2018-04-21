#include "projectile.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592653589793238462643383279

Projectile::Projectile( glm::vec3 initPosition, int shapeType, std::shared_ptr<Renderer> renderer, std::shared_ptr<glm::vec3> shipPosition, std::shared_ptr<Target> target ) : PhysicsObject( initPosition, shapeType, renderer ) {
  
  velocity_ = glm::normalize( target -> position() - initPosition ) * 100.0f;
  
  target_ = target;
  
  calculateRotation( 1.0f );
  
  objectPosition_.updatePosition( velocity_, 0.01f );
  
  model_  = glm::translate( glm::mat4(), objectPosition_.position() );
}

void Projectile::registerCollision() {
  
}

void Projectile::calculateRotation( GLfloat dt ) {
  
  GLfloat targetX = target_ -> position().x;
  GLfloat targetY = target_ -> position().y;
  GLfloat targetZ = target_ -> position().z;
  
  GLfloat projectileX = objectPosition_.x();
  GLfloat projectileY = objectPosition_.y();
  GLfloat projectileZ = objectPosition_.z();
  
  GLfloat yAngle = ( atan( ( targetX - projectileX ) / ( targetZ ) ) * 180 / PI ); 
  
  GLfloat xAngle = ( atan( ( targetY - projectileY ) / ( projectileZ - targetZ ) ) * 180 / PI );
  
  
  rotation_ = glm::rotate( glm::mat4(), glm::radians( xAngle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( yAngle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
}


void Projectile::update( GLfloat dt ) {
  
  if( objectPosition_.z() < -70.0f )
    delete_ = true;
  
  PhysicsObject::update( dt );
  
}

void Projectile::render() {
  
  PhysicsObject::render();
}

void Projectile::clean() {
  
}
