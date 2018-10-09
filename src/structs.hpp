#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#define windowWidth   1280.0f
#define windowHeight  720.0f

#include <glm/glm.hpp>

enum GameStates {
  TRANSITION, PLAY, MENU
};

enum TransitionType {
  LOADLEVEL, GAMEOVER, SPLASH
};

enum buttons {
    RIGHT = 0
  , LEFT  = 1
  , UP    = 2
  , DOWN  = 3
};

enum ShapeTypes {
  UNDEF_SHAPE, CUBE, TRIANGLE
};

struct Vertex {
  glm::vec3   position  = glm::vec3( 0, 0, 0 );
  glm::vec3   colour    = glm::vec3( 0, 0, 0 );
};

#endif //STRUCTS_HPP
