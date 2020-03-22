#include "hud.hpp"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "inputHandler.hpp"
#include "camera.hpp"

// http://www.mbsoftworks.sk/tutorials/opengl4/009-orthographic-2D-projection/inclue

Hud::Hud() {
  
  windowWidth_  = Camera::Instance() -> windowWidthF();
  windowHeight_ = Camera::Instance() -> windowHeightF();
  
  int indices[ 6 ] = { 0, 1, 2, 2, 3, 0 };
  
  vb_.init( nullptr, sizeof( float ) * 16, GL_DYNAMIC_DRAW );
  loadIndexData( indices, 6 );
  loadShader( "shaderHud.glsl" );
  
  updateBombCount( 9 );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( positionID_ ) );
  GLCall( glEnableVertexAttribArray( texCoordID_ ) );
  
  proj_ = glm::ortho( 0.0f, Camera::Instance() -> windowWidthF(), 0.0f, Camera::Instance() -> windowHeightF(), -1.0f, 1.0f );
  
  texture_ = Texture();
  texture_.initFromPNG( "hudNumbers.png" );
  
}

void Hud::updateBombCount( int bombCount ) {
  
  if( bombCount > 9 )
    bombCount = 9;
  if( bombCount < 0 )
    bombCount = 0;
  
  float left  = ( float ) bombCount / 10.0f; // how far from the left of the texture we sample from
  float right = ( ( float ) bombCount + 1.0f ) / 10.0f;
  
  // bomb hud positioning
  float distanceFromLeft    = Camera::Instance() -> windowWidthF() / 25.6f;
  float distanceFromBottom  = Camera::Instance() -> windowHeightF() / 14.40f;
  float bombHudSize         = Camera::Instance() -> windowHeightF() / 14.4f;
  
  vertexData_= {
      distanceFromLeft, distanceFromBottom, left , 0.0f // bottom left
    , distanceFromLeft + bombHudSize, distanceFromBottom, right, 0.0f // bottom right
    , distanceFromLeft + bombHudSize, distanceFromBottom + bombHudSize, right, 1.0f // top right
    , distanceFromLeft, distanceFromBottom + bombHudSize, left , 1.0f // top left
  };
  
  vb_.loadBufferData( &vertexData_[ 0 ], sizeof( vertexData_[ 0 ] ) * vertexData_.size() );
  
}

void Hud::update( unsigned short int bombCount ) {
  
  if( bombCount == prevBombCount_ )
    return;
    
  prevBombCount_ = bombCount;
  updateBombCount( bombCount );
  
}

void Hud::render() {
  
  glDisable( GL_DEPTH_TEST );
    
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&proj_ );
  
  vb_.bind();
  texture_.bind();
  GLCall( glVertexAttribPointer( positionID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, ( GLvoid* )0 ) );
  GLCall( glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, ( GLvoid* )( sizeof( float ) * 2 ) ) );
  ib_.bind();
    
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
  
  glEnable( GL_DEPTH_TEST );
  
}

Hud::~Hud() {
  
}
