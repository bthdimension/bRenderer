//
//  ShaderData.h
//  Framework
//
//  Created by David Steiner on 4/18/13.
//
//

#ifndef B_SHADER_DATA_H
#define B_SHADER_DATA_H

#include <string>

class ShaderData
{
public:
	explicit ShaderData(const std::string &shaderFile, std::string shaderVersionDesktop, std::string shaderVersionES);
	ShaderData(const std::string &vertShaderFileName, const std::string &fragShaderFileName, std::string shaderVersionDesktop, std::string shaderVersionES);
    ShaderData();
    
    ShaderData &load(const std::string &shaderFile);
    ShaderData &load(const std::string &vertShaderFileName, const std::string &fragShaderFileName);
    
    std::string getVertShaderSrc()  const   { return _vertShaderSrc; }
    std::string getFragShaderSrc()  const   { return _fragShaderSrc; }
    bool        isValid()           const   { return _valid;         }

private:
    std::string _vertShaderSrc;
    std::string _fragShaderSrc;
	std::string _shaderVersionDesktop;
	std::string _shaderVersionES;
    bool        _valid;
    
    std::string loadSrc(const std::string &fileName);

	const std::string SHADER_VERSION_MACRO = "$B_SHADER_VERSION";
};

#endif /* defined(B_SHADER_DATA_H) */
