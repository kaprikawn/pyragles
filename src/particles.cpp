#include "particles.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "glCallLog.hpp"

Particles::Particles() {
  
  loadShader( "shaderParticles.glsl" );
  
  // 252 vertices per cube - two triangles per side, six sides, six vertices per side, seven floats per vertex
  // 6 * 6 * 7
  vb_.init( nullptr, sizeof( float ) * 252 * PARTICLE_COUNT, GL_DYNAMIC_DRAW );
  
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
  unsigned int particleCount = 0;
  
  for( unsigned int i = 0; i < PARTICLE_COUNT; i++ ) {
    
    if( particles_[ i ].active ) {
      
      particles_[ i ].colour.g -= ( 6.0f * dt );
      if( particles_[ i ].colour.g < 0.0f )
        particles_[ i ].active = false;
      
      glm::vec3 position = particles_[ i ].position;
      position += ( particles_[ i ].direction * ( dt * 50 ) );
      
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
      particleCount++;
    } else {
      // do not display non active particles
      for( unsigned int j = 0; j < 108; j += 3 ) {
        bufferData.push_back( 0.0f );
        bufferData.push_back( 0.0f );
        bufferData.push_back( 1000.0f );
        bufferData.push_back( 1.0f );
        bufferData.push_back( 1.0f );
        bufferData.push_back( 1.0f );
        bufferData.push_back( 1.0f );
      }
      
    }
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

float Particles::randomFloat() {
  
  unsigned int myInt = zeroToHundread( eng );
  
  return ( float ) myInt / 100.0f;
}

bool Particles::randomBool() {
  
  unsigned int myInt = zeroToHundread( eng );
  
  if( myInt > 50 )
    return true;
  
  return false;
}

void Particles::spawnParticle( glm::vec3 newPosition, float xAngle, float yAngle ) {
  
  newPosition.y += 1.5f; // ship isn't totally centre
  
  float maxPositionDeviation = 0.3f;
  if( randomBool() ) {
    newPosition.x += ( maxPositionDeviation * randomFloat() );
  } else {
    newPosition.x -= ( maxPositionDeviation * randomFloat() );
  }
  if( randomBool() ) {
    newPosition.y += ( maxPositionDeviation * randomFloat() );
  } else {
    newPosition.y -= ( maxPositionDeviation * randomFloat() );
  }
  if( randomBool() ) {
    newPosition.z += ( maxPositionDeviation * randomFloat() * 1.2f );
  } else {
    newPosition.z -= ( maxPositionDeviation * randomFloat() * 1.2f );
  }
  
  //std::cout << "position is " << newPosition.x << " - " << newPosition.y << " - " << newPosition.z << std::endl;
  //std::cout << "xAngle_ up and down is " << xAngle << std::endl;
  //std::cout << "yAngle_ right and left is " << yAngle << std::endl;
  
  float xDirection = 0.0f;
  float yDirection = 0.0f;
  float zDirection = 1.0f;
  
  // randomise the direction slightly
  float maxDeviation = 7.0f;
  if( randomBool() ) {
    yAngle += ( maxDeviation * randomFloat() );
  } else {
    yAngle -= ( maxDeviation * randomFloat() );
  }
  if( randomBool() ) {
    xAngle += ( maxDeviation * randomFloat() );
  } else {
    xAngle -= ( maxDeviation * randomFloat() );
  }
  
  xDirection = yAngle / 90;
  yDirection = -xAngle / 90;
  
  glm::vec3 newDirection = glm::normalize( glm::vec3{ xDirection, yDirection, zDirection } );
  
  for( unsigned int i = 0; i < PARTICLE_COUNT; i++ ) {
    
    if( !particles_[ i ].active ) {
      particles_[ i ].position  = newPosition;
      particles_[ i ].colour    = { 1.0f, 1.0f, 0.0f, 1.0f };
      particles_[ i ].scale     = 0.1f;
      particles_[ i ].direction = newDirection;
      particles_[ i ].active    = true;
      break;
    }
    
  }
}

Particles::~Particles() {
  
}