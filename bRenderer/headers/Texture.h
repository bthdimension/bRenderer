#ifndef B_TEXTURE_H
#define B_TEXTURE_H

#include <memory>
#include <string>
#include "Renderer_GL.h"

class TextureData;

/** @brief Texture
*	@author Rahul Mukhi, David Steiner
*/
class Texture
{
public:

	/* Functions */

	/**	@brief Constructor
	*	@param[in] data The texture data to be used
	*/
    Texture(const TextureData &data);

	/**	@brief Constructor
	*/
    Texture();

	/**	@brief Destructor
	*/
    ~Texture();
    
	/**	@brief Returns texture id
	*/
    GLuint getTextureID();

	/**	@brief Binds the texture
	*	@param[in] texUnit Specifies which texture unit to bind it to
	*/
    void bind(GLint texUnit = GL_TEXTURE0);
    
private:

	/* Variables */

    GLuint _textureID;
};

typedef std::shared_ptr< Texture >  TexturePtr;

#endif /* defined(B_TEXTURE_H) */
