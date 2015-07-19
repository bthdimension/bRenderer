#include "../headers/AssetManagement.h"
#include <boost/lexical_cast.hpp>
#include "headers/ShaderDataFile.h"
#include "headers/ShaderData.h"

using boost::lexical_cast;

/* Public functions */

void AssetManagement::setShaderVersionDesktop(const std::string &shaderVersionDesktop)
{
	_shaderVersionDesktop = shaderVersionDesktop;
}

void AssetManagement::setShaderVersionES(const std::string &shaderVersionES)
{
	_shaderVersionES = shaderVersionES;
}

void AssetManagement::setAmbientColor(const vmml::Vector3f &ambientColor)
{
	_ambientColor = ambientColor;
}

MaterialPtr AssetManagement::loadMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, bool diffuseLighting, bool specularLighting)
{
	// log activity
	bRenderer::log("loading Material: " + materialName, bRenderer::LM_SYS);

	if (getModel(materialName))
		return _materials[materialName];

	return createMaterial(materialName, OBJLoader::loadMaterial(fileName, materialName), loadShaderFile(shaderName.empty() ? materialName : shaderName, shaderMaxLights, variableNumberOfLights, ambientLighting, diffuseLighting, specularLighting));
}

MaterialPtr AssetManagement::loadMaterial(const std::string &fileName, const std::string &materialName, ShaderPtr shader)
{
	// log activity
	bRenderer::log("loading Material: " + materialName, bRenderer::LM_SYS);

	if (getModel(materialName))
		return _materials[materialName];

	return createMaterial(materialName, OBJLoader::loadMaterial(fileName, materialName), shader);
}

ModelPtr AssetManagement::loadModel(const std::string &fileName, bool flipT, bool flipZ, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);
	return createModel(name, modelData, shaderFromFile, shaderMaxLights, variableNumberOfLights, ambientLighting, properties);
}

ModelPtr AssetManagement::loadModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);

	return createModel(name, modelData, shader, properties);
}

ModelPtr AssetManagement::loadModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);
	return createModel(name, modelData, material, properties);
}

TexturePtr AssetManagement::loadTexture(const std::string &fileName)
{
	// get file name
	std::string name = getRawName(fileName);

	if (getTexture(name))
		return _textures[name];

	// create texture
	TextureData textureData(fileName);
	return createTexture(name, textureData);
}

ShaderPtr AssetManagement::loadShaderFile(std::string shaderName, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, bool diffuseLighting, bool specularLighting)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderDataFile shaderData(shaderName, _shaderVersionDesktop, _shaderVersionES, shaderMaxLights, variableNumberOfLights, ambientLighting, diffuseLighting, specularLighting);
	ShaderPtr shader = createShader(name, shaderData);
	if (shader) return shader;

	bRenderer::log("Couldn't load shader '" + name + "'.", bRenderer::LM_INFO);
	return nullptr;
}

ShaderPtr AssetManagement::generateShader(std::string shaderName, GLuint shaderMaxLights, bool ambientLighting, const MaterialData &materialData, bool variableNumberOfLights)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderData shaderData(shaderMaxLights, ambientLighting, materialData, variableNumberOfLights);
	return createShader(name, shaderData);
}

ShaderPtr AssetManagement::generateShader(std::string shaderName, GLuint shaderMaxLights, bool ambientLighting, bool diffuseLighting, bool specularLighting, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderData shaderData(shaderMaxLights, ambientLighting, diffuseLighting, specularLighting, ambientColor, diffuseColor, specularColor, diffuseMap, normalMap, specularMap, variableNumberOfLights);
	return createShader(name, shaderData);
}

MaterialPtr AssetManagement::createMaterial(const std::string &name, const MaterialData &materialData, ShaderPtr shader)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];

	material = MaterialPtr(new Material);
	material->initialize(this, materialData, shader);
	return material;
}

MaterialPtr AssetManagement::createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];
	ShaderPtr shader;

	if (shaderFromFile){
		bool diffuseColor = materialData.vectors.count(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR()) > 0;
		bool diffuseMap = materialData.textures.count(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()) > 0;
		bool diffuseLighting = diffuseMap || diffuseColor;
		bool specularLighting = shaderMaxLights > 0 && materialData.scalars.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT()) > 0;
		shader = loadShaderFile(name, shaderMaxLights, variableNumberOfLights, ambientLighting, diffuseLighting, specularLighting);
	}
	else{
		shader = generateShader(name, shaderMaxLights, ambientLighting, materialData, variableNumberOfLights);
	}
	material = MaterialPtr(new Material);
	material->initialize(this, materialData, shader);
	return material;
}

PropertiesPtr AssetManagement::createProperties(const std::string &name)
{
	if (getProperties(name)) return getProperties(name);
	PropertiesPtr &properties = _properties[name];

	properties = PropertiesPtr(new Properties);
	return properties;
}

ModelPtr AssetManagement::createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, bool ambientLighting, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(this, modelData, shaderMaxLights, variableNumberOfLights, shaderFromFile, ambientLighting, properties));
	return model;
}

ModelPtr AssetManagement::createModel(const std::string &name, const ModelData &modelData, ShaderPtr shader, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(this, modelData, shader, properties));
	return model;
}

ModelPtr AssetManagement::createModel(const std::string &name, const ModelData &modelData, MaterialPtr material, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(modelData, material, properties));
	return model;
}

TexturePtr AssetManagement::createTexture(const std::string &name, const TextureData &textureData)
{
	if (getTexture(name)) return getTexture(name);
	TexturePtr &texture = _textures[name];

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

TexturePtr AssetManagement::createTexture(const std::string &name, GLsizei width, GLsizei height, GLenum format, ImageDataPtr imageData)
{
	if (getTexture(name)) return getTexture(name);
	TexturePtr &texture = _textures[name];

	// create texture
	TextureData textureData(width, height, format, imageData);

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

ShaderPtr AssetManagement::createShader(const std::string &name, const IShaderData &shaderData)
{
	if (shaderData.isValid())
	{
		if (getShader(name)) return getShader(name);
		ShaderPtr &shader = _shaders[name];

		bRenderer::log("Created shader '" + name + "'.", bRenderer::LM_INFO);
		shader = ShaderPtr(new Shader(shaderData));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_POSITION(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_NORMAL(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_TANGENT(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_BITANGENT(), 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, bitangent));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_TEXCOORD(), 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
		return shader;
	}

	return nullptr;
}

CameraPtr AssetManagement::createCamera(const std::string &name)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera());
	return camera;
}

CameraPtr AssetManagement::createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(position, rotationAxes));
	return camera;
}

CameraPtr AssetManagement::createCamera(const std::string &name, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(fov, aspect, near, far));
	return camera;
}

CameraPtr AssetManagement::createCamera(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(position, rotationAxes, fov, aspect, near, far));
	return camera;
}

MatrixStackPtr AssetManagement::createMatrixStack(const std::string &name)
{
	if (getMatrixStack(name)) return getMatrixStack(name);
	MatrixStackPtr &matrixStack = _matrixStacks[name];

	matrixStack = MatrixStackPtr(new MatrixStack());
	return matrixStack;
}

LightPtr AssetManagement::createLight(const std::string &name)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light());
	return light;
}

LightPtr AssetManagement::createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, color));
	return light;
}

LightPtr AssetManagement::createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &color, GLfloat intensity, GLfloat attenuation)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, color, intensity, attenuation));
	return light;
}

LightPtr AssetManagement::createLight(const std::string &name, const vmml::Vector3f &position, const vmml::Vector3f &diffuseColor, const vmml::Vector3f &specularColor, GLfloat intensity, GLfloat attenuation)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, diffuseColor, specularColor, intensity, attenuation));
	return light;
}

FramebufferPtr AssetManagement::createFramebuffer(const std::string &name)
{
	if (getFramebuffer(name)) return getFramebuffer(name);
	FramebufferPtr &framebuffer = _framebuffers[name];
	framebuffer = FramebufferPtr(new Framebuffer());
	return framebuffer;
}

ShaderPtr AssetManagement::getShader(const std::string &name)
{
	if (_shaders.count(name) > 0)
		return _shaders[name];
	return nullptr;
}

TexturePtr AssetManagement::getTexture(const std::string &name)
{
	if (_textures.count(name) > 0)
		return _textures[name];
	return nullptr;
}

MaterialPtr AssetManagement::getMaterial(const std::string &name)
{
	if (_materials.count(name) > 0)
		return _materials[name];
	return nullptr;
}

PropertiesPtr AssetManagement::getProperties(const std::string &name)
{
	if (_properties.count(name) > 0)
		return _properties[name];
	return nullptr;
}

ModelPtr AssetManagement::getModel(const std::string &name)
{
	if (_models.count(name) > 0)
		return _models[name];
	return nullptr;
}

CameraPtr AssetManagement::getCamera(const std::string &name)
{
	if (_cameras.count(name) > 0)
		return _cameras[name];
	return nullptr;
}

MatrixStackPtr AssetManagement::getMatrixStack(const std::string &name)
{
	if (_matrixStacks.count(name) > 0)
		return _matrixStacks[name];
	return nullptr;
}

LightPtr AssetManagement::getLight(const std::string &name)
{
	if (_lights.count(name) > 0)
		return _lights[name];
	return nullptr;
}

FramebufferPtr AssetManagement::getFramebuffer(const std::string &name)
{
	if (_framebuffers.count(name) > 0)
		return _framebuffers[name];
	return nullptr;
}

std::string AssetManagement::getShaderVersionDesktop()
{
	return _shaderVersionDesktop;
}

std::string AssetManagement::getShaderVersionES()
{
	return _shaderVersionES;
}

vmml::Vector3f AssetManagement::getAmbientColor()
{
	return _ambientColor;
}

void AssetManagement::deleteShader(const std::string &name)
{
	_shaders.erase(name);
}

void AssetManagement::deleteTexture(const std::string &name)
{
	_textures.erase(name);
}

void AssetManagement::deleteMaterial(const std::string &name)
{
	_materials.erase(name);
}

void AssetManagement::deleteProperties(const std::string &name)
{
	_properties.erase(name);
}

void AssetManagement::deleteModel(const std::string &name)
{
	_models.erase(name);
}

void AssetManagement::deleteCamera(const std::string &name)
{
	_cameras.erase(name);
}

void AssetManagement::deleteMatrixStack(const std::string &name)
{
	_matrixStacks.erase(name);
}

void AssetManagement::deleteLight(const std::string &name)
{
	_lights.erase(name);
}

void AssetManagement::deleteFramebuffer(const std::string &name)
{
	_framebuffers.erase(name);
}

void AssetManagement::clear()
{
	_shaders.clear();
	_textures.clear();
	_materials.clear();
	_properties.clear();
	_models.clear();
	_cameras.clear();
	_matrixStacks.clear();
	_lights.clear();
	_framebuffers.clear();

	_ambientColor = bRenderer::DEFAULT_AMBIENT_COLOR();

	_shaderVersionDesktop = bRenderer::DEFAULT_SHADER_VERSION_DESKTOP();
	_shaderVersionES = bRenderer::DEFAULT_SHADER_VERSION_ES();
}

/* Private functions */

std::string AssetManagement::getRawName(const std::string &fileName, std::string *ext)
{
	std::string rawName = fileName;
	int indexSlash = rawName.find_last_of("/\\");

	if (indexSlash != std::string::npos)
	{
		rawName = rawName.substr(indexSlash + 1);
	}

	int indexDot = rawName.find_last_of(".");
	if (indexDot != std::string::npos)
	{
		if (ext) *ext = rawName.substr(indexDot + 1, std::string::npos);
		return rawName.substr(0, indexDot);
	}
	return rawName;
}
