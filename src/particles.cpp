#include "particles.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "glCallLog.hpp"

Particles::Particles() {
  
  loadShader( "shaderParticles.glsl" );
  
  vb_.init( nullptr, 10340, GL_DYNAMIC_DRAW );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  colourID_   = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( colourID_ );
  
  modelMatrix_ = glm::mat4( 1.0f );
  
  for( unsigned int i = 0; i < PARTICLE_COUNT; i++ ) {
    Particle newParticle;
    if( i == 1 ) {
      newParticle.direction.x = 2.0f;
      newParticle.direction.y = 0.0f;
    }
    particles_[ i ] = newParticle;
  }
  
}

void Particles::update( float dt ) {
  
  std::vector<float> bufferData;
  
  for( unsigned int i = 0; i < PARTICLE_COUNT; i++ ) {
    
    particles_[ i ].colour.g -= ( 0.4f * dt );
    if( particles_[ i ].colour.g < 0.0f )
      particles_[ i ].active = false;
    
    if( !particles_[ i ].active )
      continue;
    
    glm::vec3 position = particles_[ i ].position;
    position += ( particles_[ i ].direction * dt );
    
    particles_[ i ].position = position;
    
    for( unsigned int j = 0; j < 108; j += 3 ) {
      
      bufferData.push_back( ( cubeVertices_[ j ] * particles_[ i ].scale ) + position.x );
      bufferData.push_back( ( cubeVertices_[ j + 1 ] * particles_[ i ].scale ) + position.y );
      bufferData.push_back( ( cubeVertices_[ j + 2 ] * particles_[ i ].scale ) + position.z );
      bufferData.push_back( particles_[ i ].colour.r );
      bufferData.push_back( particles_[ i ].colour.g );
      bufferData.push_back( particles_[ i ].colour.b );
      bufferData.push_back( particles_[ i ].colour.a );
      
    }
    
    particles_[ i ].scale += ( particles_[ i ].scaleIncrease * dt );
    
  }
  
  vb_.loadBufferData( &bufferData[ 0 ], sizeof( bufferData[ 0 ] ) * bufferData.size() );
  
}

void Particles::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, 0 );
  glVertexAttribPointer( colourID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( const void* )( sizeof( float ) * 3 ) );
  
  glDrawArrays( GL_TRIANGLES, 0, 36 * PARTICLE_COUNT );
  
}

Particles::~Particles() {
  
}