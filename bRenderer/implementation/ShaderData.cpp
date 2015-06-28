#include "../headers/ShaderData.h"
#include "../headers/Logger.h"
#include "../headers/OSdetect.h"
#include "../headers/Configuration.h"
#include "../headers/ShaderSource.h"

ShaderData::ShaderData()
	: _valid(false)
{}

ShaderData::ShaderData(GLuint maxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap)
{
	create(maxLights, ambientColor, diffuseColor, specularColor, diffuseMap, normalMap, specularMap);
}

ShaderData &ShaderData::create(GLuint maxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap)
{
	_maxLights = maxLights;

	_ambientColor = ambientColor;
	_diffuseColor = diffuseColor;
	_specularColor = specularColor;

	_diffuseMap = diffuseMap;
	_normalMap = normalMap;
	_specularMap = specularMap;

	
	initializeSourceCommonVariables();
	createVertShader();
	createFragShader();

	bRenderer::log("/////////////////  Generated shader code: \n");
	bRenderer::log(_vertShaderSrc);
	bRenderer::log(_fragShaderSrc);
	bRenderer::log("\n ///////////////// END \n");

	_valid = true;
	return *this;
}

void ShaderData::initializeSourceCommonVariables()
{
	std::string common = "";

#ifdef OS_DESKTOP	
	common = bRenderer::SHADER_SOURCE_HEAD_DESKTOP;
#endif
#ifdef OS_IOS
	common = bRenderer::SHADER_SOURCE_HEAD_ES;
#endif	
	// lights
	common += bRenderer::SHADER_SOURCE_NUM_LIGHTS;
	for (int i = 0; i < _maxLights; i++)
		common += bRenderer::shader_source_light_properties(i, _normalMap);
	// varyings
	common += bRenderer::SHADER_SOURCE_VARYINGS;

	_vertShaderSrc = common;
	_fragShaderSrc = common;
}

void ShaderData::createVertShader()
{
	// matrices
	_vertShaderSrc += bRenderer::SHADER_SOURCE_MATRICES;
	// attributes
	_vertShaderSrc += bRenderer::SHADER_SOURCE_ATTRIBUTES;
	
	if (_normalMap){
		// main function begin
		_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_BEGIN_TBN;
		// main function lights
		for (int i = 0; i < _maxLights; i++)
			_vertShaderSrc += bRenderer::shader_source_function_tangentSurface2light_tbn(i);
	}
	else
	{
		// main function begin
		_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_BEGIN;
	}
	// main function end 
	_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END;
}

void ShaderData::createFragShader()
{
	// colors 
	_fragShaderSrc += bRenderer::SHADER_SOURCE_COLORS;
	// textures
	_fragShaderSrc += bRenderer::SHADER_SOURCE_TEXTURES;

	// main function begin
	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN;

	if (_ambientColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT;

	if (_normalMap)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_NORMAL_MAP;
	else
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL;

	if (_diffuseColor)
	{
		// initialize
		if (_maxLights > 0)
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE;
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS;
		// lighting
		for (int i = 0; i < _maxLights; i++)
			_fragShaderSrc += bRenderer::shader_source_function_lighting(i, _normalMap);

		if (_diffuseMap)
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE_MAP;
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE;
	}

	if (_specularColor)
	{
		////////////////////////
		//
		//	TODO
		//
		///////////////////////
	}

	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART1;

	if (_ambientColor && (_diffuseColor || _specularColor))
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_AMBIENT + "+";
	else if (_ambientColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_AMBIENT;

	if (_diffuseColor && _specularColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_DIFFUSE + "+";
	else if (_diffuseColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_DIFFUSE;

	if (_specularColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_SPECULAR;

	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART2;

}