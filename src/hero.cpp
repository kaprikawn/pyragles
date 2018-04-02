#include <iostream>
#include <cmath>
#include "hero.hpp"
#include "inputHandler.hpp"

#define PI 3.141592653589793238462643383279

Hero::Hero( int shapeType, Target* target ) : GlObject( shapeType, 0.0f, 0.0f, -7.0f ) {
  target_ = target;
}

void Hero::handleInput( float dt ) {
  
}

GLfloat differenceBetween( float target, float current ) {
  float squared = sqrt( pow( target - current, 2.0f ) );
  
  if( squared < 0.2 )
    squared = 0.0f;
    
  return squared;
}

void Hero::calculateRotation( float dt ) {
  
  yAngle_ = -( atan( ( target_ -> position().coordinates().x - position_.coordinates().x ) / ( position_.coordinates().z - target_ -> position().coordinates().z ) ) * 180 / PI ); // face left and right
  
  xAngle_ = ( atan( ( target_ -> position().coordinates().y - position_.coordinates().y ) / ( position_.coordinates().z - target_ -> position().coordinates().z ) ) * 180 / PI ); // pitch up and down
  
  if( velocity_.getX() == 0.0f && zAngle_ == 0.0f ) {
    zAngle_ = 0.0f;
  } else if( velocity_.getX() > 0.3f && zAngle_ > -maxRotationZ_ ) {
    zAngle_ -= differenceBetween( -maxRotationZ_, zAngle_ ) * 5.0f * dt;
  } else if( velocity_.getX() < -0.3f && zAngle_ < maxRotationZ_ ) {
    zAngle_ += differenceBetween( maxRotationZ_, zAngle_ ) * 5.0f * dt;
  } else {
    zAngle_ += -zAngle_ * dt;
  }
  
  rotation_ = glm::rotate( glm::mat4(), glm::radians( xAngle_ ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  rotation_ = glm::rotate( rotation_, glm::radians( zAngle_ ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void Hero::updatePosition( float dt ) {
  
  heroX_ = position_.coordinates().x;
  heroY_ = position_.coordinates().y;
  targetX_ = target_ -> position().coordinates().x;
  targetY_ = target_ -> position().coordinates().y;
  
  if( targetX_ > heroX_ + buffer || targetX_ < heroX_ - buffer ) {
    velocity_.setX( ( targetX_ - heroX_ ) * catchup * dt );
  } else {
    velocity_.setX( 0.0f );
  }
  
  if( targetY_ > heroY_ + buffer || targetY_ < heroY_ - buffer ) {
    velocity_.setY( ( targetY_ - heroY_ ) * catchup * dt );
  } else {
    velocity_.setY( 0.0f );
  }
  
  position_.updatePosition( velocity_, dt );
}

void Hero::update( float dt ) {
  
  Hero::updatePosition( dt );
  
  Hero::calculateRotation( dt );
  
  model_ = glm::translate( glm::mat4(), position_.coordinates() );
  model_ *= rotation_;
  
  GlObject::update( dt );
  
}

void Hero::render() {
  GlObject::render();
}

void Hero::clean() {
  GlObject::clean();
}
