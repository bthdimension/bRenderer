#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <iostream>	
#include "vmmlib/vector.hpp"
#include "vmmlib/matrix.hpp"
#include "vmmlib/addendum.hpp"

class Camera
{
public:
    
    Camera();
    ~Camera();
    
    vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up);
    void moveCamera(const float camSpeed);
    void rotateCamera(const vmml::vec3f &axis, float rotation);
    void resetCamera();
    
     vmml::mat4f getViewMatrix();
    
private:
    
    vmml::vec3f _position, _view;
    float _speed;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif /* defined(CAMERA_H) */
