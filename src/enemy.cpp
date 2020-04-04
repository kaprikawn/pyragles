#include "enemy.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Enemy::Enemy() {
  
}

bool Enemy::init( std::string modelFilename ) {
  
  position_.z = -20.0f;
  position_.y =  5.0f;
  position_.x =  5.0f;
  
  velocity_.z = 10.0f;
  
  bool gltfLoaded = GameObject::loadModelFromGltf( modelFilename );
  if( !gltfLoaded )
    return false;
  
  GameObject::loadVertexData( model_ -> vertexData(), model_ -> vertexDataSize() );
  GameObject::loadIndexData( model_ -> indexData(), model_ -> indexCount() );
  GameObject::loadShader( "shaderBasic.glsl" );
  
  texture_ = Texture();
  texture_.init( model_ -> textureData(), model_ -> textureWidth(), model_ -> textureHeight() );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  //normalID_   = glGetAttribLocation( shader_.rendererID(),  "aNormal" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  GLCall( glEnableVertexAttribArray( positionID_ ) );
  //GLCall( glEnableVertexAttribArray( normalID_ ) );
  GLCall( glEnableVertexAttribArray( texCoordID_ ) );
  
  originalCollider_ = model_ -> getCollider();
  if( originalCollider_.size() > 0 ) {
    hasCollider_  = true;
    collider_ = originalCollider_;
  }
  
  return true;
}

void Enemy::update( float dt ) {
  
  updatePosition( velocity_, dt );
  
  yAngle_ += 100.0f * dt;
  if( yAngle_ > 360.0f )
    yAngle_ -= 360.0f;
  rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  
  modelMatrix_ = glm::translate( glm::mat4( 1.0f ), position_ );
  modelMatrix_ *= rotationMatrix_;
  
  GameObject::updateCollider();
  
}

void Enemy::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform1i( "uTexture", 0 );
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  texture_.bind();
  GLCall( glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* ) 0 ) );
  //GLCall( glVertexAttribPointer( normalID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 3 ) ) );
  GLCall( glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 6 ) ) );
  ib_.bind();
    
  GLCall( glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 ) );
}

Enemy::~Enemy() {
  
}

