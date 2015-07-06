#include "../headers/Renderer.h"
#include "../headers/IRenderProject.h"
#include <boost/lexical_cast.hpp>
#include "headers/ShaderDataFile.h"
#include "headers/ShaderData.h"

using boost::lexical_cast;

/* Private constructor */
Renderer::Renderer()
{
	reset();
}

/* Public functions */

View *Renderer::getView()
{
	return &_view;
}

bool Renderer::isInitialized()
{
	return _initialized;
}

void Renderer::setRenderProject(IRenderProject *p)
{
	_renderProject = p;
}

void Renderer::setInitFunction(void(*f)())
{
	_initFunction = f;
}

void Renderer::setLoopFunction(void(*f)(const double deltaTime, const double elapsedTime))
{
	_loopFunction = f;
}

void Renderer::setTerminateFunction(void(*f)())
{
	_terminateFunction = f;
}

void Renderer::setShaderVersionDesktop(const std::string &shaderVersionDesktop)
{
	_shaderVersionDesktop = shaderVersionDesktop;
}

void Renderer::setShaderVersionES(const std::string &shaderVersionES)
{
	_shaderVersionES = shaderVersionES;
}

void Renderer::setAmbientColor(const vmml::vec3f &ambientColor)
{
	_ambientColor = ambientColor;
}

bool Renderer::initRenderer()
{
    if (_initialized)
        return true;
    
    // initialize view
    if (!_view.initView())
        return false;
    
	// OpenGL
	// clear
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// for Alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // call static function if set
    if (_initFunction)
        _initFunction();
    
    // call member function if set
    if (_renderProject)
        _renderProject->initFunction();

	_initialized = true;
    
    return true;
}

bool Renderer::initRenderer(bool fullscreen)
{
    if (_initialized)
        return true;
    
    // initialize view
    if (!_view.initView(fullscreen))
        return false;
    
    return initRenderer();
}

bool Renderer::initRenderer(GLint width, GLint height, bool fullscreen)
{
    if (_initialized)
        return true;
    
    // initialize view
    if (!_view.initView(width, height, fullscreen))
        return false;
    
    return initRenderer();
}

MaterialPtr Renderer::loadMaterial(const std::string &fileName, const std::string &materialName, const std::string &shaderName, GLuint shaderMaxLights, bool variableNumberOfLights)
{
	// log activity
	bRenderer::log("loading Material: " + materialName, bRenderer::LM_SYS);

	if (getModel(materialName))
		return _materials[materialName];

	return createMaterial(materialName, OBJLoader::loadMaterial(fileName, materialName), loadShaderFile(shaderName.empty() ? materialName : shaderName, shaderMaxLights, variableNumberOfLights));
}

MaterialPtr Renderer::loadMaterial(const std::string &fileName, const std::string &materialName, ShaderPtr shader)
{
	// log activity
	bRenderer::log("loading Material: " + materialName, bRenderer::LM_SYS);

	if (getModel(materialName))
		return _materials[materialName];

	return createMaterial(materialName, OBJLoader::loadMaterial(fileName, materialName), shader);
}

ModelPtr Renderer::loadModel(const std::string &fileName, bool flipT, bool flipZ, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, PropertiesPtr properties)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	if (getModel(name))
		return _models[name];

	// create model
	ModelData modelData(fileName, flipT, flipZ);
	return createModel(name, modelData, shaderFromFile, shaderMaxLights, variableNumberOfLights, properties);
}

ModelPtr Renderer::loadModel(const std::string &fileName, bool flipT, bool flipZ, ShaderPtr shader, PropertiesPtr properties)
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

ModelPtr Renderer::loadModel(const std::string &fileName, bool flipT, bool flipZ, MaterialPtr material, PropertiesPtr properties)
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

TexturePtr Renderer::loadTexture(const std::string &fileName)
{
	// get file name
	std::string name = getRawName(fileName);

	if (getTexture(name))
		return _textures[name];

	// create texture
	TextureData textureData(fileName);
	return createTexture(name, textureData);
}

ShaderPtr Renderer::loadShaderFile(std::string shaderName, GLuint shaderMaxLights, bool variableNumberOfLights)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderDataFile shaderData(shaderName, _shaderVersionDesktop, _shaderVersionES, shaderMaxLights, variableNumberOfLights);
	ShaderPtr shader = createShader(name, shaderData);
	if (shader) return shader;

	bRenderer::log("Couldn't load shader '" + name + "'.", bRenderer::LM_INFO);
	return nullptr;
}

ShaderPtr Renderer::loadShader(std::string shaderName, GLuint shaderMaxLights, bool ambientColor, bool diffuseColor, bool specularColor, bool diffuseMap, bool normalMap, bool specularMap, bool variableNumberOfLights)
{
	std::string name = getRawName(shaderName);

	if (getShader(name))
		return _shaders[name];

	ShaderData shaderData(shaderMaxLights, ambientColor, diffuseColor, specularColor, diffuseMap, normalMap, specularMap, variableNumberOfLights);
	return createShader(name, shaderData);
}

MaterialPtr Renderer::createMaterial(const std::string &name, const MaterialData &materialData, ShaderPtr shader)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];

	material = MaterialPtr(new Material);
	material->initialize(this, materialData, shader);
	return material;
}

MaterialPtr Renderer::createMaterialShaderCombination(const std::string &name, const MaterialData &materialData, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights)
{
	if (getMaterial(name)) return getMaterial(name);
	MaterialPtr &material = _materials[name];
	ShaderPtr shader;
	
	if (shaderFromFile)
		shader = loadShaderFile(name, shaderMaxLights, variableNumberOfLights);
	else{
		// create shader fitting the needs of the material
		bool ambientColor = materialData.vectors.count(bRenderer::WAVEFRONT_MATERIAL_AMBIENT_COLOR) > 0;
		bool diffuseColor = materialData.vectors.count(bRenderer::WAVEFRONT_MATERIAL_DIFFUSE_COLOR) > 0;
		bool specularColor = materialData.vectors.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_COLOR) > 0 && materialData.scalars.count(bRenderer::WAVEFRONT_MATERIAL_SPECULAR_EXPONENT) > 0;
		bool diffuseMap = materialData.textures.count(bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP) > 0;
		bool normalMap = materialData.textures.count(bRenderer::DEFAULT_SHADER_UNIFORM_NORMAL_MAP) > 0;
		bool specularMap = materialData.textures.count(bRenderer::DEFAULT_SHADER_UNIFORM_SPECULAR_MAP) > 0;
		shader = loadShader(name, shaderMaxLights, ambientColor, diffuseColor, specularColor, diffuseMap, normalMap, specularMap, variableNumberOfLights);
	}
	material = MaterialPtr(new Material);
	material->initialize(this, materialData, shader);
	return material;
}

PropertiesPtr Renderer::createProperties(const std::string &name)
{
	if (getProperties(name)) return getProperties(name);
	PropertiesPtr &properties = _properties[name];

	properties = PropertiesPtr(new Properties);
	return properties;
}

ModelPtr Renderer::createModel(const std::string &name, const ModelData &modelData, bool shaderFromFile, GLuint shaderMaxLights, bool variableNumberOfLights, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(this, modelData, shaderMaxLights, variableNumberOfLights, shaderFromFile, properties));
	return model;
}

ModelPtr Renderer::createModel(const std::string &name, const ModelData &modelData, ShaderPtr shader, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(this, modelData, shader, properties));
	return model;
}

ModelPtr Renderer::createModel(const std::string &name, const ModelData &modelData, MaterialPtr material, PropertiesPtr properties)
{
	if (getModel(name)) return getModel(name);
	ModelPtr &model = _models[name];

	model = ModelPtr(new Model(modelData, material, properties));
	return model;
}

TexturePtr Renderer::createTexture(const std::string &name, const TextureData &textureData)
{
	if (getTexture(name)) return getTexture(name);
	TexturePtr &texture = _textures[name];

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

TexturePtr Renderer::createTexture(const std::string &name, GLsizei width, GLsizei height, GLenum format, ImageDataPtr imageData)
{
	if (getTexture(name)) return getTexture(name);
	TexturePtr &texture = _textures[name];

	// create texture
	TextureData textureData(width, height, format, imageData);

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

ShaderPtr Renderer::createShader(const std::string &name, const IShaderData &shaderData)
{
	if (shaderData.isValid())
	{
		if (getShader(name)) return getShader(name);
		ShaderPtr &shader = _shaders[name];

		bRenderer::log("Created shader '" + name + "'.", bRenderer::LM_INFO);
		shader = ShaderPtr(new Shader(shaderData));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_POSITION, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_NORMAL, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_TANGENT, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, bitangent));
		shader->registerAttrib(bRenderer::DEFAULT_SHADER_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
		return shader;
	}

	return nullptr;
}

CameraPtr Renderer::createCamera(const std::string &name)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera());
	return camera;
}

CameraPtr Renderer::createCamera(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &rotationAxes)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(position, rotationAxes));
	return camera;
}

CameraPtr Renderer::createCamera(const std::string &name, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(fov, aspect, near, far));
	return camera;
}

CameraPtr Renderer::createCamera(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	if (getCamera(name)) return getCamera(name);
	CameraPtr &camera = _cameras[name];

	camera = CameraPtr(new Camera(position, rotationAxes, fov, aspect, near, far));
	return camera;
}

MatrixStackPtr Renderer::createMatrixStack(const std::string &name)
{
	if (getMatrixStack(name)) return getMatrixStack(name);
	MatrixStackPtr &matrixStack = _matrixStacks[name];

	matrixStack = MatrixStackPtr(new MatrixStack());
	return matrixStack;
}

LightPtr Renderer::createLight(const std::string &name)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light());
	return light;
}

LightPtr Renderer::createLight(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &color)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, color));
	return light;
}

LightPtr Renderer::createLight(const std::string &name, const vmml::vec3f &position, const vmml::vec3f &color, GLfloat intensity, GLfloat attenuation)
{
	if (getLight(name)) return getLight(name);
	LightPtr &light = _lights[name];

	light = LightPtr(new Light(position, color, intensity, attenuation));
	return light;
}

FramebufferPtr Renderer::createFramebuffer(const std::string &name)
{
	if (getFramebuffer(name)) return getFramebuffer(name);
	FramebufferPtr &framebuffer = _framebuffers[name];
	framebuffer = FramebufferPtr(new Framebuffer());
	return framebuffer;
}

void Renderer::drawModel(const std::string &modelName, const std::string &cameraName, const vmml::mat4f & modelMatrix, bool ambient)
{
	std::vector<std::string> lightNames;
	for (auto i = _lights.begin(); i != _lights.end(); ++i)
		lightNames.push_back(i->first);
	drawModel(modelName, cameraName, modelMatrix, lightNames, ambient);
}

void Renderer::drawModel(const std::string &modelName, const std::string &cameraName, const vmml::mat4f & modelMatrix, const std::vector<std::string> &lightNames, bool ambient)
{
	drawModel(modelName, modelMatrix, getCamera(cameraName)->getViewMatrix(), getCamera(cameraName)->getProjectionMatrix(), lightNames, ambient);
}

void Renderer::drawModel(const std::string &modelName, const vmml::mat4f &modelMatrix, const vmml::mat4f &viewMatrix, const vmml::mat4f &projectionMatrix, const std::vector<std::string> &lightNames, bool ambient)
{
	Model::GroupMap &groupsCaveStart = getModel(modelName)->getGroups();
	for (auto i = groupsCaveStart.begin(); i != groupsCaveStart.end(); ++i)
	{
		Geometry &geometry = i->second;
		ShaderPtr shader = geometry.getMaterial()->getShader();
		if (shader)
		{
			shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_PROJECTION_MATRIX, projectionMatrix);
			shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_MODEL_VIEW_MATRIX, viewMatrix*modelMatrix);

			// Light
			GLfloat numLights = lightNames.size();
			bool variableNumberOfLights = shader->hasVariableNumberOfLights();
			GLuint maxLights = shader->getMaxLights();
			if (numLights > maxLights)
				numLights = maxLights;

			if (variableNumberOfLights)
				shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_NUMBER_OF_LIGHTS, numLights);
			for (int i = 0; i < numLights; i++){
				std::string pos = lexical_cast< std::string >(i);
				shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_POSITION_VIEW_SPACE + pos, (viewMatrix*getLight(lightNames[i])->getPosition()));
				shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_COLOR + pos, getLight(lightNames[i])->getColor());
				shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_INTENSITY + pos, getLight(lightNames[i])->getIntensity());
				shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_LIGHT_ATTENUATION + pos, getLight(lightNames[i])->getAttenuation());
			}
			// ambient
			if (ambient)
				shader->setUniform(bRenderer::DEFAULT_SHADER_UNIFORM_AMBIENT_COLOR, getAmbientColor());
		}
		else
		{
			bRenderer::log("No shader available.", bRenderer::LM_WARNING);
		}
		geometry.draw();
	}
}

ShaderPtr Renderer::getShader(const std::string &name)
{
	if (_shaders.count(name) > 0)
		return _shaders[name];
	return nullptr;
}

TexturePtr Renderer::getTexture(const std::string &name)
{
	if (_textures.count(name) > 0)
		return _textures[name];
	return nullptr;
}

MaterialPtr Renderer::getMaterial(const std::string &name)
{
	if (_materials.count(name) > 0)
		return _materials[name];
	return nullptr;
}

PropertiesPtr Renderer::getProperties(const std::string &name)
{
	if (_properties.count(name) > 0)
		return _properties[name];
	return nullptr;
}

ModelPtr Renderer::getModel(const std::string &name)
{
	if (_models.count(name) > 0)
		return _models[name];
	return nullptr;
}

CameraPtr Renderer::getCamera(const std::string &name)
{
	if (_cameras.count(name) > 0)
		return _cameras[name];
	return nullptr;
}

MatrixStackPtr Renderer::getMatrixStack(const std::string &name)
{
	if (_matrixStacks.count(name) > 0)
		return _matrixStacks[name];
	return nullptr;
}

LightPtr Renderer::getLight(const std::string &name)
{
	if (_lights.count(name) > 0)
		return _lights[name];
	return nullptr;
}

FramebufferPtr Renderer::getFramebuffer(const std::string &name)
{
	if (_framebuffers.count(name) > 0)
		return _framebuffers[name];
	return nullptr;
}

std::string Renderer::getShaderVersionDesktop()
{
	return _shaderVersionDesktop;
}

std::string Renderer::getShaderVersionES()
{
	return _shaderVersionES;
}

vmml::vec3f Renderer::getAmbientColor()
{
	return _ambientColor;
}

vmml::mat4f Renderer::createPerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	return Camera::createPerspective(fov, aspect, near, far);
}

vmml::mat4f Renderer::lookAt(const vmml::vec3f &eye, const vmml::vec3f &target, const vmml::vec3f &up)
{
	return Camera::lookAt(eye, target, up);
}

/* Private functions */

std::string Renderer::getRawName(const std::string &fileName, std::string *ext)
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

void Renderer::reset()
{
	_stopTime = 0;
	_initialTime = 0;
	_elapsedTime = 0;
	_ambientColor = bRenderer::DEFAULT_AMBIENT_COLOR;
	_shaderVersionDesktop = bRenderer::DEFAULT_SHADER_VERSION_DESKTOP;
	_shaderVersionES = bRenderer::DEFAULT_SHADER_VERSION_ES;

	_shaders.clear();
	_textures.clear();
	_materials.clear();
	_models.clear();
	_cameras.clear();
	_matrixStacks.clear();
	_lights.clear();
}