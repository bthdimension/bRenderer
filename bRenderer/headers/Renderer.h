#ifndef B_RENDERER_H
#define B_RENDERER_H

///* bRenderer includes */
//#include "OSdetect.h"
//#include "bRenderer_GL.h"
//#include "Logger.h"
//#include "FileHandler.h"
//#include "MatrixStack.h"
//#include "Camera.h"
//
///* Flamework includes*/
//#include "Model.h"
//#include "Texture.h"
//#include "../bRenderer.h"
//#include "ModelData.h"
//#include "TextureData.h"
//#include "ShaderData.h"
//
///* vmmlib includes */
//#include "vmmlib/addendum.hpp"
//
class Renderer
{
public:
//	/* Typedefs */
//	typedef std::unordered_map< std::string, ShaderPtr >    Shaders;
//	typedef std::unordered_map< std::string, TexturePtr >   Textures;
//	typedef std::unordered_map< std::string, MaterialPtr >  Materials;
//	typedef std::unordered_map< std::string, ModelPtr >     Models;
	
	/* Functions */

	/**	@brief Returns the single instance of the Renderer
	 */
	static Renderer& get()
	{
		static Renderer renderer;
		return renderer;
	}

private:
	/* Functions */

	/**	@brief Private constructor
	 */
	Renderer(){}
	/**	@brief Private constructor to prevent instantiation via copy constructor
	 */
	Renderer(const Renderer&){}
	/**	@brief Private operator overloading to prevent instantiation of the renderer
	 */
	Renderer & operator = (const Renderer &){}
	/**	@brief Private destructor
	 */
	~Renderer(){}

	/* Variables */

//	Shaders     _shaders;
//	Textures    _textures;
//	Materials   _materials;
//	Models      _models;
//
//	std::string defaultMaterialName = "default";
//
//	bool initialized = false;
//	bool running = false;


};



#endif /* defined(B_RENDERER_H) */
