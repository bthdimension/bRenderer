
#include "bRenderer.h"
#include "TriangleTest.h"

using namespace std;
using namespace bRenderer;

TriangleTest triangle;

/* This function is executed when initializing the renderer */
void bInitialize()
{
	log("I just created a triangle :-D");
	triangle.initializeTriangle();
}

/* Draw your scene here */
void bLoop(const double deltaTime, const double elapsedTime)
{
	if (((int)elapsedTime % 2) >= 1)
	{
		/* Test something after 2 seconds*/
		glClearColor(0.0, 1.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
	}
	else
	{
		glClearColor(1.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	triangle.drawTriangle();


}

/* This function is executed when terminating the renderer */
void bTerminate()
{
	log("I totally terminated this Renderer :-)");
}


int main(void)
{
	char input;

	// define the functions to be called by the renderer
	defineInitFunction(bInitialize);
	defineLoopFunction(bLoop);
	defineTerminateFunction(bTerminate);

	// let the renderer create an OpenGL context and the main window
	initRenderer(1920,	1080, false);

	// start main loop 
	runRenderer();

	// stop the renderer and close the window 
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