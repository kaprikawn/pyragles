#include "floor.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Floor::Floor() {
  
  std::vector<float> vertices = {
      -100.0f, -0.1f, -100.0f, 0.87f, 0.87f, 0.623f, 0.129f
    , -100.0f, -0.1f,   10.0f, 0.87f, 0.87f, 0.623f, 0.129f 
    ,  100.0f, -0.1f,   10.0f, 0.87f, 0.87f, 0.623f, 0.129f
    ,  100.0f, -0.1f, -100.0f, 0.87f, 0.87f, 0.623f, 0.129f
  };
  
  std::vector<int> indices = {
      0, 1, 2 , 2 , 3 , 0
  };
  
  unsigned int indicesOffset = 4;
  
  for( int x = -100; x < 100; x += 4 ) {
    for( int z = -100; z < 10; z += 4 ) {
      vertices.push_back( ( float )x );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )z );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      vertices.push_back( ( float )x );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )( z + 2 ) );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      vertices.push_back( ( float )( x + 2 ) );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )( z + 2 ) );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      vertices.push_back( ( float )( x + 2 ) );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )z );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      indices.push_back( indicesOffset );
      indices.push_back( ( indicesOffset + 1 ) );
      indices.push_back( ( indicesOffset + 2 ) );
      indices.push_back( ( indicesOffset + 2 ) );
      indices.push_back( ( indicesOffset + 3 ) );
      indices.push_back( indicesOffset );
      
      indicesOffset += 4;
      
    }
  }
  
  for( int x = -102; x < 100; x += 4 ) {
    for( int z = -102; z < 10; z += 4 ) {
      vertices.push_back( ( float )x );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )z );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      vertices.push_back( ( float )x );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )( z + 2 ) );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      vertices.push_back( ( float )( x + 2 ) );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )( z + 2 ) );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      vertices.push_back( ( float )( x + 2 ) );
      vertices.push_back( 0.0f );
      vertices.push_back( ( float )z );
      vertices.push_back( 0.87f );
      vertices.push_back( 0.733f );
      vertices.push_back( 0.129f );
      vertices.push_back( 1.0f );
      
      indices.push_back( indicesOffset );
      indices.push_back( ( indicesOffset + 1 ) );
      indices.push_back( ( indicesOffset + 2 ) );
      indices.push_back( ( indicesOffset + 2 ) );
      indices.push_back( ( indicesOffset + 3 ) );
      indices.push_back( indicesOffset );
      
      indicesOffset += 4;
      
    }
  }
  
  unsigned int sizeOfVertices = sizeof( vertices[0] ) * vertices.size();
  
  loadVertexData( ( const void* )&vertices[0], sizeOfVertices );
  loadIndexData( ( const void* )&indices[0], indices.size() );
  loadShader( "shaderVertexColours.glsl" );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  colourID_   = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( colourID_ );
}


void Floor::update( float dt ) {
  modelMatrix_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, zPos_ ) );
  zPos_ += dt * SCROLL_SPEED;
  while( zPos_ > 4 )
    zPos_ -= 4;
}

void Floor::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( GLvoid* ) 0 ); 
  glVertexAttribPointer( colourID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( GLvoid* )( sizeof( float ) * 3 ) );// TODO is the last parameter needed?
  ib_.bind();
  
  glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 );
  
}

Floor::~Floor() {
  
}
