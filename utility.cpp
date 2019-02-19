//
//  utility.c
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/24/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include "utility.hpp"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static OpenGLDelegate * mDelegate = NULL;
static int mWidth = -1;
static int mHeight = -1;

static Queue<MouseEventData>    mMouseEventDataQueue(16);
static Queue<KeyboardEventData> mKeyboardEventDataQueue(16);

static void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    mWidth = width;
    mHeight = height;
    if(mDelegate != NULL)
    {
        mDelegate->windowFrameBufferResized(window, width, height);
    }
}

static void scroll_wheel_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    MouseEvent enum_event = MOUSE_EVENT_SCROLLWHEEL;
    MouseButton enum_button = MOUSE_BUTTON_NONE;
    
    MouseEventData data;
    data.button = enum_button;
    data.event = enum_event;
    data.cursor_x = 0;
    data.cursor_y = 0;
    data.scrollwheel_x = xoffset;
    data.scrollwheel_y = yoffset;
    
    mMouseEventDataQueue.add(data);
}

static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    MouseButton enum_button = MOUSE_BUTTON_LEFT;
    MouseEvent enum_event = MOUSE_EVENT_POSITION;
    
    switch(button)
    {
        case GLFW_MOUSE_BUTTON_RIGHT:
            enum_button = MOUSE_BUTTON_RIGHT;
            break;
        case GLFW_MOUSE_BUTTON_LEFT:
            enum_button = MOUSE_BUTTON_LEFT;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            enum_button = MOUSE_BUTTON_MIDDLE;
            break;
        default:
            return;
    }
    
    switch(action)
    {
        case GLFW_PRESS:
            enum_event = MOUSE_EVENT_PRESS;
            break;
        case GLFW_RELEASE:
            enum_event = MOUSE_EVENT_RELEASE;
            break;
        default:
            return;
    }
    
    MouseEventData data;
    data.button = enum_button;
    data.event = enum_event;
    data.cursor_x = 0;
    data.cursor_y = 0;
    data.scrollwheel_x = 0;
    data.scrollwheel_y = 0;
    
    //printf("mouse callback!\n");
    mMouseEventDataQueue.add(data);
}

static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    KeyboardEventData data;
    
    switch(key)
    {
        case GLFW_KEY_W:
        case GLFW_KEY_G:
        case GLFW_KEY_R:
        case GLFW_KEY_F:
        case GLFW_KEY_UP:
        case GLFW_KEY_DOWN:
        case GLFW_KEY_LEFT:
        case GLFW_KEY_RIGHT:
        case GLFW_KEY_SPACE:
            data.raw_key = key;
            data.key = (enum KeyboardKey) key;
            break;
        default:
            //printf("unknown key, returning...");
            return;
    }
    
    switch(action)
    {
        case GLFW_PRESS:
        case GLFW_RELEASE:
        case GLFW_REPEAT:
            data.raw_event = action;
            data.event = (enum KeyboardEvent) action;
            break;
        default:
            return;
    }
    
    mKeyboardEventDataQueue.add(data);
}

void Utility_startOpenglWithDelegate(OpenGLDelegate * delegate)
{
    mDelegate = delegate;
    
    // initialize glfw windowing framework
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow * window;
    
    delegate->windowDimensions(&mWidth, &mHeight);
    
    window = glfwCreateWindow(mWidth, mHeight, delegate->windowName(), NULL, NULL);
    
    if(window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(-1);
    }
    
    glfwMakeContextCurrent(window);
    
    printf("Successfully initialized glfw!\n");
    
    // initialize glad function pointer manager
    if(0 == gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        exit(-1);
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_wheel_callback);
    glfwSetKeyCallback(window, key_callback);
    
    assert(mWidth  != -1);
    assert(mHeight != -1);
    
    float red   = 0.0f;
    float green = 0.0f;
    float blue  = 0.0f;
    float alpha = 0.0f;
    delegate->windowClearColor(&red, &green, &blue, &alpha);
    
    // now we can talk to opengl
    glViewport(0, 0, mWidth, mHeight);
    
    // dark green bluish background
    glClearColor(red, green, blue, alpha);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    mDelegate->initialized(window);
    
    while(GLFW_FALSE == glfwWindowShouldClose(window))
    {
        // rendering commands here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // check and call events and swap the buffers
        glfwPollEvents();
        
        double cursor_posx;
        double cursor_posy;
        glfwGetCursorPos(window, &cursor_posx, &cursor_posy);
        
        cursor_posx -= mWidth  / 2.0;
        cursor_posy -= mHeight / 2.0;
        cursor_posx /= mWidth  / 2.0;
        cursor_posy /= mHeight / 2.0;
        cursor_posy *= -1.0;
        
        MouseEventData mouseEvent;
        mouseEvent.event = MOUSE_EVENT_POSITION;
        mouseEvent.button = MOUSE_BUTTON_NONE;
        mouseEvent.cursor_x = cursor_posx;
        mouseEvent.cursor_y = cursor_posy;
        mouseEvent.scrollwheel_x = 0;
        mouseEvent.scrollwheel_y = 0;
        
        mMouseEventDataQueue.add(mouseEvent);
        
        size_t num_mouse_events = mMouseEventDataQueue.size();
        //printf("num_mouse_events: %zu\n", num_mouse_events);
        
        for(size_t k = 0; k < num_mouse_events; k++)
        {
            MouseEventData mouse_event;
            mMouseEventDataQueue.get(&mouse_event);
            mDelegate->handleMouseEvent(mouse_event);
        }
        
        size_t num_key_events = mKeyboardEventDataQueue.size();
        //printf("num_key_events: %zu\n", num_key_events);
        
        for(size_t k = 0; k < num_key_events; k++)
        {
            KeyboardEventData key_event;
            mKeyboardEventDataQueue.get(&key_event);
            mDelegate->handleKeyboardEvent(key_event);
        }
        
        mDelegate->updateFrame(window, cursor_posx, cursor_posy);
        
        glfwSwapBuffers(window);
        
        mDelegate->frameUpdateDone();
    }
    
    glfwTerminate();
}
