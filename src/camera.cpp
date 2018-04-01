#include "camera.hpp"
#include "structs.hpp"

Camera* Camera::instance_ = 0;

Camera::Camera() {
  
  projection_ = glm::perspective( glm::radians( 60.0f ), windowWidth / windowHeight, 2.0f, 50.0f );
  
  view_ = glm::lookAt(
      glm::vec3( 0, 0, 1 )
    , glm::vec3( 0, 0, 0 )
    , glm::vec3( 0, 1, 0 )
  );
  
}
