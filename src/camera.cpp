#include "camera.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( int windowWidth, int windowHeight ) {
  
  windowWidth_    = windowWidth;
  windowHeight_   = windowHeight;
  windowWidthF_   = ( float )windowWidth;
  windowHeightF_  = ( float )windowHeight;
  
  float aspect = ( float ) windowWidth / ( float ) windowHeight;
  
  projection_ = glm::perspective( glm::radians( 70.0f ), aspect, 0.01f, 65.0f );
  
  position_ = { 3.0f, 5.0f, 10.0f };
  
  view_ = glm::lookAt(
      position_
    , glm::vec3( 0, 0, -50.0f )
    , glm::vec3( 0, 1, 0 )
  );
  
  glViewport( 0, 0, windowWidthF_, windowHeightF_ );
}

void Camera::update( std::shared_ptr<glm::vec3> shipPosition, GLfloat dt ) {
  
  glm::vec3 velocity;
  velocity.x = shipPosition -> x - position_.x;
  velocity.y = shipPosition -> y - position_.y;
  velocity.z = 0;
  
  position_ += velocity * dt * 2.0f;
  
  view_ = glm::lookAt(
      position_
    , glm::vec3( position_.x, position_.y, -50.0f )
    , glm::vec3( 0, 1, 0 )
  );
}
