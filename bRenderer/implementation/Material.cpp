#include "headers/Material.h"
#include "headers/ResourceManager.h"
#include "headers/Configuration.h"

void Material::initialize(ResourceManager *r, const MaterialData &materialData, ShaderPtr shader)
{
    for (auto i = materialData.textures.cbegin(); i != materialData.textures.cend(); ++i)
    {
        const std::string &texName = i->first;
        const std::string &texFileName = i->second;
        TexturePtr texture = r->loadTexture(texFileName);
        setTexture(texName, texture);
    }
	if (materialData.cubeTextures.size() >= 6)
	{
		TexturePtr cubeMap = r->loadCubeMap(materialData.name + bRenderer::DEFAULT_SHADER_UNIFORM_CUBE_MAP(), std::vector<std::string>({ 
			materialData.cubeTextures.at(bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_LEFT()), 
			materialData.cubeTextures.at(bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_RIGHT()),
			materialData.cubeTextures.at(bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_BOTTOM()),
			materialData.cubeTextures.at(bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_TOP()),
			materialData.cubeTextures.at(bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_FRONT()),
			materialData.cubeTextures.at(bRenderer::WAVEFRONT_MATERIAL_REFLECTION_TYPE_CUBE_BACK()),
		}));
		setTexture(bRenderer::DEFAULT_SHADER_UNIFORM_CUBE_MAP(), cubeMap);
	}
    setVectors(materialData.vectors);
    setScalars(materialData.scalars);
    setName(materialData.name);
    setShader(shader);
}

void Material::bind()
{
    _shader->bind();
    _shader->setUniforms(_textures);
    _shader->setUniforms(_vectors);
    _shader->setUniforms(_scalars);
}
