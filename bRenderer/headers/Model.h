#ifndef B_MODEL_H
#define B_MODEL_H

#include <memory>
#include <string>
#include <unordered_map>

#include "IDrawable.h"
#include "Geometry.h"
#include "Material.h"
#include "Renderer_GL.h"

class AssetManagement;
class ModelData;

/** @brief A 3d model that can be rendered to the screen
*	@author Rahul Mukhi, David Steiner
*/
class Model : public IDrawable
{
public:
	/* Typedefs */
	typedef std::unordered_map< std::string, GeometryPtr > GroupMap;

	/* Functions */

	/**	@brief Constructor
	*/
	explicit Model() {}

	/**	@brief Constructor
	*	@param[in] a Asset management
	*	@param[in] modelData
	*	@param[in] shaderMaxLights The maximum light sources to be used
	*	@param[in] variableNumberOfLights True if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*	@param[in] shaderFromFile Set true if for every material a shader file with the same name should be loaded
	*	@param[in] ambientLighting Set true if the shader supports ambient lighting
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	explicit Model(AssetManagement *a, const ModelData &modelData, GLuint shaderMaxLights, bool variableNumberOfLights, bool shaderFromFile, bool ambientLighting, PropertiesPtr properties = nullptr);

	/**	@brief Constructor
	*	@param[in] a Asset management
	*	@param[in] modelData
	*	@param[in] shader
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	explicit Model(AssetManagement *a, const ModelData &modelData, ShaderPtr shader, PropertiesPtr	properties = nullptr);

	/**	@brief Constructor
	*	@param[in] modelData
	*	@param[in] material 
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	explicit Model(const ModelData &modelData, MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Virtual destructor
	*/
	virtual ~Model() {}
    
	/**	@brief Draws the model to the screen
	*	@param[in] mode
	*/
	virtual void draw(GLenum mode = GL_TRIANGLES);

	/**	@brief Draws the specified group of geometry to the screen
	*	@param[in] groupName Name of the group to draw
	*	@param[in] mode
	*/
	virtual void draw(const std::string &groupName, GLenum mode = GL_TRIANGLES);
    
	/**	@brief Returns the material of the model
	*/
    MaterialPtr getMaterial()   { return _material; }
    
	/**	@brief Sets the material of the model
	*	@param[in] arg The material for the model
	*/
    void setMaterial(MaterialPtr arg)
    {
        _material = arg;
        
        for (auto i = _groups.begin(); i != _groups.end(); ++i)
        {
            i->second->setMaterial(_material);
        }
    }

	/**	@brief Returns the properties of the model
	*/
	PropertiesPtr getProperties()   { return _properties; }

	/**	@brief Sets the properties of the model
	*	@param[in] arg The properties for the model
	*/
	void setProperties(PropertiesPtr arg)
	{
		_properties = arg;

		for (auto i = _groups.begin(); i != _groups.end(); ++i)
		{
			i->second->setProperties(_properties);
		}
	}
    
	/**	@brief Returns the groups of geometry of the model
	*/
    GroupMap &getGroups()   { return _groups; }

	/**	@brief Returns the bounding box of the geometry in object space
	*/
	vmml::AABBf     getBoundingBoxObjectSpace()			{ return _boundingBox; }

	/**	@brief Sets the bounding box of the geometry in object space
	*	@param[in] arg The bounding box for the geometry in object space
	*/
	void            setBoundingBoxObjectSpace(vmml::AABBf arg)						{ _boundingBox = arg; }

	/**	@brief Adds geometry to the model
	*	@param[in] name	Name of the geometry
	*	@param[in] geometry A pointer to the geometry
	*/
	void			addGeometry(std::string name, GeometryPtr geometry)				{ _groups.insert(GroupMap::value_type(name, geometry)); }

	/**	@brief Removes geometry from the model
	*	@param[in] name	Name of the geometry
	*/
	void			removeGeometry(std::string name)								{ _groups.erase(name); }

private:

	/* Variables */

    GroupMap        _groups;
    MaterialPtr     _material;
	PropertiesPtr	_properties;
	vmml::AABBf		_boundingBox;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif /* defined(B_MODEL_H) */
