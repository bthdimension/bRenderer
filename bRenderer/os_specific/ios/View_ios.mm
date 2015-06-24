#import "BView.h"
#include "../../headers/View.h"
#include <vector>

static std::vector<BView *> _views;

/* Constructor and destructor */
View::View()
{
	
}

View::~View()
{
    
}

/* Public functions */

bool View::initView()
{
    if (_initialized)
        return true;
    
    bRenderer::log("Initializing view", bRenderer::LM_SYS);
    
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    BView * view = [[BView alloc] initWithFrame:frame];
    [[[[UIApplication sharedApplication] windows] objectAtIndex:0] addSubview:view];
    
    _views.push_back(view);
    _viewNo = _views.size()-1;
    
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
        [_views.at(_viewNo) setViewWidth:width setViewHeight:height];
        _fullscreen = false;
    }
    
    return true;
}

void View::terminateView()
{
	setRunning(false);

    [_views.at(_viewNo) removeFromSuperview];
    _views.at(_viewNo) = nil;
    _views.erase(_views.begin() + _viewNo);

	_initialized = false;
}

bool View::isInitialized()
{
    return _initialized;
}

bool View::isRunning()
{
    return [_views.at(_viewNo) isRunning];
}

bool View::isFullscreen()
{
    return _fullscreen;
}

GLint View::getWidth()
{
    return [_views.at(_viewNo) getViewWidth];
}

GLint View::getHeight()
{
    return [_views.at(_viewNo) getViewHeight];
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
    return [_views.at(_viewNo) getViewPositionX];
}

GLint View::getPositionY()
{
    return [_views.at(_viewNo) getViewPositionY];
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

UIView* View::getUIView()
{
    return _views.at(_viewNo);
}

void View::attachToUIView(UIView* view)
{
    [_views.at(_viewNo) removeFromSuperview];
    [view addSubview:_views.at(_viewNo)];
    
    if(_fullscreen)
        [_views.at(_viewNo) setFullscreen];
}

double View::getTime()
{
    return [_views.at(_viewNo) getTime];
}

void View::setRunning(bool running)
{
    if(running)
        [_views.at(_viewNo) runRenderer];
    else
        [_views.at(_viewNo) stopRenderer];
}

void View::setFullscreen(bool fullscreen)
{
    if(fullscreen){
        [_views.at(_viewNo) setFullscreen];
        _fullscreen = true;
    }
    else
        bRenderer::log("Set width and height to set fullscreen to flase", bRenderer::LM_WARNING);
}


void View::setWidth(GLint width)
{
    [_views.at(_viewNo) setViewWidth:width setViewHeight:getHeight()];
    _fullscreen = false;
}

void View::setHeight(GLint height)
{
    [_views.at(_viewNo) setViewWidth:getWidth() setViewHeight:height];
    _fullscreen = false;
}

void View::setSize(GLint width, GLint height)
{
    [_views.at(_viewNo) setViewWidth:width setViewHeight:height];
    _fullscreen = false;
}

void View::setPosition(GLint x, GLint y)
{
    [_views.at(_viewNo) setViewPositionX:x setViewPositionY:y];
}

void View::setContextCurrent()
{
	[_views.at(_viewNo) setContextCurrent];
}

void View::swapBuffers()
{
    bRenderer::log("Not yet supported on iOS", bRenderer::LM_WARNING);
}

static void windowSizeChanged(View::GLFWwindow* window, int width, int height)
{
    // Not implemented on iOS
}
