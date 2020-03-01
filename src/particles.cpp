#include "particles.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "glCallLog.hpp"

Particles::Particles() {
  
  GLCall( glGenBuffers( 1, &rendererID_ ) );
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, rendererID_ ) );
  GLCall( glBufferData( GL_ARRAY_BUFFER, 1024, nullptr, GL_DYNAMIC_DRAW ) );
  
}

void Particles::update( float dt ) {
  
  // for( unsigned int i = 0; i < particlePool_.size(); i++ ) {
  //   if( particlePool_[ i ].active ) {
      
  //   }
  // }
  
}

void Particles::render( glm::mat4 viewProjectionMatrix ) {
  
}

Particles::~Particles() {
  
}

