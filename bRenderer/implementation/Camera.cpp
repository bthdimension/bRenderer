#include "../headers/Camera.h"
#include <cassert>

/* Constructor and Destructor */

Camera::Camera() : _orientation(0.0f, 0.0f, -1.0f), _position(0.0f, 0.0f, 0.0f) {}

Camera::Camera(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far) : _orientation(0.0f, 0.0f, -1.0f), _position(0.0f, 0.0f, 0.0f)
{
	_fov = fov;
	_aspect = aspect;
	_near = near;
	_far = far;
}

Camera::Camera(vmml::vec3f position, vmml::vec3f orientation)
{
	_position = position;
	_orientation = orientation;
}

Camera::Camera(vmml::vec3f position, vmml::vec3f orientation, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	_position = position;
	_orientation = orientation;
	_fov = fov;
	_aspect = aspect;
	_near = near;
	_far = far;
}

Camera::~Camera()
{
    
}

/* Public Functions */

void Camera::moveCamera(GLfloat camSpeed)
{
	_position -= camSpeed * _orientation;
}

void Camera::rotateCamera(const vmml::vec3f &axis, GLfloat rotation)
{
	_orientation = vmml::vec3f(vmml::create_rotation(-rotation, axis));
}

void Camera::resetCamera()
{
    _position = vmml::vec3f(0.0f,0.0f,0.0f);
	_orientation = vmml::vec3f(0.0f, 0.0f, -1.0f);
}

void Camera::setFieldOfView(GLfloat fov)
{
	_fov = fov;
}

void Camera::setAspectRatio(GLfloat aspect)
{
	_aspect = aspect;
}

void Camera::setNearClippingPlane(GLfloat near)
{
	_near = near;
}

void Camera::setFarClippingPlane(GLfloat far)
{
	_far = far;
}

vmml::mat4f Camera::getViewMatrix(){
	return lookAt(_position, _position + _orientation, vmml::vec3f::UP);
}

vmml::mat4f Camera::getProjectionMatrix()
{
	return createPerspective(_fov, _aspect, _near, _far);
}

/* Static Functions */

vmml::mat4f Camera::lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up){
	vmml::vec3f zaxis = vmml::normalize(eye - target);
	vmml::vec3f xaxis = vmml::normalize(vmml::cross<3>(up, zaxis));
	vmml::vec3f yaxis = vmml::cross<3>(zaxis, xaxis);

	vmml::mat4f view;
	view.set_row(0, vmml::vec4f(xaxis.x(), xaxis.y(), xaxis.z(), -vmml::dot(xaxis, eye)));
	view.set_row(1, vmml::vec4f(yaxis.x(), yaxis.y(), yaxis.z(), -vmml::dot(yaxis, eye)));
	view.set_row(2, vmml::vec4f(zaxis.x(), zaxis.y(), zaxis.z(), -vmml::dot(zaxis, eye)));
	view.set_row(3, vmml::vec4f(0, 0, 0, 1.0));

	return view;
}

vmml::mat4f Camera::createPerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	vmml::mat4f perspective = vmml::mat4f::IDENTITY;

	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			perspective.at(i, j) = 0.0f;
		}
	}

	GLfloat angle = (fov / 180.0f) * M_PI_F;
	GLfloat f = 1.0f / tan(angle * 0.5f);

	perspective.at(0, 0) = f / aspect;
	perspective.at(1, 1) = f;
	perspective.at(2, 2) = (far + near) / (near - far);
	perspective.at(2, 3) = -1.0f;
	perspective.at(3, 2) = (2.0f * far*near) / (near - far);


	return perspective;
}