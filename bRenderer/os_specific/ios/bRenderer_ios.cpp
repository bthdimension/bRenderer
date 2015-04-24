#include "../../headers/OSdetect.h"

#ifdef OS_IOS

#include "../../bRenderer.h"
#include "bLinkToView.h"

namespace bRenderer
{
    /* Internal variables */
    
    static bool _initialized = false;
    static bool _running = false;
    static bool _fullscreen = true;
    
//    static GLint _width = 640;
//    static GLint _height = 480;
    
    static double _currentTime = 0;
    static double _elapsedTime = 0;
    
	RenderProject * _renderProject;

    static void(*_initFunction)();
    static void(*_loopFunction)(const double deltaTime, const double elapsedTime);
    static void(*_terminateFunction)();
    
    /* Internal functions */
    
    void passFullscreen(bool f)
    {
        _fullscreen = f;
    }
    
    void passRunning(bool r)
    {
        _running = r;
    }
    
    void passTime(double t)
    {
        _currentTime = t;
        
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
        return getViewWidth();
    }
    
    GLint getWindowHeight()
    {
        return getViewHeight();
    }
    
    GLint getWindowPositionX()
    {
        return getViewPositionX();
    }
    
    GLint getWindowPositionY()
    {
        return getViewPositionY();
    }
    
    void getWindowPosition(GLint* x, GLint* y)
    {
        *x = getViewPositionX();
        *y = getViewPositionY();
    }
    
    void setWindowFullscreen(bool fullscreen)
    {
        if(fullscreen)
            setViewFullScreen();
        else
            log("Set width and height to set fullscreen to flase", LM_WARNING);
    }
    
    void setWindowWidth(GLint width)
    {
        setViewSize(width, getViewHeight());
    }

    void setWindowHeight(GLint height)
    {
        setViewSize(getViewWidth(), height);
    }
    
    void setWindowSize(GLint width, GLint height)
    {
        setViewSize(width, height);
    }
    
    void setWindowPosition(GLint x, GLint y)
    {
        setViewPosition(x, y);
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
        log("Initializing bRenderer", LM_SYS);
        
        if (_initFunction)
            _initFunction();

		if (_renderProject)
			_renderProject->initFunction();
        
        _initialized = true;
        
        return true;
    }

	bool initRenderer(bool fullscreen)
	{
        if(!fullscreen)
            log("Can't set fullscreen to flase without setting width and height on iOS.", LM_WARNING);
        
        setViewFullScreen();
		if (initRenderer())
			return true;
		else
			return false;
	}

	bool initRenderer(GLint width, GLint height, bool fullscreen)
	{
        if(fullscreen){
            log("Can't set width and height when using fullscreen on iOS.", LM_WARNING);
            setViewFullScreen();
        }
        else{
            setViewSize(width, height);
        }
        
		if (initRenderer())
			return true;
		else
			return false;
	}
    
    void runRenderer()
    {
        _running = true;
        setRunning(_running);
		log("Renderer started", LM_SYS);
    }

	void render()
	{
		// render here
		if (_loopFunction)
			_loopFunction(_currentTime-_elapsedTime, _currentTime);

		if (_renderProject)
			_renderProject->loopFunction(_currentTime-_elapsedTime, _currentTime);

		// adjust time 
		_elapsedTime = _currentTime;
	}
    
    void stopRenderer()
    {
        _running = false;
        setRunning(_running);
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
        
        setRunning(_running);
        
        log("Renderer terminated", LM_SYS);        
        
//        exit(0);
    }
    
    
    
} // namespace bRenderer

#endif