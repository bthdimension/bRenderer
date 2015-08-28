#include "headers/MatrixStack.h"

void MatrixStack::pushTranslation(const vmml::Matrix4f &transformationMatrix) 
{
	modelMatrixStack.push_back(transformationMatrix);
    
    //for Normal Matrix: negate translation values
    vmml::Matrix4f transformationMatrixInv = vmml::Matrix4f::IDENTITY;
    for (int i=0; i<3; ++i) {
        transformationMatrixInv(i,3) = -transformationMatrix(i,3);
    }
    normalMatrixStack.push_back(transformationMatrixInv);
}
void MatrixStack::pushScaling(const vmml::Matrix4f &transformationMatrix) 
{
	modelMatrixStack.push_back(transformationMatrix);
    
    //for Normal Matrix: 1/(scaling values)
    vmml::Matrix4f transformationMatrixInv = vmml::Matrix4f::IDENTITY;
    for (int i=0; i<3; ++i) {
        if(transformationMatrix(i,i) !=0) {
            transformationMatrixInv.at(i,i) = (1/transformationMatrix(i,i));
        }
    }
	normalMatrixStack.push_back(transformationMatrixInv);
    
}

void MatrixStack::pushRotation(const vmml::Matrix4f &transformationMatrix) 
{
	modelMatrixStack.push_back(transformationMatrix);
    
    //for Normal Matrix: transpose rotation values
    vmml::Matrix4f transformationMatrixInv = vmml::Matrix4f::IDENTITY;
    transformationMatrix.transpose_to(transformationMatrixInv);
	normalMatrixStack.push_back(transformationMatrixInv);
}

void MatrixStack::popMatrixStack() 
{
	modelMatrixStack.pop_back();
	normalMatrixStack.pop_back();
}

void MatrixStack::clearMatrixStack()
{
	modelMatrixStack.clear();
	normalMatrixStack.clear();
}


vmml::Matrix4f MatrixStack::getModelMatrix() 
{
    vmml::Matrix4f transformationMatrix = vmml::Matrix4f::IDENTITY;
    
    //multiply all Transformations on the Stack to get the final Transformation Matrix
	for (std::vector<vmml::Matrix4f>::reverse_iterator it = modelMatrixStack.rbegin(); it != modelMatrixStack.rend(); ++it)
    {
		transformationMatrix = transformationMatrix * *it;
    }
        
    return transformationMatrix;
}

vmml::Matrix4f MatrixStack::getNormalMatrix() 
{
	vmml::Matrix4f transformationMatrix = vmml::Matrix4f::IDENTITY;

	//multiply all Transformations on the Stack to get the final Transformation Matrix
	for (std::vector<vmml::Matrix4f>::iterator it = modelMatrixStack.begin(); it != modelMatrixStack.end(); ++it)
	{
		transformationMatrix = transformationMatrix * *it;
	}

	vmml::Matrix4f normalMatrix = vmml::Matrix4f::IDENTITY;
	transformationMatrix.transpose_to(normalMatrix);
	return normalMatrix;
}
