#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <glm/glm.hpp>

enum GameStates {
  TRANSITION, PLAY, MENU
};

enum TransitionType {
  LOADLEVEL, GAMEOVER, SPLASH
};

enum ShapeTypes {
  UNDEF_SHAPE, CUBE, TRIANGLE
};

struct Vertex {
  glm::vec3   position  = glm::vec3( 0, 0, 0 );
  glm::vec3   colour    = glm::vec3( 0, 0, 0 );
};

#endif //STRUCTS_HPP
