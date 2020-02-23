#include "floor.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Floor::Floor() {
  
  std::cout << "calling floor constructor\n";
  
  // model_ = std::make_unique<Model>();
  
  // float vertices[] = {
  //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
  //   ,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f 
  //   ,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
  //   , -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
  // };
  
  // const unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
  
  // loadVertexData( ( const void* )vertices, sizeof( vertices ) );
  // loadIndexData( indices, 6 );
  // loadShader( "shaderVertexColours.glsl" );
  
  // positionID_ = glGetAttribLocation( shader_.rendererID(),  "aPosition" );
  // colourID_   = glGetAttribLocation( shader_.rendererID(),  "aColour" );
  // mvpID_      = glGetUniformLocation( shader_.rendererID(), "uMVP" );
  
  // glEnableVertexAttribArray( positionID_ );
  // glEnableVertexAttribArray( colourID_ );
}


void Floor::update( float dt ) {
  // yAngle_ += dt * 100;
  // if( yAngle_ > 360 )
  //   yAngle_ -= 360;
    
  // rotationMatrix_ = glm::rotate( glm::mat4( 1.0f ), glm::radians( yAngle_ ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
  // modelMatrix_ = glm::mat4( 1.0f );
  // modelMatrix_ *= rotationMatrix_;
}

void Floor::render( glm::mat4 viewProjectionMatrix ) {
  
  // mvp_ = viewProjectionMatrix * modelMatrix_;
  
  // shader_.bind();
  // shader_.setUniform4fv( "uMVP", ( const float* )&mvp_ );
  
  // vb_.bind();
  // texture_.bind();
  // glVertexAttribPointer( positionID_, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( GLvoid* ) 0 );
  // glVertexAttribPointer( colourID_  , 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 7, ( GLvoid* )( sizeof( float ) * 3 ) );
  // ib_.bind();
    
  // glDrawElements( GL_TRIANGLES, model_ -> indexCount(), GL_UNSIGNED_INT, 0 );
  
}

Floor::~Floor() {}
