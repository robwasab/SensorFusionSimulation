//
//  OpenGLFloor.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/21/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLFloor.hpp"

OpenGLFloor::OpenGLFloor(float width, float height, float spacing):
OpenGLPrimitive(OpenGLPrimitive::COLOR_GREY, NULL)
{
    assert(spacing > 0);
    mSpacing = spacing;
    mWidth   = width;
    mHeight  = height;
    
    finishInitializing();
}

static OpenGLVertexAttributes createAttrFromParameters(float x, float z, const float rgb[3])
{
    OpenGLVertexAttributes attr;
    memcpy(attr.colors, rgb, sizeof(attr.colors));
    
    attr.normal[0] = 0.0f;
    attr.normal[1] = 1.0f;
    attr.normal[2] = 0.0f;
    
    attr.position[0] = x;
    attr.position[1] = 0.0f;
    attr.position[2] = z;
    
    attr.textureLocation[0] = x;
    attr.textureLocation[1] = z;
    
    return attr;
}

void OpenGLFloor::initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                      const float rgb[],
                                      GLuint * drawing_mode,
                                      GLuint * polygon_mode,
                                      float * mColorTextureRatio)
{
    size_t xdir_num_lines = (size_t) floorf(mWidth / mSpacing);
    
    float xstart = -mWidth/2.0f;
    float zstart = -mHeight/2.0f;
    float zstop  =  mHeight/2.0f;
    
    
    for(size_t k = 0; k < xdir_num_lines; k++)
    {
        OpenGLVertexAttributes attr;
        
        attr = createAttrFromParameters(xstart, zstart, rgb);
        
        verticies->push_back(attr);
        
        attr = createAttrFromParameters(xstart, zstop, rgb);
        
        verticies->push_back(attr);
        
        xstart += mSpacing;
    }
    
    float xstop = mWidth/2.0f;
    xstart = -mWidth/2.0f;
    
    size_t zdir_num_lines = (size_t) floorf(mHeight / mSpacing);
    
    for(size_t k = 0; k < zdir_num_lines; k++)
    {
        OpenGLVertexAttributes attr;
        
        attr = createAttrFromParameters(xstart, zstart, rgb);
        
        verticies->push_back(attr);
        
        attr = createAttrFromParameters(xstop, zstart, rgb);
        
        verticies->push_back(attr);
        
        zstart += mSpacing;
    }
    
    *drawing_mode = GL_LINES;
    *polygon_mode = GL_LINE;
}
