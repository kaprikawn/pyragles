#include "indexBuffer.hpp"
#include <GLES2/gl2.h>
#include "glCallLog.hpp"

IndexBuffer::IndexBuffer() {
  
}

void IndexBuffer::init( const void* data, unsigned int count ) {
  count_ = count;
  GLCall( glGenBuffers( 1, &rendererID_ ) );
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rendererID_ ) );
  GLCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), data, GL_STATIC_DRAW ) );
}

void IndexBuffer::bind() const {
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, rendererID_ ) );
}

void IndexBuffer::unbind() const {
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
}

IndexBuffer::~IndexBuffer() {
  GLCall( glDeleteBuffers( 1, &rendererID_ ) );
}
