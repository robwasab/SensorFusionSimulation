//
//  OpenGLComposite.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLComposite.hpp"

OpenGLComposite::OpenGLComposite():
OpenGLDrawable()
{
    mDrawables = new std::vector<OpenGLDrawable *>;
}

OpenGLComposite::~OpenGLComposite()
{
    std::vector<OpenGLDrawable *>::iterator iterator;
    
    for(iterator = mDrawables->begin(); iterator != mDrawables->end(); ++iterator)
    {
        OpenGLDrawable * drawable = *iterator;
        delete drawable;
    }
    
    delete mDrawables;
}

void OpenGLComposite::addDrawable(OpenGLDrawable * drawable)
{
    mDrawables->push_back(drawable);
}

void OpenGLComposite::drawWithShaderAndTransform(glm::mat4 view_transform)
{
    std::vector<OpenGLDrawable *>::iterator iterator;
    
    glm::mat4 model_transform = this->getTransformationMatrix();
    
    glm::mat4 total_transform = view_transform * model_transform;
    
    for(iterator = mDrawables->begin(); iterator != mDrawables->end(); ++iterator)
    {
        OpenGLDrawable * drawable = *iterator;
        drawable->drawWithShaderAndTransform(total_transform);
    }
}
