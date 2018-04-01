#include <iostream>
#include <cmath>
#include "hero.hpp"
#include "inputHandler.hpp"

GLfloat maxValX_ = 6.0f;
GLfloat maxValY_ = 3.0f;

Hero::Hero() : GlObject() {
  
}

void Hero::handleInput( float dt ) {
  
  GLfloat velMultiplier_ = 9.0f;
  joyAxisX_ = TheInputHandler::Instance() -> joyAxisX();
  joyAxisY_ = TheInputHandler::Instance() -> joyAxisY();
  
  directionPressed_ = false;
  
  if( joyAxisX_ == 0.0f ) {
    if( TheInputHandler::Instance() -> isPressed( RIGHT ) ) {
      joyAxisX_ = 1.0f;
    } else if( TheInputHandler::Instance() -> isPressed( LEFT ) ) {
      joyAxisX_ = -1.0f;
    }
    directionPressed_ = true;
  }
  
  if( joyAxisY_ == 0.0f ) {
    if( TheInputHandler::Instance() -> isPressed( RISE ) ) {
      joyAxisY_ = 1.0f;
    } else if( TheInputHandler::Instance() -> isPressed( FALL ) ) {
      joyAxisY_ = -1.0f;
    }
    directionPressed_ = true;
  }
  
  // horizontal movement
  if( joyAxisX_ > 0.0f ) {
    
    if( position_.coordinateValues().getX() < maxValX_ ) {
      velocity_.setX( joyAxisX_ * velMultiplier_ );
    } else {
      velocity_.setX( 0.0f );
    }
    directionPressed_ = true;
  } else if( joyAxisX_ < 0.0f ) {
    if( position_.coordinateValues().getX() > -maxValX_ ) {
      velocity_.setX( joyAxisX_ * velMultiplier_ );
    } else {
      velocity_.setX( 0.0f );
    }
    directionPressed_ = true;
  } else {
    velocity_.setX( 0.0f );
  }
  
  // vertical movement
  if( joyAxisY_ < 0.0f ) { // rise
    if( position_.coordinateValues().getY() < maxValY_ ) {
      velocity_.setY( -joyAxisY_ * velMultiplier_ );
    } else {
      velocity_.setY( 0.0f );
    }
    directionPressed_ = true;
  } else if( joyAxisY_ > 0.0f ) { // fall
    if( position_.coordinateValues().getY() > -maxValY_ ) {
      velocity_.setY( -joyAxisY_ * velMultiplier_ );
    } else {
      velocity_.setY( 0.0f );
    }
    directionPressed_ = true;
  } else {
    velocity_.setY( 0.0f );
  }
}

GLfloat differenceBetween( float target, float current ) {
  float squared = sqrt( pow( target - current, 2.0f ) );
  
  if( squared < 0.2 )
    squared = 0.0f;
    
  return squared;
}

void Hero::calculateRotation( float dt ) {
  
  float xVelocity_    = velocity_.getX();
  float yVelocity_    = velocity_.getY();
  float maxRotationZ_ = 10.0f; // tilt
  float maxRotationY_ = 40.0f; // turn
  float maxRotationX_ = 30.0f; // up/down pitch
  
  if( xVelocity_ > 0.0f ) { // going right
    
    if( zAngle_ > -maxRotationZ_ ) {
      zAngle_ -= differenceBetween( -maxRotationZ_, zAngle_ ) * 5.0f * dt;
    }
    
    if( yAngle_ > -maxRotationY_ ) {
      yAngle_ -= differenceBetween( -maxRotationY_, yAngle_ ) * 5.0f * dt;
    }
    
  } else if( xVelocity_ < 0.0f ) { // going left
    
    if( zAngle_ < maxRotationZ_ ) {
      zAngle_ += differenceBetween( maxRotationZ_, zAngle_ ) * 5.0f * dt;
    }
    
    if( yAngle_ < maxRotationY_ ) {
      yAngle_ += differenceBetween( maxRotationY_, yAngle_ ) * 5.0f * dt;
    }
    
  } else { // not moving left or right
    
    if( zAngle_ > -0.3f && zAngle_ < 0.3f ) {
      zAngle_ = 0.0f;
    } else if( zAngle_ < 0.0f ) {
      zAngle_ += differenceBetween( 0.0f, zAngle_ ) * 10.0f * dt;
    } else if( zAngle_ > 0.0f ) {
      zAngle_ -= differenceBetween( 0.0f, zAngle_ ) * 10.0f * dt;
    }
    
    if( yAngle_ > -0.3f && yAngle_ < 0.3f ) {
      yAngle_ = 0.0f;
    } else if( yAngle_ < 0.0f ) {
      yAngle_ += differenceBetween( 0.0f, yAngle_ ) * 10.0f * dt;
    } else if( yAngle_ > 0.0f ) {
      yAngle_ -= differenceBetween( 0.0f, yAngle_ ) * 10.0f * dt;
    }
    
  }
  
  if( yVelocity_ > 0.0f ) { // going up
    //xAngle_ = 20.0f;
    if( xAngle_ < maxRotationX_ )
      xAngle_ += differenceBetween( -maxRotationX_, xAngle_ ) * 5.0f * dt;
      
    if( xAngle_ > maxRotationX_ )
      xAngle_ = maxRotationX_;
    
  } else if( yVelocity_ < 0.0f ) { // going down
    
    if( xAngle_ > -maxRotationX_ )
      xAngle_ -= differenceBetween( -maxRotationX_, xAngle_ ) * 5.0f * dt;
      
    if( xAngle_ < -maxRotationX_ )
      xAngle_ = -maxRotationX_;
    
  } else { // not moving up or down
  
    if( xAngle_ > -0.3f && xAngle_ < 0.3f ) {
      xAngle_ = 0.0f;
    } else if( xAngle_ < 0.0f ) {
      xAngle_ += differenceBetween( 0.0f, xAngle_ ) * 10.0f * dt;
    } else if( xAngle_ > 0.0f ) {
      xAngle_ -= differenceBetween( 0.0f, xAngle_ ) * 10.0f * dt;
    }
  }
}

void Hero::updatePosition( float dt ) {
  position_.updatePosition( velocity_, dt );
  coordinates_ = position_.coordinateValues();
  
}

void Hero::update( float dt ) {
  GlObject::update( dt );
  
  Hero::handleInput( dt );
  
  Hero::calculateRotation( dt );
  
  Hero::updatePosition( dt );
  
  model_ = glm::translate( glm::mat4(), glm::vec3( coordinates_.getX(), coordinates_.getY(), -10.0f ) );
  
  rotation_ = glm::rotate( glm::mat4(), glm::radians( xAngle_ ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( zAngle_ ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
  
  //rotation_ = glm::rotate( rotation_, glm::radians( xAngle_ ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  
}

void Hero::render() {
  GlObject::render();
}

void Hero::clean() {
  GlObject::clean();
}
