//
//  IDrawable.h
//  Framework
//
//  Created by David Steiner on 3/19/13.
//

#ifndef B_I_DRAWABLE_H
#define B_I_DRAWABLE_H

#include "Renderer_GL.h"

class IDrawable
{
public:
    virtual void draw(GLenum mode) = 0;
    
};

#endif /* defined(B_I_DRAWABLE_H) */
