#include "enemy.hpp"
#include <iostream>
#include "global.hpp"

Enemy::Enemy( PhysicsObjectParams physicsObjectParams ) : PhysicsObject( physicsObjectParams ) {
  velocity_.z = SCROLL_SPEED;
}

void Enemy::update( GLfloat dt, bool skipMove ) {
  PhysicsObject::update( dt, skipMove );
}

void Enemy::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

void Enemy::clean() {
  PhysicsObject::clean();
}

void Enemy::calculateRotation( GLfloat dt ) {
  
}

void Enemy::registerCollision( CollisionData collisionData, CollisionProperties collisionProperties ) {
  
}

CollisionProperties Enemy::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}