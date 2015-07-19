#include "../../headers/OSdetect.h"

#ifdef B_OS_DESKTOP

#include "../../headers/Renderer.h"
#include "../../headers/IRenderProject.h"

/* Public functions */

void Renderer::runRenderer()
{
	_running = true;
	bRenderer::log("Renderer started", bRenderer::LM_SYS);

	_initialTime += (glfwGetTime() - _initialTime) - _stopTime;

	// loop until the user closes the window
	while (_running && _view->isRunning())
	{
		draw();

		// poll for and process events
		glfwPollEvents();
	}

	if (!_view->isRunning())
		terminateRenderer();
}

void Renderer::stopRenderer()
{
	_stopTime = (glfwGetTime() - _initialTime);
	_running = false;
	bRenderer::log("Renderer stopped", bRenderer::LM_SYS);
}

void Renderer::terminateRenderer()
{
	_running = false;
	_initialized = false;

	if (_terminateFunction)
		_terminateFunction();

	if (_renderProject)
		_renderProject->terminateFunction();

	_view->terminateView();

	reset();

	bRenderer::log("Renderer terminated", bRenderer::LM_SYS);
}

/* Private functions */

void Renderer::draw()
{
    // get time
    double currentTime = (glfwGetTime() - _initialTime);
    
	_view->setContextCurrent();

	// clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render here
	if (_loopFunction)
		_loopFunction(currentTime - _elapsedTime, currentTime);

	if (_renderProject)
		_renderProject->loopFunction(currentTime - _elapsedTime, currentTime);

	// adjust time
	_elapsedTime = currentTime;

	// display
	_view->presentBuffer();
}

#endif