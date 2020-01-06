#include "gameObject.hpp"
#include <iostream>


GameObject::GameObject() {
  
}

void GameObject::loadGltf( const std::string filename ) {
  
  gltf_ = std::make_unique<Gltf>( filename );
  
  vb_ = std::make_shared<VertexBuffer>();
  vb_ -> init( gltf_ -> vertexData(), gltf_ -> vertexBufferSize() );
  
  ib_ = std::make_shared<IndexBuffer>();
  ib_ -> init( gltf_ -> indexBufferData(), gltf_ -> indexBufferCount() );
  
  shader_ = std::make_shared<Shader>();
  shader_ -> init( "basic.glsl" );
  
}

void GameObject::draw( std::shared_ptr<Renderer> renderer ) {
  
  renderer -> draw( vb_, ib_, shader_ );
  
}

GameObject::~GameObject() {
  
}
