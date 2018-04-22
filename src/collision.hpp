#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <memory>
#include "collisionData.hpp"

class PhysicsObject;

class Collision {
  
  private:
    
  public:
    
    Collision(){}
    ~Collision(){}
    
    CollisionData collisionData( std::shared_ptr<PhysicsObject> a, std::shared_ptr<PhysicsObject> b );
    
};

#endif // COLLISION_HPP