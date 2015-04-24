//
//  TextureData.h
//  Framework
//
//  Created by David Steiner.
//
//

#ifndef B_GEOMETRY_DATA_H
#define B_GEOMETRY_DATA_H

#include <vector>
#include <unordered_map>
#include "Renderer_GL.h"
#include "vmmlib/vector.hpp"
#include "MaterialData.h"
#include "Texture.h"

class GeometryDataImpl;

struct IndexData
{
    GLushort vertexIndex;
    GLushort texCoordsIndex;
    GLushort normalIndex;
    
    IndexData()
    : vertexIndex(0)
    , texCoordsIndex(0)
    , normalIndex(0)
    {}
};

struct TexCoord
{
    GLfloat s;
    GLfloat t;
};

struct Color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct Point3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Vector3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Vertex
{
    Point3      position;
    Vector3     normal;
    Vector3     tangent;
    Vector3     bitangent;
    TexCoord    texCoord;
};

typedef GLushort Index;

struct GeometryData
{
    typedef std::vector< Vertex >   VboVertices;
    typedef std::vector< GLushort > VboIndices;
    
    std::vector< IndexData > indices;
    
    VboVertices vboVertices;
    VboIndices  vboIndices;
    
    MaterialData materialData;
};

typedef std::shared_ptr< GeometryData > GeometryDataPtr;

#endif /* defined(B_GEOMETRY_DATA_H) */
