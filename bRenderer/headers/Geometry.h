#ifndef B_GEOMETRY_H
#define B_GEOMETRY_H

#include <string>
#include <fstream>
#include <memory>

#include "GeometryData.h"
#include "Material.h"
#include "Texture.h"
#include "Properties.h"
#include "Renderer_GL.h"
#include "IDrawable.h"

/** @brief The geometry that can be rendered to the screen
*	@author Rahul Mukhi, David Steiner
*/
class Geometry : public IDrawable
{
public:
	/* Typedefs */
	typedef std::shared_ptr< Vertex >   VertexDataPtr;
    typedef std::shared_ptr< GLushort > IndexDataPtr;

	/* Functions */

	/**	@brief Draws the geometry to the screen
	*	@param[in] mode 
	*/
    void draw(GLenum mode = GL_TRIANGLES);
    
	/**	@brief Initializes the geometry object based on geometry data 
	*	@param[in] geometryData	
	*/
    void initialize(GeometryDataPtr geometryData);

	/**	@brief Returns a pointer to the vertices of the geometry
	*/
    VertexDataPtr   getVertexData()                     { return _vertexData;   }

	/**	@brief Returns a pointer to the indices of the geometry
	*/
    IndexDataPtr    getIndexData()                      { return _indexData;    }
    
	/**	@brief Sets the vertices of the geometry
	*	@param[in] arg The new vertices that should be used for the geometry
	*/
    void            setVertexData(VertexDataPtr arg)    { _vertexData   = arg;  }

	/**	@brief Sets the indices of the geometry
	*	@param[in] arg The new indices that should be used for the geometry
	*/
    void            setIndexData(IndexDataPtr arg)      { _indexData    = arg;  }
    
	/**	@brief Returns a pointer to the material of the geometry
	*/
    MaterialPtr     getMaterial()                       { return _material;     }

	/**	@brief Sets the material of the geometry
	*	@param[in] arg The new material that should be used for the geometry
	*/
    void            setMaterial(MaterialPtr arg)        { _material     = arg;  }

	/**	@brief Returns a pointer to the properties of the geometry
	*/
	PropertiesPtr     getProperties()                       { return _properties; }

	/**	@brief Sets the properties of the geometry
	*	@param[in] arg The properties for the geometry
	*/
	void            setProperties(PropertiesPtr arg)        { _properties = arg; }

private:

	/* Functions */

	/**	@brief Initializes a vertex buffer for the geometry data
	*/
	void initializeVertexBuffer();

	/**	@brief Allocates the vertex data
	*/
	VertexDataPtr   allocVertexData(size_t nVertices);

	/**	@brief Allocates the index data
	*/
	IndexDataPtr    allocIndexData(size_t nIndices);

	/**	@brief Copies the given vertices into the geometry object
	*	@param[in] arg The vertices that should be used in the geometry 
	*/
	VertexDataPtr   copyVertexData(const GeometryData::VboVertices &arg);

	/**	@brief Copies the given indices into the geometry object
	*	@param[in] arg The indices that should be used in the geometry
	*/
	IndexDataPtr    copyIndexData(const GeometryData::VboIndices &arg);

	/* Variables */

    GLuint _indexBuffer, _vertexBuffer;
    size_t _nIndices, _nVertices;
    
    VertexDataPtr   _vertexData;
    IndexDataPtr    _indexData;
    
    MaterialPtr _material;
	PropertiesPtr _properties;
};

typedef std::shared_ptr<Geometry> GeometryPtr;

#endif /* defined(B_GEOMETRY_H) */
