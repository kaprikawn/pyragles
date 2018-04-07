#ifndef GLBUFFER_HPP
#define GLBUFFER_HPP

#include <GLES2/gl2.h>

class GlBuffer {
  private:
    
    GLuint      vbo_;
    GLuint      ibo_;
    
    GLsizeiptr  vertexOffset_ = 0;
    GLsizeiptr  indexOffset_  = 0;
    GLsizeiptr  ret_          = 0;
    
  public:
    GlBuffer( GLsizeiptr vertexBufferSize, GLsizeiptr indexBufferSize, GLenum usage = GL_STATIC_DRAW );
    ~GlBuffer(){}
    
    GLsizeiptr  addVertexBufferData( GLsizeiptr size, const GLvoid* data );
    GLsizeiptr  addIndexBufferData ( GLsizeiptr size, const GLvoid* data );
    
    GLsizeiptr  vertexOffset()  { return vertexOffset_; }
    GLsizeiptr  indexOffset()   { return indexOffset_; }
    
};

#endif //GLBUFFER_HPP
