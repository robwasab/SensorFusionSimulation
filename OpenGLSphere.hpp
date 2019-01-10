//
//  OpenGLSphere.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLSphere_hpp
#define OpenGLSphere_hpp

#include <stdio.h>

#include "OpenGLPrimitive.hpp"

class OpenGLSphere: public OpenGLPrimitive
{
private:
    float mPhiStepRadian;
    float mThetaStepRadian;
    
    int mTotalPhiSteps;
    int mTotalThetaSteps;
    
public:
    OpenGLSphere(int pnts_per_revolution=50,
                 float rgb[]=NULL,
                 const char texture_filename[]=NULL);
    
    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio);

};

#endif /* OpenGLSphere_hpp */
