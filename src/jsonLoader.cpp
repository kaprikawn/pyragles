#include "jsonLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../deps/json.hpp"

void JsonLoader::loadLevel( int levelNumber ) {
  std::cout << "loading level " << levelNumber << std::endl;
  
  std::stringstream ss;
  ss << "assets/level" << levelNumber << ".json";
  std::string filename = ss.str();
  
  std::ifstream fin( filename, std::ifstream::binary );
  nlohmann::json j;
  fin >> j;
  
  nlohmann::json e = j[ "enemies" ];
  for( nlohmann::json::iterator it1 = e.begin(); it1 != e.end(); ++it1 ) {
    nlohmann::json enemy = *it1;
    /*
struct PhysicsObjectParams {
  int         shapeType;
  int         objectType      = 0;
  bool        canFire         = false;
  bool        damageShip      = false;
  bool        damageEnemy     = false;
  GLfloat     velMultiplier   = 0.0f;
  GLfloat     timeUntilSpawn  = 0.0f;
  glm::vec3   initPosition    = { 0, 0, 0 };
  BufferData  bufferData;
  unsigned int                          spawnerID       = 0;
  std::shared_ptr<Mesh>                 mesh;
  std::shared_ptr<Renderer>             renderer;
  std::shared_ptr<InputHandler>         inputHandler;
  std::shared_ptr<glm::vec3>            shipPosition;
};*/
    
    PhysicsObjectParams newEnemy;
    newEnemy.canFire        = enemy[ "canFire" ];
    newEnemy.timeUntilSpawn = enemy[ "timeUntilSpawn" ];
    newEnemy.initPosition.x = enemy[ "initPosition" ][ "x" ];
    
    
  }
}