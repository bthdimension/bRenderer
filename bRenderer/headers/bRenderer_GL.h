#pragma once

#include "OSdetect.h"



#ifdef OS_DESKTOP
	/* OpenGL imports handled by GLEW */
    #include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif
#ifdef OS_IOS
	/* Import OpenGL ES 2.0 */
	#include <OpenGLES/ES2/gl.h>
	#include <OpenGLES/ES2/glext.h>
#endif