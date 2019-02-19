//
//  OpenGLFloor.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/21/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLFloor_hpp
#define OpenGLFloor_hpp

#include <stdio.h>
#include "OpenGLPrimitive.hpp"

class OpenGLFloor: public OpenGLPrimitive
{
private:
    float mSpacing;
    float mWidth;
    float mHeight;
public:
    OpenGLFloor(float width, float height, float spacing);
    
    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio) override;

};

#endif /* OpenGLFloor_hpp */
