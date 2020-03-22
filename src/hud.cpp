#include "hud.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>
#include "inputHandler.hpp"
#include "camera.hpp"
#include "glCallLog.hpp"

// http://www.mbsoftworks.sk/tutorials/opengl4/009-orthographic-2D-projection/inclue

Hud::Hud() {
  
  healthVb_      = VertexBuffer();
  bombsVb_       = VertexBuffer();
  healthShader_  = Shader();
  bombsShader_   = Shader();
  ib_            = IndexBuffer();
  
  windowWidth_  = Camera::Instance() -> windowWidthF();
  windowHeight_ = Camera::Instance() -> windowHeightF();
  
  int indices[ 6 ] = { 0, 1, 2, 2, 3, 0 };
  ib_.init( indices, 6 );
  indexCount_ = ib_.getCount();
  
  // bombs counter
  bombsVb_.init( nullptr, sizeof( float ) * 16, GL_DYNAMIC_DRAW );
  bombsShader_.init( "shaderHud.glsl" );
  bombsPositionID_  = glGetAttribLocation( bombsShader_.rendererID(),  "aPosition" );
  bombsTexCoordID_  = glGetAttribLocation( bombsShader_.rendererID(),  "aTexCoord" );
  bombsMvpID_       = glGetUniformLocation( bombsShader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( bombsPositionID_ ) );
  GLCall( glEnableVertexAttribArray( bombsTexCoordID_ ) );
  
  updateBombCount( 9 );
  
  texture_ = Texture();
  texture_.initFromPNG( "hudNumbers.png" );
  
  // health
  healthVb_.init( nullptr, sizeof( float ) * 24, GL_DYNAMIC_DRAW );
  healthShader_.init( "shaderVertexColours.glsl" );
  healthPositionID_  = glGetAttribLocation( healthShader_.rendererID(),  "aPosition" );
  healthColourID_    = glGetAttribLocation( healthShader_.rendererID(),  "aColour" );
  healthMvpID_       = glGetUniformLocation( healthShader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( healthPositionID_ ) );
  GLCall( glEnableVertexAttribArray( healthColourID_ ) );
  
  updateHealthBar( targetHealth_ );
  targetHealth_ = 1;
}

void Hud::updateBombCount( unsigned short int bombCount ) {
  
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
  
  std::vector<float> vertexData = {
      distanceFromLeft              , distanceFromBottom              , left , 0.0f // bottom left
    , distanceFromLeft + bombHudSize, distanceFromBottom              , right, 0.0f // bottom right
    , distanceFromLeft + bombHudSize, distanceFromBottom + bombHudSize, right, 1.0f // top right
    , distanceFromLeft              , distanceFromBottom + bombHudSize, left , 1.0f // top left
  };
  
  bombsVb_.loadBufferData( &vertexData[ 0 ], sizeof( vertexData[ 0 ] ) * vertexData.size() );
  
}

void Hud::update( float dt, unsigned short int bombCount ) {
  
  if( displayedHealth_ > ( float ) targetHealth_ ) {
    displayedHealth_ -= 50.0f * dt;
    if( displayedHealth_ < ( float ) targetHealth_ )
      displayedHealth_ = ( float ) targetHealth_;
    
    updateHealthBar( displayedHealth_ );
  }
  
  if( bombCount == prevBombCount_ )
    return;
    
  prevBombCount_ = bombCount;
  updateBombCount( bombCount );
  
}

void Hud::updateHealthBar( float displayedHealth ) {
  
  float decimal = displayedHealth / 100.0f;
  
  // https://coderwall.com/p/dvsxwg/smoothly-transition-from-green-to-red
  float hue = 120.0f * decimal * 0.8f;
  glm::vec3 rgb = glm::rgbColor( glm::vec3( hue, 1.0f, 1.0f ) );
  
  float distanceFromLeft    = Camera::Instance() -> windowWidthF() / 10.0f;
  float distanceFromBottom  = Camera::Instance() -> windowHeightF() / 14.40f;
  float height              = Camera::Instance() -> windowHeightF() / 14.4f;
  float width               = 150.0f * decimal;
  
  std::vector<float> vertexData = {
      distanceFromLeft        , distanceFromBottom          , rgb.r, rgb.g, rgb.b, 1.0f
    , distanceFromLeft + width, distanceFromBottom          , rgb.r, rgb.g, rgb.b, 1.0f
    , distanceFromLeft + width, distanceFromBottom + height , rgb.r, rgb.g, rgb.b, 1.0f
    , distanceFromLeft        , distanceFromBottom + height , rgb.r, rgb.g, rgb.b, 1.0f
  };
  
  healthVb_.loadBufferData( &vertexData[ 0 ], sizeof( vertexData[ 0 ] ) * vertexData.size() );
  
}

void Hud::render() {
  
  glm::mat4 proj = glm::ortho( 0.0f, Camera::Instance() -> windowWidthF(), 0.0f, Camera::Instance() -> windowHeightF(), -1.0f, 1.0f );
  
  glDisable( GL_DEPTH_TEST );
  
  // bombs counter
  bombsShader_.bind();
  bombsShader_.setUniform4fv( "uMVP", ( const float* )&proj );
  
  bombsVb_.bind();
  texture_.bind();
  GLCall( glVertexAttribPointer( bombsPositionID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, ( GLvoid* )0 ) );
  GLCall( glVertexAttribPointer( bombsTexCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, ( GLvoid* )( sizeof( float ) * 2 ) ) );
  ib_.bind();
    
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
  
  // health
  healthShader_.bind();
  healthShader_.setUniform4fv( "uMVP", ( const float* )&proj );
  healthVb_.bind();
  GLCall( glVertexAttribPointer( healthPositionID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( GLvoid* )0 ) );
  GLCall( glVertexAttribPointer( healthColourID_, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( GLvoid* )( sizeof( float ) * 2 ) ) );
  
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
  
  glEnable( GL_DEPTH_TEST );
  
}

Hud::~Hud() {
  
}
