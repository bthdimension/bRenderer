//
//  ShaderData.cpp
//  Framework
//
//  Created by David Steiner on 4/19/13.
//
//

#include <string>
#include <fstream>
#include <iostream>
#include "../headers/ShaderData.h"
#include "../headers/Logger.h"
#include "../headers/FileHandler.h"
#include "../headers/OSdetect.h"

ShaderData::ShaderData(const std::string &shaderFile, std::string shaderVersionDesktop, std::string shaderVersionES)
	: _valid(true), _shaderVersionDesktop(shaderVersionDesktop), _shaderVersionES(shaderVersionES)
{
    load(shaderFile);
}

ShaderData::ShaderData(const std::string &vertShaderFileName, const std::string &fragShaderFileName, std::string shaderVersionDesktop, std::string shaderVersionES)
	: _valid(false), _shaderVersionDesktop(shaderVersionDesktop), _shaderVersionES(shaderVersionES)
{
    load(vertShaderFileName, fragShaderFileName);
}

ShaderData::ShaderData()
:   _valid(false)
{}

ShaderData &ShaderData::load(const std::string &shaderFile)
{
    return load(shaderFile + ".vert", shaderFile + ".frag");
}

ShaderData &ShaderData::load(const std::string &vertShaderFileName, const std::string &fragShaderFileName)
{
    _vertShaderSrc = loadSrc(vertShaderFileName);
    _fragShaderSrc = loadSrc(fragShaderFileName);

	size_t i;
	size_t s = SHADER_VERSION_MACRO.size();
#ifdef OS_DESKTOP	
	while ((i = _vertShaderSrc.find(SHADER_VERSION_MACRO)) != std::string::npos)
		_vertShaderSrc.replace(i, s, _shaderVersionDesktop);
	while ((i = _fragShaderSrc.find(SHADER_VERSION_MACRO)) != std::string::npos)
        _fragShaderSrc.replace(i, s, _shaderVersionDesktop);
#endif
#ifdef OS_IOS
	while ((i = _vertShaderSrc.find(SHADER_VERSION_MACRO)) != std::string::npos)
		_vertShaderSrc.replace(i, s, _shaderVersionES);
	while ((i = _fragShaderSrc.find(SHADER_VERSION_MACRO)) != std::string::npos)
		_fragShaderSrc.replace(i, s, _shaderVersionES);
#endif
    return *this;
}

std::string ShaderData::loadSrc(const std::string &fileName)
{
	bRenderer::log("Trying to load shader file " + fileName, bRenderer::LM_SYS);
    
    _valid = false;
    
    if (!bRenderer::fileExists(fileName))
    {
		bRenderer::log("Shader file doesn't exist: " + fileName, bRenderer::LM_ERROR);
        return std::string();
    }
    
	std::ifstream file(bRenderer::getFilePath(fileName), std::ifstream::in);
    
    std::string ret;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.length() > 0)
        {
            _valid = true;
        }
        ret += line + "\n";
    }
    
    return ret;
}
