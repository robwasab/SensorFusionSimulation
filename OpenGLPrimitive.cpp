//
//  OpenGLDrawable.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrimitive.hpp"
#include "OpenGLShader.hpp"
#include "stb_image.hpp"
#include "OpenGLArrow.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define VERTEX_SHADER_FILENAME        "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/OpenGLPrimitive_VertexShader.fsh"
#define FRAGMENT_SHADER_FILENAME      "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/OpenGLPrimitive_FragmentShader.fsh"

static void initalizeTexture(const char filename[],
                             GLuint * texture_id,
                             GLuint picture_data_format,
                             bool flip);

OpenGLShader * OpenGLPrimitive::mShader = NULL;

float OpenGLPrimitive::COLOR_BLUE[]       = {0.0, 0.0, 1.0};
float OpenGLPrimitive::COLOR_RED[]        = {1.0, 0.0, 0.0};
float OpenGLPrimitive::COLOR_GREEN[]      = {0.0, 1.0, 0.0};
float OpenGLPrimitive::COLOR_YELLOW[]     = {1.0, 1.0, 0.0};
float OpenGLPrimitive::COLOR_ORANGE[]     = {1.0, 0.6, 0.0};
float OpenGLPrimitive::COLOR_PURPLE[]     = {0.4, 0.0, 1.0};
float OpenGLPrimitive::COLOR_DARK_GREEN[] = {0.0, 0.6, 0.0};
float OpenGLPrimitive::COLOR_PINK[]       = {0.8, 0.4, 1.0};
float OpenGLPrimitive::COLOR_GREY[]       = {0.5, 0.5, 0.5};
float OpenGLPrimitive::COLOR_BLACK[]      = {0.0, 0.0, 0.0};
float OpenGLPrimitive::COLOR_WHITE[]      = {1.0, 1.0, 1.0};

OpenGLPrimitive::OpenGLPrimitive(float rgb[], const char texture_filename[])
{    
    if(OpenGLPrimitive::mShader == NULL)
    {
        printf("Initializing OpenGLPrimitive shader\n");
        
        OpenGLPrimitive::mShader = new OpenGLShader(VERTEX_SHADER_FILENAME, FRAGMENT_SHADER_FILENAME);
    }
    else
    {
        //printf("OpenGLPrimitive shader already intialized!\n");
    }
    
    mTextureId = 0;
    mTextureEnabled = false;
    
    if(texture_filename != NULL)
    {
        initalizeTexture(texture_filename, &mTextureId, GL_RGB, true);
        mTextureEnabled = true;
    }
    
    if(rgb == NULL)
    {
        memcpy(mRGB, COLOR_ORANGE, sizeof(mRGB));
    }
    else
    {
        memcpy(mRGB, rgb, sizeof(mRGB));
    }
    
    mVAO = 0;
    mVBO = 0;
    
    mDrawingMode = GL_TRIANGLES;
    mPolygonMode = GL_LINE;
    mColorTextureRatio = 1.0;
    mFinishedInitializing = false;
    mNormalArrow = NULL;
}

void OpenGLPrimitive::finishInitializing(void)
{
    if(mFinishedInitializing == true)
    {
        // call has already been made
        assert(false);
    }
    mFinishedInitializing = true;
    
    std::vector<OpenGLVertexAttributes> verticies;
    
    initializeVerticies(&verticies, mRGB, &mDrawingMode, &mPolygonMode, &mColorTextureRatio);
    
    if(mTextureEnabled)
    {
        mColorTextureRatio = 0.0;
    }
    else
    {
        mColorTextureRatio = 1.0;
    }
    
    assert(verticies.size() > 0);
    
    mNumVerticies = verticies.size();
    mVerticies = new OpenGLVertexAttributes[mNumVerticies];
    
    std::vector<OpenGLVertexAttributes>::iterator iterator;
    
    size_t k = 0;
    
    for(iterator = verticies.begin(); iterator != verticies.end(); ++iterator)
    {
        mVerticies[k++] = *iterator;
    }
    
    // Open GL part
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(OpenGLVertexAttributes) * mNumVerticies, mVerticies, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    void * pos_offset = (void *) offsetof(OpenGLVertexAttributes, position);
    void * color_offset = (void *) offsetof(OpenGLVertexAttributes, colors);
    void * texture_offset = (void *) offsetof(OpenGLVertexAttributes, textureLocation);
    
    GLsizei stride = sizeof(OpenGLVertexAttributes);
    
    // position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, pos_offset);
    
    // color data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, color_offset);
    
    // texture data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, texture_offset);
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLPrimitive::~OpenGLPrimitive()
{
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteTextures(1, &mTextureId);
    if(NULL != mNormalArrow)
    {
        delete mNormalArrow;
    }
    delete [] mVerticies;
}

void OpenGLPrimitive::setColorToTextureRatio(float ratio)
{
    if(ratio > 1.0 || ratio < 0.0) return;
    
    mColorTextureRatio = ratio;
}

void OpenGLPrimitive::drawWithShaderAndTransform(glm::mat4 view_transform)
{
    assert(mFinishedInitializing == true);
    
    glm::mat4 transform = view_transform * this->getTransformationMatrix();
    
    assert(OpenGLPrimitive::mShader != NULL);
    
    OpenGLPrimitive::mShader->use();
    
    OpenGLPrimitive::mShader->setMat4("transform", transform);
    
    if(mTextureEnabled)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
        // set the uniform to use GL_TEXTURE0
        mShader->setInt("uniform_texture", 0);
    }
    
    // we want the color of whatever the texture or vertex color says
    mShader->setFloat("uniform_color_scale", 1.0);
    
    // set the uniform color ratio
    mShader->setFloat("uniform_color_ratio", mColorTextureRatio);
    
    glBindVertexArray(mVAO);
    glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode);
    
    glDrawArrays(mDrawingMode, 0, (GLsizei) mNumVerticies);
    
    if(false == mTextureEnabled)
    {
        // we want black
        mShader->setFloat("uniform_color_scale", 0.0);
        
        glDrawArrays(GL_LINES, 0, (GLsizei) mNumVerticies);
    }
}

void OpenGLPrimitive::drawNormalsWithTransform(glm::mat4 view_transform)
{
    assert(mFinishedInitializing == true);
    
    if(mNormalArrow == NULL)
    {
        mNormalArrow = new OpenGLArrow(COLOR_RED);
        mNormalArrow->setScale(0.1);
    }
    
    OpenGLArrow * arrow = (OpenGLArrow *) mNormalArrow;
    
    for(int k = 0; k < mNumVerticies; k++)
    {
        OpenGLVertexAttributes attr = mVerticies[k];
        arrow->setPosition(attr.position[0], attr.position[1], attr.position[2]);
        arrow->setVectorDimensions(attr.normal[0], attr.normal[1], attr.normal[2]);
        arrow->setScale(0.1f);
        
        glm::mat4 transform = view_transform * this->getTransformationMatrix();
        
        arrow->drawWithShaderAndTransform(transform);
    }
}


static void initalizeTexture(const char filename[],
                             GLuint * texture_id,
                             GLuint picture_data_format,
                             bool flip)
{
    int width, height, nrChannels;
    
    stbi_set_flip_vertically_on_load(flip);
    
    uint8_t * data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    assert(data != NULL);
    
    printf("texture width : %d\n", width);
    printf("texture height: %d\n", height);
    printf("texture color channels: %d\n", nrChannels);
    
    glGenTextures(1, texture_id);
    glBindTexture(GL_TEXTURE_2D, *texture_id);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    glTexImage2D(/*target*/ GL_TEXTURE_2D,
                 /*level*/  0,
                 /*internal format*/ GL_RGB,
                 /*width*/  width,
                 /*height*/ height,
                 /*border*/ 0,
                 /*pixel data format*/ picture_data_format,
                 /*pixel data type*/ GL_UNSIGNED_BYTE,
                 /*data*/ data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
}

