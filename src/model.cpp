#include "model.hpp"
#include <iostream>
#include <regex>
#include "../deps/stb_image.h"

bool Model::loadFromGltf( const std::string& filename ) {
  
  std::string filepath = "./assets/" + filename;
  
  fs_.open( filepath, std::ifstream::binary );
  
  unsigned int magic;
  unsigned int version;
  unsigned int length;
  
  fs_.read( ( char* )&magic  , 4 );
  fs_.read( ( char* )&version, 4 );
  fs_.read( ( char* )&length , 4 );
  
  if( magic != 1179937895 ) {
    std::cout << "Error : " << filename << " does not appear to be a glb file" << std::endl;
    return false;
  }
  
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
  
  unsigned int binChunkLength;
  unsigned int binChunkType;
  
  // binary data
  fs_.read( ( char* )&binChunkLength , 4 );
  fs_.read( ( char* )&binChunkType   , 4 );
  
  //std::cout << "json is\n" << j << std::endl;

  binChunkDataStartByte_ = binStartByte_ + 4 + 4; // start of the actual binary data
  
  nlohmann::json nodes = json_[ "nodes" ];
  for( nlohmann::json::iterator it1 = nodes.begin(); it1 != nodes.end(); ++it1 ) {
    nlohmann::json node = *it1;
    
    if( node.contains( "mesh" ) && node.contains( "name" ) ) {
      int mesh          = node[ "mesh" ];
      std::string name  = node[ "name" ];
      
      if( name == "Collider" ) {
        
        unsigned int positionIndex = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "POSITION" ];
        
        collider_ = Model::loadCollider( mesh );
        
        for( unsigned int i = 0; i < collider_.size(); i += 3 ) {
          std::cout << "x1 is " << collider_[ i + 0 ].x <<  " y1 is " << collider_[ i + 0 ].y << " z1 is " << collider_[ i + 0 ].z << std::endl;
          std::cout << "x2 is " << collider_[ i + 1 ].x <<  " y2 is " << collider_[ i + 1 ].y << " z2 is " << collider_[ i + 1 ].z << std::endl;
          std::cout << "x3 is " << collider_[ i + 2 ].x <<  " y3 is " << collider_[ i + 2 ].y << " z3 is " << collider_[ i + 2 ].z << std::endl;
        }
        printf( "#####################\n" );
        if( collider_.size() > 0 ) {
          hasCollider_ = true;
          colliderVertexCount_ = collider_.size() * 3;
        }
        
      } else {
        
        mesh_ = mesh;
        name_ = name;
        Model::loadModel( mesh );
        
      }
    }
  }
  
  Model::loadTexture();
  
  return true;
}

void Model::loadModel( int mesh ) {
  
  unsigned int positionIndex    = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "POSITION" ];
  unsigned int normalIndex      = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "NORMAL" ];
  unsigned int texcoord_0Index  = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "TEXCOORD_0" ];
  unsigned int indicesIndex     = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "indices" ];
  
  unsigned int positionsCount;
  unsigned int normalsCount;
  unsigned int uvCount;
  
  positions_    = Model::positions( positionIndex, positionsCount );
  normals_      = Model::normals( normalIndex, normalsCount );
  texcoord_0s_  = Model::texcoord_0s( texcoord_0Index, uvCount );
  indices_      = Model::indices( indicesIndex );
  indexCount_   = indices_.size();
  
  if( positionsCount == uvCount && positionsCount > 0 ) {
    useUvData_ = true;
    for( unsigned int i = 0; i < positionsCount; i++ ) {
      vertexData_.push_back( positions_[i][0] );
      vertexData_.push_back( positions_[i][1] );
      vertexData_.push_back( positions_[i][2] );
      vertexData_.push_back( 1.0f );
      vertexData_.push_back( normals_[i][0] );
      vertexData_.push_back( normals_[i][1] );
      vertexData_.push_back( normals_[i][2] );
      vertexData_.push_back( texcoord_0s_[i][0] );
      vertexData_.push_back( texcoord_0s_[i][1] );
      
      vertexDataSize_ += ( sizeof( float ) * 9 );
    }
  } else {
    for( unsigned int i = 0; i < positionsCount; i++ ) {
      vertexData_.push_back( positions_[i][0] );
      vertexData_.push_back( positions_[i][1] );
      vertexData_.push_back( positions_[i][2] );
      
      vertexDataSize_ += ( sizeof( float ) * 3 );
    }
  }
}

std::vector<glm::vec4> Model::loadCollider( int mesh ) {
  
  unsigned int positionIndex    = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "attributes" ][ "POSITION" ];
  unsigned int indicesIndex     = json_[ "meshes" ][ mesh ][ "primitives" ][ 0 ][ "indices" ];
  
  unsigned int positionsCount;
  
  std::vector<glm::vec4>    positions;
  std::vector<unsigned int> indices;
  
  positions    = Model::positions( positionIndex, positionsCount );
  indices      = Model::indices( indicesIndex );
  
  std::vector<glm::vec4> collider;
  
  for( unsigned int i = 0; i < indices.size(); i++ ) {
    collider.push_back( positions[ indices[ i ] ] );
  }
  
  return collider;
}

void Model::loadTexture() {
  auto images_found = json_.find( "images" );
  if( images_found == json_.end() )
    return;
  
  unsigned int imageBufferView = json_[ "images" ][ 0 ][ "bufferView" ];
  unsigned int imageByteOffset = json_[ "bufferViews" ][ imageBufferView ][ "byteOffset" ];
  unsigned int imagebyteLength = json_[ "bufferViews" ][ imageBufferView ][ "byteLength" ];
  
  fs_.seekg( binChunkDataStartByte_ + imageByteOffset );
  unsigned char pngbuf[ imagebyteLength ];
  fs_.read( ( char* )&pngbuf, imagebyteLength );
  
  textureData_ = stbi_load_from_memory( pngbuf, imagebyteLength, &textureWidth_, &textureHeight_, &textureBpp_, 4 );
  
}

std::vector<glm::vec4> Model::positions( unsigned int positionIndex, unsigned int &positionsCount ) {
  
  std::vector<glm::vec4> myVecs;
  
  nlohmann::json accessor = json_[ "accessors" ][ positionIndex ];
  int bufferViewIndex     = accessor[ "bufferView" ];
  int count               = accessor[ "count" ];
  
  positionsCount = ( unsigned int )count;
  
  nlohmann::json bufferView = json_[ "bufferViews" ][ bufferViewIndex ];
  
  int byteOffset  = bufferView[ "byteOffset" ];
  //int byteLength  = bufferView[ "byteLength" ];
  //int buffer      = bufferView[ "buffer" ];
  
  unsigned int startPosition = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startPosition );
  
  do {
    glm::vec4 myVec = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    fs_.read( ( char* )&myVec.x , 4 );
    fs_.read( ( char* )&myVec.y , 4 );
    fs_.read( ( char* )&myVec.z , 4 );
    
    myVecs.push_back( myVec );
    
    count--;
    
  } while( count > 0 );
  
  return myVecs;
}

std::vector<glm::vec3> Model::normals( unsigned int normalIndex, unsigned int &normalCount ) {
  
  std::vector<glm::vec3> myVecs;
  
  nlohmann::json accessor = json_[ "accessors" ][ normalIndex ];
  int bufferViewIndex     = accessor[ "bufferView" ];
  int count               = accessor[ "count" ];
  
  normalCount = ( unsigned int )count;
  
  nlohmann::json bufferView = json_[ "bufferViews" ][ bufferViewIndex ];
  
  int byteOffset  = bufferView[ "byteOffset" ];
  //int byteLength  = bufferView[ "byteLength" ];
  //int buffer      = bufferView[ "buffer" ];
  
  unsigned int startnormal = binChunkDataStartByte_ + byteOffset;
  fs_.seekg( startnormal );
  
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

std::vector<glm::vec2> Model::texcoord_0s( unsigned int texcoord_0Index, unsigned int &uvCount ) {
  
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

std::vector<GLuint> Model::indices( unsigned int indicesIndex ) {
  
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

std::vector<GLfloat> Model::floats( unsigned int byteOffset, unsigned int byteLength ) {
  
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

std::vector<GLushort> Model::ushorts( unsigned int byteOffset, unsigned int byteLength ) {
  
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

Model::~Model() {

}
