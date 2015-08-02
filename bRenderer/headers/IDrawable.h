#ifndef B_I_DRAWABLE_H
#define B_I_DRAWABLE_H

#include "Renderer_GL.h"

/** @brief Interface for drawable objects
*	@author David Steiner
*/
class IDrawable
{
public:

	/**	@brief Virtual destructor
	*/
	virtual ~IDrawable(){}

	/**	@brief Draws an object to the screen
	*	@param[in] mode
	*/
    virtual void draw(GLenum mode) = 0;
    
};

#endif /* defined(B_I_DRAWABLE_H) */
