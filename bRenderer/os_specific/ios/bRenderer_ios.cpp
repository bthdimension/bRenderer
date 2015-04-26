#include "../../headers/OSdetect.h"

#ifdef OS_IOS

#include "../../bRenderer.h"
#include "bLinkToView.h"

namespace bRenderer
{
    /* Internal variables */
    
    static View _view;
    
    static bool _initialized = false;
    
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
        return _view.isRunning();
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
		// render here
		if (_loopFunction)
			_loopFunction(_currentTime - _elapsedTime, _currentTime);

		if (_renderProject)
			_renderProject->loopFunction(_currentTime - _elapsedTime, _currentTime);

		// adjust time 
		_elapsedTime = _currentTime;
	}

	void runRenderer()
	{
		_view.setRunning(true);
		log("Renderer started", LM_SYS);
	}

	void stopRenderer()
	{
		_view.setRunning(false);
		log("Renderer stopped", LM_SYS);
	}

	void terminateRenderer()
	{
		_initialized = false;

		if (_terminateFunction)
			_terminateFunction();

		if (_renderProject)
			_renderProject->terminateFunction();

		_view.setRunning(false);

		log("Renderer terminated", LM_SYS);

		//        exit(0);
	}


} // namespace bRenderer

#endif