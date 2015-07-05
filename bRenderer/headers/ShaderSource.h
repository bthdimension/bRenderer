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

	static std::string shader_source_light_properties(GLuint maxLights, bool normalMap)
	{
		std::string lighting = "";
		for (GLuint light_number = 0; light_number < maxLights; light_number++){
			std::string num = lexical_cast<std::string>(light_number);
			lighting += "uniform vec4 " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE + num + SHADER_SOURCE_LINE_ENDING
				+ "uniform vec3 " + DEFAULT_SHADER_UNIFORM_LIGHT_COLOR + num + SHADER_SOURCE_LINE_ENDING
				+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY + num + SHADER_SOURCE_LINE_ENDING
				+ "uniform float " + DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION + num + SHADER_SOURCE_LINE_ENDING;
			if (normalMap)
				lighting += "varying vec3 lightVectorTangentSpace_" + num + SHADER_SOURCE_LINE_ENDING;
			else
				lighting += "varying vec3 lightVectorViewSpace_" + num + SHADER_SOURCE_LINE_ENDING;
		}
		return lighting;
	}

	// Matrices
	const std::string SHADER_SOURCE_MATRICES =
		"uniform mat4 " + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX + SHADER_SOURCE_LINE_ENDING
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
		+ "varying vec4 posVaryingViewSpace" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_VARYINGS_NORMAL =
		+ "varying vec3 normalVaryingViewSpace" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_VARYINGS_CAMERA_TANGENT =
		+"varying vec3 surfaceToCameraTangentSpace" + SHADER_SOURCE_LINE_ENDING;

	// Colors 
	const std::string SHADER_SOURCE_COLORS =
		"uniform vec3 " + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_AMBIENT_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_DIFFUSE_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform vec3 " + WAVEFRONT_MATERIAL_SPECULAR_COLOR + SHADER_SOURCE_LINE_ENDING
		+ "uniform float " + WAVEFRONT_MATERIAL_SPECULAR_EXPONENT + SHADER_SOURCE_LINE_ENDING;

	// Textures
	const std::string SHADER_SOURCE_TEXTURES =
		"uniform sampler2D " + DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP + SHADER_SOURCE_LINE_ENDING
		+ "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_NORMAL_MAP + SHADER_SOURCE_LINE_ENDING
		+ "uniform sampler2D " + DEFAULT_SHADER_UNIFORM_SPECULAR_MAP + SHADER_SOURCE_LINE_ENDING;

/* Vertex Shader */

	// Vertex Shader Main Function
	//Begin
	static std::string shader_source_function_vertex_main_begin(bool normalMap)
	{
		std::string main = 
			"void main() {" + SHADER_SOURCE_LINE_BREAK
			+ "posVaryingViewSpace = " + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX + "*" + DEFAULT_SHADER_ATTRIBUTE_POSITION + SHADER_SOURCE_LINE_ENDING
			+ "texCoordVarying = " + DEFAULT_SHADER_ATTRIBUTE_TEXCOORD + SHADER_SOURCE_LINE_ENDING;
		if (!normalMap)
			main += "normalVaryingViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX + ")*" + DEFAULT_SHADER_ATTRIBUTE_NORMAL + SHADER_SOURCE_LINE_ENDING;
		return main;
	}
	// TBN for normal map
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_TBN =
		+"vec3 vertexNormal_ViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX + ") * " + DEFAULT_SHADER_ATTRIBUTE_NORMAL + SHADER_SOURCE_LINE_ENDING
		+ "vec3 vertexTangent_ViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX + ") * " + DEFAULT_SHADER_ATTRIBUTE_TANGENT + SHADER_SOURCE_LINE_ENDING
		+ "vec3 vertexBitangent_ViewSpace = mat3(" + DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX + ") * " + DEFAULT_SHADER_ATTRIBUTE_BITANGENT + SHADER_SOURCE_LINE_ENDING
		+ "vec3 firstRow = vec3(vertexTangent_ViewSpace.x, vertexBitangent_ViewSpace.x, vertexNormal_ViewSpace.x)" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 secondRow = vec3(vertexTangent_ViewSpace.y, vertexBitangent_ViewSpace.y, vertexNormal_ViewSpace.y)" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 thirdRow = vec3(vertexTangent_ViewSpace.z, vertexBitangent_ViewSpace.z, vertexNormal_ViewSpace.z)" + SHADER_SOURCE_LINE_ENDING
		+ "mat3 TBN = mat3(firstRow, secondRow, thirdRow)" + SHADER_SOURCE_LINE_ENDING;
	// Camera tangent space
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_CAMERA_TANGENT_SPACE =
		+"surfaceToCameraTangentSpace = TBN*( - posVaryingViewSpace.xyz )" + SHADER_SOURCE_LINE_ENDING;
	// Lighting
	static std::string shader_source_function_lightVector(GLuint maxLights, bool normalMap, bool variableNumberOfLights)
	{
		std::string lighting = "";
		for (GLuint light_number = 0; light_number < maxLights; light_number++){
			std::string num = lexical_cast<std::string>(light_number);
			std::string numPP = lexical_cast<std::string>(light_number + 1);
			if (variableNumberOfLights)
				lighting += "if(" + DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS + " >= " + numPP + ".0){" + SHADER_SOURCE_LINE_BREAK;
			if (normalMap) 
				lighting += "lightVectorTangentSpace_" + num + " = TBN*(" + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE + num + ".xyz - posVaryingViewSpace.xyz)" + SHADER_SOURCE_LINE_ENDING;
			else
				lighting += "lightVectorViewSpace_" + num + " = " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE + num + ".xyz - posVaryingViewSpace.xyz" + SHADER_SOURCE_LINE_ENDING;
		}
		if (variableNumberOfLights){
			for (GLuint light_number = 0; light_number < maxLights; light_number++)
				lighting += "} ";
		}
		return lighting;
	}	
	// End
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END =
		+ "gl_Position = " + DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX + "*posVaryingViewSpace" + SHADER_SOURCE_LINE_ENDING
		+ "}" + SHADER_SOURCE_LINE_BREAK;

/* Fragment Shader */

	// Fragment Shader Main Function
	// Begin
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_MAIN_BEGIN = "void main() {" + SHADER_SOURCE_LINE_BREAK;
	
	// Ambient
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT = "vec4 ambient  = vec4(clamp(" + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR + ", 0.0, 1.0), 0.0)" + SHADER_SOURCE_LINE_ENDING;
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_AMBIENT_COLOR = "vec4 ambient  = vec4(clamp(" + DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR + "+" + WAVEFRONT_MATERIAL_AMBIENT_COLOR + ", 0.0, 1.0), 0.0)" + SHADER_SOURCE_LINE_ENDING;

	// Normals
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_TANGENT_SPACE = "vec3 surfaceNormal = normalize(texture2D(" + DEFAULT_SHADER_UNIFORM_NORMAL_MAP + ", texCoordVarying.st).xyz *2.0 - 1.0)" + SHADER_SOURCE_LINE_ENDING;
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_SURFACE_NORMAL_VIEW_SPACE = "vec3 surfaceNormal = normalize(normalVaryingViewSpace)" + SHADER_SOURCE_LINE_ENDING;

	// Initialize diffuse lighting
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE =
		"vec4 diffuse = vec4(0.0,0.0,0.0,1.0)" + SHADER_SOURCE_LINE_ENDING
		+ "float distanceToLight = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "float intensityBasedOnDist = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "float intensity = 0.0" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_DIFFUSE_NO_LIGHTS =
		"vec4 diffuse = vec4(1.0,1.0,1.0,1.0)" + SHADER_SOURCE_LINE_ENDING;

	// Initialize specular lighting
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_VIEW_SPACE =
		"float specularity = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "float specularCoefficient = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 surfaceToCamera = normalize(-posVaryingViewSpace.xyz)" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_TANGENT_SPACE =
		"float specularity = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "float specularCoefficient = 0.0" + SHADER_SOURCE_LINE_ENDING
		+ "vec3 surfaceToCamera = normalize(surfaceToCameraTangentSpace)" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_INIT_SPECULAR_NO_LIGHTS =
		"float specularity = 0.0" + SHADER_SOURCE_LINE_ENDING;

	// Lighting
	static std::string shader_source_function_lighting(GLuint maxLights, bool normalMap, bool specularColor, bool variableNumberOfLights)
	{
		std::string lighting = "";
		for (GLuint light_number = 0; light_number < maxLights; light_number++){
			std::string num = lexical_cast<std::string>(light_number);
			std::string numPP = lexical_cast<std::string>(light_number + 1);
			if (variableNumberOfLights)
				lighting += "if(" + DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS + " >= " + numPP + ".0){" + SHADER_SOURCE_LINE_BREAK;
			if (normalMap)
				lighting += "intensity = max(dot(surfaceNormal, normalize(lightVectorTangentSpace_" + num + ")), 0.0)" + SHADER_SOURCE_LINE_ENDING;
			else
				lighting += "intensity = max(dot(surfaceNormal, normalize(lightVectorViewSpace_" + num + ")), 0.0)" + SHADER_SOURCE_LINE_ENDING;

			lighting += "if (intensity > 0.0){" + SHADER_SOURCE_LINE_BREAK
				+ "intensity = clamp(intensity, 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING
				+ "distanceToLight = distance(posVaryingViewSpace, " + DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE + num + ")" + SHADER_SOURCE_LINE_ENDING
				+ "intensityBasedOnDist = " + DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY + num + " / (" + DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION + num + "*distanceToLight*distanceToLight)" + SHADER_SOURCE_LINE_ENDING
				+ "intensityBasedOnDist = clamp(intensityBasedOnDist, 0.0, 1.0)" + SHADER_SOURCE_LINE_ENDING
				+ "diffuse += vec4(" + DEFAULT_SHADER_UNIFORM_LIGHT_COLOR + num + " * intensity * intensityBasedOnDist, 0.0);	}" + SHADER_SOURCE_LINE_BREAK;
			if (specularColor) {
				if (normalMap)
					lighting += "specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-normalize(lightVectorTangentSpace_" + num + "), surfaceNormal))), ";
				else
					lighting += "specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-normalize(lightVectorViewSpace_" + num + "), surfaceNormal))), ";
				lighting += WAVEFRONT_MATERIAL_SPECULAR_EXPONENT + ")" + SHADER_SOURCE_LINE_ENDING;
				lighting += "specularity += specularCoefficient * intensity * intensityBasedOnDist" + SHADER_SOURCE_LINE_ENDING;
			}
		}
		if (variableNumberOfLights){
			for (GLuint light_number = 0; light_number < maxLights; light_number++)
				lighting += "} ";
		}
		return lighting;
	}

	// Finalize diffuse lighting
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE_MAP =
		"diffuse = diffuse * vec4(" + WAVEFRONT_MATERIAL_DIFFUSE_COLOR + ",1.0) * texture2D(" + DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP + ", texCoordVarying.st)" + SHADER_SOURCE_LINE_ENDING;
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_DIFFUSE =
		"diffuse = diffuse * vec4(" + WAVEFRONT_MATERIAL_DIFFUSE_COLOR +",1.0)" + SHADER_SOURCE_LINE_ENDING;

	// Finalize specular lighting
	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_SPECULAR =
		"vec4 specular = specularity * vec4(" + WAVEFRONT_MATERIAL_SPECULAR_COLOR + ", 0.0)" + SHADER_SOURCE_LINE_ENDING;

	const std::string SHADER_SOURCE_FUNCTION_FRAGMENT_FINALIZE_SPECULAR_MAP =
		"vec4 specular = specularity * vec4(" + WAVEFRONT_MATERIAL_SPECULAR_COLOR + ", 0.0) * texture2D(" + DEFAULT_SHADER_UNIFORM_SPECULAR_MAP + ", texCoordVarying.st)" + SHADER_SOURCE_LINE_ENDING;

	// End
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART1 = "gl_FragColor = clamp(";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_AMBIENT = "ambient";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_DIFFUSE = "diffuse";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_SPECULAR = "specular";
	const std::string SHADER_SOURCE_FUNCTION_VERTEX_MAIN_END_PART2 = ", 0.0, 1.0);}";





} // namespace bRenderer

#endif /* defined(B_SHADER_SOURCE_H) */