#include "glWindow.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include "game.hpp"
#include "shader.hpp"

int GlWindow::init() {

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
  
  glClearColor( 0.0f, 0.0f, 0.4f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  glViewport( 0, 0, windowWidth, windowHeight );
  
  glUseProgram( programID_ ); // use the program object
  
  positionID_ = glGetAttribLocation( programID_,  "aPosition" );
  colourID_   = glGetAttribLocation( programID_,  "aColour" );
  mvpID_      = glGetUniformLocation( programID_, "uMVP" );
  
  shape_ = std::make_unique<Shape>( CUBE ); // get a cube (verts, indices etc.)
  
  glGenBuffers( 1, &vbo_ );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
  glBufferData( GL_ARRAY_BUFFER, shape_ -> vertexBufferSize(), shape_ -> getVertexDataPointer(), GL_STATIC_DRAW );
  
  glGenBuffers( 1, &ibo_ );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, shape_ -> indexBufferSize(), shape_ -> getIndexDataPointer(), GL_STATIC_DRAW );
  
  return 0;
}

void GlWindow::update( float dt ) {
  
  glUseProgram( programID_ );
  
  projection_ = glm::perspective( glm::radians( 45.0f ), windowWidth / windowHeight, 0.1f, 100.0f );
  
  /*view_ = glm::lookAt(
      glm::vec3( 0, 0, 3 )
    , glm::vec3( 0, 0, 0 )
    , glm::vec3( 0, 1, 0 )
  );*/
  
  model_ = glm::translate( projection_, glm::vec3( 0.0f, 0.0f, -10.0f ) );
  
  mvp_ = glm::rotate( model_, glm::radians( 30.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  
  //mvp_ = projection_ * model_ * rotation_;
  
  // Send our transformation to the currently bound shader, 
  // in the "MVP" uniform
  glUniformMatrix4fv( mvpID_, 1, GL_FALSE, &mvp_[0][0] );
  
  // load the vertex data
  glEnableVertexAttribArray( positionID_ );
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), ( GLvoid* ) 0 );
  
  // load the colour data
  glEnableVertexAttribArray( colourID_ );
  glVertexAttribPointer( colourID_, 3, GL_FLOAT, GL_FALSE, shape_ -> getStride(), shape_ -> colorOffset() );
  
}

void GlWindow::render() {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  glDrawElements( GL_TRIANGLES, shape_ -> numIndices(), GL_UNSIGNED_INT, 0 );
  
  SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}

