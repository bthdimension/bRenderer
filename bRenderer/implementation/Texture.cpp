//
//  Texture.cpp
//  Framework
//
//  Created by Rahul Mukhi on 1/16/13.
//  Reworked by David Steiner
//

#include "../headers/Texture.h"
#include "../headers/TextureData.h"
#include "../headers/bRenderer_GL.h"

Texture::Texture(const TextureData &data)
{
    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    
    if (_textureID)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, data.getFormat(), data.getWidth(), data.getHeight(), 0, data.getFormat(), GL_UNSIGNED_BYTE, data.getImageData().get());

        glGenerateMipmap(GL_TEXTURE_2D);

    }
}

Texture::Texture()
{
    
}

Texture::~Texture()
{
    
}

GLuint Texture::getTextureID()
{
    return _textureID;
}

void Texture::bind(GLint texUnit)
{
    glActiveTexture(texUnit);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}