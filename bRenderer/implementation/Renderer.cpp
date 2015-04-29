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

void Renderer::setShaderVersionDesktop(std::string shaderVersionDesktop)
{
	_shaderVersionDesktop = shaderVersionDesktop;
}

void Renderer::setShaderVersionES(std::string shaderVersionES)
{
	_shaderVersionES = shaderVersionES;
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

	ShaderData shaderData(shaderName, _shaderVersionDesktop, _shaderVersionES);
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

CameraPtr Renderer::createCamera(const std::string &name)
{
	CameraPtr &camera = _cameras[name];
	if (camera) return camera;

	camera = CameraPtr(new Camera());
	return camera;
}

CameraPtr Renderer::createCamera(const std::string &name, vmml::vec3f position, vmml::vec3f orientation)
{
	CameraPtr &camera = _cameras[name];
	if (camera) return camera;

	camera = CameraPtr(new Camera(position, orientation));
	return camera;
}

CameraPtr Renderer::createCamera(const std::string &name, vmml::vec3f position, vmml::vec3f orientation, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	CameraPtr &camera = _cameras[name];
	if (camera) return camera;

	camera = CameraPtr(new Camera(position, orientation, fov, aspect, near, far));
	return camera;
}

MatrixStackPtr Renderer::createMatrixStack(const std::string &name)
{
	MatrixStackPtr &matrixStack = _matrixStacks[name];
	if (matrixStack) return matrixStack;

	matrixStack = MatrixStackPtr(new MatrixStack());
	return matrixStack;
}

ModelPtr Renderer::getModel(const std::string &name)
{
	return _models[name];
}

CameraPtr Renderer::getCamera(const std::string &name)
{
	return _cameras[name];
}

MatrixStackPtr Renderer::getMatrixStack(const std::string &name)
{
	return _matrixStacks[name];
}

vmml::mat4f Renderer::createPerspective(float fov, float aspect, float near, float far)
{
	return Camera::createPerspective(fov, aspect, near, far);
}

vmml::mat4f Renderer::lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up)
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

