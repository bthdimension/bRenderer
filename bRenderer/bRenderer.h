#pragma once
#include "headers/OSdetect.h"
#include "headers/bRenderer_GL.h"
#include "headers/Logger.h"
#include <sstream>  

namespace bRenderer
{
	/**	@brief Returns true if the renderer has already been initialized
	 */
	bool isInitialized();

	/**	@brief Returns true if the renderer is running 
	 */
	bool isRunning();

	/**	@brief Returns the width of the window in pixels
	 */
	GLint getWindowWidth();

	/**	@brief Returns the height of the window in pixels
	 */
	GLint getWindowHeight();

#ifdef OS_DESKTOP
    /* Window settings exclusively for desktop operating systems */

	/**	@brief Returns the GLFW window 
	 *
	 *	This function returns the window created using GLFW. 
	 *	This is useful if GLFW functionality is used within the application.
	 */
	GLFWwindow* getWindow();

	/**	@brief Sets the width of the window in pixels
	 *	@param[in] width The width in pixels
	 */
	void setWindowWidth(GLint width);

	/**	@brief Sets the height of the window in pixels
	 *	@param[in] height The height in pixels
	 */
	void setWindowHeight(GLint height);

	/**	@brief Sets the size of the window in pixels
	 *	@param[in] width The width in pixels
	 *	@param[in] height The height in pixels
	 */
	void setWindowSize(GLint width, GLint height);

#endif

	/**	@brief Defines a function that gets called when initializing the renderer
	 *	
	 *	In an application a function can be defined that gets called immediately after the renderer is initialized.
	 *
	 *	@param[in] f The function to be called
	 */
	void defineInitFunction(void(*f)());

	/**	@brief Defines a function that gets called repeatedly when looping
	*
	*	The defined function gets called repeatedly while the renderer is running. 
	*	Usually the scene gets drawn and setup in the defined function.
	*
	*	@param[in] f The function to be called
	*/
	void defineLoopFunction(void(*f)(const double deltaTime, const double elapsedTime));

	/**	@brief Defines a function that gets called when terminating the renderer 
	*
	*	The defined function gets called when the renderer is terminated. 
	*	Here allocated memory can be freed, instances deleted and statuses changed.
	*
	*	@param[in] f The function to be called
	*/
	void defineTerminateFunction(void(*f)());

#ifdef OS_IOS
    /* Initialization method for iOS without full screen and window size options */

	/**	@brief Do all necessary initializations for the renderer to be ready to run
	 */
	bool initRenderer();
#endif
#ifdef OS_DESKTOP
    /* Initialization methods for desktop with full screen and window size options */

	/**	@brief Do all necessary initializations for the renderer to be ready to run
	 *	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	 */
	bool initRenderer(bool fullscreen = false);

	/**	@brief Do all necessary initializations for the renderer to be ready to run
 	 *	@param[in] width The width of the window in pixels
	 *	@param[in] height The height of the window in pixels
	 *	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	 */
	bool initRenderer(GLint width, GLint height, bool fullscreen = false);
#endif

	/**	@brief Start the render loop
	 */
	void runRenderer();

	/**	@brief Stop the render loop
	 */
	void stopRenderer();

	/**	@brief Terminate the renderer
	 */
	void terminateRenderer();


} // namespace bRenderer
