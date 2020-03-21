#include "hud.hpp"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"
#include "inputHandler.hpp"

// http://www.mbsoftworks.sk/tutorials/opengl4/009-orthographic-2D-projection/inclue

Hud::Hud() {
  
  windowWidth_  = Camera::Instance() -> windowWidthF();
  windowHeight_ = Camera::Instance() -> windowHeightF();
  
  vertexData_= {
      100.0f, 100.0f, 0.5f, 0.0f // bottom left
    , 200.0f, 100.0f, 1.0f, 0.0f // bottom right
    , 200.0f, 200.0f, 1.0f, 1.0f // top right
    , 100.0f, 200.0f, 0.5f, 1.0f // top left
  };
  
  int indices[ 6 ] = { 0, 1, 2, 2, 3, 0 };
  
  loadVertexData( &vertexData_[ 0 ], sizeof( vertexData_[ 0 ] ) * vertexData_.size(), GL_DYNAMIC_DRAW );
  loadIndexData( indices, 6 );
  loadShader( "shaderHud.glsl" );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( positionID_ ) );
  GLCall( glEnableVertexAttribArray( texCoordID_ ) );
  
  proj_ = glm::ortho( 0.0f, Camera::Instance() -> windowWidthF(), 0.0f, Camera::Instance() -> windowHeightF(), -1.0f, 1.0f );
  
  texture_ = Texture();
  texture_.initFromPNG( "hudNumbersA.png" );
  
}

void Hud::decreaseBombCount() {
  
  std::cout << "letting off bomb\n";
  
  vertexData_= {
      100.0f, 100.0f, 0.0f, 0.0f // bottom left
    , 200.0f, 100.0f, 0.5f, 0.0f // bottom right
    , 200.0f, 200.0f, 0.5f, 1.0f // top right
    , 100.0f, 200.0f, 0.0f, 1.0f // top left
  };
  
  vb_.loadBufferData( &vertexData_[ 0 ], sizeof( vertexData_[ 0 ] ) * vertexData_.size() );
  
}

void Hud::update() {
  
  if( InputHandler::Instance() -> justPressed( BOMB ) ) {
    decreaseBombCount();
  }
  
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
