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
	static GLuint loadProgramVSGSFS(const char* _fileNameVS, const char* _fileNameGS, const char* _fileNameFS);
	static GLuint TextureFromFile(const char* filename);
	static void TextureFromFileAttach(const char* filename, GLuint role);
};