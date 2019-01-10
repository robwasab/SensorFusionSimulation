//
//  OpenGLPrismWall.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrismWall.hpp"

OpenGLPrismWall::OpenGLPrismWall(OpenGLPrimitive * face,
                                 glm::vec3 extrude_direction,
                                 float height,
                                 float rgb[3],
                                 const char texture_filename[]):
OpenGLPrimitive(rgb, texture_filename),
mHeight(height),
mFace(face),
mExtrudeDirection(extrude_direction)
{
    assert(face != NULL);
    assert(height > 0);
    finishInitializing();
}

static glm::vec3 vecFromAttr(OpenGLVertexAttributes attr)
{
    return glm::vec3(attr.position[0], attr.position[1], attr.position[2]);
}


void OpenGLPrismWall::initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                          const float rgb[],
                                          GLuint * drawing_mode,
                                          GLuint * polygon_mode,
                                          float * mColorTextureRatio)
{
    *drawing_mode = GL_TRIANGLE_STRIP;
    *polygon_mode = GL_FILL;
    
    std::vector<OpenGLVertexAttributes> * edge = mFace->getPrismEdge();
    assert(edge != NULL);
    
    std::vector<OpenGLVertexAttributes>::iterator iterator;
    
    float texture_increment = 1.0 / edge->size();
    float texture_x = 0.0f;
    
    for(iterator = edge->begin(); iterator != edge->end(); iterator++)
    {
        OpenGLVertexAttributes vertex_attr = *iterator;
        
        glm::vec3 vertex = vecFromAttr(vertex_attr);
        
        glm::vec3 vertex_extruded = vertex + mExtrudeDirection * mHeight;
        
        
        OpenGLVertexAttributes vertex_extruded_attr;
        
        memcpy(&vertex_attr.colors, rgb, sizeof(float)*3);
        vertex_attr.textureLocation[0] = texture_x;
        vertex_attr.textureLocation[1] = 0.0;
        // no need to modify vertex_attr's position
        
        memcpy(&vertex_extruded_attr.colors, rgb, sizeof(float)*3);
        vertex_extruded_attr.textureLocation[0] = texture_x;
        vertex_extruded_attr.textureLocation[1] = 1.0;
        vertex_extruded_attr.position[0] = vertex_extruded.x;
        vertex_extruded_attr.position[1] = vertex_extruded.y;
        vertex_extruded_attr.position[2] = vertex_extruded.z;
        
        verticies->push_back(vertex_attr);
        verticies->push_back(vertex_extruded_attr);
        
        texture_x += texture_increment;
    }
    
    // we need to add the first point to complete the wall
    iterator = edge->begin();
    OpenGLVertexAttributes vertex_attr = *iterator;
    memcpy(&vertex_attr.colors, rgb, sizeof(float)*3);
    vertex_attr.textureLocation[0] = 0.0;
    vertex_attr.textureLocation[1] = 0.0;
    
    verticies->push_back(vertex_attr);
}

