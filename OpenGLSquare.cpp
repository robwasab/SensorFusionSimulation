//
//  OpenGLSquare.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/30/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include <glm/glm.hpp>

#include "OpenGLSquare.hpp"
#include "stb_image.hpp"

static void initalizeTexture(const char filename[],
                             GLuint * texture_id,
                             GLuint picture_data_format,
                             bool flip);


OpenGLSquare::OpenGLSquare(const char texture_file[], double r, double b, double g)
{
    initalizeTexture(texture_file, &mTextureId, GL_RGB, true);
    mScale = glm::mat4(1.0);
    mRotation = glm::mat4(1.0);
    mTranslate = glm::mat4(1.0);
    
    float verticies[] =
    {
         // positions     // colors        // texture coords
         0.5, 0.5, 0.0,   1.0, 0.0, 0.0,   1.0, 1.0, // top right
         0.5,-0.5, 0.0,   0.0, 1.0, 0.0,   1.0, 0.0, // bot right
        -0.5,-0.5, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0, // bot left
        
        -0.5,-0.5, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0, // bot left
        -0.5, 0.5, 0.0,   1.0, 1.0, 0.0,   0.0, 1.0, // top left
         0.5, 0.5, 0.0,   1.0, 0.0, 0.0,   1.0, 1.0, // top right
    };
    
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    void * pos_offset = (void *) 0;
    void * color_offset = (void *) (3 * sizeof(float));
    void * texture_offset = (void *) (6 * sizeof(float));
    
    GLsizei stride = sizeof(float) * 8;

    // position data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, pos_offset);
    
    // color data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, color_offset);
    
    // texture data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, texture_offset);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLSquare::setLength(double length)
{
    mScale = glm::mat4(1.0);
    mScale = glm::scale(mScale, glm::vec3(length, length, length));
}

void OpenGLSquare::setRotation(float degrees, glm::vec3 rotation_axis)
{
    mRotation = glm::mat4(1.0);
    mRotation = glm::rotate(mRotation, glm::radians(degrees), rotation_axis);
}

void OpenGLSquare::setLocation(double x, double y, double z)
{
    mTranslate = glm::mat4(1.0);
    mTranslate = glm::translate(mTranslate, glm::vec3(x, y, z));
}
    
void OpenGLSquare::drawWithShaderAndTransform(OpenGLShader * shader, glm::mat4 view_transform)
{
    glm::mat4 transform = view_transform * mTranslate * mRotation * mScale;
    
    shader->use();
    shader->setMat4("transform", transform);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    
    glBindVertexArray(mVAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
