#include "enemy.hpp"
#include <iostream>
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "global.hpp"

Enemy::Enemy( PhysicsObjectParams physicsObjectParams ) : PhysicsObject( physicsObjectParams ) {
  
  // declare movements in reverse order so the next movement is
  // always at the back for convenient access via .back()
  NextMovement nextMovement50;
  nextMovement50.y        = 10;
  nextMovement50.vec3.y   = 10;
  nextMovement50.duration = 50.0f;
  movements_.push_back( nextMovement50 );
  
  NextMovement nextMovement49;
  nextMovement49.x        = -10;
  nextMovement49.vec3.x   = -10;
  movements_.push_back( nextMovement49 );
  
  Timer fireTimer50;
  fireTimer50.setCountdownTimer( 50 );
  fireTimers_.push_back( fireTimer50 );
  
  Timer fireTimer49;
  fireTimer49.setCountdownTimer( 3 );
  fireTimers_.push_back( fireTimer49 );
  
  velocity_.x = movements_.back().x;
  
  glm::vec3 currentVelocity = { movements_.back().x, movements_.back().y, movements_.back().z };
  glm::vec3 targetVelocity  = { movements_.back().x, movements_.back().y, movements_.back().z };
  glm::vec3 changeMagnitude = { 1.0f, 1.0f, 1.0f };
  
  movement_.defineChange( currentVelocity, targetVelocity, changeMagnitude );
  
  // set duration of this movement until next change in direction
  movementTimer_.setCountdownTimer( movements_.back().duration );
}

void Enemy::update( GLfloat dt, bool skipMove ) {
  
  movementTimer_.update( dt );
  
  if( movementTimer_.timeLeft() == 0.0f ) {
    
    glm::vec3 currentVelocity = { movements_.back().x, movements_.back().y, movements_.back().z };
    movements_.pop_back();
    glm::vec3 targetVelocity = { movements_.back().x, movements_.back().y, movements_.back().z };
    glm::vec3 changeMagnitude = { 1.0f, 1.0f, 1.0f };
    movement_.defineChange( currentVelocity, targetVelocity, changeMagnitude );
    
    movementTimer_.setCountdownTimer( movements_.back().duration );
  }
  
  fireTimers_.back().update( dt );
  if( fireTimers_.back().timeLeft() == 0.0f ) {
    fire_ = true;
    fireTimers_.pop_back();
  }
  
  glm::vec3 currentVelocity = { velocity_.x, velocity_.y, velocity_.z };
  glm::vec3 newVelocity = movement_.changeVelocity( currentVelocity, dt );
  
  velocity_.x = newVelocity.x;
  velocity_.y = newVelocity.y;
  velocity_.z = newVelocity.z;
  
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
  
  if( lastCollisionID_ == collisionProperties.objectID )
    return;
  
  lastCollisionID_ = collisionProperties.objectID;
  
  hitpoints -= collisionProperties.damageToEnemy;
  
  if( hitpoints <= 0 )
    objectState_ = DEAD;
}

CollisionProperties Enemy::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}
