#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "shape.hpp"
#include "vector.hpp"
#include "position.hpp"

#include <iostream>


class GlObject {
  protected:
    
    bool        invertY_ = true;
    
    GLuint      vbo_;
    GLuint      ibo_;
    
    GLint       positionID_;
    GLint       colourID_;
    GLint       mvpID_;
    
    GLsizeiptr  vertexOffset_; // location of beginning of data in vertex buffer
    GLsizeiptr  indexOffset_;  // location of beginning of data in index buffer
    
    GLvoid*     colourOffset_;
    
    glm::mat4   projection_ = glm::mat4( 1.0f );
    glm::mat4   view_       = glm::mat4( 1.0f );
    glm::mat4   model_      = glm::mat4( 1.0f );
    glm::mat4   rotation_   = glm::mat4( 1.0f );
    glm::mat4   mvp_        = glm::mat4( 1.0f );
    
    Vector      velocity_;
    Position    position_;
    GLfloat     yAngle_ = 0.0f;
    GLfloat     xAngle_ = 0.0f;
    GLfloat     zAngle_ = 0.0f;
    
    bool        fire_   = false;
    
    bool        delete_ = false;
    
    std::unique_ptr<Shape>  shape_;
    
  public:
    GlObject( int shapeType, GLfloat x, GLfloat y, GLfloat z, GLuint programID );
    virtual ~GlObject(){}
    
    virtual int   init( int shapeType, GLuint programID );
    virtual void  update( float dt );
    virtual void  render();
    virtual void  clean();
    virtual void  calculateRotation( float dt ){}
    
    bool fire() {
      if( fire_ ) {
        fire_ = false;
        return true;
      }
      return false;
    }
    
    GLsizeiptr  vertexBufferSize()  { return shape_ -> vertexBufferSize(); }
    GLsizeiptr  indexBufferSize()   { return shape_ -> indexBufferSize(); }
    GLfloat*    vertexDataPointer() { return shape_ -> getVertexDataPointer(); }
    GLuint*     indexDataPointer()  { return shape_ -> getIndexDataPointer(); }
    int         shapeType()         { return shape_ -> shapeType(); }
    
    void setOffsetLocations( GLsizeiptr vertexOffset, GLsizeiptr indexOffset ) {
      vertexOffset_ = vertexOffset;
      indexOffset_  = indexOffset;
      
      GLsizeiptr colourOffset = vertexOffset_ + shape_ -> colorOffset();      
      colourOffset_ = (GLvoid*)colourOffset;
    }
    
    glm::vec3 coordinates() { return position_.coordinates(); } 
    bool deleteObject() { return delete_; }
    
};

#endif //GLOBJECT_HPP
