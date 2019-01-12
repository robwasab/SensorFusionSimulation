//
//  OpenGLCircle.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLCircle.hpp"

static float circle_center[3] = {0.0f, 0.0f, 0.0f};

OpenGLCircle::OpenGLCircle(int pts_per_revolution,
                           float rgb[3],
                           const char texture_filename[],
                           bool finish_initializing):
OpenGLConehat(pts_per_revolution,
              rgb,
              texture_filename,
              circle_center,
              false)
{
    mPrismEdge = NULL;
    
    if(true == finish_initializing)
    {
        finishInitializing();
    }
}

void OpenGLCircle::initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                       const float rgb[],
                                       GLuint * drawing_mode,
                                       GLuint * polygon_mode,
                                       float * mColorTextureRatio)
{
    OpenGLConehat::initializeVerticies(verticies,
                                       rgb,
                                       drawing_mode,
                                       polygon_mode,
                                       mColorTextureRatio);
    
    assert(mPrismEdge == NULL);
    
    std::vector<OpenGLVertexAttributes>::iterator iterator;
    
    iterator = verticies->begin();
    
    //skip the first point
    iterator++;
    
    mPrismEdge = new std::vector<OpenGLVertexAttributes>();
    
    for(/* no init */; iterator != verticies->end(); iterator++)
    {
        mPrismEdge->push_back(*iterator);
    }
}

std::vector<OpenGLVertexAttributes> * OpenGLCircle::getPrismEdge(void)
{
    return mPrismEdge;
}
