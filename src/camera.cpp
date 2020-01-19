#include "camera.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

Camera::Camera( int windowWidth, int windowHeight ) {
  
  windowWidth_  = windowWidth;
  windowHeight_ = windowHeight;
  
  float aspect = ( float ) windowWidth / ( float ) windowHeight;
  
  projection_ = glm::perspective( glm::radians( 70.0f ), aspect, 0.01f, 65.0f );
  
  position_ = { 0.0f, START_Y, START_Z };
  
  view_ = glm::lookAt(
      position_
    , glm::vec3( 0, 0, -50.0f )
    , glm::vec3( 0, 1, 0 )
  );
}

void Camera::update( GLfloat dt ) {
  
  view_ = glm::lookAt(
      position_
    , glm::vec3( 0, 0, -50.0f )
    , glm::vec3( 0, 1, 0 )
  );
}
