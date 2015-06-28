#ifndef B_RENDERER_H
#define B_RENDERER_H

/* bRenderer includes */
#include "OSdetect.h"
#include "Renderer_GL.h"
#include "Logger.h"
#include "View.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Configuration.h"
//#include "../os_specific/ios/BViewLink.h"

/* Flamework includes*/
#include "headers/Model.h"
#include "headers/Texture.h"
#include "headers/ModelData.h"
#include "headers/OBJLoader.h"
#include "headers/TextureData.h"
#include "headers/IShaderData.h"

/* vmmlib includes */
#include "vmmlib/addendum.hpp"

class IRenderProject;

/** @brief The main class that is able to initialize and maintain everything that is necessary to render an image to the screen
*	@author Benjamin Bürgisser
*/
class Renderer
{
    friend class BViewLink;
public:
	/* Typedefs */
	typedef std::unordered_map< std::string, ShaderPtr >		Shaders;
	typedef std::unordered_map< std::string, TexturePtr >		Textures;
	typedef std::unordered_map< std::string, MaterialPtr >		Materials;
	typedef std::unordered_map< std::string, ModelPtr >			Models;
	typedef std::unordered_map< std::string, CameraPtr >		Cameras;
	typedef std::unordered_map< std::string, MatrixStackPtr >	MatrixStacks;
	typedef std::unordered_map< std::string, LightPtr >			Lights;

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
	void setRenderProject(IRenderProject *p);

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
	void setShaderVersionDesktop(const std::string &shaderVersionDesktop);

	/**	@brief Set the shader version used on mobile devices systems
	*	@param[in] shaderVersionES The shader version used on mobile devices systems, e.g. "#version 100"
	*/
	void setShaderVersionES(const std::string &shaderVersionES);

	/**	@brief Set an ambient color for the scene
	*	@param[in] ambientColor Ambient color for the scene
	*/
	void setAmbientColor(const vmml::vec3f &ambientColor);

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

	/**	@brief Load a material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderMaxLights The maximum light sources to be used (optional)
	*/
	MaterialPtr loadMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName = "", GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS);

	/**	@brief Load a material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*	@param[in] shader Custom shader for the model
	*/
	MaterialPtr loadMaterial(const std::string &fileName, const std::string &materialName, ShaderPtr shader);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] shaderMaxLights The maximum light sources to be used (optional)
	*
	*	This function will automatically create one shader for every material of the model
	*
	*/
	ModelPtr loadModel(const std::string &fileName, bool flipT = false, bool flipZ = false, bool shaderFromFile = false, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] shader Custom shader for the model
	*/
	ModelPtr loadModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] material Custom material for the model
	*/
	ModelPtr loadModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material);

	/**	@brief Load a texture
	*	@param[in] fileName File name including extension
	*/
	TexturePtr loadTexture(const std::string &fileName);

	/**	@brief Load a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderMaxLights The maximum light sources to be used (optional)
	*
	*	If no shaders with the chosen name exist or no name is passed to the function 
	*	the default shader will be used.
	*
	*/
	ShaderPtr loadShaderFile(std::string shaderName, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS);

	/**	@brief Load a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderMaxLights The maximum light sources to be used
	*	@param[in] ambientColor
	*	@param[in] diffuseColor
	*	@param[in] specularColor
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring (optional)
	*	@param[in] normalMap Set true if a texture should be used to define the normals (optional)
	*	@param[in] specularMap Set true if a texture should be used to define specularity (optional)
	*/
	ShaderPtr loadShader(std::string shaderName, GLuint shaderMaxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap = false, bool normalMap = false, bool specularMap = false);

	/**	@brief Create a material
	*	@param[in] name Name of the material
	*	@param[in] materialData
	*	@param[in] shader
	*/
	MaterialPtr createMaterial(const std::string &name, const MaterialData &materialData, ShaderPtr shader);

	/**	@brief Create a material and a shader fitting its needs
	*	@param[in] name Name of the material and the shader
	*	@param[in] materialData
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] shaderMaxLights
	*/
	MaterialPtr createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] shaderMaxLights The maximum light sources to be used (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] shader
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, ShaderPtr shader);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] material
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, MaterialPtr material);

	/**	@brief Create a texture
	*	@param[in] name The raw name of the texture
	*	@param[in] textureData
	*/
	TexturePtr createTexture(const std::string &name, const TextureData &textureData);

	/**	@brief Create a shader
	*	@param[in] name The raw name of the shader
	*	@param[in] shaderData
	*/
	ShaderPtr createShader(const std::string &name, const IShaderData &shaderData);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*/
	CameraPtr createCamera(const std::string &name);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] position Position of the camera
	*	@param[in] rotation Rotation matrix of the camera
	*/
	CameraPtr createCamera(const std::string &name, const vmml::vec3f &position, const vmml::mat4f &rotation);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	CameraPtr createCamera(const std::string &name, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Create a camera
	*	@param[in] name Name of the camera
	*	@param[in] position Position of the camera
	*	@param[in] rotation Rotation matrix of the camera
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	CameraPtr createCamera(const std::string &name, const vmml::vec3f &position, const vmml::mat4f &rotation, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Create a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	MatrixStackPtr createMatrixStack(const std::string &name);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*/
	LightPtr createLight(const std::string &name);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light
	*/
	LightPtr createLight(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &color);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*/
	LightPtr createLight(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &color, GLfloat intensity, GLfloat attenuation);

	/**	@brief Draw specified model into the buffer
	*
	*	This function is used for the default shader. 
	*	The projection, view and model matrices are read from the specified camera and matrix stack.
	*	In this function all lights created within this renderer instance are used to illuminate the model.
	*
	*	@param[in] modelName Name of the model
	*	@param[in] cameraName Name of the camera
	*	@param[in] matrixStackName Name of the matrix stack
	*/
	void drawModel(const std::string &modelName, const std::string &cameraName, const std::string &matrixStackName);

	/**	@brief Draw specified model into the buffer
	*
	*	This function is used for the default shader.
	*	The projection, view and model matrices are read from the specified camera and matrix stack. 
	*	In this function one specific light can be chosen to illuminate the model.
	*
	*	@param[in] modelName Name of the model
	*	@param[in] cameraName Name of the camera
	*	@param[in] matrixStackName Name of the matrix stack
	*	@param[in] lightName Name of the light
	*/
	void drawModel(const std::string &modelName, const std::string &cameraName, const std::string &matrixStackName, const std::vector<std::string> &lightNames);

	/**	@brief Get a shader
	*	@param[in] name Name of the shader
	*/
	ShaderPtr getShader(const std::string &name);

	/**	@brief Get a texture
	*	@param[in] name Name of the texture
	*/
	TexturePtr getTexture(const std::string &name);

	/**	@brief Get a material
	*	@param[in] name Name of the material
	*/
	MaterialPtr getMaterial(const std::string &name);

	/**	@brief Get a 3D model
	*	@param[in] name Name of the model
	*/
	ModelPtr getModel(const std::string &name);

	/**	@brief Get a camera
	*	@param[in] name Name of the camera
	*/
	CameraPtr getCamera(const std::string &name);

	/**	@brief Get a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	MatrixStackPtr getMatrixStack(const std::string &name);

	/**	@brief Get a light
	*	@param[in] name Name of the light
	*/
	LightPtr getLight(const std::string &name);

	/**	@brief Get the shader version used on desktop systems
	*/
	std::string getShaderVersionDesktop();

	/**	@brief Get the shader version used on mobile devices systems
	*/
	std::string getShaderVersionES();

	/**	@brief Get the ambient color of the scene
	*/
	vmml::vec3f getAmbientColor();

	/**	@brief Create a 3D perspective
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	vmml::mat4f createPerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Create a simple look at matrix
	*	@param[in] eye Specifies the position of the eye point
	*	@param[in] target Specifies the position of the reference point
	*	@param[in] up Specifies the direction of the up vector
	*/
	vmml::mat4f lookAt(const vmml::vec3f &eye, const vmml::vec3f &target, const vmml::vec3f &up);

private:
	/* Functions */

	/**	@brief Private constructor
	 */
	Renderer();

	/**	@brief Private constructor to prevent instantiation via copy constructor
	 */
	Renderer(const Renderer&){}

	/**	@brief Private operator overloading to prevent instantiation of the renderer
	 */
	Renderer & operator = (const Renderer &){}

	/**	@brief Private destructor
	 */
	~Renderer(){}

    /**	@brief Draw the scene
     */
    void draw(double currentTime);
    
	/**	@brief Get the name of a file from the filename
	*	@param[in] fileName The filename
	*	@param[in] ext The extension
	*/
	std::string getRawName(const std::string &fileName, std::string *ext = nullptr);

	/**	@brief Reset all variables
	*/
	void reset();

	/* Variables */

	bool _initialized;
	bool _running;

	double _elapsedTime, _stopTime, _initialTime;

	View _view;

	IRenderProject *_renderProject;
	
	void(*_initFunction)();
	void(*_loopFunction)(const double deltaTime, const double elapsedTime);
	void(*_terminateFunction)();

	Shaders			_shaders;
	Textures		_textures;
	Materials		_materials;
	Models		    _models;
	Cameras			_cameras;
	MatrixStacks	_matrixStacks;
	Lights			_lights;

	vmml::vec3f		_ambientColor;

	std::string		_shaderVersionDesktop;
	std::string		_shaderVersionES;
	std::string		_defaultShaderName;
	
};



#endif /* defined(B_RENDERER_H) */
