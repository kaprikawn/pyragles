#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <memory>

class CameraClass {
  private:
    
    glm::mat4   projection_;
    glm::mat4   view_;
    
    glm::vec3   position_;
    
    int windowWidth_;
    int windowHeight_;
    
    float   windowWidthF_;
    float   windowHeightF_;
    
    CameraClass  (){}
    ~CameraClass (){}
    static CameraClass* instance_;
    
  public:
    
    void init( int windowWidth, int windowHeight );
    void update( std::shared_ptr<glm::vec3> shipPosition, GLfloat dt );
  
    glm::mat4 viewProjectionMatrix() {
      return projection_ * view_;
    }
    
    int windowWidth()   { return windowWidth_; }
    int windowHeight()  { return windowHeight_; }
    
    float windowWidthF()    { return windowWidthF_; }
    float windowHeightF()   { return windowHeightF_; }
    
    static CameraClass* Instance() {
      if( instance_ == 0 ) {
        instance_ = new CameraClass();
      }
      return instance_;
    }
};

typedef CameraClass Camera;


#endif //CAMERA_HPP