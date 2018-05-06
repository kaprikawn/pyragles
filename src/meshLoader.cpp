#include "meshLoader.hpp"
#include <iostream>

void MeshLoader::addVertex( glm::vec3 position, glm::vec3 colour, int shapeType ) {
  
  shapeVertices_[ shapeType ].push_back( position );
  
  Vertex vertex = { position, colour };
  
  vertices_[ shapeType ].push_back( vertex );
  
  vertexPositions_[ shapeType ].push_back( position );
  
  if( !loadMesh_[ shapeType ] )
    loadMesh_[ shapeType ] = true;
}

void MeshLoader::generateMeshes() {
  
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
  
  // enemy pod
  currentShape  = ENEMY_POD;
  colour        = { 0, 1, 0 };
  
  // object part 
  position = { 0.00f, 1.00f, 1.00f };
  addVertex( position, colour, currentShape );
  position = { -0.87f, 1.00f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { -0.87f, 1.00f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 0.00f, 1.00f, -1.00f };
  addVertex( position, colour, currentShape );
  position = { 0.87f, 1.00f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 0.87f, 1.00f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { 0.50f, 0.00f, 0.87f };
  addVertex( position, colour, currentShape );
  position = { -0.50f, 0.00f, 0.87f };
  addVertex( position, colour, currentShape );
  position = { -1.00f, 0.00f, -0.00f };
  addVertex( position, colour, currentShape );
  position = { -0.50f, 0.00f, -0.87f };
  addVertex( position, colour, currentShape );
  position = { 0.50f, 0.00f, -0.87f };
  addVertex( position, colour, currentShape );
  position = { 1.00f, 0.00f, 0.00f };
  addVertex( position, colour, currentShape );
  position = { 0.00f, 0.50f, 0.00f };
  addVertex( position, colour, currentShape );
  position = { 0.00f, 1.40f, 0.00f };
  addVertex( position, colour, currentShape );

  indices = {
      2, 13, 3
    , 9, 2, 3
    , 2, 8, 1
    , 7, 0, 1
    , 6, 5, 0
    , 5, 11, 4
    , 10, 3, 4
    , 6, 12, 5
    , 6, 0, 12
    , 11, 5, 12
    , 4, 11, 12
    , 4, 12, 10
    , 10, 12, 3
    , 3, 12, 9
    , 9, 12, 2
    , 8, 2, 12
    , 1, 8, 12
    , 1, 12, 7
    , 7, 12, 0
    , 5, 13, 0
    , 13, 1, 0
    , 1, 13, 2
    , 3, 13, 4
    , 4, 13, 5
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
  
  // arch
  currentShape  = ARCH;
  colour        = { 1, 0, 0 };
  
  // object part 001
  position = { -3.00f, 3.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 2.70f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -3.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 4.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -1.60f, 3.10f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 4.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 1.60f, 3.10f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 3.00f, 3.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 2.70f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 3.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  
  colour = { 0, 0, 1 };
  // object part 003
  position = { 4.00f, -0.00f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, 3.30f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, 3.30f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, 0.00f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 2.70f, 4.70f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { 2.70f, 4.70f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { -2.70f, 4.70f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { -2.70f, 4.70f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, 3.30f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, 3.30f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, 0.00f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, -0.00f, 0.50f };
  addVertex( position, colour, currentShape );
  
  colour = { 0.741, 0.741, 0.741 };
  // object part 005
  position = { 2.00f, 2.70f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 0.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 2.70f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 1.60f, 3.10f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 1.60f, 3.10f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -1.60f, 3.10f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -1.60f, 3.10f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 2.70f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 2.70f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 0.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  
  colour = { 0, 1, 0 };
  // object part 002
  position = { -3.00f, 3.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -3.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 4.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 4.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 3.00f, 3.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 3.00f, -0.00f, 1.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, 3.30f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, -0.00f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { 2.70f, 4.70f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { -2.70f, 4.70f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, 3.30f, 0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, -0.00f, 0.50f };
  addVertex( position, colour, currentShape );
  
  colour = { 1, 1, 1 };
  // object part 004
  position = { -3.00f, 3.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 2.70f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 0.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -3.00f, 0.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -2.00f, 4.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -1.60f, 3.10f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 4.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 1.60f, 3.10f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 3.00f, 3.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 2.70f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 3.00f, 0.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { 2.00f, 0.00f, -1.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, 0.00f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { -4.00f, 3.30f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { -2.70f, 4.70f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 2.70f, 4.70f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, 3.30f, -0.50f };
  addVertex( position, colour, currentShape );
  position = { 4.00f, 0.00f, -0.50f };
  addVertex( position, colour, currentShape );

  indices = {
      0, 2, 3
    , 1, 4, 5
    , 5, 6, 7
    , 7, 8, 9
    , 9, 10, 11
    , 0, 1, 2
    , 1, 0, 4
    , 5, 4, 6
    , 7, 6, 8
    , 9, 8, 10
    , 12, 14, 15
    , 13, 17, 14
    , 16, 19, 17
    , 18, 21, 19
    , 22, 20, 21
    , 12, 13, 14
    , 13, 16, 17
    , 16, 18, 19
    , 18, 20, 21
    , 22, 23, 20
    , 24, 26, 27
    , 28, 27, 29
    , 30, 29, 31
    , 32, 31, 33
    , 32, 34, 35
    , 24, 25, 26
    , 28, 24, 27
    , 30, 28, 29
    , 32, 30, 31
    , 32, 33, 34
    , 41, 42, 43
    , 40, 44, 42
    , 39, 45, 44
    , 38, 46, 45
    , 37, 46, 36
    , 41, 40, 42
    , 40, 39, 44
    , 39, 38, 45
    , 38, 36, 46
    , 37, 47, 46
    , 48, 50, 51
    , 49, 52, 53
    , 53, 54, 55
    , 55, 56, 57
    , 57, 58, 59
    , 51, 61, 48
    , 52, 61, 62
    , 54, 62, 63
    , 56, 63, 64
    , 58, 64, 65
    , 48, 49, 50
    , 49, 48, 52
    , 53, 52, 54
    , 55, 54, 56
    , 57, 56, 58
    , 51, 60, 61
    , 52, 48, 61
    , 54, 52, 62
    , 56, 54, 63
    , 58, 56, 64
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
  
  setOffsets();
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
