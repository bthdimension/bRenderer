#ifndef B_MATRIX_STACK_H
#define B_MATRIX_STACK_H

#include <iostream>
#include "vmmlib/addendum.hpp"
#include "vmmlib/vector.hpp"
#include <stack>

class MatrixStack
{
public:
    MatrixStack();
    ~MatrixStack();

    void pushTranslation(vmml::mat4f transformationMatrix);
    void pushScaling(vmml::mat4f transformationMatrix);
    void pushRotation(vmml::mat4f transformationMatrix);
    void popMatrixStack();
    vmml::mat4f getModelMatrix();
    vmml::mat4f getNormalMatrix();
private:
    std::stack<vmml::mat4f> modelMatrixStack;
    std::stack<vmml::mat4f> normalMatrixStack;
};

#endif /* defined(B_MATRIX_STACK_H) */
