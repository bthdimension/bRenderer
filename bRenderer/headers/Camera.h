#ifndef B_CAMERA_H
#define B_CAMERA_H

#include <memory>
#include <iostream>	
#include "Renderer_GL.h"
#include "vmmlib/vector.hpp"
#include "vmmlib/matrix.hpp"
#include "vmmlib/addendum.hpp"

class Camera
{
public:

	/* Functions */
    
	/**	@brief Constructor loading standard values for position, orientation and projection
	*/
    Camera();

	/**	@brief Constructor loading standard values for position and orientation
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	Camera(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Constructor loading standard values for projection
	*	@param[in] position Position of the camera
	*	@param[in] orientation Orientation of the camera in radians
	*/
	Camera(vmml::vec3f position, vmml::vec3f orientation);

	/**	@brief Constructor
	*	@param[in] position Position of the camera
	*	@param[in] orientation Orientation of the camera in radians
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	Camera(vmml::vec3f position, vmml::vec3f orientation, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Destructor
	*/
    ~Camera();

	/**	@brief Moves the camera forward with a certain speed
	*	@param[in] camSpeed The velocity of the movement
	*/
    void moveCamera(GLfloat camSpeed);

	/**	@brief Rotates the camera around a given axis
	*	@param[in] axis The axis to rotate around
	*	@param[in] rotation The rotation in radians
	*/
    void rotateCamera(const vmml::vec3f &axis, GLfloat rotation);

	/**	@brief Resets camera to position (0,0,0) and aligns it to z-axis
	*/
    void resetCamera();

	/**	@brief Sets field of view
	*	@param[in] fov Field of view
	*/
	void setFieldOfView(GLfloat fov);

	/**	@brief Sets aspect ratio
	*	@param[in] aspect Aspect ratio
	*/
	void setAspectRatio(GLfloat aspect);

	/**	@brief Sets near clipping plane
	*	@param[in] near Near clipping plane
	*/
	void setNearClippingPlane(GLfloat near);

	/**	@brief Sets far clipping plane
	*	@param[in] far Far clipping plane
	*/
	void setFarClippingPlane(GLfloat far);
    
	/**	@brief Returns the view matrix of the camera
	*/
    vmml::mat4f getViewMatrix();

	/**	@brief Returns the view projection of the camera
	*/
	vmml::mat4f getProjectionMatrix();

	/* Static Functions */

	/**	@brief Create a simple look at matrix
	*	@param[in] eye Specifies the position of the eye point
	*	@param[in] target Specifies the position of the reference point
	*	@param[in] up Specifies the direction of the up vector
	*/
	static vmml::mat4f lookAt(vmml::vec3f eye, vmml::vec3f target, vmml::vec3f up);

	/**	@brief Create a 3D perspective
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	static vmml::mat4f createPerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);
    
private:

	/* Variables */

	vmml::vec3f _position, _orientation;

	GLfloat _fov = 90.0f; 
	GLfloat _aspect = 4.0f / 3.0f;
	GLfloat _near = -1.0f;
	GLfloat _far = 1280.0f;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif /* defined(B_CAMERA_H) */
