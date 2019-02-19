//
//  OpenGLGraph.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/26/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLGraph_hpp
#define OpenGLGraph_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "OpenGLShader.hpp"
#include "OpenGLDrawable.hpp"
#include "OpenGLPrimitive.hpp"

#include <vector>

// This class draws data in NDC: +1 to -1 in x and y.
// It has no concept of what a pixel is, or how many pixels the window has.
class OpenGLGraph
{
    
private:
    float mColorTextureRatio;
    
    GLuint mVAO;
    GLuint mVBO;
    GLuint mTextureId;
    bool mTextureEnabled;
    float * mRGB;
    
    size_t mPlotDataLen;
    
    float mMaximumX;
    float mMaximumY;
    glm::mat4 mTransform;
    
public:
    class Point
    {
    public:
        GLfloat x;
        GLfloat y;
    };
    Point * mPlotData;
    
    static OpenGLShader * mShader;
    
    OpenGLGraph(size_t num_points=256, float max_x=1.0f, float max_y=1.0f);
    virtual ~OpenGLGraph();
    
    void setMaximumDimensions(float max_x, float max_y);
    
    // implementor updates the plot data in this callback
    virtual void updatePlotData(Point * data, size_t len) = 0;
    
    virtual const char * getTitle(void) {
        return "Untitled";
    }
    
    // draws data with the maximums scaled to 1
    virtual void drawWithTransform(glm::mat4 transform) final;
};

#endif /* OpenGLGraph_hpp */
