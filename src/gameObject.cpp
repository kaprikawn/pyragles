#include "gameObject.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject() {
  std::cout << "calling gameobject constructor\n";
  modelMatrix_  = glm::mat4( 1.0f );
  vb_ = VertexBuffer();
  ib_ = IndexBuffer();
  shader_ = Shader();
  model_ = std::make_unique<Model>();
  
  
}

void GameObject::loadVertexData( const void* data, unsigned int size ) {
  vb_.init( data, size );
}

void GameObject::loadIndexData( const unsigned int* data, unsigned int count ) {
  ib_.init( data, count );
  indexCount_ = count;
}

void GameObject::loadShader( const std::string& filename ) {
  shader_.init( filename );
}

bool GameObject::loadModelFromGltf( std::string modelName ) {
  
  bool modelLoaded = model_ -> loadFromGltf( modelName );
  if( !modelLoaded )
    return false;
  
  indexCount_ = model_ -> indexCount();
  
  loadVertexData( model_ -> vertexData(), model_ -> vertexDataSize() );
  loadIndexData( model_ -> indexData(), indexCount_ );
  loadShader( "shaderBasic.glsl" );
  
  shader_ = Shader();
  shader_.init( "shaderBasic.glsl" );
  
  // https://www.raywenderlich.com/3047-opengl-es-2-0-for-iphone-tutorial-part-2-textures
  texture_ = Texture();
  texture_.init( model_ -> textureData(), model_ -> textureWidth(), model_ -> textureHeight() );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  normalID_   = glGetAttribLocation( shader_.rendererID(),  "aNormal" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( normalID_ );
  glEnableVertexAttribArray( texCoordID_ );
  
  return true;
}

void GameObject::update( float dt ) {
  
  yAngle_ += dt * 100;
  if( yAngle_ > 360 )
    yAngle_ -= 360;
    
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  modelMatrix_ = glm::mat4( 1.0f );
  modelMatrix_ *= rotationMatrix_;
  
}

void GameObject::render( glm::mat4 viewProjectionMatrix ) {
  
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
    
  glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 );
  
}

GameObject::~GameObject() {
  
}