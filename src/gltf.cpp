#include "gltf.hpp"
#include <iostream>
#include <regex>

Gltf::Gltf( const std::string& filename ) {
  
  std::string filepath = "./assets/" + filename;
  
  fs_.open( filepath, std::ifstream::binary );
  
  fs_.read( ( char* )&magic_, 4 );
  fs_.read( ( char* )&version_, 4 );
  fs_.read( ( char* )&length_, 4 );
  
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
  
  //std::cout << "json is\n" << j << std::endl;

  binChunkDataStartByte_ = binStartByte_ + 4 + 4; // start of the actual binary data
  
  nlohmann::json nodes = json_[ "nodes" ];
  for( nlohmann::json::iterator it1 = nodes.begin(); it1 != nodes.end(); ++it1 ) {
    nlohmann::json node = *it1;
    
    if( node.contains( "mesh" ) && node.contains( "name" ) ) {
      int mesh          = node[ "mesh" ];
      std::string name  = node[ "name" ];
      
      gltfNodes_.push_back( gltfNode( mesh, name ) );
    }
  }
}

std::vector<glm::vec3> Gltf::positions( int positionIndex ) {
  
  std::vector<glm::vec3> myVecs;
  
  nlohmann::json accessor = json_[ "accessors" ][ positionIndex ];
  int bufferViewIndex     = accessor[ "bufferView" ];
  int count               = accessor[ "count" ];
  
  nlohmann::json bufferView = json_[ "bufferViews" ][ bufferViewIndex ];
  
  int byteOffset  = bufferView[ "byteOffset" ];
  //int byteLength  = bufferView[ "byteLength" ];
  //int buffer      = bufferView[ "buffer" ];
  
  uint32_t startPosition = binChunkDataStartByte_ + byteOffset;
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

std::vector<GLuint> Gltf::indices( int indicesIndex, GLuint starting_index ) {
  
  std::vector<GLuint> myVec;
  
  nlohmann::json accessor = json_[ "accessors" ][ indicesIndex ];
  int bufferViewIndex     = accessor[ "bufferView" ];
  int count               = accessor[ "count" ];
  
  nlohmann::json bufferView = json_[ "bufferViews" ][ bufferViewIndex ];
  
  int byteOffset  = bufferView[ "byteOffset" ];
  //int byteLength  = bufferView[ "byteLength" ];
  //int buffer      = bufferView[ "buffer" ];
  
  uint32_t startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    GLuint myIndex;
    fs_.read( ( char* )&myIndex, 2 );
    
    myIndex += starting_index;
    
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

uint32_t get_starting_index( std::vector<GltfNode> gltfNodes ) {
  
  // if the object is split into multiple nodes, want to start the
  // indices at the last max + 1, yes, I know this is janky
  
  if( gltfNodes.empty() )
    return 0;
  
  GLuint max = 0;
  
  for( uint32_t i = 0; i < gltfNodes.size(); i++ ) {
    for( uint32_t j = 0; j < gltfNodes[ i ].indices.size(); j++ ) {
      GLuint thisIndex = gltfNodes[ i ].indices[ j ];
      if( thisIndex >= max )
        max = thisIndex;
    }
  }
  
  max++;
  return max;
}

GltfNode Gltf::gltfNode( int mesh, std::string name ) {
  GltfNode gltfNode;
  
  GLuint starting_index = get_starting_index( gltfNodes_ );
  
  gltfNode.mesh             = mesh;
  gltfNode.name             = name;
  gltfNode.positionIndex    = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "POSITION" ];
  gltfNode.normalIndex      = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "NORMAL" ];
  gltfNode.texcoord_0Index  = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "TEXCOORD_0" ];
  gltfNode.indicesIndex     = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "indices" ];
  
  gltfNode.positions  = Gltf::positions( gltfNode.positionIndex );
  gltfNode.indices    = Gltf::indices( gltfNode.indicesIndex, starting_index );
  
  gltfNode.colour = colour( name );
  
  return gltfNode;
}

std::vector<GLfloat> Gltf::floats( uint32_t byteOffset, uint32_t byteLength ) {
  
  std::vector<GLfloat> myFloats;
  
  uint32_t bytesLeft = byteLength;
  
  uint32_t startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    GLfloat myFloat;
    fs_.read( ( char* )&myFloat , 4 );
    
    myFloats.push_back( myFloat );
    
    bytesLeft -= 4;
  } while( bytesLeft > 0 );
  
  return myFloats;
}

std::vector<GLushort> Gltf::ushorts( uint32_t byteOffset, uint32_t byteLength ) {
  
  std::vector<GLushort> myUshorts;
  
  uint32_t bytesLeft = byteLength;
  
  uint32_t startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    GLushort myUshort;
    fs_.read( ( char* )&myUshort , 2 );
    
    myUshorts.push_back( myUshort );
    
    bytesLeft -= 2;
  } while( bytesLeft > 0 );
  
  return myUshorts;
}

void Gltf::dataDumpBinary() {
  
  std::vector<std::map<std::string, uint>> bufferViews;
  
  nlohmann::json jsonBufferViews = json_[ "bufferViews" ];
  for( nlohmann::json::iterator it1 = jsonBufferViews.begin(); it1 != jsonBufferViews.end(); ++it1 ) {
    nlohmann::json jsonBufferView = *it1;
    
    std::map<std::string, uint> newBufferView;
    newBufferView[ "buffer" ]     = jsonBufferView[ "buffer" ];
    newBufferView[ "byteLength" ] = jsonBufferView[ "byteLength" ];
    newBufferView[ "byteOffset" ] = jsonBufferView[ "byteOffset" ];
    
    bufferViews.push_back( newBufferView );
  }
  
  std::map<uint, std::string> componentTypeLookup;
  
  componentTypeLookup[ 5120 ] = "BYTE";
  componentTypeLookup[ 5121 ] = "UNSIGNED_BYTE";
  componentTypeLookup[ 5122 ] = "SHORT";
  componentTypeLookup[ 5123 ] = "UNSIGNED_SHORT";
  componentTypeLookup[ 5125 ] = "UNSIGNED_INT";
  componentTypeLookup[ 5126 ] = "FLOAT";
  
  nlohmann::json accessors = json_[ "accessors" ];
  
  for( nlohmann::json::iterator it1 = accessors.begin(); it1 != accessors.end(); ++it1 ) {
    nlohmann::json accessor = *it1;
    
    uint bufferView     = accessor[ "bufferView" ];
    uint componentType  = accessor[ "componentType" ];
    std::string type    = accessor[ "type" ];
    //uint count          = accessor[ "count" ];
    
    uint byteOffset     = bufferViews[ bufferView ][ "byteOffset" ];
    uint byteLength     = bufferViews[ bufferView ][ "byteLength" ];
    
    //std::cout << "type is " << componentTypeLookup[ componentType ] << " for " << type << std::endl;
    //std::cout << "byteOffset is " << bufferViews[ bufferView ][ "byteOffset" ] << std::endl;
    
    if( componentTypeLookup[ componentType ] == "FLOAT" ) {
      std::cout << "######## FLOATS " << type << "##########\n";
      std::vector<GLfloat> myFloats = Gltf::floats( byteOffset, byteLength );
      for( uint32_t i = 0; i < myFloats.size(); i++ ) {
        std::cout << "float is " << myFloats[ i ] << std::endl;
      }
    } else if( componentTypeLookup[ componentType ] == "UNSIGNED_SHORT" ) {
      std::cout << "######## SHORTS " << type << "##########\n";
      std::vector<GLushort> myUshorts = Gltf::ushorts( byteOffset, byteLength );
      for( uint32_t i = 0; i < myUshorts.size(); i++ ) {
        std::cout << "ushort is " << myUshorts[ i ] << std::endl;
      }

    } else {
      std::cout << "type is " << componentTypeLookup[ componentType ] << " for " << type << std::endl;
    }
  }

}