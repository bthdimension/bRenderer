#include "../headers/Configuration.h"

namespace bRenderer
{
	/* Renderer configuration */
	vmml::Vector3f DEFAULT_AMBIENT_COLOR() { static const vmml::Vector3f x = vmml::Vector3f(0.0f, 0.0f, 0.0f); return x; }

	/* Logger */
	std::string LOG_MODE_INFO() { static const std::string x = "INFO"; return x; }
	std::string LOG_MODE_WARNING() { static const std::string x = "WARNING"; return x; }
	std::string LOG_MODE_ERROR() { static const std::string x = "ERROR"; return x; }
	std::string LOG_MODE_SYSTEM() { static const std::string x = "SYSTEM"; return x; }

	/* Desktop specific view configuration */
	GLint DEFAULT_VIEW_WIDTH() { static const GLint x = 640; return x; }
	GLint DEFAULT_VIEW_HEIGHT() { static const GLint x = 480; return x; }
	std::string DEFAULT_WINDOW_TITLE() { static const std::string x = "bRenderer"; return x; }

	/* Desktop specific file path configuration */
	std::string DEFAULT_FILE_PATH() { static const std::string x = "data/"; return x; }

	/* OBJ loader default group name */
	std::string DEFAULT_GROUP_NAME() { static const std::string x = "default"; return x; }

	/* Shader macros*/
	std::string SHADER_VERSION_MACRO() { static const std::string x = "$B_SHADER_VERSION"; return x; }
	std::string SHADER_MAX_LIGHTS_MACRO() { static const std::string x = "$B_SHADER_MAX_LIGHTS"; return x; }

	/* Shader configuration */
	std::string DEFAULT_SHADER_VERSION_DESKTOP() { static const std::string x = "#version 120"; return x; }
	std::string DEFAULT_SHADER_VERSION_ES() { static const std::string x = "#version 100"; return x; }
	GLuint DEFAULT_SHADER_MAX_LIGHTS() { static const GLuint x = 3; return x; }
	std::string DEFAULT_FRAGMENT_SHADER_FILENAME_EXTENSION() { static const std::string x = ".frag"; return x; }
	std::string DEFAULT_VERTEX_SHADER_FILENAME_EXTENSION() { static const std::string x = ".vert"; return x; }

	/* Shader uniforms */
	std::string DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP() { static const std::string x = "DiffuseMap"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_SPECULAR_MAP() { static const std::string x = "SpecularMap"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_NORMAL_MAP() { static const std::string x = "NormalMap"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_NORMAL_MATRIX() { static const std::string x = "NormalMatrix"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_MODEL_MATRIX() { static const std::string x = "ModelMatrix"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_VIEW_MATRIX() { static const std::string x = "ViewMatrix"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX() { static const std::string x = "ModelViewMatrix"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX() { static const std::string x = "ProjectionMatrix"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS() { static const std::string x = "numLights"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE() { static const std::string x = "lightPositionViewSpace_"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_DIFFUSE_LIGHT_COLOR() { static const std::string x = "lightDiffuseColor_"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_SPECULAR_LIGHT_COLOR() { static const std::string x = "lightSpecularColor_"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY() { static const std::string x = "lightIntensity_"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION() { static const std::string x = "lightAttenuation_"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_LIGHT_RADIUS() { static const std::string x = "lightRadius_"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR() { static const std::string x = "ambientColor"; return x; }
	std::string DEFAULT_SHADER_UNIFORM_TRANSPARENCY() { static const std::string x = "transparency"; return x; }

	/* Shader attributes */
	std::string DEFAULT_SHADER_ATTRIBUTE_POSITION() { static const std::string x = "Position"; return x; }
	std::string DEFAULT_SHADER_ATTRIBUTE_NORMAL() { static const std::string x = "Normal"; return x; }
	std::string DEFAULT_SHADER_ATTRIBUTE_TANGENT() { static const std::string x = "Tangent"; return x; }
	std::string DEFAULT_SHADER_ATTRIBUTE_BITANGENT() { static const std::string x = "Bitangent"; return x; }
	std::string DEFAULT_SHADER_ATTRIBUTE_TEXCOORD() { static const std::string x = "TexCoord"; return x; }

	/* Material key words */
	std::string WAVEFRONT_MATERIAL_NEWMTL() { static const std::string x = "newmtl"; return x; }
	std::string WAVEFRONT_MATERIAL_AMBIENT_COLOR() { static const std::string x = "Ka"; return x; }
	std::string WAVEFRONT_MATERIAL_DIFFUSE_COLOR() { static const std::string x = "Kd"; return x; }
	std::string WAVEFRONT_MATERIAL_SPECULAR_COLOR() { static const std::string x = "Ks"; return x; }
	std::string WAVEFRONT_MATERIAL_TRANSMISSION_FILTER() { static const std::string x = "Tf"; return x; }
	std::string WAVEFRONT_MATERIAL_SPECULAR_EXPONENT() { static const std::string x = "Ns"; return x; }
	std::string WAVEFRONT_MATERIAL_OPTICAL_DENSITY() { static const std::string x = "Ni"; return x; }
	std::string WAVEFRONT_MATERIAL_ILLUMINATION_MODEL() { static const std::string x = "illum"; return x; }
	std::string WAVEFRONT_MATERIAL_DIFFUSE_MAP() { static const std::string x = "map_Kd"; return x; }
	std::string WAVEFRONT_MATERIAL_SPECULAR_MAP() { static const std::string x = "map_Ks"; return x; }
	std::string WAVEFRONT_MATERIAL_NORMAL_MAP() { static const std::string x = "map_Bump"; return x; }
	std::string WAVEFRONT_MATERIAL_DISSOLVE() { static const std::string x = "d"; return x; }

	/* Camera configuration */
	GLfloat DEFAULT_FIELD_OF_VIEW() { static const GLfloat x = 60.0f; return x; }
	GLfloat DEFAULT_ASPECT_RATIO() { static const GLfloat x = 4.0f / 3.0f; return x; }
	GLfloat DEFAULT_NEAR_CLIPPING_PLANE() { static const GLfloat x = -1.0f; return x; }
	GLfloat DEFAULT_FAR_CLIPPING_PLANE() { static const GLfloat x = 1.0f; return x; }
	vmml::Vector3f DEFAULT_CAMERA_POSITION() { static const vmml::Vector3f x = vmml::Vector3f(0.0f, 0.0f, 0.0f); return x; }
	vmml::Vector3f DEFAULT_CAMERA_ROTATION_AXES() { static const vmml::Vector3f x = vmml::Vector3f(0.0f, 0.0f, 0.0f); return x; }

	/* Light configuration */
	vmml::Vector4f DEFAULT_LIGHT_POSITION()	{ static const vmml::Vector4f x = vmml::Vector4f(0.0f, 0.0f, 0.0f, 1.0f); return x; }
	vmml::Vector3f DEFAULT_LIGHT_COLOR()	{ static const vmml::Vector3f x = vmml::Vector3f(1.0f, 1.0f, 1.0f); return x; }
	GLfloat DEFAULT_LIGHT_INTENSITY()		{ static const GLfloat x = 1000.0f; return x; }
	GLfloat DEFAULT_LIGHT_ATTENUATION()		{ static const GLfloat x = 1.0f; return x; }
	GLfloat DEFAULT_LIGHT_RADIUS()			{ static const GLfloat x = 10000.0f; return x; }

} // namespace bRenderer
