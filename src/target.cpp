#include "target.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

Target::Target( PhysicsObjectParams physicsObjectParams ) : PhysicsObject( physicsObjectParams ) {
  
  inputHandler_ = physicsObjectParams.inputHandler;
  shipPosition_ = physicsObjectParams.shipPosition;
}

void Target::handleInput( GLfloat dt ) {
  
  bool invertY = true;
  
  GLfloat multiplier  = 100.0f;
  GLfloat resetSpeed  = 20.0f;
  
  GLfloat joyAxisX    = inputHandler_ -> joyAxisX();
  acceleration_.x     = joyAxisX * multiplier;
  
  velocity_.x += acceleration_.x * dt;
  
  GLfloat xMax = 20.0f;
  if( velocity_.x > xMax )
    velocity_.x = xMax;
  if( velocity_.x < -xMax )
    velocity_.x = -xMax;
    
  if( joyAxisX == 0.0f ) { // drift back to ship
    
    GLfloat shipX   = shipPosition_ -> x;
    GLfloat targetX = mesh_ -> x();
    
    if( targetX > shipX ) {
      mesh_ -> setX ( targetX - resetSpeed * dt );
      if( mesh_ -> x() < shipX )
        mesh_ -> setX ( shipX );
    } else if( targetX < shipX ) {
      mesh_ -> setX ( targetX + resetSpeed * dt );
      if( mesh_ -> x() > shipX )
        mesh_ -> setX ( shipX );
    }
    velocity_.x = 0.0f;
  }
  
  GLfloat joyaxisY    = inputHandler_ -> joyAxisY( invertY );
  acceleration_.y     = joyaxisY * multiplier;
  
  velocity_.y += acceleration_.y * dt;
  
  GLfloat yMax = 20.0f;
  if( velocity_.y > yMax )
    velocity_.y = yMax;
  if( velocity_.y < -yMax )
    velocity_.y = -yMax;
  
  if( joyaxisY == 0.0f ) { // drift back to ship
    
    GLfloat shipY   = shipPosition_ -> y;
    GLfloat targetY = mesh_ -> y();
    
    if( targetY > shipY ) {
      mesh_ -> setY ( targetY - resetSpeed * dt );
      if( mesh_ -> y() < shipY )
        mesh_ -> setY ( shipY );
    } else if( targetY < shipY ) {
      mesh_ -> setY ( targetY + resetSpeed * dt );
      if( mesh_ -> y() > shipY )
        mesh_ -> setY ( shipY );
    }
    velocity_.y = 0.0f;
  }
  
}

void Target::update( GLfloat dt, bool skipMove ) {
  
  Target::handleInput( dt );
  
  GLfloat maxDist = 10.0f;
  
  GLfloat targetX = mesh_ -> x();
  GLfloat targetY = mesh_ -> y();
  GLfloat shipX   = shipPosition_ -> x;
  GLfloat shipY   = shipPosition_ -> y;
  
  if( targetX > shipX + maxDist ) {
    mesh_ -> setX ( shipX + maxDist );
  } else if( targetX < shipX - maxDist ) {
    mesh_ -> setX( shipX - maxDist );
  }
  
  if( targetY > shipY + maxDist ) {
    mesh_ -> setY ( shipY + maxDist );
  } else if( targetY < shipY - maxDist ) {
    mesh_ -> setY( shipY - maxDist );
  }
  
  if( mesh_ -> y() < FLOOR_Y + 1.3f ) {
    mesh_ -> setY ( FLOOR_Y + 1.3f );
  }
  PhysicsObject::update( dt, skipMove );
}

void Target::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

CollisionProperties Target::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}

void Target::clean() {
  
}
