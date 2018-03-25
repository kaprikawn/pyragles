#include <iostream>
#include "hero.hpp"
#include "inputHandler.hpp"

Hero::Hero() : GlObject() {
  
}

void Hero::handleInput( float dt ) {
  
  if( TheInputHandler::Instance() -> isPressed( RIGHT ) ) {
    
    velocity_.setX( 9.0f );
    
    // tilt
    if( zAngle_ > -10.0f ) {
      if( zAngle_ > 0.0f ) {
        zAngle_ -= 40.0f * dt;
      } else {
        zAngle_ -= 20.0f * dt;
      }
    }
    
    if( yAngle_ > -20.0f ) {
      if( yAngle_ > 0.0f ) {
        yAngle_ -= 100.0f * dt;
      } else {
        yAngle_ -= 80.0f * dt;
      }
    }
    
  } else if( TheInputHandler::Instance() -> isPressed( LEFT ) ) {
    
    velocity_.setX( -9.0f );
    
    if( zAngle_ < 10.0f ) {
      if( zAngle_ < 0.0f ) {
        zAngle_ += 40.0f * dt;
      } else {
        zAngle_ += 20.0f * dt;
      }
    }
    
    if( yAngle_ < 20.0f ) {
      if( yAngle_ < 0.0f ) {
        yAngle_ += 100.0f * dt;
      } else {
        yAngle_ += 80.0f * dt;
      }
    }
    
  } else {
    velocity_.setX( 0.0f );
    if( zAngle_ > 0.0f ) {
      zAngle_ -= 80.0f * dt;
    } else if( zAngle_ < 0.0f ) {
      zAngle_ += 80.0f * dt;
    }
    
    if( yAngle_ > 0.0f ) {
      yAngle_ -= 80.0f * dt;
    } else if( yAngle_ < 0.0f ) {
      yAngle_ += 80.0f * dt;
    }
  }
  
  if( TheInputHandler::Instance() -> isPressed( UP ) ) {
    velocity_.setY( -9.0f );
    xAngle_ = -20.0f;
    
  } else if( TheInputHandler::Instance() -> isPressed( DOWN ) ) {
    velocity_.setY( 9.0f );
    //xAngle_ = 20.0f;
    
    if( xAngle_ < 100.f ) {
      xAngle_ += 30.f * dt;
    }
    
    
    
  } else {
    velocity_.setY( 0.0f );
    xAngle_ = 0.0f;
  }
  
  if( zAngle_ < 0.05f && zAngle_ > -0.05f )
    zAngle_ = 0.0f;
  if( xAngle_ < 0.05f && xAngle_ > -0.05f )
    xAngle_ = 0.0f;
  if( yAngle_ < 0.5f && yAngle_ > -0.05f )
    yAngle_ = 0.0f;
}

void calculateRotation( float dt ) {

}

void Hero::updatePosition( float dt ) {
  position_.updatePosition( velocity_, dt );
  coordinates_ = position_.coordinateValues();
  
}

void Hero::update( float dt ) {
  GlObject::update( dt );
  
  Hero::handleInput( dt );
  
  Hero::calculatRotation( dt );
  
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
