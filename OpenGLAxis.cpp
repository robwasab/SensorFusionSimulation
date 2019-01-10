//
//  OpenGLAxis.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLAxis.hpp"

OpenGLAxis::OpenGLAxis():
OpenGLComposite()
{
    mXAxis = new OpenGLArrow(OpenGLPrimitive::COLOR_RED);
    mYAxis = new OpenGLArrow(OpenGLPrimitive::COLOR_GREEN);
    mZAxis = new OpenGLArrow(OpenGLPrimitive::COLOR_BLUE);
    
    mXAxis->setOrientation(-90, ROTATION_ZAXIS);
    mZAxis->setOrientation(+90, ROTATION_XAXIS);
    
    mXAxis->addPosition(0.0, 0.0, 0.0);
    mYAxis->addPosition(0.0, 0.0, 0.0);
    mZAxis->addPosition(0.0, 0.0, 0.0);
    
    this->addDrawable(mXAxis);
    this->addDrawable(mYAxis);
    this->addDrawable(mZAxis);
}

OpenGLAxis::~OpenGLAxis()
{
    delete mXAxis;
    delete mYAxis;
    delete mZAxis;
}
