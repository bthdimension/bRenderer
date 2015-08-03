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
	GeometryPtr geometry;
	std::string instanceName;
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
	*	@param[in] materialName
	*	@param[in] geometryName
	*	@param[in] instanceName
	*	@param[in] geometry
	*	@param[in] distanceToCamera The distance of the transparent geometry to the camera (optional)
	*	@param[in] isTransparent Set true if the object is transparent (optional)
	*	@param[in] blendSfactor Specifies how the red, green, blue, and alpha source blending factors are computed (optional)
	*	@param[in] blendDfactor Specifies how the red, green, blue, and alpha destination blending factors are computed (optional)
	*/
	void submitToRenderQueue(GLuint programID, const std::string &materialName, const std::string &geometryName, const std::string &instanceName, GeometryPtr geometry, GLfloat distanceToCamera = 0.0, bool isTransparent = false, GLenum blendSfactor = GL_SRC_ALPHA, GLenum blendDfactor = GL_ONE_MINUS_SRC_ALPHA);

	/**	@brief Draw all geometry in the queue in a sorted manner
	*/
	void draw(GLenum mode = GL_TRIANGLES);

	/**	@brief Remove all render calls
	*/
	void clear();


private:

	/* Variables */
	
	RenderCallsOpaque			_renderCallsOpaque;
	RenderCallsTransparent		_renderCallsTransparent;

};

typedef std::shared_ptr<RenderQueue> RenderQueuePtr;

#endif /* defined(B_RENDERQUEUE_H) */
