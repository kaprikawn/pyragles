#include "gameObject.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject() {
  modelMatrix_ = glm::mat4( 1.0f );
}

void GameObject::loadVertexData( const void* data, unsigned int size ) {
  vb_ = VertexBuffer();
  vb_.init( data, size );
}

void GameObject::loadIndexData( const unsigned int* data, unsigned int count ) {
  ib_ = IndexBuffer();
  ib_.init( data, count );
}

void GameObject::loadShader( const std::string& filename ) {
  
}

void GameObject::init( std::string modelName, std::shared_ptr<Camera> camera ) {
  
  gltf_ = std::make_unique<Gltf>();
  gltf_ -> init( modelName );
  
  loadVertexData( gltf_ -> vertexData(), gltf_ -> vertexDataSize() );
  
  loadIndexData( gltf_ -> indexData(), gltf_ -> indexCount() );
  
  shader_ = Shader();
  shader_.init( "basic.glsl" );
  
  // https://www.raywenderlich.com/3047-opengl-es-2-0-for-iphone-tutorial-part-2-textures
  texture_ = Texture();
  texture_.init( gltf_ -> textureData(), gltf_ -> textureWidth(), gltf_ -> textureHeight() );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( texCoordID_ );
  
}

void GameObject::update() {
  
}

void GameObject::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform1i( "uTexture", 0 );
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  texture_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, ( GLvoid* ) 0 );
  glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 5, ( GLvoid* )( sizeof( float ) * 3 ) );
  ib_.bind();
    
  glDrawElements( GL_TRIANGLES, gltf_ -> indexCount(), GL_UNSIGNED_INT, 0 );
  
}

GameObject::~GameObject() {
  
}