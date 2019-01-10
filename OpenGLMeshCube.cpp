//
//  OpenGLMeshCube.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLMeshCube.hpp"
#include "OpenGLMeshSquare.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "OpenGLPrimitive.hpp"

OpenGLMeshCube::OpenGLMeshCube():
OpenGLComposite()
{
    
    glm::vec3 xaxis = glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 yaxis = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 zaxis = glm::vec3(0.0, 0.0, 1.0);
    
    OpenGLMeshSquare * front  = new OpenGLMeshSquare(OpenGLPrimitive::COLOR_RED);
    OpenGLMeshSquare * back   = new OpenGLMeshSquare(OpenGLPrimitive::COLOR_GREY);
    OpenGLMeshSquare * left   = new OpenGLMeshSquare(OpenGLPrimitive::COLOR_GREY);
    OpenGLMeshSquare * right  = new OpenGLMeshSquare(OpenGLPrimitive::COLOR_GREY);
    OpenGLMeshSquare * top    = new OpenGLMeshSquare(OpenGLPrimitive::COLOR_GREY);
    OpenGLMeshSquare * bottom = new OpenGLMeshSquare(OpenGLPrimitive::COLOR_GREY);
    
    bottom->setOrientation(180.0f, zaxis);
    
    left->setOrientation(90.0f, yaxis);
    right->setOrientation(90.0f, yaxis);
    
    top->setOrientation(-90, xaxis);
    bottom->setOrientation(90, xaxis);
    
    front->setPosition(0, 0, 1);
    back->setPosition(0, 0, -1);
    
    left->setPosition(-1, 0, 0);
    right->setPosition(1, 0, 0);
    
    top->setPosition(0, 1, 0);
    bottom->setPosition(0, -1, 0);
    
    this->addDrawable(top);
    this->addDrawable(bottom);
    this->addDrawable(left);
    this->addDrawable(right);
    this->addDrawable(front);
    this->addDrawable(back);
}
