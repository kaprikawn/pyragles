#include "indexBuffer.hpp"
#include "renderer.hpp"

IndexBuffer::IndexBuffer() {
  
}

void IndexBuffer::init( const void* data, unsigned int count ) {
  count_ = count;
  GLCall( glGenBuffers( 1, &rendererID_ ) );
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rendererID_ ) );
  GLCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), data, GL_STATIC_DRAW ) );
}

void IndexBuffer::bind() const {
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rendererID_ );
}

void IndexBuffer::unbind() const {
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

IndexBuffer::~IndexBuffer() {
  GLCall( glDeleteBuffers( 1, &rendererID_ ) );
}
