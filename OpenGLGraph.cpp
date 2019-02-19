//
//  OpenGLGraph.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/26/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLGraph.hpp"

#define VERTEX_SHADER       "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/OpenGLGraph_VertexShader.fsh"
#define FRAGMENT_SHADER     "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/OpenGLGraph_FragmentShader.fsh"

OpenGLShader * OpenGLGraph::mShader = NULL;

OpenGLGraph::OpenGLGraph(size_t num_points, float max_x, float max_y)
{
    assert(max_x > 0);
    assert(max_y > 0);
    assert(num_points >= 4);
    
    mMaximumX = max_x;
    mMaximumY = max_y;
    mPlotDataLen = num_points;
    mRGB = OpenGLPrimitive::COLOR_RED;
    
    if(mShader == NULL)
    {
        mShader = new OpenGLShader(VERTEX_SHADER, FRAGMENT_SHADER);
    }
    
    mTransform = glm::mat4(1.0f);
    mTransform = glm::scale(mTransform, glm::vec3(1.0f/mMaximumX, 1.0f/mMaximumY, 1.0f));
    
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    
    glBufferData(GL_ARRAY_BUFFER,               // target
                 sizeof(Point) * mPlotDataLen,  // size of buffer
                 NULL,                          // data to copy
                 GL_DYNAMIC_DRAW);              // usage
    
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(0,                    // vertex attribute to target
                          2,                    // number of components per attr
                          GL_FLOAT,             // data type of each component in attr
                          GL_FALSE,             // normalized?
                          sizeof(Point),        // stride
                          0);                   // offset in element
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    mPlotData = new Point[mPlotDataLen];
    memset(mPlotData, 0, sizeof(Point) * mPlotDataLen);
}

OpenGLGraph::~OpenGLGraph()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);

    delete mPlotData;
}

void OpenGLGraph::setMaximumDimensions(float max_x, float max_y)
{
    assert(max_x > 0);
    assert(max_y > 0);
    
    mMaximumX = max_x;
    mMaximumY = max_y;
    
    mTransform = glm::mat4(1.0f);
    mTransform = glm::scale(mTransform, glm::vec3(1.0f/mMaximumX, 1.0f/mMaximumY, 1.0f));
}

// draws data with the maximums scaled to 1
void OpenGLGraph::drawWithTransform(glm::mat4 transform)
{
    glm::mat4 view_transform = transform * mTransform;
    
    mShader->use();
    mShader->setMat4("transform", view_transform);
    mShader->setVec3("color", mRGB[0], mRGB[1], mRGB[2]);
    
    updatePlotData(mPlotData, mPlotDataLen);
    
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    
    glBufferSubData(GL_ARRAY_BUFFER,                // target
                    0,                              // offset
                    sizeof(Point) * mPlotDataLen,   // size
                    mPlotData);
    
    glDrawArrays(OpenGLPrimitive::VERTEX_INTERPRET_MODE_LINE_STRIP, 0, (GLsizei) mPlotDataLen);
    
    #if 0
    Point border[] =
    {
        {-mMaximumX, mMaximumY},
        { mMaximumX, mMaximumY},
        { mMaximumX,-mMaximumY},
        {-mMaximumX,-mMaximumY},
    };
    
    float * border_color = OpenGLPrimitive::COLOR_BLACK;
    
    mShader->setVec3("color", border_color[0], border_color[1], border_color[2]);
    
    glBufferSubData(GL_ARRAY_BUFFER,     // target
                    0,                   // offset
                    sizeof(Point) * 4,   // size
                    border);
    
    glDrawArrays(OpenGLPrimitive::VERTEX_INTERPRET_MODE_LINE_STRIP, 0, 4);
    #endif
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
