#include "gltf.hpp"
#include <iostream>
#include <regex>
#include "../deps/stb_image.h"

Gltf::Gltf() {
  
}

void Gltf::init( const std::string& filename ) {
  
  std::string filepath = "./assets/" + filename;
  
  fs_.open( filepath, std::ifstream::binary );
  
  fs_.read( ( char* )&magic_  , 4 );
  fs_.read( ( char* )&version_, 4 );
  fs_.read( ( char* )&length_ , 4 );
  
  fs_.read( ( char* )&jsonChunkLength_, 4 );
  fs_.read( ( char* )&jsonChunkType_  , 4 );
  
  binStartByte_ = jsonChunkDataStartByte_ + jsonChunkLength_; // start of binary buffer
  
  char jsonBuffer[ jsonChunkLength_ ];
  std::string j;
  fs_.read( ( char* )&jsonBuffer, jsonChunkLength_ );
  
  for( unsigned int i = 0; i < jsonChunkLength_; i++ ) {
    j += jsonBuffer[ i ];
  }
  
  json_ = nlohmann::json::parse( j );
  
  // binary data
  fs_.read( ( char* )&binChunkLength_ , 4 );
  fs_.read( ( char* )&binChunkType_   , 4 );
  
  std::cout << "json is\n" << j << std::endl;

  binChunkDataStartByte_ = binStartByte_ + 4 + 4; // start of the actual binary data
  
  nlohmann::json nodes = json_[ "nodes" ];
  for( nlohmann::json::iterator it1 = nodes.begin(); it1 != nodes.end(); ++it1 ) {
    nlohmann::json node = *it1;
    
    if( node.contains( "mesh" ) && node.contains( "name" ) ) {
      int mesh          = node[ "mesh" ];
      std::string name  = node[ "name" ];
      
      mesh_             = mesh;
      name_             = name;
      
      unsigned int positionIndex    = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "POSITION" ];
      //unsigned int normalIndex      = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "NORMAL" ];
      unsigned int texcoord_0Index  = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "TEXCOORD_0" ];
      unsigned int indicesIndex     = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "indices" ];
      
      unsigned int positionCount;
      unsigned int uvCount;
      
      positions_        = Gltf::positions( positionIndex, positionCount );
      texcoord_0s_      = Gltf::texcoord_0s( texcoord_0Index, uvCount );
      indices_          = Gltf::indices( indicesIndex );
      //colour_           = Gltf::colour( name );
      
      if( positionCount == uvCount && positionCount > 0 ) {
        useUvData_ = true;
        for( unsigned int i = 0; i < positionCount; i++ ) {
          vertexData_.push_back( positions_[i][0] );
          vertexData_.push_back( positions_[i][1] );
          vertexData_.push_back( positions_[i][2] );
          vertexData_.push_back( texcoord_0s_[i][0] );
          vertexData_.push_back( texcoord_0s_[i][1] );
          
          vertexDataSize_ += ( sizeof( float ) * 5 );
        }
      } else {
        for( unsigned int i = 0; i < positionCount; i++ ) {
          vertexData_.push_back( positions_[i][0] );
          vertexData_.push_back( positions_[i][1] );
          vertexData_.push_back( positions_[i][2] );
          
          vertexDataSize_ += ( sizeof( float ) * 3 );
        }
      }
    }
  }
  
  Gltf::loadTexture();
}

void Gltf::loadTexture() {
  auto images_found = json_.find( "images" );
  if( images_found == json_.end() )
    return;
  
  // dirty workaround until I find how to load directly from glb
  //stbi_set_flip_vertically_on_load( 1 );
  //textureData_ = stbi_load( "./res/models/wood_texture.jpg", &textureWidth_, &textureHeight_, &textureBpp_, 4 );
  
  unsigned int imageBufferView = json_[ "images" ][ 0 ][ "bufferView" ];
  unsigned int imageByteOffset = json_[ "bufferViews" ][ imageBufferView ][ "byteOffset" ];
  unsigned int imagebyteLength = json_[ "bufferViews" ][ imageBufferView ][ "byteLength" ];
  
  fs_.seekg( binChunkDataStartByte_ + imageByteOffset );
  unsigned char pngbuf[ imagebyteLength ];
  fs_.read( ( char* )&pngbuf, imagebyteLength );
  
  //unsigned char stbi_uc;
  
  textureData_ = stbi_load_from_memory( pngbuf, imagebyteLength, &textureWidth_, &textureHeight_, &textureBpp_, 4 );
  
}

std::vector<glm::vec3> Gltf::positions( unsigned int positionIndex, unsigned int &positionCount ) {
  
  std::vector<glm::vec3> myVecs;
  
  nlohmann::json accessor = json_[ "accessors" ][ positionIndex ];
  int bufferViewIndex     = accessor[ "bufferView" ];
  int count               = accessor[ "count" ];
  
  positionCount = ( unsigned int )count;
  
  nlohmann::json bufferView = json_[ "bufferViews" ][ bufferViewIndex ];
  
  int byteOffset  = bufferView[ "byteOffset" ];
  //int byteLength  = bufferView[ "byteLength" ];
  //int buffer      = bufferView[ "buffer" ];
  
  unsigned int startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    glm::vec3 myVec = { 0.0f, 0.0f, 0.0f };
    
    fs_.read( ( char* )&myVec.x , 4 );
    fs_.read( ( char* )&myVec.y , 4 );
    fs_.read( ( char* )&myVec.z , 4 );
    
    myVecs.push_back( myVec );
    
    count--;
    
  } while( count > 0 );
  
  return myVecs;
}

std::vector<glm::vec2> Gltf::texcoord_0s( unsigned int texcoord_0Index, unsigned int &uvCount ) {
  
  std::vector<glm::vec2> myVecs;
  
  nlohmann::json accessor       = json_[ "accessors" ][ texcoord_0Index ];
  unsigned int bufferViewIndex  = accessor[ "bufferView" ];
  unsigned int count            = accessor[ "count" ];
  
  uvCount = count;
  
  nlohmann::json bufferView     = json_[ "bufferViews" ][ bufferViewIndex ];
  
  unsigned int byteOffset       = bufferView[ "byteOffset" ];
  unsigned int startPosition    = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    glm::vec2 myVec = { 0, 0 };
    
    fs_.read( ( char* )&myVec[ 0 ], 4 );
    fs_.read( ( char* )&myVec[ 1 ], 4 );
    
    myVecs.push_back( myVec );
    
    count--;
    
  } while( count > 0 );
  
  return myVecs;
}

std::vector<GLuint> Gltf::indices( unsigned int indicesIndex ) {
  
  std::vector<GLuint> myVec;
  
  nlohmann::json accessor = json_[ "accessors" ][ indicesIndex ];
  int bufferViewIndex     = accessor[ "bufferView" ];
  int count               = accessor[ "count" ];
  
  nlohmann::json bufferView = json_[ "bufferViews" ][ bufferViewIndex ];
  
  int byteOffset  = bufferView[ "byteOffset" ];
  //int byteLength  = bufferView[ "byteLength" ];
  //int buffer      = bufferView[ "buffer" ];
  
  unsigned int startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    GLuint myIndex;
    fs_.read( ( char* )&myIndex, 2 );
    
    myVec.push_back( myIndex );
    
    count --;
    
  } while( count > 0 );
  
  return myVec;

}

glm::vec3 colour( std::string nodeName ) {
  
  // regex is c++ is rubbish
  
  glm::vec3 colour = { 1, 0, 0 }; // default red
  
  std::regex regexRed( "r([0-9]*[.])?[0-9]+" );
  std::smatch matches;
  std::regex_search( nodeName, matches, regexRed );
  
  if( matches.empty() )
    return colour;
  
  std::string::size_type sz;     // alias of size_t
  std::regex char_replace( "[a-z]" );
  
  std::string red = std::regex_replace( matches.str( 0 ), char_replace, "" );
  //std::cout << "red is " << red << std::endl;
  float r = std::stof( red, &sz );
  
  std::regex regexGreen( "g([0-9]*[.])?[0-9]+" );
  std::regex_search( nodeName, matches, regexGreen );
  if( matches.empty() )
    return colour;
  std::string green = std::regex_replace( matches.str( 0 ), char_replace, "" );
  float g = std::stof( green, &sz );
  
  std::regex regexBlue( "b([0-9]*[.])?[0-9]+" );
  std::regex_search( nodeName, matches, regexBlue );
  if( matches.empty() )
    return colour;
  std::string blue = std::regex_replace( matches.str( 0 ), char_replace, "" );
  float b = std::stof( blue, &sz );
  
  colour.r = r;
  colour.g = g;
  colour.b = b;
  
  return colour;
}

std::vector<GLfloat> Gltf::floats( unsigned int byteOffset, unsigned int byteLength ) {
  
  std::vector<GLfloat> myFloats;
  
  unsigned int bytesLeft = byteLength;
  
  unsigned int startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    GLfloat myFloat;
    fs_.read( ( char* )&myFloat , 4 );
    
    myFloats.push_back( myFloat );
    
    bytesLeft -= 4;
  } while( bytesLeft > 0 );
  
  return myFloats;
}

std::vector<GLushort> Gltf::ushorts( unsigned int byteOffset, unsigned int byteLength ) {
  
  std::vector<GLushort> myUshorts;
  
  unsigned int bytesLeft = byteLength;
  
  unsigned int startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    GLushort myUshort;
    fs_.read( ( char* )&myUshort , 2 );
    
    myUshorts.push_back( myUshort );
    
    bytesLeft -= 2;
  } while( bytesLeft > 0 );
  
  return myUshorts;
}

Gltf::~Gltf() {
  if( textureData_ )
    delete textureData_;
  
  textureData_ = nullptr;
}
