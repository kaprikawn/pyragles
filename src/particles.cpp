#include "particles.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "glCallLog.hpp"

Particles::Particles() {
  
  loadShader( "shaderParticles.glsl" );
  
  vb_.init( nullptr, 1024, GL_DYNAMIC_DRAW );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  //colourID_   = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  //glEnableVertexAttribArray( colourID_ );
  
  modelMatrix_ = glm::mat4( 1.0f );
  
  for( unsigned int i = 0; i < PARTICLE_COUNT; i++ ) {
    Particle newParticle;
    particles_[ i ] = newParticle;
  }
  
}

void Particles::update( float dt ) {
  
  std::vector<float> bufferData;
  
  for( unsigned int i = 0; i < PARTICLE_COUNT; i++ ) {
    
    if( !particles_[ i ].active )
      continue;
    
    glm::vec3 position = particles_[ i ].position;
    position.x += 0 * dt;
    position.y += 2 * dt;
    position.z += 0 * dt;
    
    particles_[ i ].position = position;
    
    for( unsigned int j = 0; j < 108; j += 3 ) {
      float x = cubeVertices_[ j ]     * particles_[ i ].scale;
      float y = cubeVertices_[ j + 1 ] * particles_[ i ].scale;
      float z = cubeVertices_[ j + 2 ] * particles_[ i ].scale;
      
      x += position.x;
      y += position.y;
      z += position.z;
      
      
      bufferData.push_back( x );
      bufferData.push_back( y );
      bufferData.push_back( z );
    }
    
    particles_[ i ].scale += ( 0.1f * dt );
    
  }
  
  vb_.loadBufferData( &bufferData[ 0 ], sizeof( float ) * 108 );
  
}

void Particles::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  glDrawArrays( GL_TRIANGLES, 0, 36 );
  
}

Particles::~Particles() {
  
}

