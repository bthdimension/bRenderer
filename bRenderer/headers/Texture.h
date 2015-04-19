//
//  Texture.h
//  Framework
/*
    Loads and binds texture.
*/
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#ifndef FRAMEWORK_TEXTURE_H
#define FRAMEWORK_TEXTURE_H

#include <memory>
#include <string>
#include <unordered_map>
#include "bRenderer_GL.h"

class TextureData;

class Texture
{
public:
    Texture(const TextureData &data);
    Texture();
    ~Texture();
    
    GLuint getTextureID();
    void bind(GLint texUnit = GL_TEXTURE0);
    
private:
    GLuint _textureID;
};

typedef std::shared_ptr< Texture >  TexturePtr;

#endif
