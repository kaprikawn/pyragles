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
    
    GLuint      vbo_;
    GLuint      ibo_;
    
    GLint       positionID_;
    GLint       colourID_;
    GLint       mvpID_;
    
    GLsizeiptr  vertexOffset_; // location of beginning of data in vertex buffer
    GLsizeiptr  indexOffset_;  // location of beginning of data in index buffer
    
    glm::mat4   projection_;
    glm::mat4   view_;
    glm::mat4   model_;
    glm::mat4   rotation_;
    glm::mat4   mvp_;
    
    Vector      velocity_;
    Position    position_;
    GLfloat     yAngle_ = 0.0f;
    GLfloat     xAngle_ = 0.0f;
    GLfloat     zAngle_ = 0.0f;
    
    std::unique_ptr<Shape>  shape_;
    
  public:
    GlObject( int shapeType, GLfloat x, GLfloat y, GLfloat z, GLuint programID );
    virtual ~GlObject(){}
    
    virtual int   init( int shapeType, GLuint programID );
    virtual void  update( float dt );
    virtual void  render();
    virtual void  clean();
    
    GLsizeiptr  vertexBufferSize()  { return shape_ -> vertexBufferSize(); }
    GLsizeiptr  indexBufferSize()   { return shape_ -> indexBufferSize(); }
    GLfloat*    vertexDataPointer() { return shape_ -> getVertexDataPointer(); }
    GLuint*     indexDataPointer()  { return shape_ -> getIndexDataPointer(); }
    
    void setOffsetLocations( GLsizeiptr vertexOffset, GLsizeiptr indexOffset ) {
      vertexOffset_ = vertexOffset;
      indexOffset_  = indexOffset;
    }
    
};

#endif //GLOBJECT_HPP
