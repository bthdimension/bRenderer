#include "../headers/Framebuffer.h"
#include "../headers/Logger.h"
#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

/* Constructors */

Framebuffer::Framebuffer()
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	_width = vp[2];
	_height = vp[3];
	_preserveCurrentFramebuffer = false;
	_autoResize = true;

	create();
}

Framebuffer::Framebuffer(GLint width, GLint height)
{
	_width = width;
	_height = height;
	_preserveCurrentFramebuffer = false;
	_autoResize = false;
	
	create();
}

/* Public Functions */

void Framebuffer::bind(bool preserveCurrentFramebuffer)
{
	_preserveCurrentFramebuffer = preserveCurrentFramebuffer;
	if (_preserveCurrentFramebuffer)
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_oldFbo);

	if (_autoResize){
		GLint vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);
		resize(vp[2], vp[3]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::bind(TexturePtr texture, bool preserveCurrentFramebuffer)
{
	_preserveCurrentFramebuffer = preserveCurrentFramebuffer;
	if (_preserveCurrentFramebuffer)
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_oldFbo);

	if (_autoResize){
		GLint vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);
		resize(vp[2], vp[3]);
	}
	// Resize texture 
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getTextureID(), 0);
	// Important: glClear has to be called after binding the texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (_preserveCurrentFramebuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, _oldFbo);
}

void Framebuffer::unbind(GLint fbo)
{
	unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

GLint Framebuffer::getCurrentFramebuffer()
{
	GLint currentFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo);
	return currentFbo;
}

void Framebuffer::resize(GLint width, GLint height)
{
	if (_width != width || _height != height){
		_width = width;
		_height = height;

		// depthbuffer
		glBindRenderbuffer(GL_RENDERBUFFER, _rbo_depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

/* Private Functions */

void Framebuffer::create()
{
	// depthbuffer
	glGenRenderbuffers(1, &_rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// framebuffer
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo_depth);
	GLenum status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
		bRenderer::log("Framebuffer status: " + lexical_cast< std::string >(status), bRenderer::LM_ERROR);
	}

	// clear
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// unbind
	unbind();
}