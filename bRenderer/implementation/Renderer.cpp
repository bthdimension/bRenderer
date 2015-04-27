#include "../headers/Renderer.h"
#include "../headers/RenderProject.h"

/* Public functions */

View *Renderer::getView()
{
	return &_view;
}

bool Renderer::isInitialized()
{
	return _initialized;
}

void Renderer::setRenderProject(RenderProject *p)
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

bool Renderer::initRenderer()
{
    if (_initialized)
        return true;
    
    // initialize view
    if (!_view.initView())
        return false;
    
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

ModelPtr Renderer::loadModel(const std::string &fileName, bool flipT, bool flipZ)
{
	// log activity
	bRenderer::log("loading Model: " + fileName, bRenderer::LM_SYS);

	// get file name
	std::string name = getRawName(fileName);

	// create model
	ModelData modelData(fileName, flipT, flipZ);
	return createModel(name, modelData);
}

TexturePtr Renderer::loadTexture(const std::string &fileName)
{
	// get file name
	std::string name = getRawName(fileName);

	// create texture
	TextureData textureData(fileName);
	return createTexture(name, textureData);
}

ShaderPtr Renderer::loadShader(const std::string &shaderName)
{
	std::string name = getRawName(shaderName);

	ShaderData shaderData(shaderName);
	ShaderPtr shader = createShader(name, shaderData);
	if (shader) return shader;

	shader = loadShader(_defaultMaterialName);
	if (shader) return shader;

	bRenderer::log("Couldn't load shader '" + name + "'.", bRenderer::LM_INFO);
	return nullptr;
}

MaterialPtr Renderer::createMaterial(const std::string &name, const MaterialData &materialData)
{
	MaterialPtr &material = _materials[name];
	if (material) return material;

	material = MaterialPtr(new Material);
	material->initialize(this, materialData, loadShader(name));
	return material;
}

ModelPtr Renderer::getModel(const std::string &name)
{
	return _models[name];
}

vmml::mat4f Renderer::createPerspective(float fov, float aspect, float near, float far)
{
	vmml::mat4f perspective = vmml::mat4f::IDENTITY;

	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			perspective.at(i, j) = 0.0f;
		}
	}

	float angle = (fov / 180.0f) * M_PI_F;
	float f = 1.0f / tan(angle * 0.5f);

	perspective.at(0, 0) = f / aspect;
	perspective.at(1, 1) = f;
	perspective.at(2, 2) = (far + near) / (near - far);
	perspective.at(2, 3) = -1.0f;
	perspective.at(3, 2) = (2.0f * far*near) / (near - far);


	return perspective;
}

vmml::mat4f Renderer::lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up)
{
	vmml::vec3f zaxis = vmml::normalize(eye - target);
	vmml::vec3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
	vmml::vec3f yaxis = vmml::cross<3>(zaxis, xaxis);

	vmml::mat4f view;
	view.set_row(0, vmml::vec4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, eye)));
	view.set_row(1, vmml::vec4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, eye)));
	view.set_row(2, vmml::vec4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, eye)));
	view.set_row(3, vmml::vec4f(0, 0, 0, 1.0));

	return view;
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

ModelPtr Renderer::createModel(const std::string &name, const ModelData &modelData)
{
	ModelPtr &model = _models[name];
	if (model) return model;

	model = ModelPtr(new Model(this, modelData));
	return model;
}

TexturePtr Renderer::createTexture(const std::string &name, const TextureData &textureData)
{
	TexturePtr &texture = _textures[name];
	if (texture) return texture;

	texture = TexturePtr(new Texture(textureData));

	return texture;
}

ShaderPtr Renderer::createShader(const std::string &name, const ShaderData &shaderData)
{
	ShaderPtr &shader = _shaders[name];
	if (shader) return shader;

	if (shaderData.isValid())
	{
		bRenderer::log("Created shader '" + name + "'.", bRenderer::LM_INFO);
		shader = ShaderPtr(new Shader(shaderData));
		shader->registerAttrib("Position", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
		shader->registerAttrib("Normal", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal));
		shader->registerAttrib("Tangent", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent));
		shader->registerAttrib("Bitangent", 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, bitangent));
		shader->registerAttrib("TexCoord", 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texCoord));
		return shader;
	}

	return nullptr;
}