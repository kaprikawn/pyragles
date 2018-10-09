#include <iostream>
#include <cmath>
#include "target.hpp"
#include "inputHandler.hpp"

Target::Target( int shapeType, GLuint programID ) : GlObject( shapeType, 0.0f, 0.0f, -15.0f, programID ) {
  maxValX_ = 7.0f;
  maxValY_ = 4.0f;
}

void Target::handleInput( float dt ) {
  
  GLfloat velMultiplier_ = 13.0f;
  
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
  
  // horizontal movement
  if( joyAxisX_ > 0.0f ) {
    if( position_.coordinates().x < maxValX_ ) {
      velocity_.setX( joyAxisX_ * velMultiplier_ );
    } else {
      velocity_.setX( 0.0f );
    }
  } else if( joyAxisX_ < 0.0f ) {
    if( position_.coordinates().x > -maxValX_ ) {
      velocity_.setX( joyAxisX_ * velMultiplier_ );
    } else {
      velocity_.setX( 0.0f );
    }
  } else {
    velocity_.setX( 0.0f );
  }
  
  // vertical movement
  if( joyAxisY_ < 0.0f ) { // rise
    if( position_.coordinates().y < maxValY_ ) {
      velocity_.setY( -joyAxisY_ * velMultiplier_ );
    } else {
      velocity_.setY( 0.0f );
    }
  } else if( joyAxisY_ > 0.0f ) { // fall
    if( position_.coordinates().y > -maxValY_ ) {
      velocity_.setY( -joyAxisY_ * velMultiplier_ );
    } else {
      velocity_.setY( 0.0f );
    }
  } else {
    velocity_.setY( 0.0f );
  }
  
}

void Target::update( float dt ) {
  
  Target::handleInput( dt );
  
  position_.updatePosition( velocity_, dt );
  
  model_ = glm::translate( glm::mat4( 1.0f ), position_.coordinates() );
  
  rotation_ = glm::mat4( 1.0f );
}

void Target::render() {
  GlObject::render();
}

void Target::clean() {
  GlObject::clean();
}
