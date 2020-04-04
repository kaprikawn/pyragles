#include "gameObject.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

GameObject::GameObject() {
  modelMatrix_    = glm::mat4( 1.0f );
  rotationMatrix_ = glm::mat4( 1.0f );
  mvp_            = glm::mat4( 1.0f );
  acceleration_   = glm::vec3{ 0.0f, 0.0f, 0.0f };
  velocity_       = glm::vec3{ 0.0f, 0.0f, 0.0f };
  position_       = glm::vec3{ 0.0f, 0.0f, 0.0f };
  vb_             = VertexBuffer();
  ib_             = IndexBuffer();
  shader_         = Shader();
  model_          = std::make_unique<Model>();
  texture_        = Texture();
}

void GameObject::recalculateMeshPosition() {
  
}

void GameObject::updatePosition(  glm::vec3 velocity, GLfloat dt, bool skip ) {
  if( !skip )
    position_ += velocity * dt;
}

void GameObject::update( float dt ) {
  
}

void GameObject::render( glm::mat4 viewProjectionMatrix ) {
  
  mvp_ = viewProjectionMatrix * modelMatrix_;
  
  shader_.bind();
  shader_.setUniform1i( "uTexture", 0 );
  shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  vb_.bind();
  texture_.bind();
  glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* ) 0 );
  glVertexAttribPointer( normalID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 3 ) );
  glVertexAttribPointer( texCoordID_, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 8, ( GLvoid* )( sizeof( float ) * 6 ) );
  ib_.bind();
    
  glDrawElements( GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0 );
  
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
  
  return true;
}

void GameObject::updateCollider() {
  if( !hasCollider_ )
    return;
    
  float minX, maxX, minY, maxY, minZ, maxZ;
  bool firstRun = true;
  
  for( unsigned int v = 0; v < collider_.size(); v++ ) {
    
    glm::mat4 updateMatrix = glm::translate( modelMatrix_, originalCollider_[ v ] );
    glm::vec4 transformed = updateMatrix * glm::vec4( originalCollider_[ v ], 1.0f );
    
    collider_[ v ].x = transformed.x;
    collider_[ v ].y = transformed.y;
    collider_[ v ].z = transformed.z;
    
    // for AABB - get max bounds
    if( firstRun ) {
      minX = maxX = transformed.x;
      minY = maxY = transformed.y;
      minZ = maxZ = transformed.z;
    } else {
      if( transformed.x < minX )
        minX = transformed.x;
      if( transformed.x > maxX )
        maxX = transformed.x;
      if( transformed.y < minY )
        minY = transformed.y;
      if( transformed.y > maxY )
        maxY = transformed.y;
      if( transformed.x < minZ )
        minZ = transformed.x;
      if( transformed.x > maxZ )
        maxZ = transformed.x;
    }
    
    minX_ = minX;
    maxX_ = maxX;
    minY_ = minY;
    maxY_ = maxY;
    minZ_ = minZ;
    maxZ_ = maxZ;
    
    // std::cout << "x is " << transformed.x << std::endl;
    // std::cout << "y is " << transformed.y << std::endl;
    // std::cout << "z is " << transformed.z << std::endl;
    
    firstRun = false;
  }
}

GameObject::~GameObject() {
  
}