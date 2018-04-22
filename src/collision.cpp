#include "collision.hpp"
#include <iostream>
#include "mesh.hpp"
#include "physicsObject.hpp"
#include "triangleOverlap.h"

bool areCollidingAABB( AABB a, AABB b ) {
  
  if( 
       a.right  <= b.left
    || a.left   >= b.right
    || a.bottom >= b.top
    || a.top    <= b.bottom
    || a.front  >= b.back
    || a.back   <= b.front
  ) {
    return false;
  }
  
  /* std::cout << a.right << " <= " << b.left << std::endl;
  std::cout << a.left << " >= " << b.right << std::endl;
  std::cout << a.bottom << " >= " << b.top << std::endl;
  std::cout << a.top << " <= " << b.bottom << std::endl;
  std::cout << a.front << " >= " << b.back << std::endl;
  std::cout << a.back << " <= " << b.front << std::endl; */
    
  return true;
}

bool areCollisingMesh( std::vector<std::array<glm::vec3, 3>> meshA, std::vector<std::array<glm::vec3, 3>> meshB ) {
  
  for( unsigned int a = 0; a < meshA.size(); a++ ) {
    for( unsigned int b = 0; b < meshB.size(); b++ ) {
      
      real p1[3] = { meshA[a][0].x, meshA[a][0].y, meshA[a][0].z };
      real q1[3] = { meshA[a][1].x, meshA[a][1].y, meshA[a][1].z };
      real r1[3] = { meshA[a][2].x, meshA[a][2].y, meshA[a][2].z };
      
      real p2[3] = { meshB[b][0].x, meshB[b][0].y, meshB[b][0].z };
      real q2[3] = { meshB[b][1].x, meshB[b][1].y, meshB[b][1].z };
      real r2[3] = { meshB[b][2].x, meshB[b][2].y, meshB[b][2].z };
      
      if( tri_tri_overlap_test_2d( p1, q1, r1, p2, q2, r2 ) == 1)
        return true;
    }
  }
  
  return false;
}

CollisionData Collision::collisionData( std::shared_ptr<PhysicsObject> a, std::shared_ptr<PhysicsObject> b ) {
  
  CollisionData collisionData;
  
  if( !areCollidingAABB( a -> aabb(), b -> aabb() ) )
    return collisionData;
  
  if( !areCollisingMesh( a -> mesh(), b -> mesh() ) )
    return collisionData;
  
  collisionData.setCollisionData();
  
  return collisionData;
}