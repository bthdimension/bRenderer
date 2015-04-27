#include "../../headers/OSdetect.h"

#ifdef OS_IOS

#include "../../headers/Renderer.h"
#include "../../headers/RenderProject.h"

/* Public functions */

bool Renderer::isRunning()
{
    return _view.isRunning();
}

void Renderer::runRenderer()
{
    _view.setRunning(true);
    bRenderer::log("Renderer started", bRenderer::LM_SYS);
}

void Renderer::stopRenderer()
{
    _view.setRunning(false);
    bRenderer::log("Renderer stopped", bRenderer::LM_SYS);
}

void Renderer::terminateRenderer()
{
    _initialized = false;
    
    if (_terminateFunction)
        _terminateFunction();
    
    if (_renderProject)
        _renderProject->terminateFunction();
    
    _view.setRunning(false);
    
    bRenderer::log("Renderer terminated", bRenderer::LM_SYS);
    
    //        exit(0);
}

void Renderer::draw()
{
    // render here
    if (_loopFunction)
        _loopFunction(_currentTime - _elapsedTime, _currentTime);
    
    if (_renderProject)
        _renderProject->loopFunction(_currentTime - _elapsedTime, _currentTime);
    
    // adjust time
    _elapsedTime = _currentTime;
}

#endif