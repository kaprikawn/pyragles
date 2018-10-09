#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>

class Camera {
  private:
    
    glm::mat4   projection_ = glm::mat4( 1.0f );
    glm::mat4   view_       = glm::mat4( 1.0f );
    
    GLsizei windowWidth_;
    GLsizei windowHeight_;
    
  public:
    Camera( int windowWidth, int windowHeight );
    ~Camera(){}
  
  glm::mat4 viewProjectionMatrix() {
    return projection_ * view_;
  }
  
  GLsizei windowWidth()   { return windowWidth_; }
  GLsizei windowHeight()  { return windowHeight_; }
};

typedef Camera TheCamera;

#endif //CAMERA_HPP