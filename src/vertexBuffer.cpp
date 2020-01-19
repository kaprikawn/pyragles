#include "vertexBuffer.hpp"
#include <iostream>
#include "renderer.hpp"

VertexBuffer::VertexBuffer() {
  
}

void VertexBuffer::init( const void* data, unsigned int size ) {
  GLCall( glGenBuffers( 1, &rendererID_ ) );
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, rendererID_ ) );
  GLCall( glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW ) );
}

void VertexBuffer::bind() const {
  glBindBuffer( GL_ARRAY_BUFFER, rendererID_ );
}

void VertexBuffer::unbind() const {
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

VertexBuffer::~VertexBuffer() {
  GLCall( glDeleteBuffers( 1, &rendererID_ ) );
}
