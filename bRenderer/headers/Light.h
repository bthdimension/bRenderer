#ifndef B_LIGHT_H
#define B_LIGHT_H

#include <memory>
#include "vmmlib/vector.hpp"
#include "Renderer_GL.h"

///////////////////////////////////////////////
//	
//	TODO: create spot lights and sun light
//
///////////////////////////////////////////////

/** @brief A light source to brighten up your day! 
*	@author Benjamin Bürgisser
*/
class Light
{
public:

	/* Functions */

	/**	@brief Constructor loading standard values for position, color, intensity and attenuation
	*/
	Light();

	/**	@brief Constructor loading standard values for intensity and attenuation
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light
	*/
	Light(const vmml::vec3f &position, const vmml::vec3f &color);

	/**	@brief Constructor 
	*	@param[in] position Position of the light
	*	@param[in] color Color of the light
	*	@param[in] intensity Intensity of the light
	*	@param[in] attenuation Attenuation of the light
	*/
	Light(const vmml::vec3f &position, const vmml::vec3f &color, GLfloat intensity, GLfloat attenuation);

	/**	@brief Destructor
	*/
	~Light();

	/**	@brief Sets the position of the light
	*	@param[in] position Position of the light
	*/
	void setPosition(const vmml::vec3f &position);

	/**	@brief Sets the color of the light
	*	@param[in] color Color of the light
	*/
	void setColor(const vmml::vec3f &color);

	/**	@brief Sets the intensity of the light
	*	@param[in] intensity Intensity of the light
	*/
	void setIntensity(GLfloat intensity);

	/**	@brief Sets the attenuation of the light
	*	@param[in] attenuation Attenuation of the light
	*/
	void setAttenuation(GLfloat attenuation);

	/**	@brief Returns the position of the light
	*/
	vmml::vec4f getPosition();

	/**	@brief Returns the color of the light
	*/
	vmml::vec3f getColor();

	/**	@brief Returns the intensity of the light
	*/
	GLfloat getIntensity();

	/**	@brief Returns the attenuation of the light
	*/
	GLfloat getAttenuation();

private:

	/* Variables */

	vmml::vec4f _position;
	vmml::vec3f _color;
	GLfloat _intensity;
	GLfloat _attenuation;

};

typedef std::shared_ptr<Light> LightPtr;

#endif /* defined(B_LIGHT_H) */
