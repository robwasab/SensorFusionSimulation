//
//  OpenGLDelegate.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLDelegate_hpp
#define OpenGLDelegate_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Queue.h"

enum MouseEvent
{
    MOUSE_EVENT_PRESS = 1,
    MOUSE_EVENT_RELEASE = 2,
    MOUSE_EVENT_POSITION = 3,
    MOUSE_EVENT_SCROLLWHEEL = 4,
};

enum MouseButton
{
    MOUSE_BUTTON_LEFT = 1,
    MOUSE_BUTTON_RIGHT = 2,
    MOUSE_BUTTON_MIDDLE = 3,
    MOUSE_BUTTON_NONE = 4,
};

struct MouseEventData
{
    enum MouseEvent event;
    enum MouseButton button;
    float cursor_x;
    float cursor_y;
    float scrollwheel_x;
    float scrollwheel_y;
};

typedef struct MouseEventData MouseEventData;

class OpenGLDelegate
{
    enum
    {
        STATE_CAMERA_IDLE = 1,
        STATE_CAMERA_ADJUST_ORIENTATION = 2,
        STATE_CAMERA_ADJUST_POSITION = 3,
        
    } mState;
    
private:
    glm::mat4 mPerspectiveTransform;
    glm::mat4 mLookAtTransform;
    glm::mat4 mTransformationMatrix;
    int mWidth;
    int mHeight;
    float mAspectRatio;
    bool mTransformationMatrixDirty;
    float last_cursor_x;
    float last_cursor_y;
    
    // camera position is stored in spherical coordinates
    float mPhi;
    float mTheta;
    float mR;
    glm::vec3 mCameraTarget;
    
    
public:
    OpenGLDelegate(int width=800, int height=600, float near_plane=0.1f, float far_plane=100.0f);
    
    /**
     * Return the name of the window.
     */
    virtual const char * windowName(void) = 0;
    
    /**
     * Get the window dimensions.
     */
    virtual void windowDimensions(int * width, int * height)
    {
        *width = mWidth;
        *height = mHeight;
    }
    
    /**
     * Get the clear color.
     */
    virtual void windowClearColor(float * red, float * green, float * blue, float * alpha) = 0;
    
    /**
     * Called after open gl has been initialized.
     */
    virtual void initialized(GLFWwindow * window) = 0;
    
    /**
     * Called whenever the window is resized.
     */
    virtual void windowFrameBufferResized(GLFWwindow * window, int width, int height) = 0;
    
    /**
     * Called when there are mouse events.
     */
    virtual void handleMouseEvent(MouseEventData mouse_event_data);
    
    /**
     * Called everytime the application needs to interact with opengl.
     * Cursor coords are normalized to the window.
     */
    virtual void updateFrame(GLFWwindow * window, double cursor_posx, double cursor_posy) = 0;
    
    glm::vec3 cameraPositionCartesian(void);
    glm::vec3 returnCartesianFromSpherical(glm::vec3 spherical);

    void updateLookAtTransform(void);
    
    glm::mat4 getTransformationMatrix(void);
    
};

#endif /* OpenGLDelegate_hpp */
