#include "floor.hpp"
#include <iostream>

Floor::Floor() {
  
  float data[ 24 ] = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    ,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    , -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    ,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f
  };
  
  vb_ = VertexBuffer();
  vb_.init( ( const void* )data, sizeof( data ) );
  
  const unsigned int indices[ 6 ] = { 0, 1, 2, 1, 2, 3 };
  
  ib_ = IndexBuffer();
  ib_.init( indices, 6 );
  
  Floor::loadShader( "shaderVertexColours.glsl" );

}

void Floor::loadVertexData() {
  
  
  
}

void Floor::loadIndexData() {
  
  
}

void Floor::loadShader( const std::string& fileaname ) {
  
}

Floor::~Floor() {
  
}
