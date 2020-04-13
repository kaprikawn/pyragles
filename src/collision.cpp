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
<<<<<<< HEAD
    } else {
      
=======
        // printf( "fp############################\n" );
    } else {
      
      // std::cout << "hey x is " << collider[ i ].x << std::endl;
      
>>>>>>> 8dfd2ca90e05b58fd3497c968b0c437a51bc8539
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
<<<<<<< HEAD
=======
        
      // if( collider[ i ].x < -1000000.0f || collider[i].x > 1000000.0f )
      //   printf( "############################\n" );
>>>>>>> 8dfd2ca90e05b58fd3497c968b0c437a51bc8539
    }
    
    myAABB.firstPass = false;
  }
  
<<<<<<< HEAD
=======
  // std::cout << "minX is " << myAABB.minX << std::endl;
  // std::cout << "maxX is " << myAABB.maxX << std::endl;
  
>>>>>>> 8dfd2ca90e05b58fd3497c968b0c437a51bc8539
  return myAABB;
}

bool areCollidingAABB( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 ) {
  
<<<<<<< HEAD
  AABB myAABB1 = getAABB( collider1 );
  AABB myAABB2 = getAABB( collider2 );
  
=======
  // do AABB detection as a first pass - is more efficient
  
  // for( unsigned int i = 0; i < collider2.size(); i++ ) {
  //   printf( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
    
  //   std::cout << "x is " << collider2[ i ].x << std::endl;
    
  //   printf( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
  // }
  
  AABB myAABB1 = getAABB( collider1 );
  AABB myAABB2 = getAABB( collider2 );
  
  // std::cout << "myAABB1.maxX = " << myAABB1.maxX << std::endl;
  // std::cout << "myAABB2.minX = " << myAABB2.minX << std::endl;
  
>>>>>>> 8dfd2ca90e05b58fd3497c968b0c437a51bc8539
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

<<<<<<< HEAD

bool areCollidingActual( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 ) {
  
  for( unsigned int i = 0; i < collider1.size(); i += 4 ) {
    for( unsigned int j = 0; j < collider2.size(); j += 4 ) {
      
      real p1[ 3 ] = { collider1[ i + 0 ].x, collider1[ i + 0 ].y, collider1[ i + 0 ].z };
      real q1[ 3 ] = { collider1[ i + 1 ].x, collider1[ i + 1 ].y, collider1[ i + 1 ].z };
      real r1[ 3 ] = { collider1[ i + 2 ].x, collider1[ i + 2 ].y, collider1[ i + 2 ].z };
      
      real p2[ 3 ] = { collider2[ j + 0 ].x, collider2[ j + 0 ].y, collider2[ j + 0 ].z };
      real q2[ 3 ] = { collider2[ j + 1 ].x, collider2[ j + 1 ].y, collider2[ j + 1 ].z };
      real r2[ 3 ] = { collider2[ j + 2 ].x, collider2[ j + 2 ].y, collider2[ j + 2 ].z };
=======
// bool areCollisingMesh( std::vector<std::array<glm::vec3, 3>> meshA, std::vector<std::array<glm::vec3, 3>> meshB ) {
  
//   for( unsigned int a = 0; a < meshA.size(); a++ ) {
//     for( unsigned int b = 0; b < meshB.size(); b++ ) {
      
//       real p1[3] = { meshA[a][0].x, meshA[a][0].y, meshA[a][0].z };
//       real q1[3] = { meshA[a][1].x, meshA[a][1].y, meshA[a][1].z };
//       real r1[3] = { meshA[a][2].x, meshA[a][2].y, meshA[a][2].z };
      
//       real p2[3] = { meshB[b][0].x, meshB[b][0].y, meshB[b][0].z };
//       real q2[3] = { meshB[b][1].x, meshB[b][1].y, meshB[b][1].z };
//       real r2[3] = { meshB[b][2].x, meshB[b][2].y, meshB[b][2].z };
      
//       if( tri_tri_overlap_test_2d( p1, q1, r1, p2, q2, r2 ) == 1)
//         return true;
//     }
//   }
  
//   return false;
// }

bool areCollidingActual( std::vector<glm::vec4> collider1, std::vector<glm::vec4> collider2 ) {
  
  for( unsigned int i = 0; i < collider1.size(); i += 3 ) {
    for( unsigned int j = 0; j < collider2.size(); j += 3 ) {
      
      real p1[ 3 ] = { collider1[ i + 0 ].x, collider1[ i + 0 ].y, collider1[ i + 0 ].z };
      real q1[ 3 ] = { collider1[ i + 1 ].x, collider1[ i + 1 ].y, collider1[ i + 1 ].z };
      real r1[ 3 ] = { collider1[ i + 3 ].x, collider1[ i + 2 ].y, collider1[ i + 2 ].z };
      
      real p2[ 3 ] = { collider2[ j + 0 ].x, collider2[ j + 0 ].y, collider2[ j + 0 ].z };
      real q2[ 3 ] = { collider2[ j + 1 ].x, collider2[ j + 1 ].y, collider2[ j + 1 ].z };
      real r2[ 3 ] = { collider2[ j + 3 ].x, collider2[ j + 2 ].y, collider2[ j + 2 ].z };
>>>>>>> 8dfd2ca90e05b58fd3497c968b0c437a51bc8539
      
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

