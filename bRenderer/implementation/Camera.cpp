#include "../headers/Camera.h"
#include <cassert>

Camera::Camera() : _view(0.0f,0.0f,-1.0f), _position(0.0f,0.0f,0.0f)
{
    
}

Camera::~Camera()
{
    
}

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

void Camera::moveCamera(const float camSpeed)
{
    _position += camSpeed * _view;
}

void Camera::rotateCamera(const vmml::vec3f &axis, float rotation)
{
    _view = vmml::mat3f(vmml::create_rotation(-rotation, axis));
}

void Camera::resetCamera()
{
    _position = vmml::vec3f(0.0f,0.0f,0.0f);
    _view = vmml::vec3f(0.0f,0.0f,-1.0f);
}

vmml::mat4f Camera::getViewMatrix(){
    return lookAt(_position, _position + _view, vmml::vec3f::UP);
}