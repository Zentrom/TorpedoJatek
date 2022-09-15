#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include <SDL_image.h>

class GLUtils {
public:
	static GLuint loadShader(GLenum _shaderType, const char* _fileName);
	static GLuint TextureFromFile(const char* filename);
};