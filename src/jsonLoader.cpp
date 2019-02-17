#include "jsonLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../deps/json.hpp"

void JsonLoader::loadLevel( int levelNumber ) {
  
  std::map <std::string, int> shapeTypesLookup;
  shapeTypesLookup[ "ARCH" ] = 5;
  
  std::cout << "loading level " << levelNumber << std::endl;
  
  std::stringstream ss;
  ss << "assets/level" << levelNumber << ".json";
  std::string filename = ss.str();
  
  std::ifstream fin( filename, std::ifstream::binary );
  nlohmann::json j;
  fin >> j;
  
  nlohmann::json meshes = j[ "meshes" ];
  for( nlohmann::json::iterator it1 = meshes.begin(); it1 != meshes.end(); ++it1 ) {
    nlohmann::json mesh = *it1;
    std::string meshType = mesh[ "meshType" ];
    
    std::cout << "meshType is " << meshType << std::endl;
    std::cout << "as an int is " << shapeTypesLookup[ meshType ] << std::endl;
    std::cout << "try again " << shapeTypesLookup[ mesh[ "meshType" ] ] << std::endl;
    
  }
  
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
    newEnemy.initPosition.y = enemy[ "initPosition" ][ "y" ];
    newEnemy.initPosition.z = enemy[ "initPosition" ][ "z" ];
    
    
  }
  
  nlohmann::json scenary = j[ "scenary" ];
  for( nlohmann::json::iterator it1 = scenary.begin(); it1 != scenary.end(); ++it1 ) {
    nlohmann::json s = *it1;
    
    std::string shapeType   = s[ "shapeType" ];
    std::string objectType  = s[ "objectType" ];
    std::string meshType    = s[ "meshType" ];
    GLfloat timeUntilSpawn  = s[ "timeUntilSpawn" ];
    
  }
  
}