#include "projectile.hpp"
#include <iostream>
#include "global.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"

#define PI 3.141592653589793238462643383279

Projectile::Projectile( PhysicsObjectParams physicsObjectParams, glm::vec3 targetPosition, bool generateSpread, unsigned int spawnerID ) : PhysicsObject( physicsObjectParams ) {
  
  shipPosition_   = physicsObjectParams.shipPosition;
  targetPosition_ = targetPosition;
  spawnerID_      = spawnerID;
  
  calculateRotation( 1.0f );
  
  mesh_ -> updatePosition( velocity_, 0.01f );
  
  velocity_ = glm::normalize( targetPosition_ - physicsObjectParams.initPosition ) * physicsObjectParams.velMultiplier;
  
  if( physicsObjectParams.damageShip )
    damageShip_ = true;
    
  if( physicsObjectParams.damageEnemy )
    damageEnemy_ = true;
}

void Projectile::update( GLfloat dt, bool skipMove ) {
  PhysicsObject::update( dt, skipMove );
  
  if( mesh_ -> position().z < -70.0f )
    delete_ = true;
}

void Projectile::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

void  Projectile::calculateRotation( GLfloat dt ) {
  
  GLfloat targetX = targetPosition_.x;
  GLfloat targetY = targetPosition_.y;
  GLfloat targetZ = targetPosition_.z;
  
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
  if( damageShip_ )
    collisionProperties.damageToShip = 1;
    
  if( damageEnemy_ )
    collisionProperties.damageToEnemy = 1;
    
  delete_ = true;
  
  return collisionProperties;
}

void Projectile::clean() {
  PhysicsObject::clean();
}

    