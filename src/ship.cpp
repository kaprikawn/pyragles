#include "ship.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

#define PI 3.141592653589793238462643383279

Ship::Ship( PhysicsObjectParams physicsObjectParams, std::shared_ptr<Target> target ) : PhysicsObject( physicsObjectParams ) {
  
  inputHandler_ = physicsObjectParams.inputHandler;
  shipPosition_ = physicsObjectParams.shipPosition;
  target_       = target;
}

void Ship::calculateRotation( GLfloat dt ) {
  
  GLfloat yAngle = 0.0f;
  GLfloat xAngle = 0.0f;
  GLfloat zAngle = zAngle_;
  
  if( objectState_ == COLLIDED ) {
    
    if( zAngle_ < -20.0f ) {
      collidedDirection_ = 10.0f;
    } else if( zAngle_ > 20.0f ) {
      collidedDirection_ = -10.0f;
    }
    zAngle = zAngle_ + collidedDirection_;
    
  } else {
    
    GLfloat targetX = target_ -> position().x;
    GLfloat targetY = target_ -> position().y;
    GLfloat targetZ = target_ -> position().z;
    
    GLfloat shipX = mesh_ -> x();
    GLfloat shipY = mesh_ -> y();
    GLfloat shipZ = mesh_ -> z();
    
    yAngle = ( atan( ( targetX - shipX ) / ( targetZ ) ) * 180 / PI ); 
    
    xAngle = ( atan( ( targetY - shipY ) / ( shipZ - targetZ ) ) * 180 / PI );
    
    zAngle = -velocity_.x * 2 ;
  }
  
  zAngle_ = zAngle;
  
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( xAngle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_, glm::radians( yAngle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_, glm::radians( zAngle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void Ship::handleInput( GLfloat dt ) {
  
  if( objectState_ == COLLIDED )
    return;
  
  if( inputHandler_ -> justPressed( FIRE ) )
    fire_ = true;
  
  GLfloat multiplier      = 8.0f;
  GLfloat standstillSpeed = 4.0f;
  
  bool invertY = true;
  
  // left and right
  GLfloat xMax      = 6.0f;
  GLfloat joyAxisX  = inputHandler_ -> joyAxisX();
  acceleration_.x   = joyAxisX * multiplier;
  
  if( joyAxisX == 0.0f ) {
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
  } else if( ( joyAxisX < 0.0f && velocity_.x > 0.0f )
          || ( joyAxisX > 0.0f && velocity_.x < 0.0f ) ) {
    acceleration_.x *= 3;
  }
  
  velocity_.x += acceleration_.x * dt;
  
  if( velocity_.x > xMax )
    velocity_.x = xMax;
  
  if( velocity_.x < -xMax )
    velocity_.x = -xMax;
  
  // up and down
  GLfloat yMax      = 6.0f;
  GLfloat joyAxisY  = inputHandler_ -> joyAxisY( invertY );
  acceleration_.y   = joyAxisY * multiplier;
  
  if( joyAxisY == 0.0f ) {
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
  } else if( ( joyAxisY < 0.0f && velocity_.y > 0.0f )
          || ( joyAxisY > 0.0f && velocity_.y < 0.0f ) ) {
    acceleration_.y *= 3;
  }
  
  velocity_.y += acceleration_.y * dt;
  
  if( velocity_.y > yMax )
    velocity_.y = yMax;
  
  if( velocity_.y < -yMax )
    velocity_.y = -yMax;
    
  
  
}

void Ship::update( GLfloat dt, bool skipMove ) {
  
  handleInput( dt );
  
  if( mesh_ -> y() < 0.55f && velocity_.y < 0.0f )
    velocity_.y = 0.0f;
  if( mesh_ -> y() > CEILING && velocity_.y > 0.0f )
    velocity_.y = 0.0f;  
  
  GLfloat maxX = 10.0f;
  if( mesh_ -> x() > maxX && velocity_.x > 0.0f )
    velocity_.x = 0.0f;
  if( mesh_ -> x() < -maxX && velocity_.x < 0.0f )
    velocity_.x = 0.0f;
  
  if( newObjectState_ == COLLIDED )
    collidedTimer_.setCoundownTimer( 0.8f );
  
  if( objectState_ == COLLIDED ) {
    collidedTimer_.update( dt );
    if( collidedTimer_.timeLeft() <= 0.0f ) {
      PhysicsObject::changeState( UNDEF_STATE );
      lastCollisionID_ = 0;
    }
  }
  
  PhysicsObject::update( dt, skipMove );
  
  shipPosition_ -> x = mesh_ -> x();
  shipPosition_ -> y = mesh_ -> y();
  shipPosition_ -> z = mesh_ -> z();
}

void Ship::render( glm::mat4 viewProjectionMatrix ) {
  PhysicsObject::render( viewProjectionMatrix );
}

void Ship::registerCollision( CollisionData collisionData, CollisionProperties collisionProperties ) {
  if( lastCollisionID_ == collisionProperties.objectID )
    return;
  
  lastCollisionID_ = collisionProperties.objectID;
  
  PhysicsObject::changeState( COLLIDED );
  
}

CollisionProperties Ship::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}

void Ship::clean() {
  PhysicsObject::clean();
}
