//
//  5_transforms_main.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/25/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.hpp"
#include "utility.hpp"
#include "stb_image.hpp"

#define VERTEX_SHADER_FILENAME        "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/5_transforms_vertex_shader.fsh"
#define FRAGMENT_SHADER_FILENAME      "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/5_transforms_fragment_shader.fsh"
#define TEXTURE_1_FILENAME            "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/wooden_container.jpg"
#define TEXTURE_2_FILENAME            "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/awesome_face.png"

enum ROTATION_AXIS
{
    ROTATION_AXIS_X,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
};

class OpenGLDelegate: public UtilityOpenGLDelegate
{
private:
    GLuint mTextureIds[2];
    GLuint mVertexAttrObj;
    GLuint mVertexBuffObj;
    OpenGLShader * mShader;
    
    glm::mat4 mTransform;
    
public:
    const char * windowName(void)
    {
        return "Textures";
    }
    
    void windowDimensions(int * width, int * height)
    {
        *width  = 800;
        *height = 600;
    }
    
    void windowClearColor(float * red, float * green, float * blue, float * alpha)
    {
        *red   = 0.2f;
        *green = 0.3f;
        *blue  = 0.3f;
        *alpha = 1.0f;
    }
    
    glm::mat4 rotateWithDegrees(glm::mat4 transform, float degrees, ROTATION_AXIS axis)
    {
        glm::vec3 rotation_vector;
        
        switch(axis)
        {
            case ROTATION_AXIS_X:
                rotation_vector = glm::vec3(1.0, 0.0, 0.0);
                break;
            case ROTATION_AXIS_Y:
                rotation_vector = glm::vec3(0.0, 1.0, 0.0);
                break;
            case ROTATION_AXIS_Z:
                rotation_vector = glm::vec3(0.0, 0.0, 1.0);
                break;
        }
        
        return glm::rotate(transform, glm::radians(degrees), rotation_vector);
    }
    
    glm::mat4 translateBy(glm::mat4 transform, float x, float y, float z)
    {
        return glm::translate(transform, glm::vec3(x, y, z));
    }
    
    void initializeTransform(void)
    {
        // init as identity
        mTransform = glm::mat4(1.0f);
        mTransform = translateBy(mTransform, 0.0, 0.25, 0.0);
        
        mTransform = rotateWithDegrees(mTransform, 45.0f, ROTATION_AXIS_Z);
        
        mShader->setMat4("transform", mTransform);
    }
    
    void initalizeTexture(const char filename[],
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
    
    void initialized(GLFWwindow * window)
    {
        memset(mTextureIds, 0, sizeof(mTextureIds));
        
        initalizeTexture(TEXTURE_1_FILENAME, &mTextureIds[0], GL_RGB,  /*flip*/false);
        initalizeTexture(TEXTURE_2_FILENAME, &mTextureIds[1], GL_RGBA, /*flip*/true);
        
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
        
        glGenVertexArrays(1, &mVertexAttrObj);
        glBindVertexArray(mVertexAttrObj);
        
        glGenBuffers(1, &mVertexBuffObj);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffObj);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
        
        mShader = new OpenGLShader(VERTEX_SHADER_FILENAME, FRAGMENT_SHADER_FILENAME);
        
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
        
        mShader->use();
        
        // set the uniform to use GL_TEXTURE0
        mShader->setInt("texture_one", 0);
        
        // set the uniform to use GL_TEXTURE1
        mShader->setInt("texture_two", 1);
        
        initializeTransform();
    }
    
    void windowFrameBufferResized(GLFWwindow * window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    
    void updateFrame(GLFWwindow * window, double cursor_posx, double cursor_posy)
    {
        //printf("[%3.3f %3.3f]\n", cursor_posx, cursor_posy);
        mShader->use();
        
        mTransform = glm::mat4(1.0f);
        mTransform = translateBy(mTransform, cursor_posx + 0.25, cursor_posy, 0.0);
        mTransform = rotateWithDegrees(mTransform, 45.0f, ROTATION_AXIS_Z);
        
        mShader->setMat4("transform", mTransform);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTextureIds[0]);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mTextureIds[1]);
        
        glBindVertexArray(mVertexAttrObj);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        mTransform = glm::mat4(1.0f);
        mTransform = translateBy(mTransform, cursor_posx - 0.25, cursor_posy, 0.0);
        mTransform = rotateWithDegrees(mTransform, 45.0f, ROTATION_AXIS_Z);
        
        mShader->setMat4("transform", mTransform);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
    }
};


int main(int argc, const char * argv[])
{
    OpenGLDelegate delegate;
    
    Utility_startOpenglWithDelegate(&delegate);
    return 0;
}
