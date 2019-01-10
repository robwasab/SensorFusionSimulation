//
//  OpenGLPrismWall.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLPrismWall_hpp
#define OpenGLPrismWall_hpp

#include <stdio.h>
#include "OpenGLPrimitive.hpp"

class OpenGLPrismWall: public OpenGLPrimitive
{
private:
    glm::vec3 mExtrudeDirection;
    float mHeight;
    OpenGLPrimitive * mFace;
    
public:
    OpenGLPrismWall(OpenGLPrimitive * face,
                    glm::vec3 extrude_direction,
                    float height,
                    float rgb[3],
                    const char texture_filename[]);

    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio);

};

#endif /* OpenGLPrismWall_hpp */
