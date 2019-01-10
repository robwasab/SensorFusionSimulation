//
//  main.c
//  HelloWindow
//
//  Created by Robby Tong on 12/23/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
    if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, const char * argv[])
{
    // initialize glfw windowing framework
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow * window;
    window = glfwCreateWindow(800, 600, "1_HelloWindow", NULL, NULL);
    
    if(window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    printf("Successfully initialized glfw!\n");
    
    // initialize glad function pointer manager
    if(0 == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // now we can talk to opengl
    glViewport(0, 0, 800, 600);
    
    // dark green bluish background
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    while(GLFW_FALSE == glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT);
        
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
