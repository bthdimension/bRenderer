#ifndef B_DEPTH_MAP_H
#define B_DEPTH_MAP_H

#include <vector>
#include "Texture.h"

/** @brief Cube map
*	@author Benjamin Bürgisser
*/
class DepthMap : public Texture
{
public:

	/* Functions */

	/**	@brief Constructor
	*	@param[in] width
	*	@param[in] height
	*/
	DepthMap(GLint width, GLint height);

	/**	@brief Virtual destructor
	*/
	virtual ~DepthMap() {}

};

typedef std::shared_ptr< DepthMap >  DepthMapPtr;

#endif /* defined(B_DEPTH_MAP_H) */
