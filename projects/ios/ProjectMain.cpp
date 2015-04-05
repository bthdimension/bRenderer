
#include "ProjectMain.h"


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
        // test something after 2 seconds
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


ProjectMain::ProjectMain()
{
    // define the functions to be called by the renderer
    defineInitFunction(bInitialize);
    defineLoopFunction(bLoop);
    defineTerminateFunction(bTerminate);
    
    // let the renderer create an OpenGL context
    initRenderer();
}


ProjectMain::~ProjectMain()
{
    log("ProjectMain object was deleted");
}