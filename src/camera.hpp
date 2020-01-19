#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <memory>

class Camera {
  private:
    
    glm::mat4   projection_;
    glm::mat4   view_;
    
    glm::vec3   position_;
    
    GLsizei windowWidth_;
    GLsizei windowHeight_;
    
    float   windowWidthF_;
    float   windowHeightF_;
    
  public:
    Camera( int windowWidth, int windowHeight );
    ~Camera(){}
    
    void update( std::shared_ptr<glm::vec3> shipPosition, GLfloat dt );
  
    glm::mat4 viewProjectionMatrix() {
      return projection_ * view_;
    }
    
    GLsizei windowWidth()   { return windowWidth_; }
    GLsizei windowHeight()  { return windowHeight_; }
    
    float windowWidthF()    { return windowWidthF_; }
    float windowHeightF()   { return windowHeightF_; }
    
};

#endif //CAMERA_HPP