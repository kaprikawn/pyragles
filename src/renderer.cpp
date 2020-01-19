#include "renderer.hpp"
#include <iostream>

void GLClearError() {
  while( glGetError() != GL_NO_ERROR );
}

bool GLLogCall( const char* function, const char* file, int line ) {
  while( GLenum error = glGetError() ) {
    std::cout << "[OpenGL error] (" << error << ")\n" << function << "\n" << file << "\n" << line << std::endl;
    return false;
  }
  return true;
}

void Renderer::clear() const {
  glClear( GL_COLOR_BUFFER_BIT );
}

void Renderer::draw( const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader ) const {
  shader.bind();
  //shader.setUniform4f( "u_Colour", glm::vec4{ r, 0.3f, 0.8f, 1.0f } ); // this isn't part of the vao code
  vb.bind();
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, 0 );
  ib.bind();
  
  GLCall( glDrawElements( GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr ) );
}