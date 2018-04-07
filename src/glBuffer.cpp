#include "glBuffer.hpp"
#include <iostream>

GlBuffer::GlBuffer( GLsizeiptr vertexBufferSize, GLsizeiptr indexBufferSize, GLenum usage ) {
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, vertexBufferSize, 0, usage );
  
  glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, 0, usage );
  
}

GLsizeiptr GlBuffer::addVertexBufferData( GLsizeiptr size, const GLvoid * data ) {
  
  ret_ = vertexOffset_; // to tell the caller where the data is stored
  
  glBufferSubData( GL_ARRAY_BUFFER, vertexOffset_, size, data );
  
  vertexOffset_ += size;
  
  return ret_;
}

GLsizeiptr GlBuffer::addIndexBufferData( GLsizeiptr size, const GLvoid * data ) {
  
  ret_ = indexOffset_; // to tell the caller where the data is stored
  
  glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, indexOffset_, size, data );
  
  indexOffset_ += size;
  
  return ret_;
}

