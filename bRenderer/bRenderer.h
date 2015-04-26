#ifndef B_BRENDERER_H
#define B_BRENDERER_H

/* bRenderer includes */
#include "headers/OSdetect.h"
#include "headers/Renderer_GL.h"
#include "headers/Logger.h"
#include "headers/FileHandler.h"
#include "headers/Renderer.h"
#include "headers/View.h"
#include "headers/RenderProject.h"
#include "headers/MatrixStack.h"
#include "headers/Camera.h"

/* Flamework includes*/
#include "headers/Model.h"
#include "headers/Texture.h"
#include "headers/ModelData.h"
#include "headers/TextureData.h"
#include "headers/ShaderData.h"

/* vmmlib includes */
#include "vmmlib/addendum.hpp"

namespace bRenderer
{
	/**	@brief Returns a pointer to the view of the renderer
	 *
	 *	This function returns a pointer to the view of the renderer. The view is where the OpenGL context and
	 *	on desktop systems a window is created and managed. On iOS a UIView is created instead.
	 *
	 */
	View *getView();

	/**	@brief Returns true if the renderer has already been initialized
	 */
	bool isInitialized();

	/**	@brief Returns true if the renderer is running 
	 */
	bool isRunning();
    
	/**	@brief Sets the project instance to be used for function calls 
	 *
	 *	The init, loop and terminate function of the set instance get called automatically.
	 *
	 *	@param[in] p The project instance
	 */
	void setRenderProject(RenderProject *p);

	/**	@brief Sets a static function that gets called when initializing the renderer
	 *	
	 *	In an application a function can be defined that gets called immediately after the renderer is initialized.
	 *
	 *	@param[in] f The function to be called
	 */
	void setInitFunction(void(*f)());

	/**	@brief Sets a static function that gets called repeatedly when looping
	 *
	 *	The set function gets called repeatedly while the renderer is running.
	 *	Usually the scene gets drawn and setup in the defined function.
	 *
	 *	@param[in] f The function to be called
	 */
	void setLoopFunction(void(*f)(const double deltaTime, const double elapsedTime));

	/**	@brief Sets a static function that gets called when terminating the renderer 
	 *
	 *	The set function gets called when the renderer is terminated.
	 *	Here allocated memory can be freed, instances deleted and statuses changed.
	 *
	 *	@param[in] f The function to be called
	 */
	void setTerminateFunction(void(*f)());

    /* Initialization method for iOS without full screen and window size options */

	/**	@brief Do all necessary initializations for the renderer to be ready to run
	 */
	bool initRenderer();

    /* Initialization methods for desktop with full screen and window size options */

	/**	@brief Do all necessary initializations for the renderer to be ready to run
	 *	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	 */
	bool initRenderer(bool fullscreen);

	/**	@brief Do all necessary initializations for the renderer to be ready to run
 	 *	@param[in] width The width of the window in pixels
	 *	@param[in] height The height of the window in pixels
	 *	@param[in] fullscreen Decides whether or not the application runs in full screen mode
	 */
	bool initRenderer(GLint width, GLint height, bool fullscreen = false);

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

	/**	@brief Create a simple look at matrix
	 *	@param[in] eye Specifies the position of the eye point
	 *	@param[in] target Specifies the position of the reference point
	 *	@param[in] up Specifies the direction of the up vector
	 */
	vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up);

} // namespace bRenderer

#endif /* defined(B_BRENDERER_H) */