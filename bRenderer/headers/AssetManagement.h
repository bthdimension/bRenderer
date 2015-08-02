#ifndef B_ASSETMANAGEMENT_H
#define B_ASSETMANAGEMENT_H

/* bRenderer includes */
#include "Renderer_GL.h"
#include "Logger.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Configuration.h"
#include "Properties.h"
#include "Framebuffer.h"
#include "Model.h"
#include "Sprite.h"
#include "Texture.h"
#include "ModelData.h"
#include "OBJLoader.h"
#include "TextureData.h"
#include "IShaderData.h"

/* vmmlib includes */
#include "vmmlib/util.hpp"

/** @brief This class manages all assets in a project and makes sure no asset is created twice
*	@author Benjamin Bürgisser
*/
class AssetManagement
{
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
	typedef std::unordered_map< std::string, FramebufferPtr >	FramebufferMap;

	/* Functions */

	/**	@brief Constructor
	*/
	AssetManagement(){}

	/**	@brief Destructor
	*/
	~AssetManagement(){}

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
	void setAmbientColor(const vmml::Vector3f &ambientColor);

	/**	@brief Load a material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*	@param[in] shaderName Name of the shader (optional)
	*	@param[in] maxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] diffuseLighting Set true if the shader supports diffuse lighting (optional)
	*	@param[in] specularLighting Set true if the shader supports specular lighting (optional)
	*/
	MaterialPtr loadMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName = "", GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, bool diffuseLighting = true, bool specularLighting = true);

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
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*
	*	This function will automatically create one shader for every material of the model
	*
	*/
	ModelPtr loadObjModel(const std::string &fileName, bool flipT = false, bool flipZ = false, bool shaderFromFile = false, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, PropertiesPtr properties = nullptr);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] shader Custom shader for the model
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr loadObjModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Load a 3D model
	*	@param[in] fileName File name including extension
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*	@param[in] material Custom material for the model
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr loadObjModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Load a texture
	*	@param[in] fileName File name including extension
	*/
	TexturePtr loadTexture(const std::string &fileName);

	/**	@brief Load a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] maxLights The maximum number of light sources to be used  (optional)
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] diffuseLighting Set true if the shader supports diffuse lighting (optional)
	*	@param[in] specularLighting Set true if the shader supports specular lighting (optional)
	*
	*	If no shaders with the chosen name exist or no name is passed to the function
	*	the default shader will be used.
	*
	*/
	ShaderPtr loadShaderFile(std::string shaderName, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, bool diffuseLighting = true, bool specularLighting = true);

	/**	@brief Generate a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderMaxLights The maximum number of light sources to be used
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] materialData All necessary information for the shader is read from the material data
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderPtr generateShader(std::string shaderName, GLuint shaderMaxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights);

	/**	@brief Generate a shader
	*	@param[in] shaderName Name of the shader
	*	@param[in] shaderMaxLights The maximum number of light sources to be used
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] diffuseLighting Set true if the shader should support diffuse lighting
	*	@param[in] specularLighting Set true if the shader should support specular lighting
	*	@param[in] ambientColor Set true if the material specifies an ambient color (usually Ka)
	*	@param[in] diffuseColor Set true if the material specifies a diffuse color (usually Kd)
	*	@param[in] specularColor Set true if the material specifies a specular color (usually Ks)
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring
	*	@param[in] normalMap Set true if a texture should be used to define the normals
	*	@param[in] specularMap Set true if a texture should be used to define specularity
	*	@param[in] transparencyValue Set true if a transparency value should be passed
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderPtr generateShader(std::string shaderName, GLuint shaderMaxLights, bool ambientLighting, bool diffuseLighting, bool specularLighting, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool transparencyValue, bool variableNumberOfLights);

	/**	@brief Create empty material
	*	@param[in] name Name of the material
	*	@param[in] shader
	*/
	MaterialPtr createMaterial(const std::string &name, ShaderPtr shader);

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
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*/
	MaterialPtr createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true);

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
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting (optional)
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights = bRenderer::DEFAULT_SHADER_MAX_LIGHTS(), bool variableNumberOfLights = false, bool ambientLighting = true, PropertiesPtr properties = nullptr);

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

	/**	@brief Create a sprite
	*	@param[in] name The raw name of the sprite
	*	@param[in] material
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createSprite(const std::string &name, MaterialPtr material, PropertiesPtr properties = nullptr);

	/**	@brief Create a sprite
	*	@param[in] name The raw name of the sprite
	*	@param[in] textureFileName	The filename of the texture that should be loaded and displayed 
	*	@param[in] shader
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	ModelPtr createSprite(const std::string &name, const std::string &textureFileName, ShaderPtr shader, PropertiesPtr properties = nullptr);

	/**	@brief Create a sprite
	*	@param[in] name The raw name of the sprite
	*	@param[in] textureFileName The filename of the texture that should be loaded and displayed
	*	@param[in] shaderMaxLights The maximum number of light sources to be used (optional)
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights (optional)
	*/
	ModelPtr createSprite(const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights = 0, bool variableNumberOfLights = false);

	/**	@brief Create a texture
	*	@param[in] name The raw name of the texture
	*	@param[in] textureData
	*/
	TexturePtr createTexture(const std::string &name, const TextureData &textureData);

	/**	@brief Create a texture
	*	@param[in] name The raw name of the texture
	*	@param[in] width
	*	@param[in] height
	*	@param[in] format
	*	@param[in] imageData
	*/
	TexturePtr createTexture(const std::string &name, GLsizei width, GLsizei height, GLenum format = GL_RGBA, ImageDataPtr imageData = nullptr);

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
	CameraPtr createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes);

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
	CameraPtr createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

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
	*	@param[in] color Color of the light for both diffuse and specular lighting
	*/
	LightPtr createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light for both diffuse and specular lighting
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*	@param[in] radius Radius of the light (clamps the light at a certain distance)
	*/
	LightPtr createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color, GLfloat intensity, GLfloat attenuation, GLfloat radius);

	/**	@brief Create a light
	*	@param[in] name Name of the light
	*	@param[in] position Position of the light
	*	@param[in] diffuseColor Color of the light for diffuse lighting
	*	@param[in] specularColor Color of the light for specular lighting
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*	@param[in] radius Radius of the light (clamps the light at a certain distance)
	*/
	LightPtr createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &diffuseColor, const vmml::Vector3f &specularColor, GLfloat intensity, GLfloat attenuation, GLfloat radius);

	/**	@brief Create a framebuffer
	*	@param[in] name Name of the framebuffer
	*/
	FramebufferPtr createFramebuffer(const std::string &name);

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

	/**	@brief Get a framebuffer
	*	@param[in] name Name of the framebuffer
	*/
	FramebufferPtr getFramebuffer(const std::string &name);

	/**	@brief Get the shader version used on desktop systems
	*/
	std::string getShaderVersionDesktop();

	/**	@brief Get the shader version used on mobile devices systems
	*/
	std::string getShaderVersionES();

	/**	@brief Get the ambient color of the scene
	*/
	vmml::Vector3f getAmbientColor();

	/**	@brief Remove a shader
	*	@param[in] name Name of the shader
	*/
	void removeShader(const std::string &name);

	/**	@brief Remove a texture
	*	@param[in] name Name of the texture
	*/
	void removeTexture(const std::string &name);

	/**	@brief Remove a material
	*	@param[in] name Name of the material
	*/
	void removeMaterial(const std::string &name);

	/**	@brief Remove properties
	*	@param[in] name Name of the properties
	*/
	void removeProperties(const std::string &name);

	/**	@brief Remove a 3D model
	*	@param[in] name Name of the model
	*/
	void removeModel(const std::string &name);

	/**	@brief Remove a camera
	*	@param[in] name Name of the camera
	*/
	void removeCamera(const std::string &name);

	/**	@brief Remove a matrix stack
	*	@param[in] name Name of the matrix stack
	*/
	void removeMatrixStack(const std::string &name);

	/**	@brief Remove a light
	*	@param[in] name Name of the light
	*/
	void removeLight(const std::string &name);

	/**	@brief Remove a framebuffer
	*	@param[in] name Name of the framebuffer
	*/
	void removeFramebuffer(const std::string &name);

	/**	@brief Clear all assets
	*/
	void clear();

private:
	/* Functions */

	/**	@brief Get the name of a file from the filename
	*	@param[in] fileName The filename
	*	@param[in] ext The extension
	*/
	std::string getRawName(const std::string &fileName, std::string *ext = nullptr);

	/* Variables */

	ShaderMap		_shaders;
	TextureMap		_textures;
	MaterialMap		_materials;
	PropertiesMap	_properties;
	ModelMap	    _models;
	CameraMap		_cameras;
	MatrixStackMap	_matrixStacks;
	LightMap		_lights;
	FramebufferMap	_framebuffers;

	vmml::Vector3f		_ambientColor;

	std::string		_shaderVersionDesktop;
	std::string		_shaderVersionES;

};

typedef std::shared_ptr< AssetManagement >  AssetManagementPtr;

#endif /* defined(B_ASSETMANAGEMENT_H) */
