#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <signal.h>
#include <GLES2/gl2.h>
#include "vertexBuffer.hpp"
#include "indexBuffer.hpp"
#include "shader.hpp"

#define ASSERT(x) if( !(x)) raise(SIGINT);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT( GLLogCall( #x, __FILE__, __LINE__ ) )


void GLClearError();
bool GLLogCall( const char* function, const char* file, int line );

class Renderer {
  
  public :
    void clear() const;
    void draw( const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader ) const;
};
    
#endif //RENDERER_HPP