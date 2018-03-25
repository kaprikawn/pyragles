#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "shape.hpp"
#include "vector.hpp"
#include "position.hpp"

class GlObject {
  protected:
    GLuint      programID_;
    
    GLuint      vbo_;
    GLuint      ibo_;
    
    GLint       positionID_;
    GLint       colourID_;
    GLint       mvpID_;
    
    glm::mat4   projection_;
    glm::mat4   view_;
    glm::mat4   model_;
    glm::mat4   rotation_;
    glm::mat4   mvp_;
    
    Vector      velocity_;
    Vector      coordinates_;
    Position    position_;
    GLfloat     yAngle_ = 0.0f;
    GLfloat     xAngle_ = 0.0f;
    GLfloat     zAngle_ = 0.0f;
    
    std::unique_ptr<Shape>  shape_;
    
  public:
    GlObject();
    virtual ~GlObject(){}
    
    virtual int   init();
    virtual void  update( float dt );
    virtual void  render();
    virtual void  clean();
    
};

#endif //GLOBJECT_HPP
