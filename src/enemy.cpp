#include "enemy.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

Enemy::Enemy( PhysicsObjectParams physicsObjectParams ) : PhysicsObject( physicsObjectParams ) {
  //velocity_.z = SCROLL_SPEED;
  velocity_.x = -4;
}

void Enemy::update( GLfloat dt, bool skipMove ) {
  
  calculateRotation( dt );
  
  PhysicsObject::update( dt, skipMove );
}

void Enemy::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

void Enemy::clean() {
  PhysicsObject::clean();
}

void Enemy::calculateRotation( GLfloat dt ) {
  
  yAngle_ += dt * 100;
  
  rotationMatrix_ = glm::rotate( glm::mat4(), glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

void Enemy::registerCollision( CollisionData collisionData, CollisionProperties collisionProperties ) {
  
}

CollisionProperties Enemy::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}