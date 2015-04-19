//
//  IDrawable.h
//  Framework
//
//  Created by David Steiner on 3/19/13.
//

#ifndef FRAMEWORK_I_DRAWABLE_H
#define FRAMEWORK_I_DRAWABLE_H

#include "bRenderer_GL.h"

class IDrawable
{
public:
    virtual void draw(GLenum mode) = 0;
    
};

#endif
