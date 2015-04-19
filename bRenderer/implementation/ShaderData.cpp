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
#include "../bRenderer.h"

ShaderData::ShaderData(const std::string &shaderFile)
:   _valid(true)
{
    load(shaderFile);
}

ShaderData::ShaderData(const std::string &vertShaderFileName, const std::string &fragShaderFileName)
:   _valid(false)
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
