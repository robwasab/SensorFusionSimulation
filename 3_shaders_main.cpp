//
//  3_shaders_main.c
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/23/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "OpenGLShader.hpp"
#include "utility.hpp"

#define VERTEX_SHADER_FILENAME      "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/3_shaders_vertex_shader.fsh"
#define FRAGMENT_SHADER_FILENAME    "/Users/robbytong/Documents/OpenGL/Tutorials/LearnOpenGL/XCode/3_shaders_fragment_shader.fsh"

struct DrawableArrays
{
    GLuint vertex_buffer_object;
    GLuint vertex_attrib_object;
    OpenGLShader * shader;
};

static void processInput(GLFWwindow * window);
static void initialize_triangle(struct DrawableArrays * triangle);
static void draw_triangle(struct DrawableArrays * obj);


class OpenGLDelegate: public UtilityOpenGLDelegate
{
private:
    struct DrawableArrays mTriangle = {0};
    
public:
    const char * windowName(void)
    {
        return "Shader";
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
    
    void initialized(GLFWwindow * window)
    {
        initialize_triangle(&mTriangle);
    }
    
    void windowFrameBufferResized(GLFWwindow * window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    
    void updateFrame(GLFWwindow * window)
    {
        processInput(window);
        draw_triangle(&mTriangle);
    }
};


int main(int argc, const char * argv[])
{
    OpenGLDelegate delegate;
    
    Utility_startOpenglWithDelegate(&delegate);
    return 0;
}


/**
 * Called at the beginning of each frame refresh iteration to check user input.
 */
static void processInput(GLFWwindow * window)
{
    if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void draw_triangle(struct DrawableArrays * obj)
{
    assert(obj->shader != NULL);
    
    obj->shader->use();
    
    glBindVertexArray(obj->vertex_attrib_object);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // color
    double time = glfwGetTime();
    
    float freq = 0.5; // Hz
    float omega = 2.0 * M_PI * freq;
    float amplitude = 0.5;
    
    GLfloat offsetx = amplitude * cos(time * omega);
    GLfloat offsety = amplitude * sin(time * omega);
    GLfloat offsetz = 0;
    
    obj->shader->setVec3("offset", offsetx, offsety, offsetz);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

/**
 * Called at program initialization once to load the vertices into the GPU.
 */
static void initialize_triangle(struct DrawableArrays * obj)
{
    float vertices[] =
    {
        // positions         // colors
        -0.5f, -0.5f, +0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        +0.5f, -0.5f, +0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        +0.0f, +0.5f, +0.0f, 0.0f, 0.0f, 1.0f, // top
    };
    
    // Create vertex attribute object.
    glGenVertexArrays(1, &obj->vertex_attrib_object);
    
    // Bind the newly created object so subsequent operations are performed on it.
    glBindVertexArray(obj->vertex_attrib_object);
    
    glGenBuffers(1, &obj->vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vertex_buffer_object);
    
    // After binding vertex_buffer_object to GL_ARRAY_BUFFER, any calls made
    // using GL_ARRAY_BUFFER as the target will affect the bound buffer -
    // vertex_buffer_object.
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // load and compile the vertex and fragment shaders
    obj->shader = new OpenGLShader(VERTEX_SHADER_FILENAME, FRAGMENT_SHADER_FILENAME);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(/*vertex attr index*/ 0,
                          /*num components*/ 3,
                          /*data ype*/ GL_FLOAT,
                          /*normalized*/ GL_FALSE,
                          /*stride*/ sizeof(float) * 6,
                          /*offset*/ (void*) 0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(/*vertex attr index*/ 1,
                          /*num components*/ 3,
                          /*data ype*/ GL_FLOAT,
                          /*normalized*/ GL_FALSE,
                          /*stride*/ sizeof(float) * 6,
                          /*offset*/ (void*)(3 * sizeof(float)));
    
    // unbind the attribute from the target.
    glBindVertexArray(0);
    
    // unbind the buffer from the target.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

