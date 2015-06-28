#include "../headers/Model.h"
#include "../headers/ModelData.h"
#include "../headers/TextureData.h"
#include "../headers/Renderer.h"

Model::Model(Renderer *r, const ModelData &modelData, GLuint shaderMaxLights, bool shaderFromFile)
{
	ModelData::GroupMap data = modelData.getData();

	for (auto i = data.begin(); i != data.end(); ++i)
	{
		Geometry &g = _groups[i->first];
		GeometryDataPtr gData = i->second;

		MaterialPtr material = r->createMaterialShaderCombination(gData->materialData.name, gData->materialData, shaderFromFile, shaderMaxLights);
		g.initialize(gData);
		g.setMaterial(material);
	}
}

Model::Model(Renderer *r, const ModelData &modelData, ShaderPtr shader)
{
    ModelData::GroupMap data = modelData.getData();

    for (auto i = data.begin(); i != data.end(); ++i)
    {
        Geometry &g = _groups[i->first];
        GeometryDataPtr gData = i->second;
		MaterialPtr material = r->createMaterial(gData->materialData.name, gData->materialData, shader);
        g.initialize(gData);
        g.setMaterial(material);
    }
}

Model::Model(const ModelData &modelData, MaterialPtr material)
{
	ModelData::GroupMap data = modelData.getData();

	for (auto i = data.begin(); i != data.end(); ++i)
	{
		Geometry &g = _groups[i->first];
		GeometryDataPtr gData = i->second;
		g.initialize(gData);
		g.setMaterial(material);
	}
}

Model::~Model()
{}

void Model::draw(GLenum mode)
{
    for (auto i = _groups.begin(); i != _groups.end(); ++i)
    {
        i->second.draw(mode);
    }
}

void Model::draw(const std::string &groupName, GLenum mode)
{
    _groups[groupName].draw(mode);
}
