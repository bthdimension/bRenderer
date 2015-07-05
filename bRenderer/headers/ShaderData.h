#ifndef B_SHADER_DATA_H
#define B_SHADER_DATA_H

#include "IShaderData.h"

/** @brief The underlying shader data is generated 
*	@author Benjamin Bürgisser
*/
class ShaderData : public IShaderData
{
public:

	/* Functions */

	/**	@brief Constructor
	*/
	ShaderData();

	/**	@brief Constructor
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] ambientColor 
	*	@param[in] diffuseColor
	*	@param[in] specularColor 
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring
	*	@param[in] normalMap Set true if a texture should be used to define the normals
	*	@param[in] specularMap Set true if a texture should be used to define specularity
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderData(GLuint maxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights);

	/**	@brief Constructor
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] ambientColor
	*	@param[in] diffuseColor
	*	@param[in] specularColor
	*	@param[in] diffuseMap Set true if a texture should be used for diffuse coloring
	*	@param[in] normalMap Set true if a texture should be used to define the normals
	*	@param[in] specularMap Set true if a texture should be used to define specularity
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderData &create(GLuint maxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights);

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
	bool hasVariableNumberOfLights() const { return _variableNumberOfLights; }

	/**	@brief Returns true if the shader is valid
	*/
	bool        isValid()           const   { return _valid; }

private:

	/* Functions */

	void initializeSourceCommonVariables();
	void createVertShader();
	void createFragShader();
	
	/* Variables */

	std::string _vertShaderSrc;
	std::string _fragShaderSrc;
	bool        _valid;

	GLuint		_maxLights;
	bool		_variableNumberOfLights;
	bool		_ambientColor;
	bool		_diffuseColor;
	bool		_specularColor;
	bool		_diffuseMap;
	bool		_normalMap;
	bool		_specularMap;

};

#endif /* defined(B_SHADER_DATA_H) */
