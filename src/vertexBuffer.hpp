#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

#include <GLES2/gl2.h>

class VertexBuffer {
  
  private :
    unsigned int rendererID_;
    
  public :
    VertexBuffer();
    ~VertexBuffer();
    
    void init( const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW );
    void bind() const;
    void unbind() const;
  
};
    
#endif //VERTEXBUFFER_HPP