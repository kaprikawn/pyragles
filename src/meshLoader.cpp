#include "meshLoader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "gltf.hpp"

void MeshLoader::addVertex( glm::vec3 position, glm::vec3 colour, int shapeType ) {
  
  shapeVertices_[ shapeType ].push_back( position );
  
  Vertex vertex = { position, colour };
  
  vertices_[ shapeType ].push_back( vertex );
  
  vertexPositions_[ shapeType ].push_back( position );
  
  if( !loadMesh_[ shapeType ] )
    loadMesh_[ shapeType ] = true;
}

void MeshLoader::generateMeshes( nlohmann::json levelJson ) {
  
  glm::vec3 position;
  glm::vec3 colour;
  int currentShape;
  
  // ship //
  currentShape = SHIP;
  // front
  position  = { 0, 0, -1 };
  colour    = { 1.0f,  1.0f,  1.0f };
  addVertex( position, colour, currentShape );
  // 1 top
  position  = { 0, 0.6f, 0.6f };
  colour    = { 0, 0.4f, 0 };
  addVertex( position, colour, currentShape );
  // 2 right
  position  = { 1, 0, 0.6f };
  addVertex( position, colour, currentShape );
  // 3 left
  position  = { -1, 0, 0.6f };
  addVertex( position, colour, currentShape );
  // 4 bottom
  position  = { 0, -0.4f, 0.6f };
  addVertex( position, colour, currentShape );
  // 5 back
  position  = { 0, 0, 1 };
  colour    = { 1, 0, 0 };
  addVertex( position, colour, currentShape );
  
  std::vector<GLuint> indices = {
      0, 1, 2 // f t r
    , 0, 1, 3 // f t l
    , 0, 4, 2 // f b r
    , 0, 4, 3 // f b l
    , 5, 1, 2 // b t r
    , 5, 1, 3 // b t l
    , 5, 4, 2 // b b r
    , 5, 4, 3 // b b l
  };
  indices_[ currentShape ] = indices;
  
  for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
    mesh_[ currentShape ].push_back(
      std::array<glm::vec3, 3> {
          vertexPositions_[ currentShape ][ indices[ i ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 1 ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 2 ] ]
      }
    );
    
    i++;
    i++;
  }
  indices.clear();
  
  // target //
  currentShape  = TARGET;
  colour        = { 0.729f, 0.129f, 0.176f };
  
  position = { -0.9f,  0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.9f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f,  0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.8f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.9f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.8f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = { -0.9f, -0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.9f, -0.9f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.8f,  0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  position = {  0.8f, -0.8f, 0.0f };
  addVertex( position, colour, currentShape );
  
  indices = {
      0,  1,  2
    , 2,  1,  3
    , 1,  4,  5
    , 4,  5,  6
    , 5,  8,  7
    , 7,  8,  9
    , 10, 11, 8
    , 10, 8,  3
  };
  indices_[ currentShape ] = indices;
  
  for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
    mesh_[ currentShape ].push_back(
      std::array<glm::vec3, 3> {
          vertexPositions_[ currentShape ][ indices[ i ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 1 ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 2 ] ]
      }
    );
    
    i++;
    i++;
  }
  indices.clear();
  
  // bullet //
  currentShape  = BULLET;
  
  GLfloat scaler = 0.4f;
  
  position  = {  0.0f,  0.0f, -1.0f };
  colour    = { 1, 0, 0 };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  colour    = { 0.0f, 0.0f, 1.0f };
  
  position  = {  0.0f,  0.6f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = {  1.0f,  0.0f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = { -1.0f,  0.0f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = {  0.0f, -0.4f,  0.6f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  position  = {  0.0f,  0.0f,  1.0f };
  colour    = {  1.0f,  1.0f,  1.0f };
  position *= scaler;
  addVertex( position, colour, currentShape );
  
  indices = {
      0, 1, 2 // f t r
    , 0, 1, 3 // f t l
    , 0, 4, 2 // f b r
    , 0, 4, 3 // f b l
    , 5, 1, 2 // b t r
    , 5, 1, 3 // b t l
    , 5, 4, 2 // b b r
    , 5, 4, 3 // b b l
  };
  indices_[ currentShape ] = indices;
  
  for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
    mesh_[ currentShape ].push_back(
      std::array<glm::vec3, 3> {
          vertexPositions_[ currentShape ][ indices[ i ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 1 ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 2 ] ]
      }
    );
    
    i++;
    i++;
  }
  indices.clear();
  
  currentShape  = ARCH;
  Gltf gltfArch( "arch.glb" );
  std::vector<GltfNode> gltfNodes = gltfArch.gltfNodes();
  for( uint32_t i = 0; i < gltfNodes.size(); i++ ) {
    glm::vec3 thisColour = gltfNodes[ i ].colour;
    std::string thisName = gltfNodes[ i ].name;
    if( thisName == "frontTop" )
      thisColour = { 0, 1, 0 };
    for( uint32_t j = 0; j < gltfNodes[ i ].positions.size(); j++ ) {
      glm::vec3 thisPosition = gltfNodes[ i ].positions[ j ];
      addVertex( thisPosition, thisColour, currentShape );
    }
    for( uint32_t k = 0; k < gltfNodes[ i ].indices.size(); k++ ) {
      GLuint thisIndex = gltfNodes[ i ].indices[ k ];
      indices.push_back( thisIndex );
    }
  }
  indices_[ currentShape ] = indices;
  
  
//   struct GltfNode {
//   std::string             name;
//   int                     mesh;
//   int                     positionIndex;
//   int                     normalIndex;
//   int                     texcoord_0Index;
//   int                     indicesIndex;
//   std::vector<glm::vec3>  positions;
//   std::vector<glm::vec3>  normals;
//   std::vector<glm::vec2>  texcoord_0s;
//   std::vector<GLuint>     indices;
//   glm::vec3               colour;
// };

  
  /*
  // floor 1
  currentShape = FLOOR1;
  colour = { 0.87f, 0.733f, 0.129f };
  
  position = { -1.0f,  0.0f, -1.0f };
  addVertex( position, colour, currentShape );
  position = { -1.0f,  0.0f,  1.0f };
  addVertex( position, colour, currentShape );
  position = {  1.0f,  0.0f,  1.0f };
  addVertex( position, colour, currentShape );
  position = {  1.0f,  0.0f, -1.0f };
  addVertex( position, colour, currentShape );
  
  indices = {
      0, 1, 2
    , 3, 2, 0
  };
  indices_[ currentShape ] = indices;
  
  for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
    mesh_[ currentShape ].push_back(
      std::array<glm::vec3, 3> {
          vertexPositions_[ currentShape ][ indices[ i ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 1 ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 2 ] ]
      }
    );
    
    i++;
    i++;
  }
  indices.clear();
  
  // floor 2
  currentShape = FLOOR2;
  colour = { 0.87f, 0.623f, 0.129f };
  
  position = { -100.0f,  0.0f, -100.0f };
  addVertex( position, colour, currentShape );
  position = { -100.0f,  0.0f, 10.0f };
  addVertex( position, colour, currentShape );
  position = {  100.0f,  0.0f, 10.0f };
  addVertex( position, colour, currentShape );
  position = {  100.0f,  0.0f, -100.0f };
  addVertex( position, colour, currentShape );
  
  indices = {
      0, 1, 2
    , 3, 2, 0
  };
  indices_[ currentShape ] = indices;
  
  for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
    mesh_[ currentShape ].push_back(
      std::array<glm::vec3, 3> {
          vertexPositions_[ currentShape ][ indices[ i ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 1 ] ]
        , vertexPositions_[ currentShape ][ indices[ i + 2 ] ]
      }
    );
    
    i++;
    i++;
  }
  indices.clear();
  */
  
  MeshLoader::loadLevel( levelJson );
  
  setOffsets();
}

void MeshLoader::loadLevel( nlohmann::json levelJson ) {
  
  std::map <std::string, int> shapeTypesLookup;
  std::vector<GLuint> indices;
  shapeTypesLookup[ "ENEMY_POD" ] = 4;
  shapeTypesLookup[ "ARCH" ]      = 5;
  shapeTypesLookup[ "FLOOR1" ]    = 6;
  shapeTypesLookup[ "FLOOR2" ]    = 7;
  
  std::map <std::string, int> ObjectTypesLookup;
  ObjectTypesLookup[ "ENEMY" ]    = 2;
  ObjectTypesLookup[ "SCENARY" ]  = 3;
  
  nlohmann::json meshes = levelJson[ "meshes" ];
  for( nlohmann::json::iterator it1 = meshes.begin(); it1 != meshes.end(); ++it1 ) {
    nlohmann::json meshContainer = *it1;
    int shapeType     = shapeTypesLookup[ meshContainer[ "shapeType" ] ];
    int currentShape  = shapeType;
    
    nlohmann::json indicesRoot  = meshContainer[ "indices" ];
    nlohmann::json meshRoot     = meshContainer[ "mesh" ];
    
    
    for( nlohmann::json::iterator meshObj = meshRoot.begin(); meshObj != meshRoot.end(); ++meshObj ) {
      
      nlohmann::json mesh             = *meshObj;
      nlohmann::json meshObjColour    = mesh[ "colour" ];
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
        
        MeshLoader::addVertex( position, colour, shapeType );
      }
      
    }
    
    for( nlohmann::json::iterator indexRoot = indicesRoot.begin(); indexRoot != indicesRoot.end(); ++indexRoot ) {
      nlohmann::json index = *indexRoot;
      
      indices.push_back( index );
    }
    
    indices_[ currentShape ] = indices;

    for( unsigned int i = 0; i < indices.size() - 2; i++ ) {
      mesh_[ currentShape ].push_back(
        std::array<glm::vec3, 3> {
            vertexPositions_[ currentShape ][ indices[ i ] ]
          , vertexPositions_[ currentShape ][ indices[ i + 1 ] ]
          , vertexPositions_[ currentShape ][ indices[ i + 2 ] ]
        }
      );
      
      i++;
      i++;
    }
    indices.clear();
  }
  
  /*
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
  */
  
}

void MeshLoader::setOffsets() {
  GLintptr vertexOffset = 0;
  GLintptr indexOffset  = 0;
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( loadMesh_[ i ] ) {
      vertexOffsets_[ i ] = vertexOffset;
      indexOffsets_[ i ]  = indexOffset;
      
      bufferData_[ i ].vertexOffset = vertexOffset;
      bufferData_[ i ].indexOffset  = indexOffset;
      bufferData_[ i ].numIndices   = indices_[ i ].size();
      
      vertexOffset  += vertices_[ i ].size() * sizeof( vertices_[ i ][ 0 ] );
      indexOffset   += indices_[ i ].size() * sizeof( indices_[ i ][ 0 ] );
    }
  }
}

GLsizeiptr MeshLoader::totalVertexBufferSize() {
  GLsizeiptr vertexBufferSize = 0;
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( loadMesh_[ i ] ) {
      vertexBufferSize += vertices_[ i ].size() * sizeof( vertices_[ i ][ 0 ] );
    }
  }
  
  return vertexBufferSize;
}

GLsizeiptr MeshLoader::totalIndexBufferSize() {
  GLsizeiptr indexBufferSize = 0;
  
  for( unsigned int i = 0; i < MAX_SHAPE; i++ ) {
    if( loadMesh_[ i ] ) {
      indexBufferSize += indices_[ i ].size() * sizeof( indices_[ i ][ 0 ] );
    }
  }
  
  return indexBufferSize;
}
