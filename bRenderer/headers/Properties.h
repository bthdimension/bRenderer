#ifndef B_PROPERTIES_H
#define B_PROPERTIES_H

#include <memory>
#include <unordered_map>
#include "vmmlib/matrix.hpp"

#include "Shader.h"

/** @brief Properties such as matrices, vectors and scalars that can be associated with a model
*	@author Benjamin Bürgisser
*/
class Properties
{
public:
	/* Typedefs */
	typedef std::unordered_map<std::string, vmml::mat4f>    Matrix4x4Map;
	typedef std::unordered_map<std::string, vmml::mat3f>    Matrix3x3Map;
	typedef std::unordered_map<std::string, vmml::vec4f>    Vector4Map;
	typedef std::unordered_map<std::string, vmml::vec3f>    Vector3Map;
	typedef std::unordered_map<std::string, float>          ScalarMap;

	/* Functions */

	/**	@brief Returns all 4x4 matrices
	*/
	const Matrix4x4Map	&getMatrices4x4()												{ return _matrices4x4; }

	/**	@brief Returns a 4x4 matrix
	*	@param[in] name Name of the matrix
	*/
	vmml::mat4f			getMatrix4x4(const std::string &name)							{ return _matrices4x4[name]; }

	/**	@brief Returns all 3x3 matrices
	*/
	const Matrix3x3Map	&getMatrices3x3()												{ return _matrices3x3; }

	/**	@brief Returns a 3x3 matrix
	*	@param[in] name Name of the matrix
	*/
	vmml::mat3f			getMatrix3x3(const std::string &name)							{ return _matrices3x3[name]; }

	/**	@brief Returns all 4x1 vectors
	*/
	const Vector4Map	&getVectors4()													{ return _vectors4; }

	/**	@brief Returns a 4x1 vector
	*	@param[in] name Name of the vector
	*/
	vmml::vec4f			getVector4(const std::string &name)								{ return _vectors4[name]; }

	/**	@brief Returns all 3x1 vectors 
	*/
	const Vector3Map	&getVectors3()													{ return _vectors3; }

	/**	@brief Returns a 3x1 vector
	*	@param[in] name Name of the vector
	*/
	vmml::vec3f			getVector3(const std::string &name)								{ return _vectors3[name]; }

	/**	@brief Returns all scalars
	*/
	const ScalarMap		&getScalars()													{ return _scalars; }

	/**	@brief Returns a scalar 
	*	@param[in] name Name of the scalar
	*/
	float				getScalar(const std::string &name)								{ return _scalars[name]; }

	/**	@brief Sets all 4x4 matrices
	*	@param[in] arg 4x4 matrices
	*/
	void				setMatrices4x4(const Matrix4x4Map& arg)							{ _matrices4x4 = arg; }

	/**	@brief Sets a 4x4 matrix
	*	@param[in] name Name of the matrix
	*	@param[in] arg 4x4 matrix
	*/
	void				setMatrix(const std::string &name, const vmml::mat4f &arg)		{ _matrices4x4[name] = arg; }

	/**	@brief Sets all 3x3 matrices
	*	@param[in] arg 3x3 matrices
	*/
	void				setMatrices3x3(const Matrix3x3Map& arg)							{ _matrices3x3 = arg; }

	/**	@brief Sets a 3x3 matrix
	*	@param[in] name Name of the matrix
	*	@param[in] arg 3x3 matrix
	*/
	void				setMatrix(const std::string &name, const vmml::mat3f &arg)		{ _matrices3x3[name] = arg; }

	/**	@brief Sets all 4x1 vectors
	*	@param[in] arg 4x1 Vectors
	*/
	void				setVectors4(const Vector4Map& arg)								{ _vectors4 = arg; }

	/**	@brief Sets a 4x1 vector
	*	@param[in] name Name of the vector
	*	@param[in] arg 4x1 vector
	*/
	void				setVector(const std::string &name, const vmml::vec4f &arg)		{ _vectors4[name] = arg; }

	/**	@brief Sets all 3x1 vectors
	*	@param[in] arg 3x1 vectors
	*/
	void				setVectors3(const Vector3Map& arg)								{ _vectors3 = arg; }

	/**	@brief Sets a 3x1 vector
	*	@param[in] name Name of the vector
	*	@param[in] arg 3x1 vector
	*/
	void				setVector(const std::string &name, const vmml::vec3f &arg)		{ _vectors3[name] = arg; }

	/**	@brief Sets all scalars
	*	@param[in] arg Scalar
	*/
	void				setScalars(const ScalarMap &arg)									{ _scalars = arg; }

	/**	@brief Sets a scalar
	*	@param[in] name Name of the scalar
	*	@param[in] arg Scalar
	*/
	void				setScalar(const std::string &name, float arg)					{ _scalars[name] = arg; }

	/**	@brief Pass properties to specified shader
	*	@param[in] shader The shader the properties are passed to
	*/
	void				passToShader(ShaderPtr shader)				
	{ 
		shader->setUniforms(_matrices4x4);
		shader->setUniforms(_matrices3x3);
		shader->setUniforms(_vectors4);
		shader->setUniforms(_vectors3);
		shader->setUniforms(_scalars);
	}


private:

	/* Variables */
	Matrix4x4Map	_matrices4x4;
	Matrix3x3Map	_matrices3x3;
	Vector4Map		_vectors4;
	Vector3Map		_vectors3;
	ScalarMap		_scalars;
};

typedef std::shared_ptr< Properties > PropertiesPtr;

#endif /* defined(B_PROPERTIES_H) */
