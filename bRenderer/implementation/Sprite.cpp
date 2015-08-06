#include "headers/Sprite.h"
#include "headers/Configuration.h"
#include "headers/ResourceManager.h"

Sprite::Sprite(MaterialPtr material, PropertiesPtr	properties)
{
	createGeometry();

	setMaterial(material);
	setProperties(properties);
}

Sprite::Sprite(ResourceManager *r, const std::string &textureFileName, const std::string &materialName, ShaderPtr shader, PropertiesPtr	properties)
{
	createGeometry();

	MaterialData md; 
	md.textures[bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()] = textureFileName;

	MaterialPtr material = r->createMaterial(materialName, md, shader);

	setMaterial(material);
	setProperties(properties);
}

Sprite::Sprite(ResourceManager *r, const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights, bool variableNumberOfLights)
{
	createGeometry();

	MaterialData md;
	md.textures[bRenderer::DEFAULT_SHADER_UNIFORM_DIFFUSE_MAP()] = textureFileName;

	ShaderPtr shader = r->generateShader(name, shaderMaxLights, false, md, variableNumberOfLights);

	MaterialPtr material = r->createMaterial(name, md, shader);

	setMaterial(material);
}

void Sprite::createGeometry()
{
	Model::GroupMap &groups = getGroups();
	GeometryPtr g = GeometryPtr(new Geometry);
	groups.insert(std::pair< std::string, GeometryPtr >(bRenderer::DEFAULT_GROUP_NAME(), g));

	GeometryDataPtr gData = GeometryDataPtr(new GeometryData);
	// Add vertices
	gData->vboVertices.push_back(Vertex(
		1.0f, 1.0f, 0.0f,		// position
		0.0f, 0.0f, -1.0f,		// normal
		-1.0f, 0.0f, 0.0f,		// tangent
		0.0f, 1.0f, 0.0f,		// bitangent
		1.0f, 1.0f				// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		-1.0f, 1.0f, 0.0f, 		// position
		0.0f, 0.0f, -1.0f,		// normal
		-1.0f, 0.0f, 0.0f,		// tangent
		0.0f, 1.0f, 0.0f,		// bitangent
		0.0f, 1.0f				// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		-1.0f, -1.0f, 0.0f,		// position
		0.0f, 0.0f, -1.0f,		// normal
		-1.0f, 0.0f, 0.0f,		// tangent
		0.0f, 1.0f, 0.0f,		// bitangent
		0.0f, 0.0f				// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		1.0f, -1.0f, 0.0f,		// position
		0.0f, 0.0f, -1.0f,		// normal
		-1.0f, 0.0f, 0.0f,		// tangent
		0.0f, 1.0f, 0.0f,		// bitangent
		1.0f, 0.0f				// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		1.0f, 1.0f, 0.0f,		// position
		0.0f, 0.0f, -1.0f,		// normal
		-1.0f, 0.0f, 0.0f,		// tangent
		0.0f, 1.0f, 0.0f,		// bitangent
		1.0f, 1.0f				// texCoord
	));
	gData->vboVertices.push_back(Vertex(
		-1.0f, -1.0f, 0.0f,		// position
		0.0f, 0.0f, -1.0f,		// normal
		-1.0f, 0.0f, 0.0f,		// tangent
		0.0f, 1.0f, 0.0f,		// bitangent
		0.0f, 0.0f				// texCoord
	));

	// Add indices
	for (GLushort i = 0; i < 6; i++)
		gData->vboIndices.push_back(i);

	// Add index data
	gData->indices.push_back(IndexData(3, 2, 0));
	gData->indices.push_back(IndexData(2, 1, 0));
	gData->indices.push_back(IndexData(0, 0, 0));
	gData->indices.push_back(IndexData(1, 5, 0));
	gData->indices.push_back(IndexData(3, 4, 0));
	gData->indices.push_back(IndexData(0, 3, 0));

	g->initialize(gData);

	setBoundingBoxObjectSpace(g->getBoundingBoxObjectSpace());
}