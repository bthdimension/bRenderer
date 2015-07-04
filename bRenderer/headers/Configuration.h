#ifndef B_CONFIGURATION_H
#define B_CONFIGURATION_H

#include <string>
#include "Renderer_GL.h"
#include "vmmlib/addendum.hpp"

namespace bRenderer
{
	/* Renderer configuration */
	const vmml::vec3f DEFAULT_AMBIENT_COLOR									= vmml::vec3f(0.0, 0.0, 0.0);

	/* Logger */
	const std::string LOG_MODE_INFO											= "INFO";
	const std::string LOG_MODE_WARNING										= "WARNING";
	const std::string LOG_MODE_ERROR										= "ERROR";
	const std::string LOG_MODE_SYSTEM										= "SYSTEM";

	/* Desktop specific view configuration */
	const int DEFAULT_VIEW_WIDTH											= 640;
	const int DEFAULT_VIEW_HEIGHT											= 480;
	const std::string DEFAULT_WINDOW_TITLE									= "bRenderer";

	/* Desktop specific file path configuration */
	const std::string DEFAULT_FILE_PATH										= "data/";

	/* Shader macros*/
	const std::string SHADER_VERSION_MACRO									= "$B_SHADER_VERSION";
	const std::string SHADER_MAX_LIGHTS_MACRO								= "$B_SHADER_MAX_LIGHTS";

	/* Shader configuration */
	const std::string DEFAULT_SHADER_VERSION_DESKTOP						= "#version 120";
	const std::string DEFAULT_SHADER_VERSION_ES								= "#version 100";
	const GLuint DEFAULT_SHADER_MAX_LIGHTS									= 3;
	const std::string DEFAULT_FRAGMENT_SHADER_FILENAME_EXTENSION			= ".frag";
	const std::string DEFAULT_VERTEX_SHADER_FILENAME_EXTENSION				= ".vert";

	/* Shader uniforms */
	const std::string DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP					= "DiffuseMap";
	const std::string DEFAULT_SHADER_UNIFORM_SPECULAR_MAP					= "SpecularMap";
	const std::string DEFAULT_SHADER_UNIFORM_NORMAL_MAP						= "NormalMap";
	const std::string DEFAULT_SHADER_UNIFORM_NORMAL_MATRIX					= "NormalMatrix";
	const std::string DEFAULT_SHADER_UNIFORM_MODEL_MATRIX					= "ModelMatrix";
	const std::string DEFAULT_SHADER_UNIFORM_VIEW_MATRIX					= "ViewMatrix";
	const std::string DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX				= "ModelViewMatrix";
	const std::string DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX				= "ProjectionMatrix";
	const std::string DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS				= "numLights";
	const std::string DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE		= "lightPositionViewSpace_";
	const std::string DEFAULT_SHADER_UNIFORM_LIGHT_COLOR					= "lightColor_";
	const std::string DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY				= "lightIntensity_";
	const std::string DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION				= "lightAttenuation_";
	const std::string DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR					= "ambientColor";
	
	/* Shader attributes */
	const std::string DEFAULT_SHADER_ATTRIBUTE_POSITION						= "Position";
	const std::string DEFAULT_SHADER_ATTRIBUTE_NORMAL						= "Normal";
	const std::string DEFAULT_SHADER_ATTRIBUTE_TANGENT						= "Tangent";
	const std::string DEFAULT_SHADER_ATTRIBUTE_BITANGENT					= "Bitangent";
	const std::string DEFAULT_SHADER_ATTRIBUTE_TEXCOORD						= "TexCoord";

	/* Material key words */
	const std::string WAVEFRONT_MATERIAL_NEWMTL								= "newmtl";
	const std::string WAVEFRONT_MATERIAL_AMBIENT_COLOR						= "Ka";
	const std::string WAVEFRONT_MATERIAL_DIFFUSE_COLOR						= "Kd";
	const std::string WAVEFRONT_MATERIAL_SPECULAR_COLOR						= "Ks";
	const std::string WAVEFRONT_MATERIAL_TRANSMISSION_FILTER				= "Tf";
	const std::string WAVEFRONT_MATERIAL_SPECULAR_EXPONENT					= "Ns";
	const std::string WAVEFRONT_MATERIAL_OPTICAL_DENSITY					= "Ni";
	const std::string WAVEFRONT_MATERIAL_ILLUMINATION_MODEL					= "illum";
	const std::string WAVEFRONT_MATERIAL_DIFFUSE_MAP						= "map_Kd";
	const std::string WAVEFRONT_MATERIAL_SPECULAR_MAP						= "map_Ks";
	const std::string WAVEFRONT_MATERIAL_NORMAL_MAP							= "map_Bump";

	/* Camera configuration */
	const GLfloat DEFAULT_FIELD_OF_VIEW										= 60.0f;
	const GLfloat DEFAULT_ASPECT_RATIO										= 4.0f / 3.0f;
	const GLfloat DEFAULT_NEAR_CLIPPING_PLANE								= -1.0f;
	const GLfloat DEFAULT_FAR_CLIPPING_PLANE								= 1.0f;
	const vmml::vec3f DEFAULT_CAMERA_POSITION								= vmml::vec3f(0.0f, 0.0f, 0.0f);
	const vmml::mat4f DEFAULT_CAMERA_ROTATION								= vmml::create_rotation(0.0f, vmml::vec3f::UNIT_Y);

	/* Light configuration */
	const vmml::vec4f DEFAULT_LIGHT_POSITION								= vmml::vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	const vmml::vec3f DEFAULT_LIGHT_COLOR									= vmml::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	const GLfloat DEFAULT_LIGHT_INTENSITY									= 1000.0f;
	const GLfloat DEFAULT_LIGHT_ATTENUATION									= 1.0f;

} // namespace bRenderer


#endif /* defined(B_CONFIGURATION_H) */
