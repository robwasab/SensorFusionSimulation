//
//  OpenGLTriangle.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/31/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#ifndef OpenGLTriangle_hpp
#define OpenGLTriangle_hpp

#include <stdio.h>
#include <stdint.h>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "OpenGLShader.hpp"
#include "OpenGLPrimitive.hpp"


class OpenGLMeshTriangle: public OpenGLPrimitive
{
public:
    glm::vec3 * mPts;
    int mRecursionDepth;
    
    size_t mVertexPointer;
    size_t mNumTriangles;
 
    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio);

    
    void tessellate(glm::vec3 A, glm::vec3 B, glm::vec3 C, int level, const float rgb[], std::vector<OpenGLVertexAttributes> * verticies);

    OpenGLVertexAttributes vec2attr(glm::vec3 vec, const float rgb[]);
    
    OpenGLMeshTriangle(glm::vec3 * pts,
                       float rgb[],
                       int depth = 1,
                       const char texture_filename[] = NULL);
    
    ~OpenGLMeshTriangle();
};
#endif /* OpenGLTriangle_hpp */
