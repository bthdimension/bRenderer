#pragma once
#include "../../headers/OSdetect.h"
#ifdef OS_IOS

#include "../../headers/bRenderer_GL.h"

/* These functions get called by the view class and serve as a bridge between the Objective-C and the c++ code */

namespace bRenderer
{
	void render();

	void setTime(double t);
    
    void setWindowSize(GLint width, GLint height);
    
    // TODO: Here user interactions could be passed as well
    
} // namespace bRenderer

#endif