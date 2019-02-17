#include "jsonLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../deps/json.hpp"

void JsonLoader::addVertex( glm::vec3 position, glm::vec3 colour, int shapeType ) {
  
  shapeVertices_[ shapeType ].push_back( position );
  
  Vertex vertex = { position, colour };
  
  vertices_[ shapeType ].push_back( vertex );
  
  vertexPositions_[ shapeType ].push_back( position );
  
  if( !loadMesh_[ shapeType ] )
    loadMesh_[ shapeType ] = true;
}

void JsonLoader::loadLevel( int levelNumber ) {
  
  std::map <std::string, int> shapeTypesLookup;
  shapeTypesLookup[ "ENEMY_POD" ] = 4;
  shapeTypesLookup[ "ARCH" ]      = 5;
  
  std::map <std::string, int> ObjectTypesLookup;
  ObjectTypesLookup[ "ENEMY" ]    = 2;
  
  std::cout << "loading level " << levelNumber << std::endl;
  
  std::stringstream ss;
  ss << "assets/level" << levelNumber << ".json";
  std::string filename = ss.str();
  
  std::ifstream fin( filename, std::ifstream::binary );
  nlohmann::json j;
  fin >> j;
  
  nlohmann::json meshes = j[ "meshes" ];
  for( nlohmann::json::iterator it1 = meshes.begin(); it1 != meshes.end(); ++it1 ) {
    nlohmann::json meshContainer = *it1;
    int shapeType = shapeTypesLookup[ meshContainer[ "shapeType" ] ];
    
    nlohmann::json meshRoot = meshContainer[ "mesh" ];
    
    for( nlohmann::json::iterator meshObj = meshRoot.begin(); meshObj != meshRoot.end(); ++meshObj ) {
      
      nlohmann::json mesh = *meshObj;
      nlohmann::json meshObjColour = mesh[ "colour" ];
      nlohmann::json meshObjVertices  = mesh[ "vertices" ];
      
      GLfloat r = meshObjColour[ 0 ];
      GLfloat g = meshObjColour[ 1 ];
      GLfloat b = meshObjColour[ 2 ];
      
      glm::vec3 colour = { r, g, b };
      
      for( nlohmann::json::iterator vertexRoot = meshObjVertices.begin(); vertexRoot != meshObjVertices.end(); ++vertexRoot ) {
        nlohmann::json vertex = *vertexRoot;
        
        GLfloat x = vertex[ 0 ];
        GLfloat y = vertex[ 1 ];
        GLfloat z = vertex[ 2 ];
        
        glm::vec3 position = { x, y, z };
        
        JsonLoader::addVertex( position, colour, shapeType );
      }
    }
  }
  
  nlohmann::json e = j[ "enemies" ];
  for( nlohmann::json::iterator it1 = e.begin(); it1 != e.end(); ++it1 ) {
    nlohmann::json enemy = *it1;
    
    PhysicsObjectParams newEnemy;
    newEnemy.shapeType      = shapeTypesLookup[ enemy[ "shapeType" ] ];
    newEnemy.objectType     = ObjectTypesLookup[ enemy[ "objectType" ] ];
    newEnemy.canFire        = enemy[ "canFire" ];
    newEnemy.damageShip     = enemy[ "damageShip" ];
    newEnemy.damageEnemy    = enemy[ "damageEnemy" ];
    newEnemy.velMultiplier  = enemy[ "velMultiplier" ];
    newEnemy.timeUntilSpawn = enemy[ "timeUntilSpawn" ];
    newEnemy.initPosition.x = enemy[ "initPosition" ][ "x" ];
    newEnemy.initPosition.y = enemy[ "initPosition" ][ "y" ];
    newEnemy.initPosition.z = enemy[ "initPosition" ][ "z" ];
  }
  
  /*nlohmann::json scenary = j[ "scenary" ];
  for( nlohmann::json::iterator it1 = scenary.begin(); it1 != scenary.end(); ++it1 ) {
    nlohmann::json s = *it1;
    
    std::string shapeType   = s[ "shapeType" ];
    std::string objectType  = s[ "objectType" ];
    std::string meshType    = s[ "meshType" ];
    GLfloat timeUntilSpawn  = s[ "timeUntilSpawn" ];
    
  }*/
  
}