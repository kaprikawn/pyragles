#include "scenary.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

Scenary::Scenary( PhysicsObjectParams physicsObjectParams ) : PhysicsObject( physicsObjectParams ) {
  
  velocity_.z = SCROLL_SPEED;
}
    
void Scenary::update( GLfloat dt, bool skipMove ) {
  
  PhysicsObject::update( dt, skipMove );
}

void Scenary::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

CollisionProperties Scenary::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  collisionProperties.damageToShip = 10;
  
  return collisionProperties;
}

void Scenary::registerCollision( CollisionData collisionData, CollisionProperties collisionProperties ) {
  
  if( lastCollisionID_ == collisionProperties.objectID )
    return;
  
  lastCollisionID_ = collisionProperties.objectID;
  
  
  
}

void Scenary::clean() {
  PhysicsObject::clean();
}