#ifndef B_SPRITE_H
#define B_SPRITE_H

#include "Model.h"

/** @brief A 2d plane that can be rendered to the screen
*	@author Benjamin Bürgisser
*/
class Sprite : public Model
{
public:
	/* Functions */

	/**	@brief Constructor
	*	@param[in] material
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	explicit Sprite(MaterialPtr material, PropertiesPtr	properties = nullptr);

	/**	@brief Constructor
	*	@param[in] a Asset management
	*	@param[in] textureFileName The filename of the texture that should be loaded and displayed
	*	@param[in] materialName The name of the material that should be created
	*	@param[in] shader
	*	@param[in] properties Properties that will be passed to the shader of the model (optional)
	*/
	explicit Sprite(AssetManagement *a, const std::string &textureFileName, const std::string &materialName, ShaderPtr shader, PropertiesPtr properties = nullptr);

	/**	@brief Constructor
	*	@param[in] a Asset management
	*	@param[in] name The name for the shader and material that will be generated
	*	@param[in] textureFileName The filename of the texture that should be loaded and displayed
	*	@param[in] shaderMaxLights The maximum number of light sources to be used 
	*	@param[in] variableNumberOfLights Set true if the number of lights may vary, otherwise the number of lights has to be the same as specified as maximum number of lights
	*/
	explicit Sprite(AssetManagement *a, const std::string &name, const std::string &textureFileName, GLuint shaderMaxLights, bool variableNumberOfLights);

	/**	@brief Destructor
	*/
	~Sprite() {}

private:

	/* Functions */
	
	void createGeometry();

};


#endif /* defined(B_SPRITE_H) */
