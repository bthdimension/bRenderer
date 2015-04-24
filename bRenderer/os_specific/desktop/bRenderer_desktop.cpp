#include "../../headers/OSdetect.h"

#ifdef OS_DESKTOP

#include <boost/lexical_cast.hpp>
#include "../../bRenderer.h"

using boost::lexical_cast;

namespace bRenderer
{
	/* Internal variables */

	char *_windowTitle = "bRenderer";

	GLFWwindow *_window;

	static bool _initialized = false;
	static bool _running = false;
	static bool _fullscreen = true;

	static GLint _width = 640;
	static GLint _height = 480;

	static double _currentTime = 0;
	static double _elapsedTime = 0;

	RenderProject * _renderProject;

	static void(*_initFunction)();
	static void(*_loopFunction)(const double deltaTime, const double elapsedTime);
	static void(*_terminateFunction)();

	/* Internal functions */

	void windowSizeChanged(GLFWwindow* window, int width, int height)
	{
		_width = width;
		_height = height;
		glViewport(0, 0, _width, _height);
		log("width: " + lexical_cast<std::string>(_width) + ", height: " + lexical_cast<std::string>(_height), LM_SYS);
	}

	/* External functions */

	bool isInitialized()
	{
		return _initialized;
	}

	bool isRunning()
	{
		return _running;
	}

	bool isFullscreen()
	{
		return _fullscreen;
	}

	GLint getWindowWidth()
	{
		return _width;
	}

	GLint getWindowHeight()
	{
		return _height;
	}

    GLint getWindowPositionX()
    {
        int x, y;
        glfwGetWindowPos(_window, &x, &y);
        return x;
    }
    
    GLint getWindowPositionY()
    {
        int x, y;
        glfwGetWindowPos(_window, &x, &y);
        return y;
    }
    
	void getWindowPosition(GLint* x, GLint* y)
	{
		glfwGetWindowPos(_window, x, y);
	}

	GLFWwindow* getWindow()
	{
		return _window;
	}

	void setWindowFullscreen(bool fullscreen)
	{
		log("Not yet supported on desktop systems", LM_WARNING);
	}

	void setWindowWidth(GLint width)
	{
		glfwSetWindowSize(_window, width, _height);
	}
	
	void setWindowHeight(GLint height)
	{
		glfwSetWindowSize(_window, _width, height);
	}

	void setWindowSize(GLint width, GLint height)
	{
		glfwSetWindowSize(_window, width, height);
	}

	void setWindowPosition(GLint x, GLint y)
	{
		glfwSetWindowPos(_window, x, y);
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
		if (initRenderer(false))
			return true;
		else
			return false;
	}

	bool initRenderer(bool fullscreen)
	{
		log("Initializing bRenderer", LM_SYS);

		// Initialize GLFW 
		if (!glfwInit())
			return false;

		// Create a windowed mode window and its OpenGL context 
		_window = glfwCreateWindow(_width, _height, _windowTitle, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		_fullscreen = fullscreen;
		
		if (!_window)
		{
			glfwTerminate();
			return false;
		}

		// Make the window's context current 
		glfwMakeContextCurrent(_window);

		// Add resize handler 
		glfwSetWindowSizeCallback(_window, windowSizeChanged);

		log("Window and Context created", LM_SYS);

		// Initialize GLEW 
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			// Problem: glewInit failed, something is seriously wrong. 
			log(glewGetErrorString(err), LM_ERROR);
			return false;
		}
		log("Using GLEW ", glewGetString(GLEW_VERSION), LM_SYS);

		if (_initFunction)
			_initFunction();

		if (_renderProject)
			_renderProject->initFunction();

		_initialized = true;
		
		return true;
	}

	bool initRenderer(GLint width, GLint height, bool fullscreen)
	{
		_width = width;
		_height = height;

		if (initRenderer(fullscreen))
			return true;
		else
			return false;
	}

	void runRenderer()
	{
		_running = true;
		glfwSetWindowShouldClose(_window, GL_FALSE);
        log("Renderer started", LM_SYS);

		// Loop until the user closes the window 
		while (_running && !glfwWindowShouldClose(_window))
		{
			double time = glfwGetTime();
			
			// Render here
			if (_loopFunction)
				_loopFunction(time-_elapsedTime, time);

			if (_renderProject)
				_renderProject->loopFunction(time - _elapsedTime, time);
			
			// Adjust time 
			_elapsedTime = time;

			// Swap front and back buffers 
			glfwSwapBuffers(_window);

			// Poll for and process events
			glfwPollEvents();
		}

		if (glfwWindowShouldClose(_window))
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
		log("Renderer terminated", LM_SYS);
	}


} // namespace bRenderer

#endif