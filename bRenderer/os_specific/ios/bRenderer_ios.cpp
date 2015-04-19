#include "../../headers/OSdetect.h"

#ifdef OS_IOS

#include "../../bRenderer.h"
#include "bLinkToView.h"

namespace bRenderer
{
    /* Internal variables */
    
    static bool initialized = false;
    static bool running = false;
    
    static GLint windowWidth = 640;
    static GLint windowHeight = 480;
    
    static double currentTime = 0;
    static double elapsedTime = 0;
    
    static void(*initFunction)();
    static void(*loopFunction)(const double deltaTime, const double elapsedTime);
    static void(*terminateFunction)();
    
    
    /* Internal functions */
    
    void setTime(double t)
    {
        currentTime = t;
    }
    
    void setWindowSize(GLint width, GLint height)
    {
        windowWidth = width;
        windowHeight = height;
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
    
    bool initRenderer()
    {
        log("Initializing bRenderer", LM_SYS);
        
        if (initFunction)
            initFunction();
        
        initialized = true;
        
        return true;
    }
    
    void runRenderer()
    {
        running = true;
		log("Renderer started", LM_SYS);
    }

	void render()
	{
		// render here
		if (loopFunction)
			loopFunction(currentTime-elapsedTime, currentTime);

		// adjust time 
		elapsedTime = currentTime;
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
        
        log("Renderer terminated", LM_SYS);
    }
    
    
    
} // namespace bRenderer

#endif