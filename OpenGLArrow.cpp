//
//  OpenGLArrow.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLArrow.hpp"

OpenGLArrow::OpenGLArrow(float rgb[3], float arrow_head_to_body_ratio):
OpenGLComposite()
{
    glm::vec3 extrude_direction = glm::vec3(0.0f, 1.0f, 0.0f);
    
    float arrow_head_height = arrow_head_to_body_ratio;
    float arrow_body_height = 1.0f - arrow_head_to_body_ratio;
    
    mArrowBody = new OpenGLPrismCylinder(extrude_direction,
                                         1.0f,
                                         rgb,
                                         NULL);
    
    mArrowHead = new OpenGLCone(10, rgb, NULL);
    
    mArrowBody->setScale(0.15, arrow_body_height, 0.15);
    mArrowHead->setScale(0.20, arrow_head_height, 0.20);
    
    mArrowHead->addPosition(0.0f, arrow_body_height/2.0f, 0.0f);
    
    mArrowHead->addPosition(0.0f, arrow_body_height/2.0f, 0.0f);
    mArrowBody->addPosition(0.0f, arrow_body_height/2.0f, 0.0f);
    
    this->addDrawable(mArrowBody);
    this->addDrawable(mArrowHead);
}

OpenGLArrow::~OpenGLArrow()
{
    delete mArrowHead;
    delete mArrowBody;
}
