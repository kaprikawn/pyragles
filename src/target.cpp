#include <iostream>
#include <cmath>
#include "target.hpp"
#include "inputHandler.hpp"

Target::Target( int shapeType, GLuint programID, std::shared_ptr<glm::vec3> heroPosition ) : GlObject( shapeType, 0.0f, 0.0f, -15.0f, programID ) {
  
  heroPosition_ = heroPosition;
  
  rotation_ = glm::mat4( 1.0f );
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
  
  currentX_         = position_.coordinates().x;
  shipX_            = heroPosition_ -> x;
  destinationX_     = shipX_ + joyAxisX_ * maxDistFromShipX_;
  currentY_         = position_.coordinates().y;
  shipY_            = heroPosition_ -> y;
  destinationY_     = shipY_ + joyAxisY_ * maxDistFromShipY_;
  
  targetX_          = currentX_;
  targetY_          = currentY_;
  
  if( joyAxisX_ > 0.0f ) {
    if( targetX_ < destinationX_ )
      targetX_ += speedFactor_ * dt;
    
    if( targetX_ > shipX_ + joyAxisX_ * maxDistFromShipX_ * 0.9f )
      targetX_ = shipX_ + joyAxisX_ * maxDistFromShipX_ * 0.9f;
    
  } else if( joyAxisX_ < 0.0f ) {
    if( targetX_ > destinationX_ )
      targetX_ -= speedFactor_ * dt;
    
    if( targetX_ < shipX_ + joyAxisX_ * maxDistFromShipX_ * 0.9f )
      targetX_ = shipX_ + joyAxisX_ * maxDistFromShipX_ * 0.9f;
      
  } else {
    if( destinationX_ > currentX_ ) {
      targetX_ += speedFactor_ * dt;
      if( targetX_ > shipX_ )
        targetX_ = shipX_;
    } else if( destinationX_ < currentX_ ) {
      targetX_ -= speedFactor_ * dt;
      if( targetX_ < shipX_ )
        targetX_ = shipX_;
    }
  }
  
  if( joyAxisY_ > 0.0f ) { // rising
    if( targetY_ < destinationY_ )
      targetY_ += speedFactor_ * dt;
    
    if( targetY_ > shipY_ + joyAxisY_ * maxDistFromShipY_ * 0.9f )
      targetY_ = shipY_ + joyAxisY_ * maxDistFromShipY_ * 0.9f;
    
  } else if( joyAxisY_ < 0.0f ) { // falling
    if( targetY_ > destinationY_ )
      targetY_ -= speedFactor_ * dt;
    
    if( targetY_ < shipY_ - -joyAxisY_ * maxDistFromShipY_ * 0.9f )
      targetY_ = shipY_ - -joyAxisY_ * maxDistFromShipY_ * 0.9f;
      
  } else {
    if( destinationY_ > currentY_ ) {
      targetY_ += speedFactor_ * dt;
      if( targetY_ > shipY_ )
        targetY_ = shipY_;
    } else if( destinationY_ < currentY_ ) {
      targetY_ -= speedFactor_ * dt;
      if( targetY_ < shipY_ )
        targetY_ = shipY_;
    }
  }
  
  position_.setCoordinates( targetX_, targetY_, heroPosition_ -> z - 15.0f );
  
}

void Target::update( float dt ) {
  
  Target::handleInput( dt );
  
  model_ = glm::translate( glm::mat4( 1.0f ), position_.coordinates() );
  
  
}

void Target::render() {
  GlObject::render();
}

void Target::clean() {
  GlObject::clean();
}
