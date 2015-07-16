#include "../../headers/View.h"
#include <vector>

/* Constructor and destructor */
View::View()
{
	
}

View::~View()
{
    _view = nil;
}

/* Public functions */

bool View::initView()
{
    if (_initialized)
        return true;
    
    bRenderer::log("Initializing view", bRenderer::LM_SYS);
    
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    _view = [[BView alloc] initWithFrame:frame];
    
    _fullscreen = true;
    
    _initialized = true;
    
    return true;
}

bool View::initView(bool fullscreen)
{
    if (_initialized)
        return true;
    
    if (!fullscreen)
        bRenderer::log("Can't set fullscreen to flase without setting width and height on iOS.", bRenderer::LM_WARNING);
    
    return initView();
}

bool View::initView(GLint width, GLint height, bool fullscreen)
{
    if (_initialized)
        return true;
    
    initView();
   
    if (fullscreen){
        bRenderer::log("Can't set width and height when using fullscreen on iOS.", bRenderer::LM_WARNING);
    }
    else{
        [_view setViewWidth:width setViewHeight:height];
        _fullscreen = false;
    }
    
    return true;
}

void View::terminateView()
{
	setRunning(false);

    [_view removeFromSuperview];
    _view = nil;

	_initialized = false;
}

bool View::isInitialized()
{
    return _initialized;
}

bool View::isRunning()
{
    return [_view isRunning];
}

bool View::isFullscreen()
{
    return _fullscreen;
}

GLint View::getWidth()
{
    return [_view getViewWidth];
}

GLint View::getHeight()
{
    return [_view getViewHeight];
}

void View::getSize(GLint* width, GLint* height)
{
    *width = getWidth();
    *height = getHeight();
}

GLint View::getScreenWidth()
{
	return [[UIScreen mainScreen] bounds].size.width;
}

GLint View::getScreenHeight()
{
	return [[UIScreen mainScreen] bounds].size.height;
}

void View::getScreenSize(GLint* width, GLint* height)
{
	*width = getScreenWidth();
	*height = getScreenHeight();
}

GLfloat View::getAspectRatio()
{
    return (GLfloat)getWidth() / (GLfloat)getHeight();
}

GLint View::getPositionX()
{
    return [_view getViewPositionX];
}

GLint View::getPositionY()
{
    return [_view getViewPositionY];
}

void View::getPosition(GLint* x, GLint* y)
{
    *x = getPositionX();
    *y = getPositionY();
}

View::GLFWwindow* View::getWindow()
{
    return 0;
}

BView* View::getUIView()
{
    return _view;
}

void View::attachToUIView(UIView* view)
{
    [_view removeFromSuperview];
    [view addSubview:_view];
    
    if(_fullscreen)
        [_view setFullscreen];
}

double View::getTime()
{
    return [_view getTime];
}

void View::setRunning(bool running)
{
    if(running)
        [_view runRenderer];
    else
        [_view stopRenderer];
}

void View::setFullscreen(bool fullscreen)
{
    if(fullscreen){
        [_view setFullscreen];
        _fullscreen = true;
    }
    else
        bRenderer::log("Set width and height to set fullscreen to flase", bRenderer::LM_WARNING);
}


void View::setWidth(GLint width)
{
    [_view setViewWidth:width setViewHeight:getHeight()];
    _fullscreen = false;
}

void View::setHeight(GLint height)
{
    [_view setViewWidth:getWidth() setViewHeight:height];
    _fullscreen = false;
}

void View::setSize(GLint width, GLint height)
{
    [_view setViewWidth:width setViewHeight:height];
    _fullscreen = false;
}

void View::setPosition(GLint x, GLint y)
{
    [_view setViewPositionX:x setViewPositionY:y];
}

void View::setContextCurrent()
{
	[_view setContextCurrent];
}

void View::swapBuffers()
{
    bRenderer::log("Not yet supported on iOS", bRenderer::LM_WARNING);
}

static void windowSizeChanged(View::GLFWwindow* window, int width, int height)
{
    // Not implemented on iOS
}
