#include "projectile.hpp"
#include <iostream>
#include "global.hpp"
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592653589793238462643383279

Projectile::Projectile( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> renderer, std::shared_ptr<glm::vec3> shipPosition, std::shared_ptr<Target> target )
  : PhysicsObject( initPosition, bufferData, mesh, renderer ) {
  
  shipPosition_ = shipPosition;
  target_       = target;
  
  calculateRotation( 1.0f );
  
  mesh_ -> updatePosition( velocity_, 0.01f );
  
  velocity_ = glm::normalize( target -> position() - initPosition ) * 100.0f;
}

void Projectile::update( GLfloat dt, bool skipMove ) {
  PhysicsObject::update( dt, skipMove );
}

void Projectile::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

void  Projectile::calculateRotation( GLfloat dt ) {
  
  GLfloat targetX = target_ -> position().x;
  GLfloat targetY = target_ -> position().y;
  GLfloat targetZ = target_ -> position().z;
  
  GLfloat projectileX = mesh_ -> x();
  GLfloat projectileY = mesh_ -> y();
  GLfloat projectileZ = mesh_ -> z();
  
  GLfloat yAngle = ( atan( ( targetX - projectileX ) / ( targetZ ) ) * 180 / PI ); 
  
  GLfloat xAngle = ( atan( ( targetY - projectileY ) / ( projectileZ - targetZ ) ) * 180 / PI );
  
  
  rotationMatrix_ = glm::rotate( glm::mat4(), glm::radians( xAngle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_, glm::radians( yAngle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

CollisionProperties Projectile::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}

void Projectile::clean() {
  PhysicsObject::clean();
}

    