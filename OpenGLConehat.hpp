//
//  OpenGLCone.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLConehat_hpp
#define OpenGLConehat_hpp

#include <stdio.h>

#include "OpenGLPrimitive.hpp"

class OpenGLConehat: public OpenGLPrimitive
{
private:
    float mPhiStepRadian;
    
    int mTotalPhiSteps;
    float mCenter[3];
public:
    OpenGLConehat(int pts_per_revolution=50,
                  float rgb[3]=NULL,
                  const char texture_filename[]=NULL,
                  float center[3]=NULL,
                  bool finish_initializing=true);
    
    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio);

};

#endif /* OpenGLCone_hpp */
