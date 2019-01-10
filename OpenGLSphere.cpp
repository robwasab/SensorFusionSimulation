//
//  OpenGLSphere.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLSphere.hpp"

#define TWO_PI (M_PI * 2.0)

void phiThetaToXyz(float phi, float theta, float outXYZ[])
{
    float x;
    float y;
    float z;
    
    // the x and y coordinates of a circle as a function of phi on the 2d plane is
    x = cos(phi);
    z = sin(phi);
    
    // the theta coordinate scales the radius of this circle
    x *= sin(theta);
    z *= sin(theta);
    
    // z is +1 at theta=0deg, 0 at theta=90deg, -1 at theta=-180deg
    y = cos(theta);
    
    outXYZ[0] = x;
    outXYZ[1] = y;
    outXYZ[2] = z;
}

OpenGLSphere::OpenGLSphere(int pts_per_revolution,
                           float rgb[],
                           const char texture_filename[]):
OpenGLPrimitive(rgb, texture_filename)
{
    // pts_per_revolution must be even
    assert(pts_per_revolution % 2 == 0);
    
    mPhiStepRadian = 2.0 * M_PI / pts_per_revolution;
    mThetaStepRadian = mPhiStepRadian;
    
    mTotalPhiSteps = pts_per_revolution;
    mTotalThetaSteps = pts_per_revolution / 2;
    
    finishInitializing();
}

static float wrapPhase(float phase)
{
    if (phase > TWO_PI) return phase - TWO_PI;
    if (phase < 0) return phase + TWO_PI;
    return phase;
}

static OpenGLVertexAttributes helperCreateVertexFromPhiThetaRgb(float phi,
                                                         float theta,
                                                         const float rgb[3])
{
    OpenGLVertexAttributes vertex;
    memcpy(&vertex.colors, rgb, sizeof(float)*3);
    
    phiThetaToXyz(phi, theta, vertex.position);
    
    vertex.textureLocation[0] = (TWO_PI - phi) / TWO_PI;
    vertex.textureLocation[1] = (M_PI - theta) / M_PI;

    return vertex;
}

void OpenGLSphere::initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                       const float rgb[],
                                       GLuint * drawing_mode,
                                       GLuint * polygon_mode,
                                       float * mColorTextureRatio)
{
    *drawing_mode = GL_TRIANGLE_STRIP;
    *polygon_mode = GL_FILL;
    *mColorTextureRatio = 0.0;
    
    float phi = 0.0f;
    float theta = 0.0f;
    
    for(int thetaStepNum = 0; thetaStepNum < mTotalThetaSteps; thetaStepNum++)
    {
        for(int phiStepNum = 0; phiStepNum <= mTotalPhiSteps; phiStepNum++)
        {
            phi = phiStepNum * mPhiStepRadian;
            theta = thetaStepNum * mThetaStepRadian;
            
            OpenGLVertexAttributes attr;
            
            attr = helperCreateVertexFromPhiThetaRgb(phi, theta, rgb);
            verticies->push_back(attr);
            
            theta = (thetaStepNum + 1) * mThetaStepRadian;
            phi = (phiStepNum + 1) * mPhiStepRadian;
            
            attr = helperCreateVertexFromPhiThetaRgb(phi, theta, rgb);
            verticies->push_back(attr);
        }
    }
}
