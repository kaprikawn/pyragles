#include "hud.hpp"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

// http://www.mbsoftworks.sk/tutorials/opengl4/009-orthographic-2D-projection/inclue

Hud::Hud() {
  
  windowWidth_  = Camera::Instance() -> windowWidthF();
  windowHeight_ = Camera::Instance() -> windowHeightF();
  
  float positions[ 6 ] = {
      -0.5f, -0.5f
    ,  0.0f,  0.5f
    ,  0.5f, -0.5f
  };
  
  int indices[ 3 ] = { 0, 1, 2 };
  
  loadVertexData( positions, sizeof( positions ) );
  loadIndexData( indices, 3 );
  loadShader( "shaderHud.glsl" );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( positionID_ ) );
  
  proj_ = glm::ortho( -2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f );
  
}

void Hud::update() {
  //modelMatrix_ = glm::translate( glm::mat4( 1.0f ), position_ );
}

void Hud::render() {
  
  glDisable( GL_DEPTH_TEST );
    
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&proj_ );
  
  vb_.bind();
  GLCall( glVertexAttribPointer( positionID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, 0 ) );
  ib_.bind();
    
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
  //glDrawArrays( GL_TRIANGLES, 0, 3 );
  
  glEnable( GL_DEPTH_TEST );
  
}

Hud::~Hud() {
  
}
