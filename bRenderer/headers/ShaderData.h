#ifndef B_SHADER_DATA_H
#define B_SHADER_DATA_H

#include "IShaderData.h"
#include "MaterialData.h"
#include "Texture.h"

/** @brief The underlying shader data is generated 
*	@author Benjamin Bürgisser
*/
class ShaderData : public IShaderData
{
public:

	/* Typedefs */
	typedef std::unordered_map<std::string, std::string>    TextureMap;
	typedef std::unordered_map<std::string, vmml::vec3f>    Vector3Map;
	typedef std::unordered_map<std::string, GLfloat>          ScalarMap;

	/* Functions */

	/**	@brief Constructor
	*/
	ShaderData();

	/**	@brief Constructor
	*	@param[in] maxLights The maximum number of light sources to be used
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] materialData All necessary information for the shader is read from the material data
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderData(GLuint maxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights);

	/**	@brief Constructor
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] diffuseLighting Set true if the shader should support diffuse lighting
	*	@param[in] specularLighting Set true if the shader should support specular lighting
	*	@param[in] ambientColor Set true if the material specifies an ambient color (usually Ka)
	*	@param[in] diffuseColor Set true if the material specifies a diffuse color (usually Kd)
	*	@param[in] specularColor Set true if the material specifies a specular color (usually Ks)
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring
	*	@param[in] normalMap Set true if a texture should be used to define the normals
	*	@param[in] specularMap Set true if a texture should be used to define specularity
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderData(GLuint maxLights, bool ambientLighting, bool diffuseLighting, bool specularLighting, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights);

	/**	@brief Constructor
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] ambientLighting Set true if the shader should support ambient lighting
	*	@param[in] diffuseLighting Set true if the shader should support diffuse lighting
	*	@param[in] specularLighting Set true if the shader should support specular lighting
	*	@param[in] ambientColor Set true if the material specifies an ambient color (usually Ka)
	*	@param[in] diffuseColor Set true if the material specifies a diffuse color (usually Kd)
	*	@param[in] specularColor Set true if the material specifies a specular color (usually Ks)
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring
	*	@param[in] normalMap Set true if a texture should be used to define the normals
	*	@param[in] specularMap Set true if a texture should be used to define specularity
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderData &create(GLuint maxLights, bool ambientLighting, bool diffuseLighting, bool specularLighting, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights);

	/**	@brief Gets the source code of the vertex shader as a string
	*/
	std::string getVertShaderSrc()  const   { return _vertShaderSrc; }

	/**	@brief Gets the source code of the fragment shader as a string
	*/
	std::string getFragShaderSrc()  const   { return _fragShaderSrc; }

	/**	@brief Get the maximum number of lights
	*/
	GLuint getMaxLights() const { return _maxLights; }

	/**	@brief Returns true if the number of lights is variable in the shader
	*/
	bool supportsVariableNumberOfLights() const { return _variableNumberOfLights; }

	/**	@brief Returns true if the shader supports ambient lighting
	*/
	bool supportsAmbientLighting() const	{ return _ambientLighting; }

	/**	@brief Returns true if the shader supports diffuse lighting
	*/
	bool supportsDiffuseLighting() const	{ return _diffuseLighting; }

	/**	@brief Returns true if the shader supports specular lighting
	*/
	bool supportsSpecularLighting() const	{ return _specularLighting; }

	/**	@brief Returns true if the shader is valid
	*/
	bool        isValid()           const	{ return _valid; }

private:

	/* Functions */

	void buildShader();
	void initializeSourceCommonVariables();
	void createVertShader();
	void createFragShader();
	void readMaterialAttributes(GLuint maxLights, bool variableNumberOfLights, bool ambientLighting, const TextureMap &t, const Vector3Map &v, const ScalarMap &s);
	
	/* Variables */

	std::string _vertShaderSrc;
	std::string _fragShaderSrc;
	bool        _valid;

	GLuint		_maxLights;
	bool		_variableNumberOfLights;
	bool		_ambientLighting;
	bool		_diffuseLighting;
	bool		_specularLighting;
	bool		_ambientColor;
	bool		_diffuseColor;
	bool		_specularColor;
	bool		_diffuseMap;
	bool		_normalMap;
	bool		_specularMap;

};

#endif /* defined(B_SHADER_DATA_H) */
