#include "gameObject.hpp"
#include <iostream>


GameObject::GameObject() {
  modelMatrix_ = glm::mat4( 1.0f );
}

void GameObject::loadVertexData( const void* data, unsigned int size ) {
  vb_ = std::make_shared<VertexBuffer>();
  vb_ -> init( data, size );
}

void GameObject::loadIndexData( const void* data, unsigned int count ) {
  ib_ = std::make_shared<IndexBuffer>();
  ib_ -> init( data, count );
}

void GameObject::loadShader( const std::string& filename ) {
  shader_ = std::make_shared<Shader>();
  shader_ -> init( filename );
}

void GameObject::loadGltf( const std::string filename ) {
  gltf_ = std::make_unique<Gltf>( filename );
}

void GameObject::update( float dt ) {
  
  
  
}

void GameObject::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_ -> setUniform4fv( "uMVP", &mvp_[ 0 ][ 0 ] );
  
  //renderer -> draw( vb_, ib_, shader_ );
  
}

GameObject::~GameObject() {
  
}
