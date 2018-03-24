#include <iostream>
#include "glObject.hpp"
#include "shader.hpp"
#include "inputHandler.hpp"
#include "camera.hpp"

GlObject::GlObject() : velocity_( 0, 0, 0 ), coordinates_( 0, 0, 0 ) {
   GlObject::init();
}

int GlObject::init() {
  
  GLuint  vs;
  GLuint  fs;
  
  std::string vsSrcStr = getFile( "./shaders/triangle.vs" );
  std::string fsSrcStr = getFile( "./shaders/triangle.fs" );
  
  const GLchar* vsSrc;
  const GLchar* fsSrc;
  vsSrc = vsSrcStr.c_str();
  fsSrc = fsSrcStr.c_str();
  
  // load the vertex / fragment shaders
  vs = loadShader( vsSrc, GL_VERTEX_SHADER );
  fs = loadShader( fsSrc, GL_FRAGMENT_SHADER );
  
  // create the program object
  programID_ = glCreateProgram();
  
  if( programID_ == 0 ) {
    std::cout << "No programID" << std::endl;
    return 0;
  }
  
  glAttachShader( programID_, vs );
  glAttachShader( programID_, fs );
  
  glLinkProgram( programID_ ); // link the program
  
  checkShaderError( programID_, GL_LINK_STATUS, true, "Error linking shader program" ); // check the link status
  
  glUseProgram( programID_ ); // use the program object
  
  positionID_ = glGetAttribLocation( programID_,  "aPosition" );
  colourID_   = glGetAttribLocation( programID_,  "aColour" );
  mvpID_      = glGetUniformLocation( programID_, "uMVP" );
  
  shape_ = std::make_unique<Shape>( SHIP );
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, shape_ -> vertexBufferSize(), shape_ -> getVertexDataPointer(), GL_STATIC_DRAW );
  
  glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, shape_ -> indexBufferSize(), shape_ -> getIndexDataPointer(), GL_STATIC_DRAW );
  
  // load the vertex data
  glEnableVertexAttribArray( positionID_ );
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), ( GLvoid* ) 0 );
  
  // load the colour data
  glEnableVertexAttribArray( colourID_ );
  glVertexAttribPointer( colourID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), shape_ -> colorOffset() );
  
  return 0;
}

void GlObject::update( float dt ) {
  
  glUseProgram( programID_ );
  
  
  
}

void GlObject::render() {
  
  
  
  mvp_ = TheCamera::Instance() -> projectionMatrix() * TheCamera::Instance() -> viewMatrix() * model_ * rotation_;
  glUniformMatrix4fv( mvpID_, 1, GL_FALSE, &mvp_[0][0] );
  
  glDrawElements( GL_TRIANGLES, shape_ -> numIndices(), GL_UNSIGNED_INT, 0 );
  
}

void GlObject::clean() {

}
