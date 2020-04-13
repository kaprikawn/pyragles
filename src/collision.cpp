#include "collision.hpp"
#include <iostream>
#include "../deps/triangleOverlap.h"

AABB getAABB( std::vector<glm::vec4> collider ) {
  
  AABB myAABB;
  
  for( unsigned int i = 0; i < collider.size(); i++ ) {
    
    if( myAABB.firstPass ) {
      myAABB.minX = myAABB.maxX = collider[ i ].x;
      myAABB.minY = myAABB.maxY = collider[ i ].y;
      myAABB.minZ = myAABB.maxZ = collider[ i ].z;
    } else {
      
      if( collider[ i ].x < myAABB.minX )
        myAABB.minX = collider[ i ].x;
      if( collider[ i ].x > myAABB.maxX )
        myAABB.maxX = collider[ i ].x;
      if( collider[ i ].y < myAABB.minY )
        myAABB.minY = collider[ i ].y;
      if( collider[ i ].y > myAABB.maxY )
        myAABB.maxY = collider[ i ].y;
      if( collider[ i ].z < myAABB.minZ )
        myAABB.minZ = collider[ i ].z;
      if( collider[ i ].z > myAABB.maxZ )
        myAABB.maxZ = collider[ i ].z;
        
    }
    
    myAABB.firstPass = false;
  }
  
  return myAABB;
}

bool areCollidingAABB( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 ) {
  
  AABB myAABB1 = getAABB( collider1 );
  AABB myAABB2 = getAABB( collider2 );
  
  if( myAABB1.maxX < myAABB2.minX )
    return false;
  if( myAABB1.minX > myAABB2.maxX )
    return false;
  if( myAABB1.maxY < myAABB2.minY )
    return false;
  if( myAABB1.minY > myAABB2.maxY )
    return false;
  if( myAABB1.maxZ < myAABB2.minZ )
    return false;
  if( myAABB1.minZ > myAABB2.maxZ )
    return false;
  
  return true;
}


bool areCollidingActual( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 ) {
  
  for( unsigned int i = 0; i < collider1.size(); i += 4 ) {
    for( unsigned int j = 0; j < collider2.size(); j += 4 ) {
      
      real p1[ 3 ] = { collider1[ i + 0 ].x, collider1[ i + 0 ].y, collider1[ i + 0 ].z };
      real q1[ 3 ] = { collider1[ i + 1 ].x, collider1[ i + 1 ].y, collider1[ i + 1 ].z };
      real r1[ 3 ] = { collider1[ i + 2 ].x, collider1[ i + 2 ].y, collider1[ i + 2 ].z };
      
      real p2[ 3 ] = { collider2[ j + 0 ].x, collider2[ j + 0 ].y, collider2[ j + 0 ].z };
      real q2[ 3 ] = { collider2[ j + 1 ].x, collider2[ j + 1 ].y, collider2[ j + 1 ].z };
      real r2[ 3 ] = { collider2[ j + 2 ].x, collider2[ j + 2 ].y, collider2[ j + 2 ].z };
      
      if( tri_tri_overlap_test_2d( p1, q1, r1, p2, q2, r2 ) == 1 )
        return true;
    }
  }
  return false;
}

Collision::Collision( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 ) {
  
  if( areCollidingAABB( collider1, collider2 ) )
    if( areCollidingActual( collider1, collider2 ) )
      areColliding_ = true;
}

Collision::~Collision() {
  
}

