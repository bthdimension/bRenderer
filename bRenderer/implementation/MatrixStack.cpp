//
//  MatrixStack.cpp
//
//  Created by Benjamin on 15/05/14.
//
//

#include "../headers/MatrixStack.h"

MatrixStack::MatrixStack(){
    
}

MatrixStack::~MatrixStack(){
    while(!modelMatrixStack.empty())
        modelMatrixStack.pop();
    while (!normalMatrixStack.empty()) {
        normalMatrixStack.pop();
    }
}

void MatrixStack::pushTranslation(vmml::mat4f transformationMatrix) {
    modelMatrixStack.push(transformationMatrix);
    
    //for Normal Matrix: negate translation values
    vmml::mat4f transformationMatrixInv = vmml::mat4f::IDENTITY;
    for (int i=0; i<3; ++i) {
        transformationMatrixInv(i,3) = -transformationMatrix(i,3);
    }
    normalMatrixStack.push(transformationMatrixInv);
}
void MatrixStack::pushScaling(vmml::mat4f transformationMatrix) {
    modelMatrixStack.push(transformationMatrix);
    
    //for Normal Matrix: 1/(scaling values)
    vmml::mat4f transformationMatrixInv = vmml::mat4f::IDENTITY;
    for (int i=0; i<3; ++i) {
        if(transformationMatrix(i,i) !=0) {
            transformationMatrixInv.at(i,i) = (1/transformationMatrix(i,i));
        }
    }
    normalMatrixStack.push(transformationMatrixInv);
    
}

void MatrixStack::pushRotation(vmml::mat4f transformationMatrix) {
    modelMatrixStack.push(transformationMatrix);
    
    //for Normal Matrix: transpose rotation values
    vmml::mat4f transformationMatrixInv = vmml::mat4f::IDENTITY;
    transformationMatrix.transpose_to(transformationMatrixInv);
    normalMatrixStack.push(transformationMatrixInv);
}

void MatrixStack::popMatrixStack() {
    modelMatrixStack.pop();
    normalMatrixStack.pop();
}


vmml::mat4f MatrixStack::getModelMatrix() {
    vmml::mat4f transformationMatrix = vmml::mat4f::IDENTITY;
    
    //multiply all Transformations on the Stack to get the final Transformation Matrix
    while ( ! modelMatrixStack.empty() )
    {
        transformationMatrix = transformationMatrix * modelMatrixStack.top();
        modelMatrixStack.pop();
    }
    
    
    return transformationMatrix;
}

vmml::mat4f MatrixStack::getNormalMatrix() {
    vmml::mat4f transformationMatrix = vmml::mat4f::IDENTITY;
    vmml::mat4f newNormalMatrix = vmml::mat4f::IDENTITY;
    
    //We need reverse order!
    std::stack<vmml::mat4f> reverseStack;
    
    while ( ! normalMatrixStack.empty() )
    {
        reverseStack.push(normalMatrixStack.top());
        normalMatrixStack.pop();
    }
    
    
    //multiply all Transformations on the reverseStack to get the final Transformation Matrix
    while ( ! reverseStack.empty() )
    {
        transformationMatrix = transformationMatrix * reverseStack.top();
        reverseStack.pop();
    }
    
    //muss ich das hier nochmals transposen oder reicht es, die Rotationmatrix jeweils schon transposed zu haben?
    transformationMatrix.transpose_to(newNormalMatrix);
    return newNormalMatrix;
}
