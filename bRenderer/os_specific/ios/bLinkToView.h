#ifndef B_LINK_TO_VIEW_H
#define B_LINK_TO_VIEW_H

#include "../../headers/OSdetect.h"
#ifdef OS_IOS

#include "../../headers/Renderer_GL.h"

/* These functions get called by the view class and serve as a bridge between the Objective-C and the c++ code */

namespace bRenderer
{
	void render();

    void passFullscreen(bool f);
    
    void passRunning(bool r);
    
	void passTime(double t);
    
    GLint getViewWidth();
    
    GLint getViewHeight();
    
    GLint getViewPositionX();
    
    GLint getViewPositionY();
    
    void setRunning(bool r);
    
    void setViewFullScreen();
    
    void setViewSize(GLint width, GLint height);
    
    void setViewPosition(GLint x, GLint y);
    
} // namespace bRenderer

#endif

#endif /* defined(B_LINK_TO_VIEW_H) */