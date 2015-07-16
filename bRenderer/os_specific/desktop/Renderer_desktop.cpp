#include "../../headers/OSdetect.h"

#ifdef OS_DESKTOP

#include "../../headers/Renderer.h"
#include "../../headers/IRenderProject.h"

/* Public functions */

bool Renderer::isRunning()
{
	return _running;
}

void Renderer::runRenderer()
{
	_running = true;
	bRenderer::log("Renderer started", bRenderer::LM_SYS);

	_initialTime += (_view->getTime() - _initialTime) - _stopTime;

	// Loop until the user closes the window 
	while (_running && _view->isRunning())
	{
		draw((_view->getTime() - _initialTime));

		// Poll for and process events
		glfwPollEvents();
	}

	if (!_view->isRunning())
		terminateRenderer();
}

void Renderer::stopRenderer()
{
	_stopTime = (_view->getTime() - _initialTime);
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

void Renderer::draw(double currentTime)
{
	_view->setContextCurrent();

	// clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render here
	if (_loopFunction)
		_loopFunction(currentTime - _elapsedTime, currentTime);

	if (_renderProject)
		_renderProject->loopFunction(currentTime - _elapsedTime, currentTime);

	// Adjust time
	_elapsedTime = currentTime;

	// Swap front and back buffers 
	_view->swapBuffers();
}

#endif