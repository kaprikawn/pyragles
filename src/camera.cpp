#include "camera.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( int windowWidth, int windowHeight ) {
  
  windowWidth_  = windowWidth;
  windowHeight_ = windowHeight;
  
  float aspect = ( float ) windowWidth / ( float ) windowHeight;
  
  projection_ = glm::perspective( glm::radians( 60.0f ), aspect, 1.0f, 65.0f );
  
  position_ = { 0.0f, 0.0f, 4.0f };
  
  view_ = glm::lookAt(
      position_
    , glm::vec3( 0, 0, -50.0f )
    , glm::vec3( 0, 1, 0 )
  );
}

void Camera::update( std::shared_ptr<glm::vec3> shipPosition, GLfloat dt ) {
  glm::vec3 velocity;
  velocity.x = shipPosition -> x - position_.x;
  velocity.y = shipPosition -> y - position_.y;
  velocity.z = 0;
  
  position_ += velocity * dt * 5.0f;
  
  view_ = glm::lookAt(
      position_
    , glm::vec3( 0, 0, -10.0f )
    , glm::vec3( 0, 1, 0 )
  );
}
