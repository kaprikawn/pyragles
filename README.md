## Pyra GLES
Companion git for following along with blog :

https://kaprikawn.wordpress.com/2018/01/07/learning-to-learn-opengl-es-2-0-part-01/

Acknowledgements

- glm header files used by terms of MIT license from https://github.com/g-truc/glm
- Project uses SDL2 http://libsdl.org/
- Project uses GLEW (Windows only) http://glew.sourceforge.net/

- stb_image.h (MIT licensed) obtained from   
https://github.com/nothings/stb/blob/master/stb_image.h

# Build on Linux

Dependencies : `git`, `git-lfs`, `sdl2` (development library), `glm`

To install on Debian (including the Pyra) :

`sudo apt install git git-lfs libsdl2-dev libglm-dev`

- Run misc/build.sh using `bash` e.g. :

`cd pyragles/misc`

`/bin/bash build.sh`

# Build on Windows

- Download and install Visual Studio
- Download and unpack [SDL2](https://libsdl.org/release/SDL2-devel-2.0.14-VC.zip) for Visual Studio
- Download and extract [GLEW](https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download) (You want `Binaries Windows 32-bit and 64-bit`, sorry for sending you to Sourceforge)
- Open `misc\build.bat` in a text editor
- Change the following lines to reflect the directories on your machine
  - `SET SDL_ROOT_DIR=C:\dev\SDL2-2.0.14` - Should be pointed to where you extracted SDL
  - `SET GLEW_ROOT_DIR=C:\dev\glew-2.2.0` - Should be pointed to where you extracted GLEW
  - `SET VCVARS_DIR=C:\"Program Files (x86)"\"Microsoft Visual Studio"\2019\Community\VC\Auxiliary\Build\` should reflect the directory of where the file `vcvarsall.bat` has been installed on your machine by Visual Studio
- Open a command line, cd into the `misc` directory, and run the build.bat file after you have saved it

In both cases, the built binary will be in the `build` directory
