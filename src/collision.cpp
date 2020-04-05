#include "collision.hpp"
#include <iostream>

AABB getAABB( std::vector<glm::vec3>collider ) {
  
  AABB myAABB;
  
  for( unsigned int i = 0; i < collider.size(); i++ ) {
    
    if( myAABB.firstPass ) {
      myAABB.minX = myAABB.maxX = collider[ i ].x;
      myAABB.minY = myAABB.maxY = collider[ i ].y;
      myAABB.minZ = myAABB.maxZ = collider[ i ].z;
        // printf( "fp############################\n" );
    } else {
      
      // std::cout << "hey x is " << collider[ i ].x << std::endl;
      
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
        
      // if( collider[ i ].x < -1000000.0f || collider[i].x > 1000000.0f )
      //   printf( "############################\n" );
    }
    
    myAABB.firstPass = false;
  }
  
  // std::cout << "minX is " << myAABB.minX << std::endl;
  // std::cout << "maxX is " << myAABB.maxX << std::endl;
  
  return myAABB;
}

bool areCollidingAABB( std::vector<glm::vec3>collider1, std::vector<glm::vec3>collider2 ) {
  
  // for( unsigned int i = 0; i < collider2.size(); i++ ) {
  //   printf( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
    
  //   std::cout << "x is " << collider2[ i ].x << std::endl;
    
  //   printf( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
  // }
  
  AABB myAABB1 = getAABB( collider1 );
  AABB myAABB2 = getAABB( collider2 );
  
  // std::cout << "myAABB1.maxX = " << myAABB1.maxX << std::endl;
  // std::cout << "myAABB2.minX = " << myAABB2.minX << std::endl;
  
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
  
  // if( 
  //      a.right  <= b.left
  //   || a.left   >= b.right
  //   || a.bottom >= b.top
  //   || a.top    <= b.bottom
  //   || a.front  >= b.back
  //   || a.back   <= b.front
  // ) {
  //   return false;
  // }
}

Collision::Collision( std::vector<glm::vec3>collider1, std::vector<glm::vec3>collider2 ) {
  
  if( areCollidingAABB( collider1, collider2 ) )
    areColliding_ = true;
  
  
}

Collision::~Collision() {
  
}

