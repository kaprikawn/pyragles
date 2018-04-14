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
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( colourID_ );
  
  shape_ = std::make_unique<Shape>( shapeType );
  
  return 0;
}

void GlObject::update( float dt ) {
  
}

void GlObject::render() {
  
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), ( GLvoid* ) vertexOffset_ );
  glVertexAttribPointer( colourID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), colourOffset_ );
  
  mvp_ = TheCamera::Instance() -> projectionMatrix() * TheCamera::Instance() -> viewMatrix() * model_;
  glUniformMatrix4fv( mvpID_, 1, GL_FALSE, &mvp_[0][0] );
  
  glDrawElements( GL_TRIANGLES, shape_ -> numIndices(), GL_UNSIGNED_INT, (void*)indexOffset_ );
}

void GlObject::clean() {

}
