#pragma once

#include "bRenderer.h"
#include "TriangleTest.h"

using namespace std;
using namespace bRenderer;

TriangleTest triangle;

void initialize()
{
	log("I just created a triangle :-D");
	triangle.initializeTriangle();
}

void loop(const GLfloat deltaTime, const GLfloat elapsedTime)
{
	triangle.drawTriangle();

	if (elapsedTime >= 2.0 && elapsedTime < 2.3)
	{
		/* Test something after 2 seconds*/
	}
}

void terminate()
{
	log("I totally terminated this Renderer :-)");
}


int main(void)
{
	char input;

	defineInitFunction(initialize);
	defineLoopFunction(loop);
	defineTerminateFunction(terminate);

	initRenderer(1600,	900);

	runRenderer();

	terminateRenderer();


	cout << "type 'q' to quit" << endl;
	while (true)
	{
		cin >> input;
		if (input == 'q')
		{
			return 0;
		}
	}

	return 0;
}