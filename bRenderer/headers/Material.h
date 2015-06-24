#ifndef B_MATERIAL_H
#define B_MATERIAL_H

#include <memory>

#include "Shader.h"
#include "Texture.h"
#include "MaterialData.h"

class Renderer;

/** @brief A material associated with textures and a shader to define the look of an object
*	@author David Steiner
*/
class Material
{
public:
	/* Typedefs */
    typedef std::unordered_map<std::string, TexturePtr>     Textures;
    typedef std::unordered_map<std::string, vmml::vec3f>    Vectors;
    typedef std::unordered_map<std::string, float>          Scalars;

	/* Functions */
    
	/**	@brief Initializes the geometry object based on material data and a shader
	*	@param[in] r Instance of the renderer main class
	*	@param[in] materialData 
	*	@param[in] shader
	*/
	void initialize(Renderer *r, const MaterialData &materialData, ShaderPtr shader);
    
	/**	@brief Bind the shader and pass the attributes of the material
	*/
    void bind();
    
	/**	@brief Returns the textures associated with the material
	*/
    const Textures  &getTextures()                      { return _textures;         }

	/**	@brief Returns a texture associated with the material
	*	@param[in] name Name of the texture
	*/
    TexturePtr      getTexture(const std::string &name) { return _textures[name];   }
    
	/**	@brief Returns the vectors associated with the material
	*/
    const Vectors   &getVectors()                       { return _vectors;          }

	/**	@brief Returns a vector associated with the material
	*	@param[in] name Name of the vector
	*/
    vmml::vec3f     getVector(const std::string &name)  { return _vectors[name];    }
    
	/**	@brief Returns the scalars associated with the material
	*/
    const Scalars   &getScalars()                       { return _scalars;          }

	/**	@brief Returns a scalar associated with the material
	*	@param[in] name Name of the scalar
	*/
    float           getScalar(const std::string &name)  { return _scalars[name];    }

	/**	@brief Sets all textures for the material
	*	@param[in] arg Textures for the material
	*/
    void            setTextures(const Textures &arg)                            { _textures = arg;          }

	/**	@brief Sets a texture for the material
	*	@param[in] name Name of the texture
	*	@param[in] arg Texture for the material
	*/
    void            setTexture(const std::string &name, TexturePtr arg)         { _textures[name] = arg;    }
    
	/**	@brief Sets all vectors for the material
	*	@param[in] arg Vectors for the material
	*/
    void            setVectors(const Vectors& arg)                              { _vectors = arg;           }

	/**	@brief Sets a vector for the material
	*	@param[in] name Name of the vector
	*	@param[in] arg Vector for the material
	*/
    void            setVector(const std::string &name, const vmml::vec3f &arg)  { _vectors[name] = arg;     }
    
	/**	@brief Sets all scalars for the material
	*	@param[in] arg Scalar for the material
	*/
    void            setScalars(const Scalars &arg)                              { _scalars = arg;           }

	/**	@brief Sets a scalar for the material
	*	@param[in] name Name of the scalar
	*	@param[in] arg Scalar for the material
	*/
    void            setScalar(const std::string &name, float arg)               { _scalars[name] = arg;     }
    
	/**	@brief Returns the shader of the material
	*/
    ShaderPtr   getShader()                 { return _shader;       }

	/**	@brief Sets the shader of the material
	*	@param[in] arg Sahder for the material
	*/
    void        setShader(ShaderPtr arg)    { _shader       = arg;  }
    
	/**	@brief Returns the name of the material
	*/
    std::string getName()                       { return _name; }

	/**	@brief Sets the name of the material
	*	@param[in] arg Name for the material
	*/
    void        setName(const std::string &arg) { _name = arg;  }
    
private:
	
	/* Variables */

    Textures    _textures;
    Vectors     _vectors;
    Scalars     _scalars;

    ShaderPtr   _shader;
    
    std::string _name;
};

typedef std::shared_ptr< Material > MaterialPtr;

#endif /* defined(B_MATERIAL_H) */
