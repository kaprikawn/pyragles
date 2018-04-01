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
  , RISE  = 2
  , FALL  = 3
  , FIRE  = 4
};

enum ShapeTypes {
  UNDEF_SHAPE, CUBE, TRIANGLE, SHIP, TARGET
};

struct Vertex {
  glm::vec3   position;
  glm::vec3   colour;
};

#endif //STRUCTS_HPP
