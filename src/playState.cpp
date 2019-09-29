#include "playState.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "shader.hpp"
#include "global.hpp"
#include "projectile.hpp"
#include "enemy.hpp"

const std::string PlayState::s_playID = "PLAY";

bool PlayState::onEnter( std::shared_ptr<InputHandler> inputHandler, std::shared_ptr<Camera> camera, int levelNumber ) {
  
  levelJson_ = PlayState::json( levelNumber );
  PlayState::loadMeshes();
  
  for( uint32_t i = 0; i < gltfObjects_.size(); i++ ) {
    std::vector<GltfNode> myGltfObject = gltfObjects_[ i ];
    for( uint32_t j = 0; j < myGltfObject.size(); j++ ) {
      GltfNode myGltfNode = myGltfObject[ j ];
      // std::cout << myGltfNode.name << std::endl;
      // for( uint32_t k = 0; k < myGltfNode.positions.size(); k++ ) {
      //   std::cout << "x is " << myGltfNode.positions[k].x;
      //   std::cout << " y is " << myGltfNode.positions[k].y;
      //   std::cout << " z is " << myGltfNode.positions[k].z << std::endl;
      // }
      
      // for( uint32_t l = 0; l < myGltfNode.indices.size(); l++ ) {
      //   std::cout << "index is " << myGltfNode.indices[ l ] << std::endl;
      // }
      
      //std::cout << "positions is " << myGltfNode.positions << std::endl;
    }
  }
  
  
  return true;
}

void PlayState::loadMeshes() {
  nlohmann::json loadMeshes = levelJson_[ "loadMeshes" ];
  
  for( nlohmann::json::iterator it1 = loadMeshes.begin(); it1 != loadMeshes.end(); ++it1 ) {
    nlohmann::json loadMesh = *it1;
    
    std::string filename = loadMesh[ "filename" ];
    
    Gltf myGltf( filename );
    gltfObjects_.push_back( myGltf.gltfNodes() );
  }
}

void PlayState::update( GLfloat dt ) {
  
}

void PlayState::render() {
  
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