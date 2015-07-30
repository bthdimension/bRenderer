#include "../headers/RenderQueue.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

void RenderQueue::submitToRenderQueue(GLuint programID, const std::string &materialName, const std::string &geometryName, GeometryPtr geometry, const Properties &properties, GLfloat distanceToCamera, bool isTransparent, GLenum blendSfactor, GLenum blendDfactor)
{
	// Transparent
	if (isTransparent){
		_renderCallsTransparent.insert(RenderCallsTransparent::value_type(distanceToCamera, RenderCall{ geometry, properties, blendSfactor, blendDfactor }));
	}
	// Opaque
	else{
		std::string key = lexical_cast<std::string>(programID)+materialName + geometryName;
		_renderCallsOpaque.insert(RenderCallsOpaque::value_type(key, RenderCall{ geometry, properties }));
	}
}

void RenderQueue::draw(GLenum mode)
{
	// Opaque
	for (auto renderCall = _renderCallsOpaque.begin(); renderCall != _renderCallsOpaque.end(); ++renderCall){
		renderCall->second.g->draw(&(renderCall->second.p), mode);
	}

	// Transparent
	for (auto renderCall = _renderCallsTransparent.rbegin(); renderCall != _renderCallsTransparent.rend(); ++renderCall){
		glBlendFunc(renderCall->second.blendSfactor, renderCall->second.blendDfactor);
		renderCall->second.g->draw(&(renderCall->second.p), mode);
	}

	// reset blend function so opaque objects are not affected
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// clear the queue
	clear();
}

void RenderQueue::clear()
{
	// Opaque
	_renderCallsOpaque.clear();

	// Transparent
	_renderCallsTransparent.clear();
}