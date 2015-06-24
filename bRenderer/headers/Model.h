#ifndef B_MODEL_H
#define B_MODEL_H

#include <memory>
#include <string>
#include <map>

#include "IDrawable.h"
#include "Geometry.h"
#include "Material.h"
#include "Renderer_GL.h"

class Renderer;
class ModelData;

/** @brief A 3d model that can be rendered to the screen
*	@author Rahul Mukhi, David Steiner
*/
class Model: public IDrawable
{
public:
	/* Typedefs */
	typedef std::map< std::string, Geometry > GroupMap;

	/* Functions */
    
	/**	@brief Constructor
	*	@param[in] r Instance of the renderer main class
	*	@param[in] modelData
	*	@param[in] shader
	*/
	explicit Model(Renderer *r, const ModelData &modelData, ShaderPtr shader = nullptr);

	/**	@brief Constructor
	*	@param[in] modelData
	*	@param[in] material 
	*/
	explicit Model(const ModelData &modelData, MaterialPtr material);

	/**	@brief Destructor
	*/
    ~Model();
    
	/**	@brief Draws the model to the screen
	*	@param[in] mode
	*/
    void draw(GLenum mode = GL_TRIANGLES);

	/**	@brief Draws the specified group of geometry to the screen
	*	@param[in] groupName Name of the group to draw
	*	@param[in] mode
	*/
    void draw(const std::string &groupName, GLenum mode = GL_TRIANGLES);
    
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
            i->second.setMaterial(_material);
        }
    }
    
	/**	@brief Returns the groups of geometry of the model
	*/
    GroupMap &getGroups()   { return _groups; }

private:

	/* Variables */

    GroupMap        _groups;
    MaterialPtr     _material;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif /* defined(B_MODEL_H) */
