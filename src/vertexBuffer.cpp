#include "vertexBuffer.hpp"
#include <iostream>
#include "glCallLog.hpp"

VertexBuffer::VertexBuffer() {
  
}

void VertexBuffer::init( const void* data, unsigned int size, GLenum usage ) {
  GLCall( glGenBuffers( 1, &rendererID_ ) );
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, rendererID_ ) );
  GLCall( glBufferData( GL_ARRAY_BUFFER, size, data, usage ) );
}

void VertexBuffer::bind() const {
  glBindBuffer( GL_ARRAY_BUFFER, rendererID_ );
}

void VertexBuffer::loadBufferData( const void* data ) const {
  glBindBuffer( GL_ARRAY_BUFFER, rendererID_ );
  GLCall( glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 9, data, GL_DYNAMIC_DRAW ) );
  //glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( float ) * 3, data );
}

void VertexBuffer::unbind() const {
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

VertexBuffer::~VertexBuffer() {
  GLCall( glDeleteBuffers( 1, &rendererID_ ) );
}
