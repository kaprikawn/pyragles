#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
  private:
    
    glm::mat4       projection_ = glm::mat4( 1.0f );
    glm::mat4       view_       = glm::mat4( 1.0f );
    
    static Camera*  instance_;
    
    Camera();
    
  public:
    ~Camera(){}
    
    void init();
    
    glm::mat4 viewMatrix() {
      return view_;
    }
    
    glm::mat4 projectionMatrix() {
      return projection_;
    }
    
    
    static Camera* Instance() {
      if( instance_ == 0 ) {
        instance_ = new Camera();
      }
      return instance_;
    }
};

typedef Camera TheCamera;

#endif //CAMERA_HPP
