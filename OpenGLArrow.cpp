//
//  OpenGLArrow.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLArrow.hpp"
#include "OpenGLMath.hpp"

OpenGLArrow::OpenGLArrow(float rgb[3], float arrow_head_to_body_ratio):
OpenGLComposite()
{
    glm::vec3 extrude_direction = glm::vec3(0.0f, 1.0f, 0.0f);
    
    assert(arrow_head_to_body_ratio <= 1.0f);
    
    float arrow_head_height = arrow_head_to_body_ratio;
    float arrow_body_height = 1.0f - arrow_head_to_body_ratio;
    
    mArrowBody = new OpenGLPrismCylinder(extrude_direction,
                                         1.0f,
                                         rgb,
                                         NULL);
    
    mArrowHead = new OpenGLCone(6, rgb, NULL);
    
    mArrowBody->setScale(0.15, arrow_body_height, 0.15);
    mArrowHead->setScale(0.20, arrow_head_height, 0.20);
    
    mArrowHead->addPosition(0.0f, arrow_body_height, 0.0f);
    
    mArrowBody->addPosition(0.0f, arrow_body_height/2.0f, 0.0f);
    
    mVectorTransformation = glm::mat4(1.0f);
    mVectorTranslation = glm::mat4(1.0f);
    
    this->addDrawable(mArrowBody);
    this->addDrawable(mArrowHead);
}

OpenGLArrow::~OpenGLArrow()
{
    delete mArrowHead;
    delete mArrowBody;
}

glm::mat4 OpenGLArrow::getTransformationMatrix(void)
{
    return mVectorTranslation * mVectorTransformation * OpenGLComposite::getTransformationMatrix();
}

void OpenGLArrow::setPosition(float x, float y, float z)
{
    mVectorTranslation = glm::mat4(1.0f);
    addPosition(x, y, z);
}

void OpenGLArrow::addPosition(float x, float y, float z)
{
    mVectorTranslation = glm::translate(mVectorTranslation, glm::vec3(x, y, z));
    OpenGLComposite::addPosition(0, 0, 0);
}


void OpenGLArrow::setVectorDimensions(float x, float y, float z)
{
    // We always start off as a unit vector (0, 1, 0) pointing in the y direction
    // That is the policy for this program.
    
    glm::vec3 start = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 end = glm::vec3(x, y, z);
    glm::vec3 end_normalized = glm::normalize(end);
    
    glm::mat3 rotate = OpenGLMath::calculateRotationMatrix2(start, end_normalized);
    
    mVectorTransformation = glm::mat4(rotate) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, length(end), 1.0f));
}
