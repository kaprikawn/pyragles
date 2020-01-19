#include "playState.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "global.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber ) {
  
  viewProjectionMatrix_ = camera -> viewProjectionMatrix();
  camera_               = camera;
  //levelJson_            = PlayState::json( levelNumber );
  
  renderer_ = std::make_shared<Renderer>();
  
  myBox_ = std::make_shared<GameObject>();
  
  float positions[] = {
      -0.5,  -0.5f
    ,  0.5f, -0.5f
    ,  0.5f,  0.5f
    , -0.5f,  0.5f
  };
  
  myBox_ -> loadVertexData( &positions[0], sizeof( positions ) );
  
  unsigned int indices[] = {
      0, 1, 2
    , 2, 3, 0
  };
  
  myBox_ -> loadIndexData( &indices[0], 6 );
  
  myBox_ -> loadShader( "basic.glsl" );
  
  
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, 0 );
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
  camera_ -> update( dt );
  
}

void PlayState::render() {
  myBox_ -> render( viewProjectionMatrix_ );
}

bool PlayState::onExit() {
  
  return true;
}

nlohmann::json PlayState::json( int levelNumber ) {
  std::stringstream ss;
  ss << "assets/level" << levelNumber << ".json";
  std::string filename = ss.str();
  
  std::ifstream fin( filename, std::ifstream::binary );
  nlohmann::json json;
  fin >> json;
  
  return json;
}

int PlayState::nextLevel() {
  return nextLevel_;
}