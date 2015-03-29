#pragma once

#include "Framework_GL.h"
#include "Logger.h"

class TriangleTest
{
private:
	GLuint vertexbuffer;
	GLuint VertexArrayID;
public:
	TriangleTest();
	~TriangleTest();

	void initializeTriangle();
	void drawTriangle();
};

