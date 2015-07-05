#ifndef B_CAMERA_H
#define B_CAMERA_H

#include <memory>
#include <iostream>	
#include "Renderer_GL.h"
#include "vmmlib/vector.hpp"
#include "vmmlib/matrix.hpp"
#include "vmmlib/addendum.hpp"

/** @brief A camera that defines the view and projection matrix in a scene
*	@author Benjamin Bürgisser
*/
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
	*	@param[in] rotationAxes Rotation axes of the camera
	*/
	Camera(const vmml::vec3f &position, const vmml::vec3f &rotationAxes);

	/**	@brief Constructor
	*	@param[in] position Position of the camera
	*	@param[in] rotationAxes Rotation axes of the camera
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	Camera(const vmml::vec3f &position, const vmml::vec3f &rotationAxes, GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);

	/**	@brief Destructor
	*/
    ~Camera();

	/**	@brief Moves the camera forward with a certain speed (based on previous position)
	*	@param[in] camSpeed The velocity of the movement
	*/
    void moveCameraForward(GLfloat camSpeed);

	/**	@brief Moves the camera to the right with a certain speed (based on previous position)
	*	@param[in] camSpeed The velocity of the movement
	*/
	void moveCameraSideward(GLfloat camSpeed);

	/**	@brief Moves the camera upwards with a certain speed (based on previous position)
	*	@param[in] camSpeed The velocity of the movement
	*/
	void moveCameraUpward(GLfloat camSpeed);

	/**	@brief Rotates the camera based on previous orientation
	*	@param[in] rotationX The rotation around the x axis in radians
	*	@param[in] rotationY The rotation around the y axis in radians
	*	@param[in] rotationZ The rotation around the z axis in radians
	*/
	void rotateCamera(GLfloat rotationX, GLfloat rotationY, GLfloat rotationZ);

	/**	@brief Resets camera to the default position orientation
	*/
    void resetCamera();

	/**	@brief Sets the position of the camera
	*	@param[in] position Position of the camera
	*/
	void setPosition(const vmml::vec3f &position);

	/**	@brief Sets the rotation matrix of the camera
	*	@param[in] rotationAxes Rotation axes of the camera
	*/
	void setRotation(const vmml::vec3f &rotationAxes);

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

	/**	@brief Returns the inverse of the view matrix of the camera
	*
	*	The inverse view matrix may be useful to keep an object at the cameras position 
	*/
	vmml::mat4f getInverseViewMatrix();

	/**	@brief Returns the view projection of the camera
	*/
	vmml::mat4f getProjectionMatrix();

	/**	@brief Returns the position of the camera
	*/
	vmml::vec3f getPosition();

	/**	@brief Returns the rotation matrix of the camera
	*/
	vmml::mat4f getRotation();

	/**	@brief Returns the orientation of the camera
	*/
	vmml::vec3f getForward();

	/**	@brief Returns the right vector of the camera
	*/
	vmml::vec3f getRight();

	/**	@brief Returns the up vector of the camera
	*/
	vmml::vec3f getUp();

	/* Static Functions */

	/**	@brief Create a simple look at matrix
	*	@param[in] eye Specifies the position of the eye point
	*	@param[in] target Specifies the position of the reference point
	*	@param[in] up Specifies the direction of the up vector
	*/
	static vmml::mat4f lookAt(const vmml::vec3f &eye, const vmml::vec3f &target, const vmml::vec3f &up);

	/**	@brief Create a 3D perspective
	*	@param[in] fov Field of view
	*	@param[in] aspect Aspect ratio
	*	@param[in] near Near clipping plane
	*	@param[in] far Far clipping plane
	*/
	static vmml::mat4f createPerspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far);
    
private:

	/* Variables */

	vmml::vec3f _position;
	vmml::vec3f _rotationAxes;

	GLfloat _fov, _aspect, _near, _far;
};

typedef std::shared_ptr<Camera> CameraPtr;

#endif /* defined(B_CAMERA_H) */
