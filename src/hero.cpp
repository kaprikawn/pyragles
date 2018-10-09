#include <iostream>
#include <cmath>
#include "hero.hpp"
#include "inputHandler.hpp"

#define PI 3.141592653589793238462643383279

Hero::Hero( int shapeType, GLuint programID, std::shared_ptr<Target> target, std::shared_ptr<glm::vec3> heroPosition ) : GlObject( shapeType, 0.0f, 0.0f, -7.0f, programID ) {
  target_ = target;
  heroPosition_ = heroPosition;
}

GLfloat differenceBetween( float target, float current ) {
  float squared = sqrt( pow( target - current, 2.0f ) );
  
  if( squared < 0.2f )
    squared = 0.0f;
    
  return squared;
}

void Hero::handleInput( float dt ) {
  
  GLfloat joyAxisX_ = TheInputHandler::Instance() -> joyAxisX();
  GLfloat joyAxisY_ = TheInputHandler::Instance() -> joyAxisY();
  
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
  
  GLfloat velocityMultiplier_ = 20.0f;
  GLfloat xCurrentVelocity_ = velocity_.x();
  GLfloat xSetVelocity_ = 0.0f;
  GLfloat xMaxVelocity_ = 8.0f;
  
  if( joyAxisX_ > 0.0f ) { // pressing right
    if( xCurrentVelocity_  < 0.0f ) // if going left but pressing right
      xCurrentVelocity_ = 0.0f; // calculate velocity from 0 to turn quicker
  
    xSetVelocity_ = xCurrentVelocity_ + dt * velocityMultiplier_ * joyAxisX_;
    
  } else if( joyAxisX_ < 0.0f ) { // pressing left
    
    if( xCurrentVelocity_  > 0.0f )
      xCurrentVelocity_ = 0.0f;
  
    xSetVelocity_ = xCurrentVelocity_ + dt * velocityMultiplier_ * joyAxisX_;
  
  } else { // not pressing left nor right
    
    if( xCurrentVelocity_ > 0.0f ) {
      
      xSetVelocity_ = xCurrentVelocity_ - differenceBetween( 0.0f, xCurrentVelocity_ ) * 0.4f;
      
      if( xCurrentVelocity_ < 0.05f )
        xSetVelocity_ = 0.0f;
        
    } else if( xCurrentVelocity_ < 0.0f ) {
      
      xSetVelocity_ = xCurrentVelocity_ + differenceBetween( 0.0f, xCurrentVelocity_ ) * 0.4f;
      
      if( xCurrentVelocity_ > 0.05f )
        xSetVelocity_ = 0.0f;
        
    }
  }
  
  if( xSetVelocity_ > xMaxVelocity_ )
    xSetVelocity_ = xMaxVelocity_;
  
  if( xSetVelocity_ < -xMaxVelocity_ )
    xSetVelocity_ = -xMaxVelocity_;
    
  velocity_.setX( xSetVelocity_ );
  
  //////////////////
  
  GLfloat yCurrentVelocity_ = velocity_.y();
  GLfloat ySetVelocity_ = 0.0f;
  GLfloat yMaxVelocity_ = 8.0f;
  
  if( joyAxisY_ > 0.0f ) { // rising
    if( yCurrentVelocity_  < 0.0f )
      yCurrentVelocity_ = 0.0f;
  
    ySetVelocity_ = yCurrentVelocity_ + dt * velocityMultiplier_ * joyAxisY_;
    
  } else if( joyAxisY_ < 0.0f ) { // falling
    
    if( yCurrentVelocity_  > 0.0f )
      yCurrentVelocity_ = 0.0f;
  
    ySetVelocity_ = yCurrentVelocity_ + dt * velocityMultiplier_ * joyAxisY_;
  
  } else { // not pressing up nor down
    
    if( yCurrentVelocity_ > 0.0f ) {
      
      ySetVelocity_ = yCurrentVelocity_ - differenceBetween( 0.0f, yCurrentVelocity_ ) * 0.4f;
      
      if( yCurrentVelocity_ < 0.02f )
        ySetVelocity_ = 0.0f;
        
    } else if( yCurrentVelocity_ < 0.0f ) {
      
      ySetVelocity_ = yCurrentVelocity_ + differenceBetween( 0.0f, yCurrentVelocity_ ) * 0.4f;
      
      if( yCurrentVelocity_ > 0.02f )
        ySetVelocity_ = 0.0f;
        
    }
  }
  
  if( ySetVelocity_ > yMaxVelocity_ )
    ySetVelocity_ = yMaxVelocity_;
  
  if( ySetVelocity_ < -yMaxVelocity_ )
    ySetVelocity_ = -yMaxVelocity_;
    
  velocity_.setY( ySetVelocity_ );
  
  if( TheInputHandler::Instance() -> justPressed( FIRE ) )
    fire_ = true;
}

void Hero::calculateRotation( float dt ) {
  
  heroX_    = position_.coordinates().x;
  heroY_    = position_.coordinates().y;
  targetX_  = target_ -> position().coordinates().x;
  targetY_  = target_ -> position().coordinates().y;
  
  yAngle_ = -( atan( ( target_ -> position().coordinates().x - position_.coordinates().x ) / ( position_.coordinates().z - target_ -> position().coordinates().z ) ) * 180 / PI ); // face left and right
  
  xAngle_ = ( atan( ( target_ -> position().coordinates().y - position_.coordinates().y ) / ( position_.coordinates().z - target_ -> position().coordinates().z ) ) * 180 / PI ); // pitch up and down
  
  if( velocity_.x() == 0.0f && zAngle_ == 0.0f ) {
    zAngle_ = 0.0f;
  } else if( velocity_.x() > 0.3f && zAngle_ > -maxRotationZ_ ) {
    zAngle_ -= differenceBetween( -maxRotationZ_, zAngle_ ) * 5.0f * dt;
  } else if( velocity_.x() < -0.3f && zAngle_ < maxRotationZ_ ) {
    zAngle_ += differenceBetween( maxRotationZ_, zAngle_ ) * 5.0f * dt;
  } else {
    zAngle_ += -zAngle_ * dt;
  }
  
  rotation_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( xAngle_ ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( zAngle_ ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void Hero::updatePosition( float dt ) {
  position_.updatePosition( velocity_, dt );
  
  heroPosition_ -> x = position_.coordinates().x;
  heroPosition_ -> y = position_.coordinates().y;
  heroPosition_ -> z = position_.coordinates().z;
}

void Hero::update( float dt ) {
  
  Hero::handleInput( dt );
  
  Hero::updatePosition( dt );
  
  model_ = glm::translate( glm::mat4( 1.0f ), position_.coordinates() );
  model_ *= rotation_;
  
  GlObject::update( dt );
  
}

void Hero::render() {
  GlObject::render();
}

void Hero::clean() {
  GlObject::clean();
}
