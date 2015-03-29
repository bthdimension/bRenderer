#include "bRenderer.h"

namespace bRenderer
{
	/* Internal variables */

	static bool initialized = false;
	static bool running = false;
	
	GLFWwindow* window;

	static GLint windowWidth = 640;
	static GLint windowHeight = 480;

	static GLfloat elapsedTime = 0;

	static void(*initFunction)();
	static void(*loopFunction)(const GLfloat deltaTime, const GLfloat elapsedTime);
	static void(*terminateFunction)();

	/* Internal functions */

	void windowSizeChanged(GLFWwindow* window, int width, int height)
	{
		windowWidth = width;
		windowHeight = height;

		std::ostringstream sizeInfo;
		sizeInfo << "width: " << windowWidth << ", height: " << windowHeight;
		log(sizeInfo.str(), LM_SYS);
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

	void defineLoopFunction(void(*f)(const GLfloat deltaTime, const GLfloat elapsedTime))
	{
		loopFunction = f;
	}

	void defineTerminateFunction(void(*f)())
	{
		terminateFunction = f;
	}

	bool initRenderer()
	{
		log("Initializing bRenderer", LM_SYS);

		/* Initialize GLFW */
		if (!glfwInit())
			return false;

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(windowWidth, windowHeight, "GLFW Window", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			return false;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* Add resize handler */
		glfwSetWindowSizeCallback(window, windowSizeChanged);

		log("Window and Context created", LM_SYS);

		/* Initialize GLEW */
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			log(glewGetErrorString(err), LM_ERROR);
			return false;
		}
		log("Using GLEW ", glewGetString(GLEW_VERSION), LM_SYS);

		if (initFunction)
			initFunction();

		initialized = true;
		
		return true;
	}

	bool initRenderer(GLint width, GLint height)
	{
		windowWidth = width;
		windowHeight = height;

		if (initRenderer())
			return true;
		else
			return false;
	}

	void runRenderer()
	{
		running = true;

		/* Loop until the user closes the window */
		while (running && !glfwWindowShouldClose(window))
		{
			GLfloat time = glfwGetTime();
			
			/* Render here */
			if (loopFunction)
				loopFunction(time-elapsedTime, time);
			
			/* Adjust time */
			elapsedTime = time;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}
	}

	void stopRenderer()
	{
		running = false;
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

	

}