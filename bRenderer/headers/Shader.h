//
//  Shader.h
//  Framework
/*
    Load, compile and use shader in the graphics manager class
*/
//  Created by Rahul Mukhi on 1/9/13.
//

#ifndef B_SHADER_H
#define B_SHADER_H

#include <memory>
#include <string>
#include <unordered_map>
#include "vmmlib/matrix.hpp"
#include "Renderer_GL.h"
#include "Texture.h"


class ShaderData;

class Shader
{
public:
    struct Attrib
    {
        GLint   loc;
        GLint   size;
        GLenum  type;
        GLsizei stride;     // size of vertex data structure
        size_t  offset;
    };
    
    typedef std::unordered_map< std::string, GLint >    Locations;
    typedef std::unordered_map< std::string, Attrib >   Attribs;
    
    virtual void bind();
    
    virtual void setUniform(const std::string &name, const vmml::mat4f &arg);
    virtual void setUniform(const std::string &name, const vmml::mat3f &arg);
    virtual void setUniform(const std::string &name, const vmml::vec4f &arg);
    virtual void setUniform(const std::string &name, const vmml::vec3f &arg);
    virtual void setUniform(const std::string &name, float arg);
    
    virtual void setUniform(const std::string &name, TexturePtr texture);
    
    virtual GLint registerUniform(const std::string &name);
    virtual GLint registerAttrib(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset);
    
    Shader(const ShaderData &shaderData);
    ~Shader();
    
    GLuint getProgramID();
    
    virtual GLint findUniformLocation(const std::string &name);
    virtual GLint findAttribLocation(const std::string &name, GLint size, GLenum type, GLsizei stride, size_t offset);
    virtual GLint getUniformLocation(const std::string &name);
    virtual GLint getAttribLocation(const std::string &name);
    
    template< typename T >
    void setUniforms(const T &arg)
    {
        for (auto i = arg.cbegin(); i != arg.cend(); ++i)
        {
            setUniform(i->first, i->second);
        }
    }
    
protected:
    virtual void resetTexUnit();
    
private:
    GLuint _programID;
    bool compile(GLuint* shader, GLenum type, const std::string &src);
    bool link();
    bool validate();
    
    GLint   _cTexUnit;
    GLint   _maxTexUnits;
    
    Locations   _uniformLocations;
    Attribs     _attribs;
};

typedef std::shared_ptr< Shader > ShaderPtr;

#endif /* defined(B_SHADER_H) */
