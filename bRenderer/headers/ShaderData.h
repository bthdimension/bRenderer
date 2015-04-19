//
//  ShaderData.h
//  Framework
//
//  Created by David Steiner on 4/18/13.
//
//

#ifndef FRAMEWORK_SHADER_DATA_H
#define FRAMEWORK_SHADER_DATA_H

#include <string>

class ShaderData
{
public:
    explicit ShaderData(const std::string &shaderFile);
    ShaderData(const std::string &vertShaderFileName, const std::string &fragShaderFileName);
    ShaderData();
    
    ShaderData &load(const std::string &shaderFile);
    ShaderData &load(const std::string &vertShaderFileName, const std::string &fragShaderFileName);
    
    std::string getVertShaderSrc()  const   { return _vertShaderSrc; }
    std::string getFragShaderSrc()  const   { return _fragShaderSrc; }
    bool        isValid()           const   { return _valid;         }

private:
    std::string _vertShaderSrc;
    std::string _fragShaderSrc;
    bool        _valid;
    
    std::string loadSrc(const std::string &fileName);
};

#endif
