#include "ship.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Ship::Ship( std::shared_ptr<InputHandler> inputHandler ) {
  inputHandler_ = inputHandler;
}

bool Ship::init( std::string modelName ) {
  
  //model_ = std::make_unique<Model>();
  bool gltfLoaded = loadModelFromGltf( modelName );
  if( !gltfLoaded )
    return false;
  
  loadVertexData( model_ -> vertexData(), model_ -> vertexDataSize() );
  loadIndexData( model_ -> indexData(), model_ -> indexCount() );
  loadShader( "shaderBasic.glsl" );
  
  // https://www.raywenderlich.com/3047-opengl-es-2-0-for-iphone-tutorial-part-2-textures
  texture_ = Texture();
  texture_.init( model_ -> textureData(), model_ -> textureWidth(), model_ -> textureHeight() );
  
  positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  normalID_   = glGetAttribLocation( shader_.rendererID(),  "aNormal" );
  texCoordID_ = glGetAttribLocation( shader_.rendererID(),  "aTexCoord" );
  mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  glEnableVertexAttribArray( positionID_ );
  glEnableVertexAttribArray( normalID_ );
  glEnableVertexAttribArray( texCoordID_ );
  
  return true;
}

void Ship::handleInput( float dt ) {
  
  float multiplier      = 8.0f;
  float standstillSpeed = 4.0f;
  
  // left and right
  float xMax      = 6.0f;
  float joyAxisX  = inputHandler_ -> joyAxisX();
  acceleration_.x   = joyAxisX * multiplier;
  
  if( joyAxisX == 0.0f ) {
    if( velocity_.x > 0.0f ) {
      acceleration_.x = -standstillSpeed;
      velocity_.x += acceleration_.x * dt;
      
      if( velocity_.x < 0.0f ) {
        velocity_.x = 0.0f;
        acceleration_.x = 0.0f;
      }
    } else if( velocity_.x < 0.0f ) {
      acceleration_.x = standstillSpeed;
      velocity_.x += acceleration_.x * dt;
      
      if( velocity_.x > 0.0f ) {
        velocity_.x = 0.0f;
        acceleration_.x = 0.0f;
      }
    }
  } else if( ( joyAxisX < 0.0f && velocity_.x > 0.0f )
          || ( joyAxisX > 0.0f && velocity_.x < 0.0f ) ) {
    acceleration_.x *= 3;
  }
  
  velocity_.x += acceleration_.x * dt;
  
  if( velocity_.x > xMax )
    velocity_.x = xMax;
  
  if( velocity_.x < -xMax )
    velocity_.x = -xMax;
  
  // up and down
  float yMax      = 6.0f;
  float joyAxisY  = inputHandler_ -> joyAxisY();
  acceleration_.y = joyAxisY * multiplier;
  
  if( joyAxisY == 0.0f ) {
    if( velocity_.y > 0.0f ) {
      acceleration_.y = -standstillSpeed;
      velocity_.y += acceleration_.y * dt;
      
      if( velocity_.y < 0.0f ) {
        velocity_.y = 0.0f;
        acceleration_.y = 0.0f;
      }
    } else if( velocity_.y < 0.0f ) {
      acceleration_.y = standstillSpeed;
      velocity_.y += acceleration_.y * dt;
      
      if( velocity_.y > 0.0f ) {
        velocity_.y = 0.0f;
        acceleration_.y = 0.0f;
      }
    }
  } else if( ( joyAxisY < 0.0f && velocity_.y > 0.0f )
          || ( joyAxisY > 0.0f && velocity_.y < 0.0f ) ) {
    acceleration_.y *= 3;
  }
  
  velocity_.y += acceleration_.y * dt;
  
  if( velocity_.y > yMax )
    velocity_.y = yMax;
  
  if( velocity_.y < -yMax )
    velocity_.y = -yMax;
}

void Ship::update( float dt ) {
  
  handleInput( dt );
  
}

void Ship::render( glm::mat4 viewProjectionMatrix ) {
  
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

Ship::~Ship() {
  
}

