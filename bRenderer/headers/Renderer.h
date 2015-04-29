#ifndef B_RENDERER_H
#define B_RENDERER_H

///* bRenderer includes */
#include "OSdetect.h"
#include "Renderer_GL.h"
#include "Logger.h"
#include "View.h"
#include "Camera.h"
#include "MatrixStack.h"

/* Flamework includes*/
#include "headers/Model.h"
#include "headers/Texture.h"
#include "headers/ModelData.h"
#include "headers/TextureData.h"
#include "headers/ShaderData.h"

/* vmmlib includes */
#include "vmmlib/addendum.hpp"

class RenderProject;

class Renderer
{
public:
	/* Typedefs */
	typedef std::unordered_map< std::string, ShaderPtr >		Shaders;
	typedef std::unordered_map< std::string, TexturePtr >		Textures;
	typedef std::unordered_map< std::string, MaterialPtr >		Materials;
	typedef std::unordered_map< std::string, ModelPtr >			Models;
	typedef std::unordered_map< std::string, CameraPtr >		Cameras;
	typedef std::unordered_map< std::string, MatrixStackPtr >	MatrixStacks;
	
	/* Functions */

	/**	@brief Returns the single instance of the Renderer
	 */
	static Renderer& get()
	{
		static Renderer renderer;
		return renderer;
	}

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

	/**	@brief Set the shader version used on desktop systems
	*	@param[in] shaderVersionDesktop The shader version used on desktop systems, e.g. "#version 120"
	*/
	void setShaderVersionDesktop(std::string shaderVersionDesktop);

	/**	@brief Set the shader version used on mobile devices systems
	*	@param[in] shaderVersionES The shader version used on mobile devices systems, e.g. "#version 100"
	*/
	void setShaderVersionES(std::string shaderVersionES);

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
    
    /**	@brief Draw the scene
     */
    void draw();

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
	*	@param[in] shaderName Name of the shader
	*/
	ShaderPtr loadShader(const std::string &shaderName);

	/**	@brief Create a material
	*	@param[in] name Name of the material
	*	@param[in] materialData
	*/
	MaterialPtr createMaterial(const std::string &name, const MaterialData &materialData);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData);

	/**	@brief Create a texture
	*	@param[in] name The raw name of the texture
	*	@param[in] textureData
	*/
	TexturePtr createTexture(const std::string &name, const TextureData &textureData);

	/**	@brief Create a shader
	*	@param[in] name The raw name of the shader
	*	@param[in] shaderData
	*/
	ShaderPtr createShader(const std::string &name, const ShaderData &shaderData);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*/
	CameraPtr createCamera(const std::string &name);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] position Position of the camera
	*	@param[in] orientation Orientation of the camera in radians
	*/
	CameraPtr createCamera(const std::string &name, vmml::vec3f position, vmml::vec3f orientation);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] position Position of the camera
	*	@param[in] orientation Orientation of the camera in radians
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	CameraPtr createCamera(const std::string &name, vmml::vec3f position, vmml::vec3f orientation, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Create a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	MatrixStackPtr createMatrixStack(const std::string &name);

	/**	@brief Get a 3D model
	*	@param[in] name Name of the model
	*/
	ModelPtr    getModel(const std::string &name);

	/**	@brief Get a camera
	*	@param[in] name Name of the camera
	*/
	CameraPtr    getCamera(const std::string &name);

	/**	@brief Get a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	MatrixStackPtr getMatrixStack(const std::string &name);

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

private:
	/* Functions */

	/**	@brief Private constructor
	 */
	Renderer(){}

	/**	@brief Private constructor to prevent instantiation via copy constructor
	 */
	Renderer(const Renderer&){}

	/**	@brief Private operator overloading to prevent instantiation of the renderer
	 */
	Renderer & operator = (const Renderer &){}

	/**	@brief Private destructor
	 */
	~Renderer(){}

	/**	@brief Get the name of a file from the filename
	*	@param[in] fileName The filename
	*	@param[in] ext The extension
	*/
	std::string getRawName(const std::string &fileName, std::string *ext = nullptr);

	/* Variables */

	bool _initialized;
	bool _running;

	double _currentTime = 0;
	double _elapsedTime = 0;

	View _view;

	RenderProject *_renderProject;
	
	void(*_initFunction)();
	void(*_loopFunction)(const double deltaTime, const double elapsedTime);
	void(*_terminateFunction)();

	Shaders			_shaders;
	Textures		_textures;
	Materials		_materials;
	Models		    _models;
	Cameras			_cameras;
	MatrixStacks	_matrixStacks;

	std::string _shaderVersionDesktop = "#version 120";
	std::string _shaderVersionES = "#version 100";

	std::string _defaultMaterialName = "default";
	

};



#endif /* defined(B_RENDERER_H) */
