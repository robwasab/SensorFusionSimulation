//
//  OpenGLDrawable.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/3/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGLDrawable.hpp"

glm::quat OpenGLDrawable::quaternionFromAngleAxis(float degrees, glm::vec3 axis)
{
    axis = glm::normalize(axis);
    
    // cos
    float c = glm::cos(glm::radians(degrees)/2);
    // sin
    float s = glm::sin(glm::radians(degrees)/2);
    
    return glm::quat(c, s * axis);
}

OpenGLDrawable::OpenGLDrawable()
{
    mScale = glm::mat4(1.0);
    mPosition = glm::mat4(1.0);
    mTransformationMatrix = glm::mat4(1.0);
    mTransformationMatrixDirty = true;
    
    mAttitude = quaternionFromAngleAxis(0, ROTATION_ZAXIS);
}

void OpenGLDrawable::setScale(float xscale, float yscale, float zscale)
{
    mScale = glm::mat4(1.0);
    mScale = glm::scale(mScale, glm::vec3(xscale, yscale, zscale));
    mTransformationMatrixDirty = true;
}

void OpenGLDrawable::setScale(float scale)
{
    setScale(scale, scale, scale);
}

void OpenGLDrawable::setPosition(float x, float y, float z)
{
    mPosition = glm::mat4(1.0);
    mPosition = glm::translate(mPosition, glm::vec3(x, y, z));
    mTransformationMatrixDirty = true;
}

void OpenGLDrawable::setOrientation(glm::quat orientation)
{
    float len = glm::length(orientation);
    assert(len <= 1.1);
    
    mAttitude = orientation;
    mTransformationMatrixDirty = true;
}

void OpenGLDrawable::setOrientation(float degrees, glm::vec3 rotation_axis)
{
    mAttitude = quaternionFromAngleAxis(degrees, rotation_axis);
    mTransformationMatrixDirty = true;
}

glm::quat OpenGLDrawable::getOrientationAsQuaternion(void)
{
    return mAttitude;
}

void OpenGLDrawable::addPosition(float x, float y, float z)
{
    mPosition = glm::translate(mPosition, glm::vec3(x, y, z));
    mTransformationMatrixDirty = true;
}

void OpenGLDrawable::addPositionX(float x)
{
    addPosition(x, 0.0, 0.0);
}

void OpenGLDrawable::addPositionY(float y)
{
    addPosition(0.0, y, 0.0);
}

void OpenGLDrawable::addPositionZ(float z)
{
    addPosition(0.0, 0.0, z);
}

void OpenGLDrawable::addOrientation(float degrees, glm::vec3 rotation_axis)
{
    glm::quat add = quaternionFromAngleAxis(degrees, rotation_axis);
    
    mAttitude = add * mAttitude;
    mTransformationMatrixDirty = true;
}

glm::mat4 OpenGLDrawable::getTransformationMatrix(void)
{
    if(true == mTransformationMatrixDirty)
    {
        mTransformationMatrix = mPosition * glm::toMat4(mAttitude) * mScale;
        mTransformationMatrixDirty = false;
    }
    return mTransformationMatrix;
}
