#include "../bRenderer.h"

namespace bRenderer
{
	/* Internal variables */
	
	typedef std::unordered_map< std::string, ShaderPtr >    Shaders;
	typedef std::unordered_map< std::string, TexturePtr >   Textures;
	typedef std::unordered_map< std::string, MaterialPtr >  Materials;
	typedef std::unordered_map< std::string, ModelPtr >     Models;

	Shaders     _shaders;
	Textures    _textures;
	Materials   _materials;
	Models      _models;

	std::string defaultMaterialName = "default";

	/* Internal functions */

	std::string getRawName(const std::string &fileName, std::string *ext = nullptr)
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

	ModelPtr createModel(const std::string &name, const ModelData &modelData)
	{
		ModelPtr &model = _models[name];
		if (model) return model;

		model = ModelPtr(new Model(modelData));
		return model;
	}

	TexturePtr createTexture(const std::string &name, const TextureData &textureData)
	{
		TexturePtr &texture = _textures[name];
		if (texture) return texture;

		texture = TexturePtr(new Texture(textureData));

		return texture;
	}

	ShaderPtr createShader(const std::string &name, const ShaderData &shaderData)
	{
		ShaderPtr &shader = _shaders[name];
		if (shader) return shader;

		if (shaderData.isValid())
		{
			log("Created shader '" + name + "'.", LM_INFO);
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

	/* External functions */

	void getWindowSize(GLint* width, GLint* height)
	{
		*width = getWindowWidth();
		*height = getWindowHeight();
	}

	GLfloat getAspectRatio(){
		return (GLfloat)getWindowWidth() / (GLfloat)getWindowHeight();
	}

	ModelPtr loadModel(const std::string &fileName, bool flipT, bool flipZ)
	{
		// log activity
		log("loading Model: " + fileName, LM_SYS);

		// get file name
		std::string name = getRawName(fileName);

		// create model
		ModelData modelData(fileName, flipT, flipZ);
		return createModel(name, modelData);
	}

	TexturePtr loadTexture(const std::string &fileName)
	{
		// get file name
		std::string name = getRawName(fileName);

		// create texture
		TextureData textureData(fileName);
		return createTexture(name, textureData);
	}

	ShaderPtr loadShader(const std::string &shaderName)
	{
		std::string name = getRawName(shaderName);

		ShaderData shaderData(shaderName);
		ShaderPtr shader = createShader(name, shaderData);
		if (shader) return shader;

		shader = loadShader(defaultMaterialName);
		if (shader) return shader;

		log("Couldn't load shader '" + name + "'.", LM_INFO);
		return nullptr;
	}

	MaterialPtr createMaterial(const std::string &name, const MaterialData &materialData)
	{
		MaterialPtr &material = _materials[name];
		if (material) return material;

		material = MaterialPtr(new Material);
		material->initialize(materialData, loadShader(name));
		return material;
	}

	ModelPtr getModel(const std::string &name)
	{
		return _models[name];
	}

	vmml::mat4f createPerspective(float fov, float aspect, float near, float far){
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

	vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up)
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

} // namespace bRenderer