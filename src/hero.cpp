#include <iostream>
#include "hero.hpp"
#include "inputHandler.hpp"

Hero::Hero() : GlObject() {
  
}

void Hero::handleInput() {

}

void Hero::update( float dt ) {

  GlObject::update( dt );
}

void Hero::render() {

  GlObject::render();
}

void Hero::clean() {
  GlObject::clean();
}
