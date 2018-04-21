#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( int windowWidth, int windowHeight ) {
  
  projection_ = glm::perspective( glm::radians( 60.0f ), ( float ) windowWidth / ( float ) windowHeight, 1.0f, 65.0f );
  
  view_ = glm::lookAt(
      glm::vec3( 0.0f, 0.0f, 4.0f )
    , glm::vec3( 0, 0, -10.0f )
    , glm::vec3( 0, 1, 0 )
  );
}

