#pragma once

/* bRenderer includes */
#include "headers/OSdetect.h"
#include "headers/bRenderer_GL.h"
#include "headers/Logger.h"
#include "headers/FileHandler.h"
#include "headers/MatrixStack.h"
#include "headers/Camera.h"

/* Flamework includes*/
#include "headers/Model.h"
#include "headers/Texture.h"

/* vmmlib includes */
#include "vmmlib/addendum.hpp"

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

	/**	@brief Returns the aspect ratio of the window
	*/
	GLfloat getAspectRatio();

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

	/**	@brief Load a 3D model
	 *	@param[in] fileName File name including extension
	 *	@param[in] flipT Flip T axis of texture
	 *	@param[in] flipZ Flip Z axis of the geometry
	 */
	  ////////////////////////////////////////////////////////////////////////////////////////
	 // TODO: give the option to load shaders and textures with different names than model //
	////////////////////////////////////////////////////////////////////////////////////////
	ModelPtr loadModel(const std::string &fileName, bool flipT, bool flipZ);

	/**	@brief Load a texture
	 *	@param[in] fileName File name
	 */
	TexturePtr loadTexture(const std::string &fileName);

	/**	@brief Load a shader
	 *	@param[in] shaderName Name of shader
	 */
	ShaderPtr loadShader(const std::string &shaderName);

	/**	@brief Load a shader
	 *	@param[in] name Name of material
	 *	@param[in] materialData
	 */
	MaterialPtr createMaterial(const std::string &name, const MaterialData &materialData);

	/**	@brief Get a 3D model
	 *	@param[in] name Name of the model
	 */
	ModelPtr    getModel(const std::string &name);

	/**	@brief Get a 3D model
	*	@param[in] name Name of the model
	*/
	ModelPtr    getModel(const std::string &name);

	/**	@brief Create a 3D perspective
	 *	@param[in] fov Field of view
	 *	@param[in] aspect Aspect ratio
	 *	@param[in] near Near clipping plane
	 *	@param[in] far Far clipping plane
	 */
	vmml::mat4f createPerspective(float fov, float aspect, float near, float far);

} // namespace bRenderer
