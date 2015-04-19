//
//  Material.cpp
//  Framework
//
//  Created by David Steiner on 4/28/14.
//
//

#include "../headers/Material.h"
#include "bRenderer.h"

void Material::initialize(const MaterialData &materialData, ShaderPtr shader)
{
    for (auto i = materialData.textures.cbegin(); i != materialData.textures.cend(); ++i)
    {
        const std::string &texName = i->first;
        const std::string &texFileName = i->second;
        TexturePtr texture = bRenderer::loadTexture(texFileName);
        setTexture(texName, texture);
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
