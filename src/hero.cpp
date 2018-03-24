#include <iostream>
#include "hero.hpp"
#include "inputHandler.hpp"

Hero::Hero() : GlObject() {
  
}

void Hero::handleInput( float dt ) {
  if( TheInputHandler::Instance() -> isPressed( RIGHT ) ) {
    velocity_.setX( 5.0f );
    zAngle_ = -10.0f;
  } else if( TheInputHandler::Instance() -> isPressed( LEFT ) ) {
    velocity_.setX( -5.0f );
    zAngle_ = 10.0f;
  } else {
    velocity_.setX( 0.0f );
    zAngle_ = 0.0f;
  }
  
  if( TheInputHandler::Instance() -> isPressed( UP ) ) {
    velocity_.setY( -5.0f );
    xAngle_ = -20.0f;
  } else if( TheInputHandler::Instance() -> isPressed( DOWN ) ) {
    velocity_.setY( 5.0f );
    xAngle_ = 20.0f;
  } else {
    velocity_.setY( 0.0f );
    xAngle_ = 0.0f;
  }
}

void Hero::updatePosition( float dt ) {
  coordinates_ = coordinates_ + velocity_ * dt;
}

void Hero::update( float dt ) {
  GlObject::update( dt );
  
  Hero::handleInput( dt );
  
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
