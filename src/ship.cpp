#include "ship.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592653589793238462643383279

Ship::Ship( glm::vec3 initPosition, BufferData bufferData, std::vector<glm::vec3> mesh, std::shared_ptr<Renderer> renderer, std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<glm::vec3>    shipPosition, std::shared_ptr<Target> target )
  : PhysicsObject( initPosition, bufferData, mesh, renderer ) {
  
  inputHandler_ = inputHandler;
  shipPosition_ = shipPosition;
  target_       = target;
  
}

void Ship::calculateRotation( GLfloat dt ) {
  
  GLfloat targetX = target_ -> position().x;
  GLfloat targetY = target_ -> position().y;
  GLfloat targetZ = target_ -> position().z;
  
  GLfloat shipX = mesh_ -> x();
  GLfloat shipY = mesh_ -> y();
  GLfloat shipZ = mesh_ -> z();
  
  GLfloat yAngle = ( atan( ( targetX - shipX ) / ( targetZ ) ) * 180 / PI ); 
  
  GLfloat xAngle = ( atan( ( targetY - shipY ) / ( shipZ - targetZ ) ) * 180 / PI );
  
  yAngle = 0.0f;
  xAngle = 0.0f;
  
  GLfloat zAngle = -velocity_.x * 2 ;
  
  rotationMatrix_ = glm::rotate( glm::mat4(), glm::radians( xAngle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_, glm::radians( yAngle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  rotationMatrix_ = glm::rotate( rotationMatrix_, glm::radians( zAngle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void Ship::handleInput( GLfloat dt ) {
  
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
  
  printf("ship\n");
}

CollisionProperties Ship::collisionProperties() {
  CollisionProperties collisionProperties;
  
  collisionProperties.objectID = objectID_;
  
  return collisionProperties;
}

void Ship::clean() {
  PhysicsObject::clean();
}
