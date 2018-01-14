#include "triangle.hpp"
#include "game.hpp"
#include <iostream>
#include <GLES2/gl2.h>

Triangle* Triangle::instance_ = 0;

void Triangle::init() {
  
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
  SDL_GL_SetSwapInterval( 0 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
  
  context_ = SDL_GL_CreateContext( TheGame::Instance() -> getWindow() );
  if( !context_ ) {
    std::cout << "Error - failed to create OpenGL context : " << SDL_GetError() << std::endl;
  }
  
  // create shader
	const char *shader_vert_src = 
		"uniform float u_time;\n"
		"attribute vec2 va_position;\n"
		"varying vec3 v_color;\n"
		"void main() {\n"
		"	v_color = vec3(1.0 - 0.5*(va_position.x+va_position.y),va_position);\n"
		"	float c = cos(u_time), s = sin(u_time);"
		"	vec2 t = mat2(c, s, -s, c)*(va_position-vec2(0.33));\n"
		"	gl_Position = vec4(t.x*3.0/5.0, t.y, 0.0, 1.0);\n"
		"}\n";
	const char *shader_frag_src =
		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"
		"varying vec3 v_color;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(v_color, 1.0);\n"
		"}\n";
	GLint is_compiled;
	GLuint program, shader_vert, shader_frag;
	program     = glCreateProgram();
	shader_vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource( shader_vert, 1, &shader_vert_src, NULL );
	glCompileShader( shader_vert );
	glGetShaderiv( shader_vert, GL_COMPILE_STATUS, &is_compiled );
	printf( "vert shader compiled %d\n", is_compiled );
	glAttachShader( program, shader_vert );
	shader_frag = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( shader_frag, 1, &shader_frag_src, NULL );
	glCompileShader( shader_frag );
	glGetShaderiv( shader_frag, GL_COMPILE_STATUS, &is_compiled );
	printf( "frag shader compiled %d\n", is_compiled );
	glAttachShader( program, shader_frag );
	glLinkProgram( program );
	glUseProgram( program );
	u_time_loc = glGetUniformLocation(program, "u_time");
	u_time = 0.0f;
	
	// create vbo
	GLuint vbo;
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	float vertex_data[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertex_data ), vertex_data, GL_STATIC_DRAW );
 
	// setup vertex attribs
	GLuint va_position = 0;
	glEnableVertexAttribArray( va_position );
	glVertexAttribPointer( va_position, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0 );
 
	glClearColor( 0.4, 0.6, 0.8, 1.0 );
  
}

void Triangle::render() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUniform1f( u_time_loc, u_time += 1.0f / 60.0f );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	SDL_GL_SwapWindow( TheGame::Instance() -> getWindow() );
}
