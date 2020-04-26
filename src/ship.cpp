#include "ship.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"
#include "inputHandler.hpp"

Ship::Ship() {
  
  position_.z = -3.0f;
  position_.y = START_Y;
  
  target_     = std::make_unique<Target>();
  particles_  = std::make_unique<Exhaust>();
}

bool Ship::init( std::string modelName ) {
  
  bool loaded = GameObject::loadModelFromGltf( "ship.glb", "shaderBasic.glsl" );
  
  return loaded;
}

void Ship::handleInput( float dt ) {
  
  if( InputHandler::Instance() -> justPressed( FIRE ) )
    spawnProjectile_ = true;
  
  float multiplier      = 8.0f;
  float standstillSpeed = 4.0f;
  
  // left and right
  float xMax      = 6.0f;
  joyAxisX_       = InputHandler::Instance() -> joyAxisX();
  acceleration_.x = joyAxisX_ * multiplier;
  
  if( joyAxisX_ == 0.0f ) {
    if( velocity_.x > 0.0f ) {
      acceleration_.x = -standstillSpeed;
      velocity_.x += acceleration_.x * dt;
      
      if( velocity_.x < 0.0f ) {
        velocity_.x = 0.0f;
        acceleration_.x = 0.0f;
      }
    } else if( velocity_.x < 0.0f ) {
      acceleration_.x = standstillSpeed;
      velocity_.x += acceleration_.x * dt;
      
      if( velocity_.x > 0.0f ) {
        velocity_.x = 0.0f;
        acceleration_.x = 0.0f;
      }
    }
  } else if( ( joyAxisX_ < 0.0f && velocity_.x > 0.0f )
          || ( joyAxisX_ > 0.0f && velocity_.x < 0.0f ) ) {
    acceleration_.x *= 3;
  }
  
  velocity_.x += acceleration_.x * dt;
  
  if( velocity_.x > xMax )
    velocity_.x = xMax;
  
  if( velocity_.x < -xMax )
    velocity_.x = -xMax;
  
  // up and down
  float yMax      = 6.0f;
  joyAxisY_  = InputHandler::Instance() -> joyAxisY();
  acceleration_.y = joyAxisY_ * multiplier;
  
  if( joyAxisY_ == 0.0f ) {
    if( velocity_.y > 0.0f ) {
      acceleration_.y = -standstillSpeed;
      velocity_.y += acceleration_.y * dt;
      
      if( velocity_.y < 0.0f ) {
        velocity_.y = 0.0f;
        acceleration_.y = 0.0f;
      }
    } else if( velocity_.y < 0.0f ) {
      acceleration_.y = standstillSpeed;
      velocity_.y += acceleration_.y * dt;
      
      if( velocity_.y > 0.0f ) {
        velocity_.y = 0.0f;
        acceleration_.y = 0.0f;
      }
    }
  } else if( ( joyAxisY_ < 0.0f && velocity_.y > 0.0f )
          || ( joyAxisY_ > 0.0f && velocity_.y < 0.0f ) ) {
    acceleration_.y *= 3;
  }
  
  velocity_.y += acceleration_.y * dt;
  
  if( velocity_.y > yMax )
    velocity_.y = yMax;
  
  if( velocity_.y < -yMax )
    velocity_.y = -yMax;
    
  if( position_.y < FLOOR_Y && velocity_.y < 0.0f )
    velocity_.y = 0.0f;
  
  if( position_.y > CEILING + 2 && velocity_.y > 0.0f )
    velocity_.y = 0.0f;
}

float differenceBetween( float target, float current ) {
  float squared = sqrt( pow( target - current, 2.0f ) );
  
  if( squared < 0.2 )
    squared = 0.0f;
    
  return squared;
}

void Ship::calculateRotation( float dt ) {
  
  float maxRotationZ_ = 20.0f; // tilt
  float maxRotationY_ = 30.0f; // turn
  float maxRotationX_ = 30.0f; // up/down pitch
  
  if( joyAxisX_ > 0.0f ) { // right
    
    if( zAngle_ > -maxRotationZ_ ) {
      zAngle_ -= differenceBetween( -maxRotationZ_, zAngle_ ) * 5.0f * dt;
    }
    
    if( yAngle_ > -maxRotationY_ ) {
      yAngle_ -= differenceBetween( -maxRotationY_, yAngle_ ) * 5.0f * dt;
    }
  } else if( joyAxisX_ < 0.0f ) { // left
    if( zAngle_ < maxRotationZ_ ) {
      zAngle_ += differenceBetween( maxRotationZ_, zAngle_ ) * 5.0f * dt;
    }
    
    if( yAngle_ < maxRotationY_ ) {
      yAngle_ += differenceBetween( maxRotationY_, yAngle_ ) * 5.0f * dt;
    }
  } else { // neither right nor left
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
  
  if( joyAxisY_ > 0.0f ) { // going up
    //xAngle_ = 20.0f;
    if( xAngle_ < maxRotationX_ )
      xAngle_ += differenceBetween( -maxRotationX_, xAngle_ ) * 5.0f * dt;
      
    if( xAngle_ > maxRotationX_ )
      xAngle_ = maxRotationX_;
    
  } else if( joyAxisY_ < 0.0f ) { // going down
    
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
  
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( xAngle_ ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_,   glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_,   glm::radians( zAngle_ ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
  
}

void Ship::update( float dt ) {
  
  handleInput( dt ); // updates velocity
  calculateRotation( dt );
  updatePosition( velocity_, dt );
  
  modelMatrix_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( position_ ) );
  modelMatrix_ *= rotationMatrix_;
  
  GameObject::updateCollider();
  
  target_ -> update( dt, position_, xAngle_, yAngle_ );
  
  // update mesh / vertices in mesh
  particles_ -> update( dt );
  particleTimer_ += ( dt * 60 );
  
  while( particleTimer_ > 0.0f ) {
    glm::vec4 particleStart = position_;
    particles_ -> spawnParticle( particleStart, xAngle_, yAngle_ );
    particleTimer_ -= 1.0f;
  }
}

void Ship::render( glm::mat4 viewProjectionMatrix ) {
  
  GameObject::render( viewProjectionMatrix );
  
  target_     -> render( viewProjectionMatrix );
  particles_  -> render( viewProjectionMatrix );
  
  
}

void Ship::registerCollision() {
  
}

Ship::~Ship() {
  
}

