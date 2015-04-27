#include "../../headers/OSdetect.h"

#ifdef OS_DESKTOP

#include "../../headers/Renderer.h"
#include "../../headers/RenderProject.h"

/* Public functions */

bool Renderer::isRunning()
{
	return _running;
}

void Renderer::runRenderer()
{
	_running = true;
	bRenderer::log("Renderer started", bRenderer::LM_SYS);

	// Loop until the user closes the window 
	while (_running && _view.isRunning())
	{
		draw();

		// Poll for and process events
		glfwPollEvents();
	}

	if (!_view.isRunning())
		terminateRenderer();
}

void Renderer::stopRenderer()
{
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

	glfwTerminate();

	bRenderer::log("Renderer terminated", bRenderer::LM_SYS);
}

void Renderer::draw()
{
	_view.setContextCurrent();
	double time = _view.getTime();

	// clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render here
	if (_loopFunction)
		_loopFunction(time - _elapsedTime, time);

	if (_renderProject)
		_renderProject->loopFunction(time - _elapsedTime, time);

	// Adjust time
	_elapsedTime = time;

	// Swap front and back buffers 
	_view.swapBuffers();
}

#endif