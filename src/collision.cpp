#include "collision.hpp"
#include <iostream>
#include "mesh.hpp"
#include "physicsObject.hpp"

bool areColliding( AABB a, AABB b ) {
  
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

CollisionData Collision::collisionData( std::shared_ptr<PhysicsObject> a, std::shared_ptr<PhysicsObject> b ) {
  
  CollisionData collisionData;
  
  if( areColliding( a -> aabb(), b -> aabb() ) )
    collisionData.setCollisionData();
  
  return collisionData;
}