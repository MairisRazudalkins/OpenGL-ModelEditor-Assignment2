#pragma once
#include <Windows.h>
#include "gl/GL.h"
#include "GL/freeglut.h"

class Texture2D
{
	GLuint id;
	int width, height;

public:
	Texture2D();
	Texture2D(char* path);
	~Texture2D();

	bool Load(char* path, int width, int height);
	bool LoadTextureTGA(const char* textureFileName);

	GLuint GetID() const { return id; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
};

