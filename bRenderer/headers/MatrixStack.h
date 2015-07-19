#ifndef B_MATRIX_STACK_H
#define B_MATRIX_STACK_H

#include <memory>
#include <iostream>
#include "vmmlib/util.hpp"
#include "vmmlib/vector.hpp"


/** @brief This is a matrix stack to temporarily store transformations 
*	@author Benjamin Bürgisser
*/
class MatrixStack
{
public:

	/* Functions */

	/**	@brief Constructor
	*/
    MatrixStack();

	/**	@brief Destructor
	*/
    ~MatrixStack();

	/**	@brief Push a translation matrix to the stack
	*	@param[in] transformationMatrix Matrix that moves an object in space
	*/
	void pushTranslation(const vmml::Matrix4f &transformationMatrix);

	/**	@brief Push a scale matrix to the stack
	*	@param[in] transformationMatrix Matrix that scales an object
	*/
	void pushScaling(const vmml::Matrix4f &transformationMatrix);

	/**	@brief Push a rotation matrix to the stack
	*	@param[in] transformationMatrix Matrix that rotates an object
	*/
	void pushRotation(const vmml::Matrix4f &transformationMatrix);

	/**	@brief Delete last element on the stack
	*/
    void popMatrixStack();

	/**	@brief Deletes all matrices in the stack
	*/
	void clearMatrixStack();

	/**	@brief Returns the model matrix as the product of all pushed transformations
	*/
    vmml::Matrix4f getModelMatrix();

	/**	@brief Returns the normal matrix as the product of all pushed transformations
	*/
    vmml::Matrix4f getNormalMatrix();

private:

	/* Variables */

    std::vector<vmml::Matrix4f> modelMatrixStack;
	std::vector<vmml::Matrix4f> normalMatrixStack;
};

typedef std::shared_ptr<MatrixStack> MatrixStackPtr;

#endif /* defined(B_MATRIX_STACK_H) */
