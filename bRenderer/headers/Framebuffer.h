#ifndef B_FRAMEBUFFER_H
#define B_FRAMEBUFFER_H

#include <memory>
#include "Renderer_GL.h"
#include "Texture.h"

/** @brief A custom framebuffer object
*	@author Benjamin Bürgisser
*/
class Framebuffer
{
public:

	/* Functions */

	/**	@brief Constructor that takes care of the resolution automatically
	*/
	Framebuffer();

	/**	@brief Constructor with fixed resolution
	*	@param[in] width
	*	@param[in] height
	*/
	Framebuffer(GLint width, GLint height);

	/**	@brief Virtual destructor
	*/
	virtual ~Framebuffer() { destroy(); }

	/**	@brief Bind the framebuffer object
	*	@param[in] preserveCurrentFramebuffer If true the framebuffer that was active before binding is bound again when unbinding
	*/
	void bind(bool preserveCurrentFramebuffer);

	/**	@brief Bind the framebuffer object and draw to the specified texture
	*	@param[in] texture The texture to draw to
	*	@param[in] preserveCurrentFramebuffer If true the framebuffer that was active before binding is bound again when unbinding
    *
    *   Hint:   Not all textures work, especially when changing resolution between frames. It's best to not use mip mapping.
    *           A suitable texture can be generated using Renderer::createTexture and not passing any data (GLenum format = GL_RGBA, ImageDataPtr imageData = nullptr)
    *
	*/
	void bind(TexturePtr texture, bool preserveCurrentFramebuffer);

	/**	@brief Unbind the framebuffer object
	*/
	void unbind();

	/**	@brief Unbind the framebuffer object and bind the specified framebuffer object
	*	@param[in] fbo The framebuffer object that should be bound after unbinding
	*/
	void unbind(GLint fbo);

	/**	@brief Updates the size of the framebuffer
	*	@param[in] width
	*	@param[in] height
	*/
	void resize(GLint width, GLint height);
    
    /**	@brief Get the framebuffer id
     */
    GLuint getFramebufferID()       { return _fbo; }
    
    /**	@brief Get the depthbuffer id
     */
    GLuint getDebthbufferID()       { return _rbo_depth; }
    
    /**	@brief Get the width of the framebuffer
     */
    GLint getWidth()                { return _width; }
    
    /**	@brief Get the height of the framebuffer
     */
    GLint getHeight()               { return _height; }

	/**	@brief Get the framebuffer currently active
	*/
	static GLint getCurrentFramebuffer();

private:

	/* Functions */

	void create();
	void destroy();

	/* Variables */
	GLint _width, _height, _oldFbo;
	GLuint _fbo, _rbo_depth;
	bool _preserveCurrentFramebuffer, _autoResize;

};

typedef std::shared_ptr<Framebuffer> FramebufferPtr;

#endif /* defined(B_FRAMEBUFFER_H) */
