#ifndef B_SHADER_SOURCE_H
#define B_SHADER_SOURCE_H

#include "Configuration.h"

namespace bRenderer
{
	extern const std::string SHADER_SOURCE_LINE_BREAK;
	extern const std::string SHADER_SOURCE_LINE_ENDING;
	extern const std::string SHADER_SOURCE_WHITE_SPACE;

	// Head
	extern const std::string SHADER_SOURCE_HEAD_ES;
	extern const std::string SHADER_SOURCE_HEAD_DESKTOP;

	// Lights
	extern const std::string SHADER_SOURCE_NUM_LIGHTS;

    std::string shader_source_light_properties(GLuint maxLights, bool normalMap, bool diffuseLighting, bool specularLighting);

	// Matrices
	extern const std::string SHADER_SOURCE_MATRICES;

	// Attributes
	extern const std::string SHADER_SOURCE_ATTRIBUTES;

	// Varyings
	extern const std::string SHADER_SOURCE_VARYINGS_TEX_COORD;

	extern const std::string SHADER_SOURCE_VARYINGS_NORMAL;

	extern const std::string SHADER_SOURCE_VARYINGS_CAMERA_TANGENT;

	extern const std::string SHADER_SOURCE_VARYINGS_CAMERA_VIEW;

	// Colors 
	extern const std::string SHADER_SOURCE_COLORS;

	// Transparency value 
	extern const std::string SHADER_SOURCE_TRANSPARENCY_VALUE;

	// Textures
	extern const std::string SHADER_SOURCE_TEXTURES;

	// Text Textures
	extern const std::string SHADER_SOURCE_TEXT_TEXTURES;

	/* Vertex Shader */

	// Vertex Shader Main Function
	//Begin
	std::string shader_source_function_vertex_main_begin(bool hasLighting, bool hasTextures, bool normalMap);

	// TBN for normal map
	extern const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_TBN;
	// Camera tangent space
	extern const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_TANGENT_SPACE;
	// Camera view space
	extern const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_VIEW_SPACE;
	// Lighting
    std::string shader_source_function_lightVector(GLuint maxLights, bool normalMap, bool variableNumberOfLights);
	
	// End
	extern const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END;

	/* Fragment Shader */

	// Fragment Shader Main Function
	// Begin
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN;

	// Ambient
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT_COLOR;

	// Normals
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_TANGENT_SPACE;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_VIEW_SPACE;

	// Initialize diffuse lighting
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS;
	// Initialize diffuse lighting with transparency value
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_TRANSPARENCY;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS_TRANSPARENCY;

	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_LIGHTING;

	// Initialize specular lighting
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_VIEW_SPACE;

	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_TANGENT_SPACE;

	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_NO_LIGHTS;
	
	// Lighting
    std::string shader_source_function_lighting(GLuint maxLights, bool normalMap, bool diffuseLighting, bool specularLighting, bool variableNumberOfLights);

	// Finalize diffuse lighting
    std::string shader_source_function_fragment_finalize_diffuse(bool diffuseColor, bool diffuseMap);

	// Finalize specular lighting
    std::string shader_source_function_fragment_finalize_specular(bool specularColor, bool specularMap);

	// End
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART1;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_AMBIENT;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_DIFFUSE;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_SPECULAR;
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_PART2;

	// End Text
	extern const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_END_TEXT;


} // namespace bRenderer

#endif /* defined(B_SHADER_SOURCE_H) */