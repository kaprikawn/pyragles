#include "renderer.hpp"
#include <iostream>
#include "camera.hpp"
#include "global.hpp"

Renderer::Renderer( GLuint programID, std::shared_ptr<Camera> camera ) {
  
  glUseProgram( programID );
  glViewport( 0, 0, camera -> windowWidth(), camera -> windowHeight() );
  
  positionID_ = glGetAttribLocation( programID,  "aPosition" );
  colourID_   = glGetAttribLocation( programID,  "aColour" );
  mvpID_      = glGetUniformLocation( programID, "uMVP" );
  
  camera_     = camera;
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( colourID_ );
}

void Renderer::generateBuffer( GLsizeiptr vertexBufferSize, GLsizeiptr indexBufferSize, GLenum usage ) {
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, vertexBufferSize, 0, GL_STATIC_DRAW );
  
  glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, 0, GL_STATIC_DRAW );
}

/* GLsizeiptr Renderer::addBufferData( int bufferType, GLsizeiptr size, const GLvoid* data ) {
  
  GLsizeiptr thisOffet = 0;
  
  if(  bufferType == VERTEX ) {
    thisOffet = vertexOffset_;
    glBufferSubData( GL_ARRAY_BUFFER, thisOffet, size, data );
    vertexOffset_ += size;
  } else if( bufferType == INDEX ) {
    thisOffet = indexOffset_;
    glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, thisOffet, size, data );
    indexOffset_ += size;
  }
  
  return thisOffet;
} */

void Renderer::addBufferData( std::shared_ptr<MeshLoader> meshLoader ) {
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( meshLoader -> loadShape( i ) ) {
      
      GLintptr offset = meshLoader -> vertexOffset( i );
      GLsizeiptr size = meshLoader -> vertexSize( i );
      
      glBufferSubData( GL_ARRAY_BUFFER, offset, size, meshLoader -> vertexData( i ) );
      
      offset        = meshLoader -> indexOffset( i );
      size          = meshLoader -> indexSize( i );
      
      glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, offset, size, meshLoader -> indexData( i ) );
    }
  }
}

void Renderer::renderObject( GLsizeiptr vertexOffset, GLsizeiptr indexOffset, GLsizei numIndices, glm::mat4 mvp ) {
  
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)vertexOffset );
  
  glVertexAttribPointer( colourID_, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( vertexOffset + sizeof( GLfloat ) * 3 ) );
  
  //glm::mat4 mvp = camera_ -> viewProjectionMatrix() * modelMatrix;
  
  glUniformMatrix4fv( mvpID_, 1, GL_FALSE, &mvp[0][0] );
  
  glDrawElements( GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (void*)indexOffset );
}
