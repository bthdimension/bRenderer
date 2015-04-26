#include "../../headers/OSdetect.h"

#ifdef OS_DESKTOP

#include "../../headers/View.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;


/* Defines */
#define DEFAULT_WIDTH	640
#define DEFAULT_HEIGHT	480

/* Constructor and destructor */
View::View()
{
	_windowTitle = "bRenderer";
	_initialized = false;
	_fullscreen = false;
}

View::~View()
{
		
}

/* Public functions */

bool View::initView()
{
	if (initView(_fullscreen))
		return true;
	else
		return false;
}

bool View::initView(bool fullscreen)
{
	if (initView(DEFAULT_WIDTH, DEFAULT_HEIGHT, fullscreen))
		return true;
	else
		return false;
}

bool View::initView(GLint width, GLint height, bool fullscreen)
{
	if (_initialized)
		return true;

	bRenderer::log("Initializing view", bRenderer::LM_SYS);

	// Initialize GLFW 
	if (!glfwInit())
		return false;

	// Create a windowed mode window and its OpenGL context 
	_window = glfwCreateWindow(width, height, _windowTitle, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
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

	bRenderer::log("Window and Context created", bRenderer::LM_SYS);

	// Initialize GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong. 
		bRenderer::log(glewGetErrorString(err), bRenderer::LM_ERROR);
		return false;
	}
	bRenderer::log("Using GLEW ", glewGetString(GLEW_VERSION), bRenderer::LM_SYS);

	_initialized = true;

	return true;
}

bool View::isInitialized()
{
	return _initialized;
}

bool View::isRunning()
{
	return !glfwWindowShouldClose(_window);
}

bool View::isFullscreen()
{
	return _fullscreen;
}

GLint View::getWidth()
{
	int w, h;
	getSize(&w, &h);
	return w;
}

GLint View::getHeight()
{
	int w, h;
	getSize(&w, &h);
	return h;
}

void View::getSize(GLint* width, GLint* height)
{
	glfwGetWindowSize(_window, width, height);
}

GLfloat View::getAspectRatio()
{
	int w, h;
	getSize(&w, &h);
	return (GLfloat)w / (GLfloat)h;
}

GLint View::getPositionX()
{
	int x, y;
	getPosition(&x, &y);
	return x;
}

GLint View::getPositionY()
{
	int x, y;
	getPosition(&x, &y);
	return y;
}

void View::getPosition(GLint* x, GLint* y)
{
	glfwGetWindowPos(_window, x, y);
}

GLFWwindow* View::getWindow()
{
	return _window;
}

double View::getTime()
{
	return glfwGetTime();
}

void View::setRunning(bool running)
{
    bRenderer::log("Not supported on desktop systems", bRenderer::LM_WARNING);
}

void View::setFullscreen(bool fullscreen)
{
	bRenderer::log("Not yet supported on desktop systems", bRenderer::LM_WARNING);
}


void View::setWidth(GLint width)
{
	glfwSetWindowSize(_window, width, getHeight());
}

void View::setHeight(GLint height)
{
	glfwSetWindowSize(_window, getWidth(), height);
}

void View::setSize(GLint width, GLint height)
{
	glfwSetWindowSize(_window, width, height);
}

void View::setPosition(GLint x, GLint y)
{
	glfwSetWindowPos(_window, x, y);
}

void View::setContextCurrent()
{
	glfwMakeContextCurrent(_window);
}

void View::swapBuffers()
{
	glfwSwapBuffers(_window);
}

/* Private static functions */

void View::windowSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	bRenderer::log("width: " + lexical_cast<std::string>(width)+", height: " + lexical_cast<std::string>(height), bRenderer::LM_SYS);
}


#endif