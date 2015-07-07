#include "../headers/Light.h"
#include "../headers/Configuration.h"

Light::Light() 
	: Light(bRenderer::DEFAULT_LIGHT_POSITION, bRenderer::DEFAULT_LIGHT_COLOR, bRenderer::DEFAULT_LIGHT_INTENSITY, bRenderer::DEFAULT_LIGHT_ATTENUATION)
{}

Light::Light(const vmml::vec3f &position, const vmml::vec3f &color)
	: Light(position, color, bRenderer::DEFAULT_LIGHT_INTENSITY, bRenderer::DEFAULT_LIGHT_ATTENUATION)
{}

Light::Light(const vmml::vec3f &position, const vmml::vec3f &color, GLfloat intensity, GLfloat attenuation)
	: _position(vmml::vec4f(position, 1.0)), _color(color), _intensity(intensity), _attenuation(attenuation)
{}

Light::~Light()
{}

void Light::setPosition(const vmml::vec3f &position)
{
	_position = vmml::vec4f(position, 1.0);
}

void Light::setColor(const vmml::vec3f &color)
{
	_color = color;
}

void Light::setIntensity(GLfloat intensity)
{
	_intensity = intensity;
}

void Light::setAttenuation(GLfloat attenuation)
{
	_attenuation = attenuation;
}

vmml::vec4f Light::getPosition()
{
	return _position;
}

vmml::vec3f Light::getColor()
{
	return _color;
}

GLfloat Light::getIntensity()
{
	return _intensity;
}

GLfloat Light::getAttenuation()
{
	return _attenuation;
}