#ifndef SDL_HPP
#define SDL_HPP

#ifdef _WIN32

#include "SDL.h"
#include <GL/glew.h>

#elif __linux__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#endif

#endif //SDL_HPP
