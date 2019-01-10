//
//  OpenGLTriangle.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/31/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include "OpenGLMeshTriangle.hpp"
#include <assert.h>
#include <string.h>

using namespace glm;



OpenGLMeshTriangle::OpenGLMeshTriangle(vec3 * pts,
                                       float * rgb,
                                       int depth,
                                       const char texture_filename[]):
mPts(pts),
mRecursionDepth(depth),
OpenGLPrimitive(rgb, texture_filename)
{
    // last class must call finish initializing to load the verticies into open gl.
    finishInitializing();
}

void OpenGLMeshTriangle::initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                             const float rgb[],
                                             GLuint * drawing_mode,
                                             GLuint * polygon_mode,
                                             float * mColorTextureRatio)
{
    assert(mRecursionDepth >= 0);
    
    *polygon_mode = GL_FILL;
    
    mVertexPointer = 0;
    mNumTriangles = (size_t) pow(6, mRecursionDepth);
    
    size_t expected_verticies = 3 * mNumTriangles;
    
    tessellate(mPts[0],
               mPts[1],
               mPts[2],
               mRecursionDepth,
               rgb,
               verticies);
    
    assert(mVertexPointer == expected_verticies);
}

OpenGLMeshTriangle::~OpenGLMeshTriangle()
{
}

void OpenGLMeshTriangle::tessellate(glm::vec3 A, glm::vec3 B, glm::vec3 C, int level, const float rgb[], std::vector<OpenGLVertexAttributes> * verticies)
{
    if(level == 0)
    {
        verticies->push_back(vec2attr(A, rgb));
        verticies->push_back(vec2attr(B, rgb));
        verticies->push_back(vec2attr(C, rgb));
        mVertexPointer += 3;
        return;
    }
    
    assert(level > 0);
    
    vec3 AB = B - A;
    vec3 BC = C - B;
    vec3 CA = A - C;
    
    vec3 mab = A + AB / 2.0f;
    vec3 mbc = B + BC / 2.0f;
    vec3 mca = C + CA / 2.0f;
    
    vec3 centroid = (A + B + C) / 3.0f;
    
    //print_vec3("cen", centroid, level);
    
    int next_level = level - 1;
    
    tessellate(A  , mab, centroid, next_level, rgb, verticies);
    tessellate(mab, B  , centroid, next_level, rgb, verticies);
    tessellate(B  , mbc, centroid, next_level, rgb, verticies);
    tessellate(mbc, C  , centroid, next_level, rgb, verticies);
    tessellate(C  , mca, centroid, next_level, rgb, verticies);
    tessellate(mca, A  , centroid, next_level, rgb, verticies);
}

OpenGLVertexAttributes OpenGLMeshTriangle::vec2attr(glm::vec3 vec, const float * rgb)
{
    OpenGLVertexAttributes attr;
    
    attr.position[0] = vec.x;
    attr.position[1] = vec.y;
    attr.position[2] = vec.z;

    attr.colors[0] = rgb[0];
    attr.colors[1] = rgb[1];
    attr.colors[2] = rgb[2];
    
    attr.textureLocation[0] = vec.x;
    attr.textureLocation[1] = vec.y;
    return attr;
}
