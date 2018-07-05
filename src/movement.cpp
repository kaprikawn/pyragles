#include <iostream>
#include "movement.hpp"

void Movement::defineChange( glm::vec3 currentVelocity, glm::vec3 targetVelocity, glm::vec3 changeMagnitude ) {
  startVelocity_    = currentVelocity;
  targetVelocity_   = targetVelocity;
  changeMagnitude_  = changeMagnitude;
}

glm::vec3 Movement::changeVelocity( glm::vec3 currentVelocity, GLfloat dt ) {
  glm::vec3 returnVelocity = currentVelocity;
  
  if( startVelocity_.x > targetVelocity_.x ) {
    if( currentVelocity.x > targetVelocity_.x ) {
      returnVelocity.x -= dt * changeMagnitude_.x;
    }
    if( returnVelocity.x < targetVelocity_.x ) {
      returnVelocity.x = targetVelocity_.x;
    }
  } else if( startVelocity_.x < targetVelocity_.x ) {
    if( currentVelocity.x < targetVelocity_.x ) {
      returnVelocity.x += dt * changeMagnitude_.x;
    }
    if( returnVelocity.x > targetVelocity_.x ) {
      returnVelocity.x = targetVelocity_.x;
    }
  }
  
  if( startVelocity_.y > targetVelocity_.y ) {
    if( currentVelocity.y > targetVelocity_.y ) {
      returnVelocity.y -= dt * changeMagnitude_.y;
    }
    if( returnVelocity.y < targetVelocity_.y ) {
      returnVelocity.y = targetVelocity_.y;
    }
  } else if( startVelocity_.y < targetVelocity_.y ) {
    if( currentVelocity.y < targetVelocity_.y ) {
      returnVelocity.y += dt * changeMagnitude_.y;
    }
    if( returnVelocity.y > targetVelocity_.y ) {
      returnVelocity.y = targetVelocity_.y;
    }
  }
  
  if( startVelocity_.z > targetVelocity_.z ) {
    if( currentVelocity.z > targetVelocity_.z ) {
      returnVelocity.z -= dt * changeMagnitude_.z;
    }
    if( returnVelocity.z < targetVelocity_.z ) {
      returnVelocity.z = targetVelocity_.z;
    }
  } else if( startVelocity_.z < targetVelocity_.z ) {
    if( currentVelocity.z < targetVelocity_.z ) {
      returnVelocity.z += dt * changeMagnitude_.z;
    }
    if( returnVelocity.z > targetVelocity_.z ) {
      returnVelocity.z = targetVelocity_.z;
    }
  }
  
  return returnVelocity;
}