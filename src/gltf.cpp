#include "gltf.hpp"
#include <iostream>
#include <fstream>
#include "../deps/json.hpp"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>

Gltf::Gltf( const std::string& filename ) {
  
  std::string filepath = "./assets/" + filename;
  
  std::ifstream fs( filepath, std::ifstream::binary );
  
  uint32_t magic;
  uint32_t version;
  uint32_t length;
  
  // header
  fs.read( ( char* )&magic, 4 );
  fs.read( ( char* )&version, 4 );
  fs.read( ( char* )&length, 4 );
  
  // json
  uint32_t chunkLength;
  uint32_t chunkType;
  fs.read( ( char* )&chunkLength, 4 );
  fs.read( ( char* )&chunkType, 4 );
  
  char jsonBuffer[ chunkLength ];
  std::string j;
  fs.read( ( char* )&jsonBuffer, chunkLength );
  
  for( unsigned int i = 0; i < chunkLength; i++ ) {
    j += jsonBuffer[ i ];
  }
  
  nlohmann::json json = nlohmann::json::parse( j );
  
  // binary data
  fs.read( ( char* )&chunkLength, 4 );
  fs.read( ( char* )&chunkType, 4 );
  
  std::cout << "json is\n" << j << std::endl;
  
  // bin
  fs.read( ( char* )&chunkLength, 4 );
  fs.read( ( char* )&chunkType, 4 );
  
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "chunkLength is " << chunkLength << std::endl;
  std::cout << "chunkType is " << chunkType << std::endl;
  
  // bin content
  GLfloat myFloat;
  //fs.read( ( char* )&myFloat, 4 );
  //std::cout << "myFloat is " << myFloat << std::endl;
  
  //glm::vec3 myVec = { 1, 2, 3 };
  glm::vec3 myVec;
  fs.read( ( char* )&myVec, 12 );
  std::cout << "x is " << myVec.x << std::endl;
  std::cout << "y is " << myVec.y << std::endl;
  std::cout << "z is " << myVec.z << std::endl;
  
}