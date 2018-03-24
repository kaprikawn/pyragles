#include "camera.hpp"
#include "structs.hpp"

Camera* Camera::instance_ = 0;

Camera::Camera() {
  
  projection_ = glm::perspective( glm::radians( 45.0f ), windowWidth / windowHeight, 0.1f, 20.0f );
  
  view_ = glm::lookAt(
      glm::vec3( 0, 0, 1 )
    , glm::vec3( 0, 0, 0 )
    , glm::vec3( 0, 1, 0 )
  );
  
  
  
}
