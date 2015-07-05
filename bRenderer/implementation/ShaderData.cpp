#include "../headers/ShaderData.h"
#include "../headers/Logger.h"
#include "../headers/OSdetect.h"
#include "../headers/Configuration.h"
#include "../headers/ShaderSource.h"

ShaderData::ShaderData()
	: _valid(false)
{}

ShaderData::ShaderData(GLuint maxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights)
{
	create(maxLights, ambientColor, diffuseColor, specularColor, diffuseMap, normalMap, specularMap, variableNumberOfLights);
}

ShaderData &ShaderData::create(GLuint maxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights)
{
	_maxLights = maxLights;
	_variableNumberOfLights = variableNumberOfLights;

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
	if (_variableNumberOfLights)
		common += bRenderer::SHADER_SOURCE_NUM_LIGHTS;
	common += bRenderer::shader_source_light_properties(_maxLights, _normalMap);
	// varyings
	common += bRenderer::SHADER_SOURCE_VARYINGS;
	if (!_normalMap && _diffuseColor)
		common += bRenderer::SHADER_SOURCE_VARYINGS_NORMAL;
	if (_normalMap && _specularColor)
		common += bRenderer::SHADER_SOURCE_VARYINGS_CAMERA_TANGENT;

	_vertShaderSrc = common;
	_fragShaderSrc = common;
}

void ShaderData::createVertShader()
{
	// matrices
	_vertShaderSrc += bRenderer::SHADER_SOURCE_MATRICES;
	// attributes
	_vertShaderSrc += bRenderer::SHADER_SOURCE_ATTRIBUTES;
	
	// main function begin
	_vertShaderSrc += bRenderer::shader_source_function_vertex_main_begin(_normalMap);
	if (_normalMap){
		// main function tbn
		_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_TBN;
		// camera tangent space
		_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_TANGENT_SPACE;
	}
	// main function lights
	_vertShaderSrc += bRenderer::shader_source_function_lightVector(_maxLights, _normalMap, _variableNumberOfLights);

	// main function end 
	_vertShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END;
}

void ShaderData::createFragShader()
{
	// textures
	_fragShaderSrc += bRenderer::SHADER_SOURCE_TEXTURES;
	// colors 
	_fragShaderSrc += bRenderer::SHADER_SOURCE_COLORS;	

	// main function begin
	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN;

	if (_ambientColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT;
	if (_normalMap)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_TANGENT_SPACE;
	else
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_VIEW_SPACE;

	if (_diffuseColor)
	{
		// initialize
		if (_maxLights > 0)
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE;
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS;
		
		if (_specularColor)
		{
			if (_maxLights > 0){
				if (_normalMap)
					_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_TANGENT_SPACE;
				else
					_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_VIEW_SPACE;
			}
			else
				_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_NO_LIGHTS;
		}
		// lighting
		_fragShaderSrc += bRenderer::shader_source_function_lighting(_maxLights, _normalMap, _specularColor, _variableNumberOfLights);

		if (_diffuseMap)
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE_MAP;
		else
			_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE;

		if (_specularColor){
			if (_specularMap)
				_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_SPECULAR_MAP;
			else
				_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_SPECULAR;
		}
	}

	

	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART1;

	if (_ambientColor && (_diffuseColor || _specularColor))
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_AMBIENT + "+";
	else if (_ambientColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_AMBIENT;

	if (_diffuseColor && _specularColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_DIFFUSE + "+" + bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_SPECULAR;
	else if (_diffuseColor)
		_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_DIFFUSE;

	_fragShaderSrc += bRenderer::SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART2;

}