#include "../../headers/OSdetect.h"

#ifdef OS_DESKTOP

#include <boost/lexical_cast.hpp>
#include "../../bRenderer.h"

using boost::lexical_cast;

namespace bRenderer
{
	/* Internal variables */

	static View _view;

	static bool _initialized = false;
	static bool _running = false;

	static double _currentTime = 0;
	static double _elapsedTime = 0;

	static RenderProject * _renderProject;

	static void(*_initFunction)();
	static void(*_loopFunction)(const double deltaTime, const double elapsedTime);
	static void(*_terminateFunction)();

	/* External functions */

	View *getView()
	{
		return &_view;
	}

	bool isInitialized()
	{
		return _initialized;
	}

	bool isRunning()
	{
		return _running;
	}

	void setRenderProject(RenderProject *p)
	{
		_renderProject = p;
	}

	void setInitFunction(void(*f)())
	{
		_initFunction = f;
	}

	void setLoopFunction(void(*f)(const double deltaTime, const double elapsedTime))
	{
		_loopFunction = f;
	}

	void setTerminateFunction(void(*f)())
	{
		_terminateFunction = f;
	}

	bool initRenderer()
	{
		if (_initialized)
			return true;

		// initialize view
		if (!_view.initView())
			return false;

		// call static function if set
		if (_initFunction)
			_initFunction();

		// call member function if set
		if (_renderProject)
			_renderProject->initFunction();

		_initialized = true;

		return true;
	}

	bool initRenderer(bool fullscreen)
	{
		if (_initialized)
			return true;

		// initialize view
		if (!_view.initView(fullscreen))
			return false;

		return initRenderer();
	}

	bool initRenderer(GLint width, GLint height, bool fullscreen)
	{
		if (_initialized)
			return true;

		// initialize view
		if (!_view.initView(width, height, fullscreen))
			return false;

		return initRenderer();
	}

	void draw()
	{
		_view.setContextCurrent();
		double time = _view.getTime();

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

	void runRenderer()
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

	void stopRenderer()
	{
		_running = false;
        log("Renderer stopped", LM_SYS);
	}

	void terminateRenderer()
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
    
    

} // namespace bRenderer

#endif