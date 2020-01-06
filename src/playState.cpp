#include "playState.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "global.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber ) {
  
  renderer_ = std::make_shared<Renderer>();
  
  myBox_ = std::make_shared<GameObject>();
  
  myBox_ -> loadGltf( "numbox.glb" );
  
  return true;
}

void PlayState::update( GLfloat dt ) {
  
}

void PlayState::render() {
  myBox_ -> draw( renderer_ );
}

void PlayState::addPhysicsObject( std::shared_ptr<PhysicsObject> physicsObject, bool init, bool isLoading ) {
  
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