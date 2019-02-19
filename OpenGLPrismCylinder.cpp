//
//  OpenGLPrismCylinder.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrismCylinder.hpp"
#include "OpenGLCircle.hpp"
#include "OpenGLMath.hpp"

class CylinderFaceGenerator: public OpenGLPrism::PrismFaceGenerator
{
public:
    OpenGLPrimitive * getNextFace(float rgb[3], const char texture_filename[])
    {
        return new OpenGLCircle(6, rgb, texture_filename);
    }
};

OpenGLPrismCylinder::OpenGLPrismCylinder(float rgb[3]):
OpenGLPrism(new CylinderFaceGenerator(),
            glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f,
            rgb,
            NULL)
{
    mVectorTransformation = glm::mat4(1.0f);
    mVectorTranslation = glm::mat4(1.0f);
    mThickness = 1.0f;
}


OpenGLPrismCylinder::OpenGLPrismCylinder(glm::vec3 extrude_direction,
                                         float height,
                                         float rgb[3],
                                         const char texture_filename[]):
OpenGLPrism(new CylinderFaceGenerator(),
            extrude_direction,
            height,
            rgb,
            texture_filename)
{
    mVectorTransformation = glm::mat4(1.0f);
    mVectorTranslation = glm::mat4(1.0f);
    mThickness = 1.0f;
}

glm::mat4 OpenGLPrismCylinder::getTransformationMatrix(void)
{
    return mVectorTranslation * mVectorTransformation * OpenGLComposite::getTransformationMatrix();
}

void OpenGLPrismCylinder::setPosition(float x, float y, float z)
{
    mVectorTranslation = glm::mat4(1.0f);
    addPosition(x, y, z);
}

void OpenGLPrismCylinder::setThickness(float thickness)
{
    assert(thickness > 0.0f);
    
    mThickness = thickness;
}

void OpenGLPrismCylinder::addPosition(float x, float y, float z)
{
    mVectorTranslation = glm::translate(mVectorTranslation, glm::vec3(x, y, z));
    OpenGLComposite::setPosition(0, 0, 0);
}


void OpenGLPrismCylinder::setVectorDimensions(float x, float y, float z)
{
    // We always start off as a unit vector (0, 1, 0) pointing in the y direction
    // That is the policy for this program.
    
    glm::vec3 start = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 end = glm::vec3(x, y, z);
    glm::vec3 end_normalized = glm::normalize(end);
    
    glm::mat3 rotate = OpenGLMath::calculateRotationMatrix2(start, end_normalized);
    
    setScale(mThickness, length(end), mThickness);
    
    mVectorTransformation = glm::mat4(rotate);
}
