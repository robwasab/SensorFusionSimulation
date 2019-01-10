//
//  OpenGLCone.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLConehat.hpp"

#define TWO_PI (M_PI * 2.0)

OpenGLConehat::OpenGLConehat(int pts_per_revolution,
                             float rgb[3],
                             const char texture_filename[],
                             float center[3],
                             bool finish_initializing):
OpenGLPrimitive(rgb, texture_filename)
{
    
    mTotalPhiSteps = pts_per_revolution;
    mPhiStepRadian = TWO_PI / pts_per_revolution;
    
    if(center == NULL)
    {
        mCenter[0] = 0.0;
        mCenter[1] = 1.0;
        mCenter[2] = 0.0;
    }
    else
    {
        memcpy(mCenter, center, sizeof(float)*3);
    }
    
    if(finish_initializing == true)
    {
        finishInitializing();
    }
}

void OpenGLConehat::initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio)
{
    *drawing_mode = GL_TRIANGLE_FAN;
    *polygon_mode = GL_FILL;
    *mColorTextureRatio = 0;
    
    OpenGLVertexAttributes vertex;
    
    // initialize the vertex with the color
    memcpy(&vertex.colors, rgb, sizeof(float) * 3);
    
    // initialize the vertex texture coord to be in the center
    vertex.textureLocation[0] = 0;
    vertex.textureLocation[1] = 0;
    
    // initialize the top point of the cone to be (0, 1, 0)
    vertex.position[0] = mCenter[0];
    vertex.position[1] = mCenter[1];
    vertex.position[2] = mCenter[2];
    
    verticies->push_back(vertex);
    
    for(int phiStepNum = 0; phiStepNum <= mTotalPhiSteps; phiStepNum++)
    {
        float x;
        float y;
        float z;
        
        float phi = phiStepNum * mPhiStepRadian;
        
        x = cos(phi);
        z = sin(phi);
        y = 0.0;
        
        vertex.position[0] = x;
        vertex.position[1] = y;
        vertex.position[2] = z;
        
        vertex.textureLocation[0] = x;
        vertex.textureLocation[1] = z;
        
        verticies->push_back(vertex);
    }
}
