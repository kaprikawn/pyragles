#include <iostream>
#include <cmath>
#include "target.hpp"
#include "inputHandler.hpp"

Target::Target( int shapeType, GLuint programID, std::shared_ptr<glm::vec3> heroPosition ) : GlObject( shapeType, 0.0f, 0.0f, -15.0f, programID ) {
  
  heroPosition_ = heroPosition;
  
  rotation_ = glm::mat4();
}

void Target::handleInput( float dt ) {
  
  joyAxisX_ = TheInputHandler::Instance() -> joyAxisX();
  joyAxisY_ = TheInputHandler::Instance() -> joyAxisY();
  
  if( joyAxisX_ == 0.0f ) {
    if( TheInputHandler::Instance() -> isPressed( RIGHT ) ) {
      joyAxisX_ = 1.0f;
    } else if( TheInputHandler::Instance() -> isPressed( LEFT ) ) {
      joyAxisX_ = -1.0f;
    }
  }
  
  if( joyAxisY_ == 0.0f ) {
    if( TheInputHandler::Instance() -> isPressed( RISE ) ) {
      joyAxisY_ = 1.0f;
    } else if( TheInputHandler::Instance() -> isPressed( FALL ) ) {
      joyAxisY_ = -1.0f;
    }
  }
  
  if( invertY_ ) {
    joyAxisY_ = -joyAxisY_;
  }
  
  GLfloat speedFactor = 30.0f;
  
  GLfloat maxDistFromShipX_ = 10.0f;
  GLfloat maxDistFromShipY_ = 5.0f;
  GLfloat currentX          = position_.coordinates().x;
  GLfloat shipX             = heroPosition_ -> x;
  GLfloat destinationX_     = shipX + joyAxisX_ * maxDistFromShipX_;
  GLfloat currentY          = position_.coordinates().y;
  GLfloat shipY             = heroPosition_ -> y;
  GLfloat destinationY_     = shipY + joyAxisY_ * maxDistFromShipY_;
  
  GLfloat x = currentX;
  GLfloat y = currentY;
  
  if( joyAxisX_ > 0.0f ) {
    if( x < destinationX_ )
      x += speedFactor * dt;
    
    if( x > shipX + joyAxisX_ * maxDistFromShipX_ * 0.9f )
      x = shipX + joyAxisX_ * maxDistFromShipX_ * 0.9f;
    
  } else if( joyAxisX_ < 0.0f ) {
    if( x > destinationX_ )
      x -= speedFactor * dt;
    
    if( x < shipX + joyAxisX_ * maxDistFromShipX_ * 0.9f )
      x = shipX + joyAxisX_ * maxDistFromShipX_ * 0.9f;
      
  } else {
    if( destinationX_ > currentX ) {
      x += speedFactor * dt;
      if( x > shipX )
        x = shipX;
    } else if( destinationX_ < currentX ) {
      x -= speedFactor * dt;
      if( x < shipX )
        x = shipX;
    }
  }
  
  if( joyAxisY_ > 0.0f ) { // rising
    if( y < destinationY_ )
      y += speedFactor * dt;
    
    if( y > shipY + joyAxisY_ * maxDistFromShipY_ * 0.9f )
      y = shipY + joyAxisY_ * maxDistFromShipY_ * 0.9f;
    
  } else if( joyAxisY_ < 0.0f ) { // falling
    if( y > destinationY_ )
      y -= speedFactor * dt;
    
    if( y < shipY - -joyAxisY_ * maxDistFromShipY_ * 0.9f )
      y = shipY - -joyAxisY_ * maxDistFromShipY_ * 0.9f;
      
  } else {
    if( destinationY_ > currentY ) {
      y += speedFactor * dt;
      if( y > shipY )
        y = shipY;
    } else if( destinationY_ < currentY ) {
      y -= speedFactor * dt;
      if( y < shipY )
        y = shipY;
    }
  }
  
  position_.setCoordinates( x, y, heroPosition_ -> z - 15.0f );
  
}

void Target::update( float dt ) {
  
  Target::handleInput( dt );
  
  //position_.updatePosition( velocity_, dt );
  
  //position_.setCoordinates( heroPosition_ -> x, heroPosition_ -> y, heroPosition_ -> z - 15.0f );
  
  model_ = glm::translate( glm::mat4(), position_.coordinates() );
  
  
}

void Target::render() {
  GlObject::render();
}

void Target::clean() {
  GlObject::clean();
}
