#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <GLES2/gl2.h>
#include <glm/glm.hpp>

#define windowWidth   1280.0f
#define windowHeight  720.0f

static const GLfloat SCROLL_SPEED = 15.0f;
static const GLfloat FLOOR_Y      = -6.5;

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
    UNDEF_SHAPE, SHIP, TARGET, BULLET
  , ARCH
  , FLOOR1, FLOOR2
  , MAX_SHAPE_NUM
  , MAX_SHAPE = MAX_SHAPE_NUM
};

struct Vertex {
  glm::vec3   position;
  glm::vec3   colour;
};

#endif //STRUCTS_HPP