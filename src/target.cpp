#include "target.hpp"
#include <iostream>
#include "glCallLog.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

Target::Target() {
  
  //    x       y    z     ?     r     g     b     a
  vertexData_ = {
      -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  1.0f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    , -1.0f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    
    ,  0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  1.0f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  1.0f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  0.8f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    
    , -1.0f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    , -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    , -0.8f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    , -1.0f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    
    , -1.0f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  1.0f,  0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    ,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    , -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
    
  };
  
  std::vector<int> indexData = {
      0,  1,  2,  2 , 3 , 0
    , 4,  5,  6,  6 , 7 , 4
    , 8,  9,  10, 10, 11, 8
    , 12, 13, 14, 14, 15, 12
  };
  
  position_.z = -10.0f;
  position_.y = 5.0f;
  
  loadVertexData( &vertexData_[ 0 ], sizeof( vertexData_[ 0 ] ) * vertexData_.size(), GL_STATIC_DRAW );
  loadIndexData( &indexData[ 0 ], indexData.size() );
  loadShader( "shaderVertexColours.glsl" );
  
  positionID_  = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  colourID_    = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  mvpID_       = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( positionID_ ) );
  GLCall( glEnableVertexAttribArray( colourID_ ) );
}

void Target::update( float dt, glm::vec4 shipPosition, float shipAngleX, float shipAngleY ) {
  
  float distanceFromShip = 7.0f;
  
  position_ = shipPosition;
  position_.z -= distanceFromShip;
  
  position_.x = -( distanceFromShip * tan( glm::radians( shipAngleY ) ) ) + shipPosition.x;
  position_.y = distanceFromShip * tan( glm::radians( shipAngleX ) ) + shipPosition.y;
  
  modelMatrix_ = glm::translate( glm::mat4( 1.0f ), glm::vec3( position_ ) );
}

void Target::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  GLCall( glVertexAttribPointer( positionID_, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* ) 0 ) );
  GLCall( glVertexAttribPointer( colourID_  , 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* ) ( sizeof( float ) * 4 ) ) );
  ib_.bind();
    
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
  
}

Target::~Target() {
  
}

