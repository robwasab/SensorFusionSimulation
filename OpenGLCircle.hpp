//
//  OpenGLCircle.h
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLCircle_h
#define OpenGLCircle_h

#include "OpenGLConehat.hpp"

class OpenGLCircle: public OpenGLConehat
{
private:
    std::vector<OpenGLVertexAttributes> * mPrismEdge;
    
public:
    OpenGLCircle(int pts_per_revolution=50,
                 float rgb[3]=NULL,
                 const char texture_filename[]=NULL);
   
    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio);

    virtual std::vector<OpenGLVertexAttributes> * getPrismEdge(void);
};

#endif /* OpenGLCircle_h */
