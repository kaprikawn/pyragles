#include "component.hpp"
#include <iostream>

Component::Component() {
  
}

bool Component::initialize() {
  return true;
}

bool Component::shutdown() {
  return true;
}

Component::~Component() {
  
}

