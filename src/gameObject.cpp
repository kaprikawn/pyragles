#include "gameObject.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject() {
  modelMatrix_    = glm::mat4( 1.0f );
  rotationMatrix_ = glm::mat4( 1.0f );
  mvp_            = glm::mat4( 1.0f );
  acceleration_   = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  velocity_       = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  position_       = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  vb_             = VertexBuffer();
  ib_             = IndexBuffer();
  shader_         = Shader();
  model_          = std::make_unique<Model>();
  texture_        = Texture();
}

void GameObject::updatePosition(  glm::vec4 velocity, GLfloat dt, bool skip ) {
  if( !skip )
    position_ += velocity * dt;
}

void GameObject::update( float dt ) {
  
}

void GameObject::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  // shader_.bind();
  // shader_.setUniform1i( "uTexture", 0 );
  // shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  // vb_.bind();
  // texture_.bind();
  // glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* ) 0 );
  // glVertexAttribPointer( normalID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 3 ) );
  // glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 6 ) );
  // ib_.bind();
    
  // glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 );
  
  if( debugCollider_ ) {
    shaderCol_.bind();
    shaderCol_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
    vbCol_.bind();
    glVertexAttribPointer( positionIDCol_, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glDrawArrays( GL_TRIANGLES, 0, model_ -> colliderVertexCount() );
    
    std::cout << "position is " << position_.x << std::endl;
  }
  
}

void GameObject::loadVertexData( const void* data, unsigned int size, GLenum usage ) {
  vb_.init( data, size, usage );
}

void GameObject::loadIndexData( const void* data, unsigned int count ) {
  ib_.init( data, count );
  indexCount_ = count;
}

void GameObject::loadShader( const std::string& filename ) {
  shader_.init( filename );
}

void GameObject::loadTexture( unsigned char* textureData, int width, int height ) {
  // https://www.raywenderlich.com/3047-opengl-es-2-0-for-iphone-tutorial-part-2-textures
  texture_.init( textureData, width, height );
}

bool GameObject::loadModelFromGltf( std::string modelName ) {
  
  bool modelLoaded = model_ -> loadFromGltf( modelName );
  if( !modelLoaded )
    return false;
  if( model_ -> hasCollider() ) {
    hasCollider_ = true;
    originalCollider_ = model_ -> collider();
    collider_         = originalCollider_;
  }
  
  indexCount_ = model_ -> indexCount();
  
  loadVertexData( model_ -> vertexData(), model_ -> vertexDataSize() );
  loadIndexData( model_ -> indexData(), indexCount_ );
  loadShader( "shaderBasic.glsl" );
  loadTexture( model_ -> textureData(), model_ -> textureWidth(), model_ -> textureHeight() );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  normalID_   = glGetAttribLocation( shader_.rendererID(),  "aNormal" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( normalID_ );
  glEnableVertexAttribArray( texCoordID_ );
  
  if( debugCollider_ ) {
    vbCol_.init( &originalCollider_[ 0 ], sizeof( originalCollider_[ 0 ] ) * originalCollider_.size() );
    shaderCol_.init( "shaderDebug.glsl" );
    positionIDCol_ = glGetAttribLocation( shaderCol_.rendererID(),  "aPosition" );
    mvpIDCol_      = glGetUniformLocation( shaderCol_.rendererID(), "uMVP" );
  }
  
  return true;
}

void GameObject::updateCollider( int debug ) {
  
  if( !hasCollider_ )
    return;
  
  glm::mat4 modelMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( position_ ) );
  modelMatrix *= rotationMatrix_;
    
  for( unsigned int v = 0; v < originalCollider_.size(); v++ ) {
    
    glm::vec4 originalPosition = glm::vec4( originalCollider_[ v ] );
    glm::vec4 newPosition = modelMatrix_ * originalPosition;
    
    collider_[ v ] = newPosition;
    
    // collider_[ v ].x = transformed.x;
    // collider_[ v ].y = transformed.y;
    // collider_[ v ].z = transformed.z;
    
    //std::cout << "x is " << transformed.x << std::endl;
    // std::cout << "y is " << transformed.y << std::endl;
    // std::cout << "z is " << transformed.z << std::endl;
    
  }
}

GameObject::~GameObject() {
  
}