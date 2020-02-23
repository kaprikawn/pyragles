#include "floor.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Floor::Floor() {}
Floor::~Floor() {}

bool Floor::init( std::string modelName ) {
  
  gltf_ = std::make_unique<Gltf>();
  bool gltfLoaded = gltf_ -> init( modelName );
  if( !gltfLoaded )
    return false;
  
  loadVertexData( gltf_ -> vertexData(), gltf_ -> vertexDataSize() );
  loadIndexData( gltf_ -> indexData(), gltf_ -> indexCount() );
  loadShader( "shaderVertexColours.glsl" );
  
  // https://www.raywenderlich.com/3047-opengl-es-2-0-for-iphone-tutorial-part-2-textures
  texture_ = Texture(); // TODO : replace this with a loadTextureData function
  texture_.init( gltf_ -> textureData(), gltf_ -> textureWidth(), gltf_ -> textureHeight() );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  normalID_   = glGetAttribLocation( shader_.rendererID(),  "aNormal" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( normalID_ );
  glEnableVertexAttribArray( texCoordID_ );
  
  return true;
}

void Floor::update( float dt ) {
  
  yAngle_ += dt * 100;
  if( yAngle_ > 360 )
    yAngle_ -= 360;
    
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  modelMatrix_ = glm::mat4( 1.0f );
  modelMatrix_ *= rotationMatrix_;
  
}

void Floor::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform1i( "uTexture", 0 );
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  texture_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* ) 0 );
  glVertexAttribPointer( normalID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 3 ) );
  glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 6 ) );
  ib_.bind();
    
  glDrawElements( GL_TRIANGLES, gltf_ -> indexCount(), GL_UNSIGNED_INT, 0 );
  
}
/*
Floor::Floor() {
  
  float data[ 28 ] = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    ,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    , -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    ,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
  };
  
  const unsigned int indices[ 6 ] = { 0, 1, 2, 1, 2, 3 };
  
  loadVertexData( ( const void* )data, sizeof( data ) );
  loadIndexData( indices, 6 );
  loadShader( "shaderVertexColours.glsl" );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  colourID_   = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( colourID_ );
  
  modelMatrix_ = glm::mat4( 1.0f );
}

bool Floor::init() {
  return true;
}

void Floor::update( float dt ) {
  
}

void Floor::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  texture_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( GLvoid* ) 0 );
  glVertexAttribPointer( colourID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( GLvoid* )( sizeof( float ) * 3 ) );
  ib_.bind();
    
  glDrawElements( GL_TRIANGLES, gltf_ -> indexCount(), GL_UNSIGNED_INT, 0 );
  
}

Floor::~Floor() {
  
}
*/