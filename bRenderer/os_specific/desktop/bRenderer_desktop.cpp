#include "../../headers/OSdetect.h"

#ifdef OS_DESKTOP

#include <boost/lexical_cast.hpp>
#include "../../bRenderer.h"

using boost::lexical_cast;

namespace bRenderer
{
	/* Internal variables */

	static bool initialized = false;
	static bool running = false;
	
	GLFWwindow* window;

	static GLint windowWidth = 640;
	static GLint windowHeight = 480;

	static double elapsedTime = 0;

	static void(*initFunction)();
	static void(*loopFunction)(const double deltaTime, const double elapsedTime);
	static void(*terminateFunction)();
    
    static GLint defaultFramebuffer = -1;

	/* Internal functions */

	void windowSizeChanged(GLFWwindow* window, int width, int height)
	{
		windowWidth = width;
		windowHeight = height;
		log("width: " + lexical_cast<std::string>(windowWidth) + ", height: " + lexical_cast<std::string>(windowWidth), LM_SYS);
	}

	/* External functions */

	bool isInitialized()
	{
		return initialized;
	}

	bool isRunning()
	{
		return running;
	}

	GLint getWindowWidth()
	{
		return windowWidth;
	}

	GLint getWindowHeight()
	{
		return windowHeight;
	}

	GLFWwindow* getWindow()
	{
		return window;
	}

	void setWindowWidth(GLint width)
	{
		glfwSetWindowSize(window, width, windowHeight);
	}
	
	void setWindowHeight(GLint height)
	{
		glfwSetWindowSize(window, windowWidth, height);
	}

	void setWindowSize(GLint width, GLint height)
	{
		glfwSetWindowSize(window, width, height);
	}

	void defineInitFunction(void(*f)())
	{
		initFunction = f;
	}

	void defineLoopFunction(void(*f)(const double deltaTime, const double elapsedTime))
	{
		loopFunction = f;
	}

	void defineTerminateFunction(void(*f)())
	{
		terminateFunction = f;
	}

	bool initRenderer(bool fullscreen)
	{
		log("Initializing bRenderer", LM_SYS);

		// Initialize GLFW 
		if (!glfwInit())
			return false;

		// Create a windowed mode window and its OpenGL context 
		if (fullscreen)
			window = glfwCreateWindow(windowWidth, windowHeight, "GLFW Window", glfwGetPrimaryMonitor(), NULL);
		else
			window = glfwCreateWindow(windowWidth, windowHeight, "GLFW Window", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			return false;
		}

		// Make the window's context current 
		glfwMakeContextCurrent(window);

		// Add resize handler 
		glfwSetWindowSizeCallback(window, windowSizeChanged);

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

		if (initFunction)
			initFunction();

		initialized = true;
		
		return true;
	}

	bool initRenderer(GLint width, GLint height, bool fullscreen)
	{
		windowWidth = width;
		windowHeight = height;

		if (initRenderer(fullscreen))
			return true;
		else
			return false;
	}

	void runRenderer()
	{
		running = true;
		glfwSetWindowShouldClose(window, GL_FALSE);
        log("Renderer started", LM_SYS);

		// Loop until the user closes the window 
		while (running && !glfwWindowShouldClose(window))
		{
			double time = glfwGetTime();
			
			// Render here
			if (loopFunction)
				loopFunction(time-elapsedTime, time);
			
			// Adjust time 
			elapsedTime = time;

			// Swap front and back buffers 
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}
	}

	void stopRenderer()
	{
		running = false;
        log("Renderer stopped", LM_SYS);
	}

	void terminateRenderer()
	{
		running = false;
		initialized = false;

		if (terminateFunction)
			terminateFunction();

		glfwTerminate();
		log("Renderer terminated", LM_SYS);
	}


} // namespace bRenderer

#endif