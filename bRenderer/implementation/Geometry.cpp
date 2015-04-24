//
//  Geometry.cpp
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#include "../headers/Geometry.h"


void Geometry::initialize(GeometryDataPtr geometryData)
{
    copyVertexData(geometryData->vboVertices);
    copyIndexData(geometryData->vboIndices);
    initializeVertexBuffer();
}

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
    allocVertexData(arg.size());
    memcpy(_vertexData.get(), &arg[0], sizeof(Vertex) * _nVertices);
    return _vertexData;
}

Geometry::IndexDataPtr Geometry::copyIndexData(const GeometryData::VboIndices &arg)
{
    allocIndexData(arg.size());
    memcpy(_indexData.get(), &arg[0], sizeof(Index) * _nIndices);
    return _indexData;
}

void Geometry::initializeVertexBuffer()
{
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _nVertices*sizeof(Vertex), _vertexData.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::draw(GLenum mode)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    
    if (_material) 
		_material->bind();
    
    glDrawElements(mode, _nIndices, GL_UNSIGNED_SHORT, _indexData.get());
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}
