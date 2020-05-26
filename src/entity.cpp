#include "entity.hpp"
#include <iostream>
#include <cassert>
#include "component.hpp"

Entity::Entity() {
  numComponents_  = 0;
  position_       = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}

bool Entity::initalize() {
  
  for( unsigned int i = 0; i < numComponents_; i++ )
    if( ! components_[ i ] -> initialize() )
      return false;
  
  return true;
}

void Entity::addComponent( Component* component ) {
  assert( numComponents_ != NUM_MAX_COMPONENTS );
  components_[ numComponents_++ ] = component;
  component -> parent_ = this;
}

void Entity::update( float dt ) {
  for( unsigned int i = 0; i < numComponents_; i++ )
    components_[ i ] -> update( dt );
}

bool Entity::shutdown() {
  return true;
}


Entity::~Entity() {
  
}

