#ifndef B_RENDERER_PROJECT_H
#define B_RENDERER_PROJECT_H

/* bRenderer includes */
#include "../bRenderer.h"

/** @brief This class needs to be inherited if the renderer should be used in an object oriented manner
*	@author Benjamin Bürgisser
*/
class IRenderProject
{
public:
	virtual ~IRenderProject(){}

	/**	@brief Function that gets called when initializing the renderer
	 */
	virtual void initFunction() = 0;

	/**	@brief Function that gets called repeatedly when looping
	*	@param[in] deltaTime The time past since last frame
	*	@param[in] elapsedTime The time past since initializing the renderer
	*/
	virtual void loopFunction(const double &deltaTime, const double &elapsedTime) = 0;

	/**	@brief Function that gets called when terminating the renderer
	 */
	virtual void terminateFunction() = 0;

protected:
    Renderer& bRenderer()
    {
        return Renderer::get();
    }
};

#endif /* defined(B_RENDERER_PROJECT_H) */
