#ifndef B_SHADER_H
#define B_SHADER_H

#include <memory>
#include <string>
#include <unordered_map>
#include "vmmlib/matrix.hpp"
#include "Renderer_GL.h"
#include "Texture.h"


class ShaderData;

/** @brief Shader 
*	@author Rahul Mukhi, David Steiner
*/
class Shader
{
public:

	/* Structs */

    struct Attrib
    {
        GLint   loc;
        GLint   size;
        GLenum  type;
        GLsizei stride;     // size of vertex data structure
        size_t  offset;
    };

	/* Typedefs */

    typedef std::unordered_map< std::string, GLint >    Locations;
    typedef std::unordered_map< std::string, Attrib >   Attribs;
    
	/* Functions */

	/**	@brief Binds the sahder and its attributes
	*/
    virtual void bind();
    
	/**	@brief Pass a 4 dimensional matrix to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 4 dimensional matrix
	*/
    virtual void setUniform(const std::string &name, const vmml::mat4f &arg);

	/**	@brief Pass a 3 dimensional matrix to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 3 dimensional matrix
	*/
    virtual void setUniform(const std::string &name, const vmml::mat3f &arg);

	/**	@brief Pass a 4 dimensional vector to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 4 dimensional vector
	*/
    virtual void setUniform(const std::string &name, const vmml::vec4f &arg);

	/**	@brief Pass a 3 dimensional vector to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg 3 dimensional vector
	*/
    virtual void setUniform(const std::string &name, const vmml::vec3f &arg);

	/**	@brief Pass a floating-point number to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg Floating-point number
	*/
    virtual void setUniform(const std::string &name, float arg);
    
	/**	@brief Pass a texture to the shader
	*	@param[in] name Uniform name
	*	@param[in] arg Texture
	*/
    virtual void setUniform(const std::string &name, TexturePtr texture);
    
	/**	@brief Register a uniform
	*	@param[in] name Uniform name
	*/
    virtual GLint registerUniform(const std::string &name);

	/**	@brief Register an attribute
	*	@param[in] name Attribute name
	*	@param[in] size
	*	@param[in] type
	*	@param[in] stride
	*	@param[in] offset
	*/
    virtual GLint registerAttrib(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset);
    
	/**	@brief Constructor
	*	@param[in] shaderData
	*/
    Shader(const ShaderData &shaderData);

	/**	@brief Destructor
	*/
    ~Shader();
    
	/**	@brief Returns the shader ID
	*/
    GLuint getProgramID();
    
	/**	@brief Returns uniform location (tries to register uniform if not already available)
	*	@param[in] name Uniform name
	*/
    virtual GLint findUniformLocation(const std::string &name);

	/**	@brief Returns attribute location (tries to register attribute if not already available)
	*	@param[in] name Attribute name
	*	@param[in] size
	*	@param[in] type
	*	@param[in] stride
	*	@param[in] offset
	*/
    virtual GLint findAttribLocation(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset);

	/**	@brief Returns uniform location
	*	@param[in] name Uniform name
	*/
    virtual GLint getUniformLocation(const std::string &name);

	/**	@brief Returns attribute location
	*	@param[in] name Attribute name
	*/
    virtual GLint getAttribLocation(const std::string &name);

	/**	@brief Get the maximum number of lights used 
	*/
	GLuint getMaxLights() { return _shaderMaxLights; }
    
    template< typename T >
	/**	@brief Sets multiple uniforms
	*	@param[in] arg Data to pass to the shader
	*/
    void setUniforms(const T &arg)
    {
        for (auto i = arg.cbegin(); i != arg.cend(); ++i)
        {
            setUniform(i->first, i->second);
        }
    }
    
protected:
	
	/* Functions */

    virtual void resetTexUnit();
    
private:

	/* Variables */

    GLuint _programID;
    bool compile(GLuint* shader, GLenum type, const std::string &src);
    bool link();
    bool validate();
    
    GLint   _cTexUnit;
    GLint   _maxTexUnits;
    
    Locations   _uniformLocations;
    Attribs     _attribs;

	GLuint		_shaderMaxLights;
};

typedef std::shared_ptr< Shader > ShaderPtr;

#endif /* defined(B_SHADER_H) */
