#ifndef B_TEXTURE_DATA_H
#define B_TEXTURE_DATA_H

#include <memory>
#include <string>
#include "Renderer_GL.h"

typedef std::shared_ptr< GLubyte > ImageDataPtr;
   
/** @brief The underlying data of a texture
*	@author David Steiner
*/
class TextureData
{
public:

	/* Functions */

	/**	@brief Constructor
	*	@param[in] fileName The name of the texture file
	*/
    explicit TextureData(const std::string &fileName);

	/**	@brief Constructor
	*/
    TextureData();
    
	/**	@brief Loads the texture from an image
	*	@param[in] fileName The name of the texture file
	*/
    TextureData &load(const std::string &fileName);
    
	/**	@brief Gets the width of the texture
	*/
    GLsizei         getWidth()      const   { return _width;        }
	
	/**	@brief Gets the height of the texture
	*/
	GLsizei         getHeight()     const   { return _height;       }

	/**	@brief Gets the format of the texture
	*/
    GLenum          getFormat()     const   { return _format;       }

	/**	@brief Gets the image data of the texture
	*/
    ImageDataPtr    getImageData()  const   { return _imageData;    }
    
private:

	/* Variables */

    GLsizei         _width;
    GLsizei         _height;
    GLenum          _format;
    ImageDataPtr    _imageData;
};

#endif /* defined(B_TEXTURE_DATA_H) */
