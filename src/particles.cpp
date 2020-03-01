#include "particles.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "glCallLog.hpp"

Particles::Particles() {
  
  loadShader( "shaderParticles.glsl" );
  
  vb_.init( nullptr, 1024, GL_DYNAMIC_DRAW );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  //colourID_   = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  //glEnableVertexAttribArray( colourID_ );
  
  myPoint_ = { 0.0f, 1.0f, -5.0f, -1.0f, -1.0f, -5.0f, 1.0f, -1.0f, -5.0f };
  
  modelMatrix_ = glm::mat4( 1.0f );
  
}

void Particles::update( float dt ) {
  
  myPoint_[ 0 ] += 2.0f * dt;
  myPoint_[ 3 ] += 2.0f * dt;
  myPoint_[ 6 ] += 2.0f * dt;
  
  unsigned int size = sizeof( myPoint_[ 0 ] ) * 9;
  size = sizeof( myPoint_[ 0 ] ) * 3;
  
  vb_.loadBufferData( &myPoint_[ 0 ], size );
  
}

void Particles::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  //glDrawArrays( GL_TRIANGLES, 0, 3 );
  glDrawArrays( GL_POINTS, 0, 1 );
  
}

Particles::~Particles() {
  
}

