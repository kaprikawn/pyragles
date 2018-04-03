#include <iostream>
#include "glObject.hpp"
#include "inputHandler.hpp"
#include "camera.hpp"

GlObject::GlObject( int shapeType, GLfloat x, GLfloat y, GLfloat z, GLuint programID ) : velocity_( 0, 0, 0 ), position_( x, y, z ) {
   GlObject::init( shapeType, programID );
}

int GlObject::init( int shapeType, GLuint programID ) {
  
  positionID_ = glGetAttribLocation( programID,  "aPosition" );
  colourID_   = glGetAttribLocation( programID,  "aColour" );
  mvpID_      = glGetUniformLocation( programID, "uMVP" );
  
  shape_ = std::make_unique<Shape>( shapeType );
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, shape_ -> vertexBufferSize(), shape_ -> getVertexDataPointer(), GL_STATIC_DRAW );
  
  glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, shape_ -> indexBufferSize(), shape_ -> getIndexDataPointer(), GL_STATIC_DRAW );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( colourID_ );
  
  return 0;
}

void GlObject::update( float dt ) {
  
}

void GlObject::render() {
  
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), ( GLvoid* ) 0 );
  glVertexAttribPointer( colourID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), shape_ -> colorOffset() );
  
  mvp_ = TheCamera::Instance() -> projectionMatrix() * TheCamera::Instance() -> viewMatrix() * model_;
  glUniformMatrix4fv( mvpID_, 1, GL_FALSE, &mvp_[0][0] );
  
  glDrawElements( GL_TRIANGLES, shape_ -> numIndices(), GL_UNSIGNED_INT, 0 );
}

void GlObject::clean() {

}
