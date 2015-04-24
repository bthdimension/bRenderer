//
//  TextureData.h
//  Framework
//
//  Created by David Steiner on 3/2/13.
//
//

#ifndef B_TEXTURE_DATA_H
#define B_TEXTURE_DATA_H

#include <memory>
#include <string>
#include "Renderer_GL.h"

typedef std::shared_ptr< GLubyte > ImageDataPtr;
    
class TextureData
{
public:
    explicit TextureData(const std::string &fileName);
    TextureData();
    
    TextureData &load(const std::string &fileName);
    
    GLsizei         getWidth()      const   { return _width;        }
    GLsizei         getHeight()     const   { return _height;       }
    GLenum          getFormat()     const   { return _format;       }
    ImageDataPtr    getImageData()  const   { return _imageData;    }
    
private:
    GLsizei         _width;
    GLsizei         _height;
    GLenum          _format;
    ImageDataPtr    _imageData;
};

#endif /* defined(B_TEXTURE_DATA_H) */
