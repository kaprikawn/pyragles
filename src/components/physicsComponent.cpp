#include "physicsComponent.hpp"
#include <iostream>
#include "../entity.hpp"

PhysicsComponent::PhysicsComponent() {
  
}

void PhysicsComponent::update( float dt ) {
  getParent() -> position_ += velocity_ * dt;
}

PhysicsComponent::~PhysicsComponent() {
  
}
