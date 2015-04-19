//
//  Geometry.h
//  Framework
/*
    Stores geometry data for one model group(or part).
    Initializes the buffer and renders the geometry.
*/
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_GEOMETRY_H
#define FRAMEWORK_GEOMETRY_H

#include <string>
#include <fstream>
#include <memory>
#include "GeometryData.h"
#include "Material.h"
#include "Texture.h"

#include "bRenderer_GL.h"


class Geometry
{
public:
    typedef std::shared_ptr< Vertex >   VertexDataPtr;
    typedef std::shared_ptr< GLushort > IndexDataPtr;

    void draw(GLenum mode = GL_TRIANGLES);
    
    void initialize(GeometryDataPtr geometryData);
    
    void initializeVertexBuffer();
    
    VertexDataPtr   allocVertexData(size_t nVertices);
    IndexDataPtr    allocIndexData(size_t nIndices);
    
    VertexDataPtr   copyVertexData(const GeometryData::VboVertices &arg);
    IndexDataPtr    copyIndexData(const GeometryData::VboIndices &arg);

    VertexDataPtr   getVertexData()                     { return _vertexData;   }
    IndexDataPtr    getIndexData()                      { return _indexData;    }
    
    void            setVertexData(VertexDataPtr arg)    { _vertexData   = arg;  }
    void            setIndexData(IndexDataPtr arg)      { _indexData    = arg;  }
    
    MaterialPtr     getMaterial()                       { return _material;     }
    void            setMaterial(MaterialPtr arg)        { _material     = arg;  }

private:
    GLuint _indexBuffer, _vertexBuffer;
    size_t _nIndices, _nVertices;
    
    VertexDataPtr   _vertexData;
    IndexDataPtr    _indexData;
    
    MaterialPtr _material;
};

typedef std::shared_ptr<Geometry> GeometryPtr;

#endif
