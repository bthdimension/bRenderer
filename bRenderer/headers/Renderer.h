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
#include "Properties.h"

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
	typedef std::unordered_map< std::string, ShaderPtr >		ShaderMap;
	typedef std::unordered_map< std::string, TexturePtr >		TextureMap;
	typedef std::unordered_map< std::string, MaterialPtr >		MaterialMap;
	typedef std::unordered_map< std::string, PropertiesPtr >	PropertiesMap;
	typedef std::unordered_map< std::string, ModelPtr >			ModelMap;
	typedef std::unordered_map< std::string, CameraPtr >		CameraMap;
	typedef std::unordered_map< std::string, MatrixStackPtr >	MatrixStackMap;
	typedef std::unordered_map< std::string, LightPtr >			LightMap;

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
	*	@param[in] shaderName Name of the shader (optional)
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*/
	MaterialPtr loadMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName = "", GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS, bool variableNumberOfLights = true);

	/**	@brief Load a material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*	@param[in] shader Custom shader for the model
	*/
	MaterialPtr loadMaterial(const std::string &fileName, const std::string &materialName, ShaderPtr shader);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture (optional)
	*	@param[in] flipZ Flip Z axis of the geometry (optional)
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded (optional)
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*
	*	This function will automatically create one shader for every material of the model
	*
	*/
	ModelPtr loadModel(const std::string &fileName, bool flipT = false, bool flipZ = false, bool shaderFromFile = false, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS, bool variableNumberOfLights = true, PropertiesPtr properties = nullptr);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] shader Custom shader for the model
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr loadModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] material Custom material for the model
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr loadModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Load a texture
	*	@param[in] fileName File name including extension
	*/
	TexturePtr loadTexture(const std::string &fileName);

	/**	@brief Load a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] maxLights The maximum number of light sources to be used  (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*
	*	If no shaders with the chosen name exist or no name is passed to the function 
	*	the default shader will be used.
	*
	*/
	ShaderPtr loadShaderFile(std::string shaderName, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS, bool variableNumberOfLights = true);

	/**	@brief Load a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] ambientColor
	*	@param[in] diffuseColor
	*	@param[in] specularColor
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring (optional)
	*	@param[in] normalMap Set true if a texture should be used to define the normals (optional)
	*	@param[in] specularMap Set true if a texture should be used to define specularity (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*/
	ShaderPtr loadShader(std::string shaderName, GLuint shaderMaxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap = false, bool normalMap = false, bool specularMap = false, bool variableNumberOfLights = true);

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
	*	@param[in] shaderMaxLights (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*/
	MaterialPtr createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS, bool variableNumberOfLights = true);

	/**	@brief Create properties
	*	@param[in] name Name of the properties
	*/
	PropertiesPtr createProperties(const std::string &name);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS, bool variableNumberOfLights = true, PropertiesPtr properties = nullptr);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] shader
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Create a model
	*	@param[in] name The raw name of the model
	*	@param[in] modelData
	*	@param[in] material
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, MaterialPtr material, PropertiesPtr	properties = nullptr);

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
	*	@param[in] rotationAxes Rotation axes of the camera
	*/
	CameraPtr createCamera(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &rotationAxes);

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
	*	@param[in] rotationAxes Rotation axes of the camera
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	CameraPtr createCamera(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

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
	*	@param[in] modelName Name of the model
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix 
	*/
	void drawModel(const std::string &modelName, const std::string &cameraName, const vmml::mat4f &modelMatrix);

	/**	@brief Draw specified model into the current framebuffer
	*	@param[in] modelName Name of the model
	*	@param[in] cameraName Name of the camera
	*	@param[in] modelMatrix 
	*	@param[in] lightNames Names of the light in a vector
	*/
	void drawModel(const std::string &modelName, const std::string &cameraName, const vmml::mat4f &modelMatrix, const std::vector<std::string> &lightNames);

	/**	@brief Draw specified model into the current framebuffer
	*	@param[in] modelName Name of the model
	*	@param[in] modelMatrix
	*	@param[in] viewMatrix
	*	@param[in] projectionMatrix
	*	@param[in] lightNames Names of the light in a vector
	*/
	void drawModel(const std::string &modelName, const vmml::mat4f &modelMatrix, const vmml::mat4f &viewMatrix, const vmml::mat4f &projectionMatrix, const std::vector<std::string> &lightNames);

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

	/**	@brief Get properties
	*	@param[in] name Name of the properties
	*/
	PropertiesPtr getProperties(const std::string &name);

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

	ShaderMap		_shaders;
	TextureMap		_textures;
	MaterialMap		_materials;
	PropertiesMap	_properties;
	ModelMap	    _models;
	CameraMap		_cameras;
	MatrixStackMap	_matrixStacks;
	LightMap		_lights;

	vmml::vec3f		_ambientColor;

	std::string		_shaderVersionDesktop;
	std::string		_shaderVersionES;

};



#endif /* defined(B_RENDERER_H) */
