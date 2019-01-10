//
//  OpenGLDrawable.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLPrimitive_hpp
#define OpenGLPrimitive_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include "OpenGLShader.hpp"
#include "OpenGLDrawable.hpp"
#include <vector>

#define TEXTURE_WOODEN_CONTAINER "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/wooden_container.jpg"

#define TEXTURE_EARTH "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/earth_low_resolution.jpg"

struct OpenGLVertexAttributes
{
    float position[3];
    float colors[3];
    float textureLocation[2];
    float normal[3];
};

typedef struct OpenGLVertexAttributes OpenGLVertexAttributes;



class OpenGLPrimitive: public OpenGLDrawable
{

private:
    float mColorTextureRatio;
    
    GLuint mVAO;
    GLuint mVBO;
    GLuint mTextureId;
    bool mTextureEnabled;
    
    GLuint mDrawingMode;
    GLuint mPolygonMode;
    
    OpenGLVertexAttributes * mVerticies;
    size_t mNumVerticies;
    
    bool mFinishedInitializing;
    float mRGB[3];

public:
   
    static OpenGLShader * mShader;
    static float COLOR_BLUE[];
    static float COLOR_RED[];
    static float COLOR_GREEN[];
    static float COLOR_YELLOW[];
    static float COLOR_ORANGE[];
    static float COLOR_PURPLE[];
    static float COLOR_DARK_GREEN[];
    static float COLOR_PINK[];
    static float COLOR_GREY[];
    
    OpenGLPrimitive(float rgb[]=NULL, const char texture_filename[]=NULL);
    virtual ~OpenGLPrimitive();
    
    virtual void initializeVerticies(std::vector<OpenGLVertexAttributes> * verticies,
                                     const float rgb[],
                                     GLuint * drawing_mode,
                                     GLuint * polygon_mode,
                                     float * mColorTextureRatio) = 0;

    void finishInitializing(void);
    
    void setColorToTextureRatio(float ratio);
    
    virtual void drawWithShaderAndTransform(glm::mat4 transform) final;
    
    virtual std::vector<OpenGLVertexAttributes> * getPrismEdge(void)
    {
        return NULL;
    }
};
#endif /* OpenGLPrimitive */
