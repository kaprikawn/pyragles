#include "enemy.hpp"
#include <iostream>
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "global.hpp"

Enemy::Enemy( PhysicsObjectParams physicsObjectParams ) : PhysicsObject( physicsObjectParams ) {
  
  NextMovement nextMovement;
  nextMovement.y = 4;
  
  movements_.push_back( nextMovement );
  
  nextMovement.x = -4;
  nextMovement.y = 0;
  
  movements_.push_back( nextMovement );
  
  
  velocity_.x = movements_.back().x;
  
  movementTimer_.setCountdownTimer( movements_.back().duration );
}

void Enemy::update( GLfloat dt, bool skipMove ) {
  
  movementTimer_.update( dt );
  if( movementTimer_.timeLeft() == 0.0f ) {
    movements_.pop_back();
    movementTimer_.setCountdownTimer( movements_.back().duration );
  }
  
  if( velocity_.x > movements_.back().x ) {
    velocity_.x -= dt * 10;
  }
  if( velocity_.y < movements_.back().y ) {
    velocity_.y += dt * 10;
  }
  
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