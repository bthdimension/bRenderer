#ifndef B_VIEW_H
#define B_VIEW_H

#include "OSdetect.h"
//#ifdef OS_IOS
//#include "../os_specific/ios/BView.h"
//#endif

#include "headers/Renderer_GL.h"
#include "headers/Logger.h"

class View
{
public:
	/* Functions */

	View();
	~View();

	/**	@brief Do all necessary initializations for the view to be ready to run
	*/
	bool initView();

	/**	@brief Do all necessary initializations for the view to be ready to run
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	*/
	bool initView(bool fullscreen);

	/**	@brief Do all necessary initializations for the view to be ready to run
	*	@param[in] width The width of the view in pixels
	*	@param[in] height The height of the view in pixels
	*	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	*/
	bool initView(GLint width, GLint height, bool fullscreen = false);

	/**	@brief Returns true if the view has already been initialized
	*/
	bool isInitialized();

	/**	@brief Returns true if the view is running
	*/
	bool isRunning();

	/**	@brief Returns true if the view runs in fullscreen mode
	*/
	bool isFullscreen();

	/**	@brief Returns the width of the view in pixels
	*/
	GLint getWidth();

	/**	@brief Returns the height of the view in pixels
	*/
	GLint getHeight();

	/**	@brief Gets the size of the view in pixels
	*	@param[in] width The width of the view
	*	@param[in] height The height of the view
	*/
	void getSize(GLint* width, GLint* height);

	/**	@brief Returns the aspect ratio of the view
	*/
	GLfloat getAspectRatio();

	/**	@brief Returns the x position of the view in pixels
	*/
	GLint getPositionX();

	/**	@brief Returns the y position of the view in pixels
	*/
	GLint getPositionY();

	/**	@brief Gets the position of the upper left corner of the view
	*	@param[in] x The x position of the upper left corner of the view
	*	@param[in] y The y position of the upper left corner of the view
	*/
	void getPosition(GLint* x, GLint* y);

#ifdef OS_DESKTOP
	/* Window settings exclusively for desktop operating systems */

	/**	@brief Returns the GLFW window
	*
	*	This function returns the window created using GLFW.
	*	This is useful if GLFW functionality is used within the application.
	*/
	GLFWwindow* getWindow();

#endif

	/**	@brief Returns the elapsed Time
	*/
	double getTime();
    
    /**	@brief Set whether view is running or not
     *	@param[in] running
     */
    void setRunning(bool running);

	/**	@brief Set fullscreen mode
	*	@param[in] fullscreen Set fullscreen to be true if fullscreen is desired
	*/
	void setFullscreen(bool fullscreen);

	/**	@brief Sets the width of the view in pixels
	*	@param[in] width The width in pixels
	*/
	void setWidth(GLint width);

	/**	@brief Sets the height of the view in pixels
	*	@param[in] height The height in pixels
	*/
	void setHeight(GLint height);

	/**	@brief Sets the size of the view in pixels
	*	@param[in] width The width in pixels
	*	@param[in] height The height in pixels
	*/
	void setSize(GLint width, GLint height);

	/**	@brief Sets the position of the upper left corner of the view
	*	@param[in] x The x position of the upper left corner of the view
	*	@param[in] y The y position of the upper left corner of the view
	*/
	void setPosition(GLint x, GLint y);

	/**	@brief Sets the context of this view to be current
	*/
	void setContextCurrent();

	/**	@brief Swaps the front and back buffers
	*/
	void swapBuffers();

private:
	/* Functions */
    
#ifdef OS_DESKTOP
	/**	@brief Gets called whenever the window size is changed
	*	@param[in] window The window that got changed
	*	@param[in] width The width in pixels
	*	@param[in] height The height in pixels
	*/
	static void windowSizeChanged(GLFWwindow* window, int width, int height);
#endif
    
	/* Variables */

    
//#ifdef OS_IOS
//	BView *view;
#ifdef OS_DESKTOP
	GLFWwindow *_window;
	char *_windowTitle;
#endif

	bool _initialized;
	bool _fullscreen;
    int _viewNo;

};


#endif /* defined(B_VIEW_H) */
