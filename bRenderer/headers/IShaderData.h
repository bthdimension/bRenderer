#ifndef B_I_SHADER_DATA_H
#define B_I_SHADER_DATA_H

#include <string>
#include "Renderer_GL.h"

/** @brief Interface for the underlying data of a shader
*	@author Benjamin Bürgisser
*/
class IShaderData
{
public:

	/* Functions */

	/**	@brief Gets the source code of the vertex shader as a string
	*/
	virtual std::string getVertShaderSrc() const  = 0;

	/**	@brief Gets the source code of the fragment shader as a string
	*/
	virtual std::string getFragShaderSrc() const = 0;

	/**	@brief Get the maximum number of lights
	*/
	virtual GLuint getMaxLights() const = 0;

	/**	@brief Returns true if the number of lights is variable in the shader
	*/
	virtual bool hasVariableNumberOfLights() const = 0;

	/**	@brief Returns true if the shader is valid
	*/
	virtual bool        isValid() const = 0;

};

#endif /* defined(B_I_SHADER_DATA_H) */
