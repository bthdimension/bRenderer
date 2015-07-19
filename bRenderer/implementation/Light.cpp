#include "../headers/Light.h"
#include "../headers/Configuration.h"

Light::Light() 
	: Light(bRenderer::DEFAULT_LIGHT_POSITION(), bRenderer::DEFAULT_LIGHT_COLOR(), bRenderer::DEFAULT_LIGHT_INTENSITY(), bRenderer::DEFAULT_LIGHT_ATTENUATION())
{}

Light::Light(const vmml::Vector3f &position, const vmml::Vector3f &color)
	: Light(position, color, bRenderer::DEFAULT_LIGHT_INTENSITY(), bRenderer::DEFAULT_LIGHT_ATTENUATION())
{}

Light::Light(const vmml::Vector3f &position, const vmml::Vector3f &color, GLfloat intensity, GLfloat attenuation)
	: Light(position, color, color, intensity, attenuation)
{}

Light::Light(const vmml::Vector3f &position, const vmml::Vector3f &diffuseColor, const vmml::Vector3f &specularColor, GLfloat intensity, GLfloat attenuation)
	: _position(vmml::Vector4f(position, 1.0)), _diffuseColor(diffuseColor), _specularColor(specularColor), _intensity(intensity), _attenuation(attenuation)
{}

Light::~Light()
{}

void Light::setPosition(const vmml::Vector3f &position)
{
	_position = vmml::Vector4f(position, 1.0);
}

void Light::setDiffuseColor(const vmml::Vector3f &color)
{
	_diffuseColor = color;
}

void Light::setSpecularColor(const vmml::Vector3f &color)
{
	_specularColor = color;
}


void Light::setIntensity(GLfloat intensity)
{
	_intensity = intensity;
}

void Light::setAttenuation(GLfloat attenuation)
{
	_attenuation = attenuation;
}

vmml::Vector4f Light::getPosition()
{
	return _position;
}

vmml::Vector3f Light::getDiffuseColor()
{
	return _diffuseColor;
}

vmml::Vector3f Light::getSpecularColor()
{
	return _specularColor;
}

GLfloat Light::getIntensity()
{
	return _intensity;
}

GLfloat Light::getAttenuation()
{
	return _attenuation;
}