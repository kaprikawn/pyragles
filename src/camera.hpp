#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera {
  private:
    
    glm::mat4   projection_ = glm::mat4( 1.0f );
    glm::mat4   view_       = glm::mat4( 1.0f );
    
  public:
    Camera();
    ~Camera(){}
  
  glm::mat4 viewProjectionMatrix() {
    return projection_ * view_;
  }
};

typedef Camera TheCamera;

#endif //CAMERA_HPP