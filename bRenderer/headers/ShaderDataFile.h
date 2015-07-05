#ifndef B_SHADER_DATA_FILE_H
#define B_SHADER_DATA_FILE_H

#include "IShaderData.h"

/** @brief The underlying shader data is loaded from shader files
*	@author David Steiner
*/
class ShaderDataFile : public IShaderData
{
public:

	/* Functions */

	/**	@brief Constructor
	*	@param[in] shaderFileName The filename of both the vertex and fragment shader (without filename extension!)
	*	@param[in] shaderVersionDesktop The shader version to be used for desktop systems
	*	@param[in] shaderVersionES The shader version to be used for embedded systems (iOS)
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	explicit ShaderDataFile(const std::string &shaderFileName, const std::string &shaderVersionDesktop, const std::string &shaderVersionES, GLuint maxLights, bool variableNumberOfLights);

	/**	@brief Constructor
	*	@param[in] vertShaderFileName The filename of the vertex shader (including filename extension!)
	*	@param[in] fragShaderFileName The filename of the fragment shader (including filename extension!)
	*	@param[in] shaderVersionDesktop The shader version to be used for desktop systems
	*	@param[in] shaderVersionES The shader version to be used for embedded systems (iOS)
	*	@param[in] maxLights The maximum number of light sources to be used 
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	ShaderDataFile(const std::string &vertShaderFileName, const std::string &fragShaderFileName, const std::string &shaderVersionDesktop, const std::string &shaderVersionES, GLuint maxLights, bool variableNumberOfLights);
    
	/**	@brief Loads shader from file
	*	@param[in] shaderFileName The filename of both the vertex and fragment shader (without filename extension!)
	*/
	ShaderDataFile &load(const std::string &shaderFileName);

	/**	@brief Loads shader from file
	*	@param[in] vertShaderFileName The filename of the vertex shader (including filename extension!)
	*	@param[in] fragShaderFileName The filename of the fragment shader (including filename extension!)
	*/
	ShaderDataFile &load(const std::string &vertShaderFileName, const std::string &fragShaderFileName);
    
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
    bool        isValid()           const   { return _valid;         }

private:

	/* Functions */

	std::string loadSrc(const std::string &fileName);

	void replaceMacro(const std::string &macro, const std::string &value);

	/* Variables */

    std::string _vertShaderSrc;
    std::string _fragShaderSrc;
	std::string _shaderVersionDesktop;
	std::string _shaderVersionES;
	GLuint		_maxLights;
	bool		_variableNumberOfLights;
    bool        _valid;

};

#endif /* defined(B_SHADER_DATA_FILE_H) */
