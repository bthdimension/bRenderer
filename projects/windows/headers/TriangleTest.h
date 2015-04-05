#pragma once

#include "bRenderer.h"

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

