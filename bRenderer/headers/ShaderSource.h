#ifndef B_SHADER_SOURCE_H
#define B_SHADER_SOURCE_H

#include "Configuration.h"
#include <boost/lexical_cast.hpp>

using boost::lexical_cast;

namespace bRenderer
{
	const std::string SHADER_SOURCE_LINE_BREAK		= "\n";
	const std::string SHADER_SOURCE_LINE_ENDING		= ";" + SHADER_SOURCE_LINE_BREAK;
	const std::string SHADER_SOURCE_WHITE_SPACE		= " ";

	// Head
	const std::string SHADER_SOURCE_HEAD_ES			= DEFAULT_SHADER_VERSION_ES + SHADER_SOURCE_LINE_BREAK + "precision mediump float" + SHADER_SOURCE_LINE_ENDING;
	const std::string SHADER_SOURCE_HEAD_DESKTOP	= DEFAULT_SHADER_VERSION_DESKTOP + SHADER_SOURCE_LINE_BREAK;

	// Lights
	const std::string SHADER_SOURCE_NUM_LIGHTS = "uniform float " + DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS + SHADER_SOURCE_LINE_ENDING;

	static std::string shader_source_light_properties(int light_number, bool normalMap)
	{
		std::string num = lexical_cast<std::string>(light_number);
		return "uniform vec4 " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION + num + SHADER_SOURCE_LINE_ENDING
			+ "uniform vec3 " + DEFAULT_SHADER_UNIFORM_LIGHT_COLOR + num + SHADER_SOURCE_LINE_ENDING
			+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY + num + SHADER_SOURCE_LINE_ENDING
			+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION + num + SHADER_SOURCE_LINE_ENDING
			+ (normalMap ? ("varying vec3 tangentSurface2light_" + num + SHADER_SOURCE_LINE_ENDING) : "");
	}

	// Matrices
	const std::string SHADER_SOURCE_MATRICES =
		"uniform mat4 " + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + SHADER_SOURCE_LINE_ENDING
		+ "uniform mat4 " + DEFAULT_SHADER_UNIFORM_VIEW_MATRIX + SHADER_SOURCE_LINE_ENDING
		+ "uniform mat4 " + DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX + SHADER_SOURCE_LINE_ENDING;

	// Attributes
	const std::string SHADER_SOURCE_ATTRIBUTES =
		"attribute vec4 " + DEFAULT_SHADER_ATTRIBUTE_POSITION + SHADER_SOURCE_LINE_ENDING
		+ "attribute vec3 " + DEFAULT_SHADER_ATTRIBUTE_NORMAL + SHADER_SOURCE_LINE_ENDING
		+ "attribute vec3 " + DEFAULT_SHADER_ATTRIBUTE_TANGENT + SHADER_SOURCE_LINE_ENDING
		+ "attribute vec3 " + DEFAULT_SHADER_ATTRIBUTE_BITANGENT + SHADER_SOURCE_LINE_ENDING
		+ "attribute vec4 " + DEFAULT_SHADER_ATTRIBUTE_TEXCOORD + SHADER_SOURCE_LINE_ENDING;

	// Varyings
	const std::string SHADER_SOURCE_VARYINGS =
		"varying vec4 texCoordVarying" + SHADER_SOURCE_LINE_ENDING
		+ "varying vec3 normalVarying" + SHADER_SOURCE_LINE_ENDING
		+ "varying vec4 posWorldSpace" + SHADER_SOURCE_LINE_ENDING;

	// Colors 
	const std::string SHADER_SOURCE_COLORS =
		"uniform vec3 " + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_AMBIENT_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_DIFFUSE_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_SPECULAR_COLOR + SHADER_SOURCE_LINE_ENDING;

	// Textures
	const std::string SHADER_SOURCE_TEXTURES =
		"uniform sampler2D " + DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP + SHADER_SOURCE_LINE_ENDING
		+ "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_NORMAL_MAP + SHADER_SOURCE_LINE_ENDING
		+ "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_SPECULAR_MAP + SHADER_SOURCE_LINE_ENDING;

	// Vertex Shader Main Function
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_BEGIN =
		"void main() {" + SHADER_SOURCE_LINE_BREAK
		+ "posWorldSpace = " + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + "*" + DEFAULT_SHADER_ATTRIBUTE_POSITION + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_BEGIN_TBN =
		"void main() {" + SHADER_SOURCE_LINE_BREAK
		+ "posWorldSpace = " + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + "*" + DEFAULT_SHADER_ATTRIBUTE_POSITION + SHADER_SOURCE_LINE_ENDING
		+ "vec3 vertexNormal_WorldSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + ") * normalize(" + DEFAULT_SHADER_ATTRIBUTE_NORMAL + ")" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 vertexTangent_WorldSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + ") * normalize(" + DEFAULT_SHADER_ATTRIBUTE_TANGENT + ")" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 vertexBitangent_WorldSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + ") * normalize(" + DEFAULT_SHADER_ATTRIBUTE_BITANGENT + ")" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 firstRow = vec3(vertexTangent_WorldSpace.x, vertexBitangent_WorldSpace.x, vertexNormal_WorldSpace.x)" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 secondRow = vec3(vertexTangent_WorldSpace.y, vertexBitangent_WorldSpace.y, vertexNormal_WorldSpace.y)" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 thirdRow = vec3(vertexTangent_WorldSpace.z, vertexBitangent_WorldSpace.z, vertexNormal_WorldSpace.z)" + SHADER_SOURCE_LINE_ENDING
		+ "mat3 TBN = mat3(firstRow, secondRow, thirdRow)" + SHADER_SOURCE_LINE_ENDING;

	static std::string shader_source_function_tangentSurface2light_tbn(int light_number)
	{
		std::string num = lexical_cast<std::string>(light_number);
		std::string numPP = lexical_cast<std::string>(light_number+1);
		return "if(numLights >= " + numPP + ".0){" + SHADER_SOURCE_LINE_BREAK
			+ "tangentSurface2light_"+num+" = normalize(TBN*(" + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION + num + ".xyz - posWorldSpace.xyz))" + SHADER_SOURCE_LINE_ENDING
			+ "} ";
	}

	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END =
		"texCoordVarying = " + DEFAULT_SHADER_ATTRIBUTE_TEXCOORD + SHADER_SOURCE_LINE_ENDING
		+ "normalVarying = normalize(mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + ")*" + DEFAULT_SHADER_ATTRIBUTE_NORMAL + ")" + SHADER_SOURCE_LINE_ENDING
		+ "gl_Position = " + DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX + "*" + DEFAULT_SHADER_UNIFORM_VIEW_MATRIX + "*" + DEFAULT_SHADER_UNIFORM_MODEL_MATRIX + "*" + DEFAULT_SHADER_ATTRIBUTE_POSITION + SHADER_SOURCE_LINE_ENDING
		+ "}" + SHADER_SOURCE_LINE_BREAK;

	// Fragment Shader Main Function
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN = "void main() {" + SHADER_SOURCE_LINE_BREAK;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT = "vec4 ambient  = vec4(clamp(" + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR + "+" + WAVEFRONT_MATERIAL_AMBIENT_COLOR + ", 0.0, 1.0), 1.0)" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_NORMAL_MAP = "vec3 surfaceNormal = normalize(texture2D(" + DEFAULT_SHADER_UNIFORM_NORMAL_MAP + ", texCoordVarying.st).xyz *2.0 - 1.0)" + SHADER_SOURCE_LINE_ENDING;
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL = "vec3 surfaceNormal = normalVarying" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE =
		"vec4 diffuse = vec4(0.0,0.0,0.0,1.0)" + SHADER_SOURCE_LINE_ENDING
		+ "float distanceToLight = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "float intensityBasedOnDist = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "float intensity = 0.0" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS =
		"vec4 diffuse = vec4(1.0,1.0,1.0, 1.0)" + SHADER_SOURCE_LINE_ENDING;

	static std::string shader_source_function_lighting(int light_number, bool normalMap)
	{
		std::string num = lexical_cast<std::string>(light_number);
		std::string numPP = lexical_cast<std::string>(light_number + 1);
		return "if(numLights >= " + numPP + ".0){" + SHADER_SOURCE_LINE_BREAK		
			+ (normalMap ? "" : ("vec3 lightVector = normalize(" + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION + num + ".xyz - posWorldSpace.xyz)" + SHADER_SOURCE_LINE_ENDING))
			+ "intensity = max(dot(surfaceNormal, " + (normalMap ? ("tangentSurface2light_" + num) : "lightVector") + "), 0.0)" + SHADER_SOURCE_LINE_ENDING
			+ "if (intensity > 0.0){" + SHADER_SOURCE_LINE_BREAK
			+ "intensity = clamp(intensity, 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING
			+ "distanceToLight = distance(posWorldSpace, " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION + num + ")" + SHADER_SOURCE_LINE_ENDING
			+ "intensityBasedOnDist = " + DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY + num + " / (" + DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION + num + "*distanceToLight*distanceToLight)" + SHADER_SOURCE_LINE_ENDING
			+ "intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING
			+ "diffuse += vec4(" + DEFAULT_SHADER_UNIFORM_LIGHT_COLOR + num + " * intensity * intensityBasedOnDist, 0.0);	}" + SHADER_SOURCE_LINE_BREAK
			+ "} ";
	}

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE_MAP =
		"diffuse = diffuse * vec4(" + WAVEFRONT_MATERIAL_DIFFUSE_COLOR + ",1.0) * texture2D(" + DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP + ", texCoordVarying.st)" + SHADER_SOURCE_LINE_ENDING;
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE =
		"diffuse = diffuse * vec4(" + WAVEFRONT_MATERIAL_DIFFUSE_COLOR +",1.0)" + SHADER_SOURCE_LINE_ENDING;
	
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART1 = "gl_FragColor = clamp(";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_AMBIENT = "ambient";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_DIFFUSE = "diffuse";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_SPECULAR = "specular";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART2 = ", 0.0, 1.0);}";





} // namespace bRenderer

#endif /* defined(B_SHADER_SOURCE_H) */