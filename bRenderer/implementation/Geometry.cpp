#include "../headers/Geometry.h"

/* Public functions */

void Geometry::initialize(GeometryDataPtr geometryData)
{
    copyVertexData(geometryData->vboVertices);
    copyIndexData(geometryData->vboIndices);
    initializeVertexBuffer();
}

void Geometry::draw(GLenum mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

	if (_material)
		_material->bind();

	if (_properties)
		_properties->passToShader(_material->getShader());

	glDrawElements(mode, _nIndices, GL_UNSIGNED_SHORT, _indexData.get());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/* Private functions */

Geometry::VertexDataPtr Geometry::allocVertexData(size_t nVertices)
{
    _nVertices  = nVertices;
    _vertexData = VertexDataPtr(new Vertex[_nVertices]);
    
    return _vertexData;
}

Geometry::IndexDataPtr Geometry::allocIndexData(size_t nIndices)
{
    _nIndices  = nIndices;
    _indexData = IndexDataPtr(new GLushort[_nIndices]);
    
    return _indexData;
}

Geometry::VertexDataPtr Geometry::copyVertexData(const GeometryData::VboVertices &arg)
{
	if (arg.size() > 0){
		allocVertexData(arg.size());
		memcpy(_vertexData.get(), &arg[0], sizeof(Vertex) * _nVertices);
	}
    return _vertexData;
}

Geometry::IndexDataPtr Geometry::copyIndexData(const GeometryData::VboIndices &arg)
{
	if (arg.size() > 0){
		allocIndexData(arg.size());
		memcpy(_indexData.get(), &arg[0], sizeof(Index) * _nIndices);
	}
    return _indexData;
}

void Geometry::initializeVertexBuffer()
{
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _nVertices*sizeof(Vertex), _vertexData.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

