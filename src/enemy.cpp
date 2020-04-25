#include "enemy.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"

Enemy::Enemy() {
  
}

bool Enemy::init( std::string modelFilename ) {
  
  position_.z = -20.0f;
  position_.y =  5.0f;
  position_.x =  4.5f;
  
  // position_.z = -3.0f;
  // position_.y = START_Y;
  //velocity_.z = 10.0f;
  
  bool gltfLoaded = GameObject::loadModelFromGltf( modelFilename, "shaderBasic.glsl" );
  
  return gltfLoaded;
}

void Enemy::update( float dt ) {
  
  yAngle_ += 100.0f * dt;
  if( yAngle_ > 360.0f )
    yAngle_ -= 360.0f;
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  
  GameObject::update( dt );
  GameObject::updateCollider( 1 );
  
}

void Enemy::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform1i( "uTexture", 0 );
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  texture_.bind();
  GLCall( glVertexAttribPointer( positionID_, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 9, ( GLvoid* ) 0 ) );
  //GLCall( glVertexAttribPointer( normalID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 9, ( GLvoid* )( sizeof( float ) * 4 ) ) );
  GLCall( glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 9, ( GLvoid* )( sizeof( float ) * 7 ) ) );
  ib_.bind();
    
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
  
  GameObject::render( viewProjectionMatrix );
}

Enemy::~Enemy() {
  
}

