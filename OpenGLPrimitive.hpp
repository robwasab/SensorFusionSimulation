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

#define TEXTURE_IPHONE_FACE "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/iphone_face.jpg"

#define TEXTURE_IPHONE_BACK "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/iphone_back.jpg"

#define TEXTURE_IPHONE_METAL "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/aluminum.jpg"


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

    OpenGLDrawable * mNormalArrow;
public:
   
    enum VERTEX_INTERPRET_MODE
    {
        VERTEX_INTERPRET_MODE_TRIANGLES         = GL_TRIANGLES,
        VERTEX_INTERPRET_MODE_TRIANGLE_STRIP    = GL_TRIANGLE_STRIP,
        VERTEX_INTERPRET_MODE_TRIANGLE_FAN      = GL_TRIANGLE_FAN,
        VERTEX_INTERPRET_MODE_LINES             = GL_LINES,
        VERTEX_INTERPRET_MODE_LINE_STRIP        = GL_LINE_STRIP,
    };
    
    enum POLYGON_FILL_MODE
    {
        POLYGON_FILL_MODE_OUTLINE = GL_LINE,
        POLYGON_FILL_MODE_FILL    = GL_FILL,
    };

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
    static float COLOR_BLACK[];
    static float COLOR_WHITE[];
    
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
    
    void drawNormalsWithTransform(glm::mat4 transform);
    
    virtual std::vector<OpenGLVertexAttributes> * getPrismEdge(void)
    {
        return NULL;
    }
};
#endif /* OpenGLPrimitive */
