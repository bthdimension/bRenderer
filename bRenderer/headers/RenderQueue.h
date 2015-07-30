#ifndef B_RENDERQUEUE_H
#define B_RENDERQUEUE_H

#include <memory>
#include <map>

#include "Renderer_GL.h"
#include "Shader.h"
#include "Material.h"
#include "Properties.h"
#include "Geometry.h"
#include "IDrawable.h"

struct RenderCall
{
	GeometryPtr g;
	Properties p;
	GLenum blendSfactor, blendDfactor;
};

/** @brief A queue that stores render calls and performs drawing in sorted order
*	@author Benjamin Bürgisser
*/
class RenderQueue : public IDrawable
{
public:
	/* Typedefs */
	typedef std::multimap< std::string, RenderCall> RenderCallsOpaque;
	typedef std::multimap< GLfloat, RenderCall> RenderCallsTransparent;

	/* Functions */

	/**	@brief Constructor
	*/
	RenderQueue(){}

	/**	@brief Destructor
	*/
	~RenderQueue(){}

	/**	@brief Add a render call to the queue
	*	@param[in] programID
	*	@param[in] geometryName
	*	@param[in] materialName
	*	@param[in] geometry
	*	@param[in] properties 
	*	@param[in] distanceToCamera The distance of the transparent geometry to the camera (optional)
	*	@param[in] opaque Set true if the object isn't transparent (optional)
	*/
	void submitToRenderQueue(GLuint programID, const std::string &geometryName, const std::string &materialName, GeometryPtr geometry, const Properties &properties, GLfloat distanceToCamera = 0.0, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA);

	/**	@brief Draw all geometry in the queue in a sorted manner
	*/
	void draw(GLenum mode = GL_TRIANGLES);

	/**	@brief Remove all render calls
	*/
	void clear();


private:

	/* Variables */
	
	/*std::vector< RenderCallOpaque >			_renderCallsOpaque;
	std::vector< RenderCallTransparent >	_renderCallsTransparent;*/
	RenderCallsOpaque			_renderCallsOpaque;
	RenderCallsTransparent		_renderCallsTransparent;

};

typedef std::shared_ptr<RenderQueue> RenderQueuePtr;

#endif /* defined(B_RENDERQUEUE_H) */
