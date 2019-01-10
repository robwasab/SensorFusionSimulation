//
//  OpenGLUnitSquare.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/31/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include "OpenGLMeshSquare.hpp"
#include "OpenGLMeshTriangle.hpp"
#include "OpenGLPrimitive.hpp"

using namespace glm;


OpenGLMeshSquare::OpenGLMeshSquare(float * rgb):
OpenGLComposite()
{
    vec3 pnts[3];
    vec3 corners[4];
    
    corners[0] = vec3(-1, -1, 0);
    corners[1] = vec3(-1, +1, 0);
    corners[2] = vec3(+1, +1, 0);
    corners[3] = vec3(+1, -1, 0);
    
    if(rgb == NULL)
    {
        rgb = OpenGLPrimitive::COLOR_GREY;
    }
    
    for(int k = 0; k < 4; k++)
    {
        pnts[0] = corners[k];
        pnts[1] = corners[(k + 1) % 4];
        pnts[2] = vec3(0, 0, 0);
        int recursive_depth = 2;
        
        OpenGLMeshTriangle * triangle = new OpenGLMeshTriangle(pnts,
                                                               rgb,
                                                               recursive_depth,
                                                               TEXTURE_WOODEN_CONTAINER);
        triangle->setColorToTextureRatio(0.5);
        this->addDrawable(triangle);
    }
}
