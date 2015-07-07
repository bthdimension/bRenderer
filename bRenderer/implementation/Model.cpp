#include "../headers/Model.h"
#include "../headers/ModelData.h"
#include "../headers/TextureData.h"
#include "../headers/Renderer.h"

Model::Model(Renderer *r, const ModelData &modelData, GLuint shaderMaxLights, bool variableNumberOfLights, bool shaderFromFile, bool ambientLighting, PropertiesPtr properties)
{
	ModelData::GroupMap data = modelData.getData();
	_properties = properties;

	for (auto i = data.begin(); i != data.end(); ++i)
	{
		Geometry &g = _groups[i->first];
		GeometryDataPtr gData = i->second;

		MaterialPtr material = r->createMaterialShaderCombination(gData->materialData.name, gData->materialData, shaderFromFile, shaderMaxLights, variableNumberOfLights, ambientLighting);
		g.initialize(gData);
		g.setMaterial(material);
		g.setProperties(properties);
	}
}

Model::Model(Renderer *r, const ModelData &modelData, ShaderPtr shader, PropertiesPtr properties)
{
    ModelData::GroupMap data = modelData.getData();
	_properties = properties;
    for (auto i = data.begin(); i != data.end(); ++i)
    {
        Geometry &g = _groups[i->first];
        GeometryDataPtr gData = i->second;
		MaterialPtr material = r->createMaterial(gData->materialData.name, gData->materialData, shader);
        g.initialize(gData);
        g.setMaterial(material);
		g.setProperties(properties);
    }
}

Model::Model(const ModelData &modelData, MaterialPtr material, PropertiesPtr properties)
{
	ModelData::GroupMap data = modelData.getData();
	_material = material;
	_properties = properties;
	for (auto i = data.begin(); i != data.end(); ++i)
	{
		Geometry &g = _groups[i->first];
		GeometryDataPtr gData = i->second;
		g.initialize(gData);
		g.setMaterial(material);
		g.setProperties(properties);
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
