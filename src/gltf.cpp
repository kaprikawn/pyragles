#include "gltf.hpp"
#include <iostream>
#include <fstream>
#include "../deps/json.hpp"

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
  
}