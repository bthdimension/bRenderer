#pragma once

#include "Framework_GL.h"
#include "Logger.h"
#include <sstream>  

namespace bRenderer
{
	bool isInitialized();
	bool isRunning();

	GLint getWindowWidth();
	GLint getWindowHeight();
	GLFWwindow* getWindow();

	void setWindowWidth(GLint width);
	void setWindowHeight(GLint height);
	void setWindowSize(GLint width, GLint height);

	void defineInitFunction(void(*f)());
	void defineLoopFunction(void(*f)(const GLfloat deltaTime, const GLfloat elapsedTime));
	void defineTerminateFunction(void(*f)());

	bool initRenderer();
	bool initRenderer(GLint width, GLint height);	

	void runRenderer();	
	void stopRenderer();

	void terminateRenderer();
}