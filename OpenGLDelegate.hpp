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

#include "OpenGLArrow.hpp"
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

enum KeyboardKey
{
    KEYBOARD_KEY_SPACE = GLFW_KEY_SPACE,
    KEYBOARD_KEY_R     = GLFW_KEY_R,
    KEYBOARD_KEY_W     = GLFW_KEY_W,
    KEYBOARD_KEY_F     = GLFW_KEY_F,
    KEYBOARD_KEY_G     = GLFW_KEY_G,
    KEYBOARD_KEY_UP    = GLFW_KEY_UP,
    KEYBOARD_KEY_DOWN  = GLFW_KEY_DOWN,
    KEYBOARD_KEY_LEFT  = GLFW_KEY_LEFT,
    KEYBOARD_KEY_RIGHT = GLFW_KEY_RIGHT,
};

enum KeyboardEvent
{
    KEYBOARD_EVENT_PRESS = GLFW_PRESS,
    KEYBOARD_EVENT_RELEASE = GLFW_RELEASE,
    KEYBOARD_EVENT_REPEAT = GLFW_REPEAT,
};

struct KeyboardEventData
{
    enum KeyboardEvent event;
    enum KeyboardKey key;
    int raw_key;
    int raw_event;
};

typedef struct MouseEventData MouseEventData;

class OpenGLDelegate
{
    enum
    {
        STATE_CAMERA_IDLE               = 1,
        STATE_CAMERA_ADJUST_ORIENTATION = 2,
        STATE_CAMERA_ADJUST_POSITION    = 3,
    } mCameraState = STATE_CAMERA_IDLE;
    
    enum
    {
        STATE_ORIENTATION_IDLE                  = 1,
        STATE_ORIENTATION_ADJUST_ROLL_AND_PITCH = 2,
        STATE_ORIENTATION_ADJUST_YAW_AND_PITCH  = 3,
    } mOrientationState = STATE_ORIENTATION_IDLE;
    
    enum
    {
        STATE_USER_INPUT_ADJUSTS_CAMERA             = 1,
        STATE_USER_INPUT_ADJUSTS_OBJECT_ORIENTATION = 2,
    } mUserInputState = STATE_USER_INPUT_ADJUSTS_CAMERA;
    
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
    
    // object orientation stored as euler angles
    float mObjectYaw;
    float mObjectPitch;
    float mObjectRoll;
    
    // keep track of frames per second
    float mLastFPSMeasTime;
    int   mFrameCount;
    float mMeasuredFPS;
    
    void modeOrbitOriginMouseEvent(MouseEventData mouse);
    void modeRotateObjectMouseEvent(MouseEventData mouse);

public:
    OpenGLDelegate(int width=800, int height=600, float near_plane=0.1f, float far_plane=100.0f);
    
    /**
     * Set the camera orbit radius.
     */
    void setCameraOrbitRadius(float radius);
    
    /**
     * Get the window dimensions.
     */
    virtual void windowDimensions(int * width, int * height)
    {
        *width = mWidth;
        *height = mHeight;
    }
    
    /**
     * Called when there are mouse events. Default implementation orbits around the origin.
     */
    virtual void handleMouseEvent(MouseEventData mouse_event_data);
    
    /**
     * Called when there is keyboard data. Default implementation switches between camera orbit
     * and rotating the object.
     */
    virtual void handleKeyboardEvent(KeyboardEventData keyboard_event_data);
    
    /**
     * Return the name of the window.
     */
    virtual const char * windowName(void) = 0;
    
    /**
     * Get the clear color.
     */
    virtual void windowClearColor(float * red, float * green, float * blue, float * alpha) = 0;
    
    /**
     * Called after open gl has been initialized. Initialize your objects.
     */
    virtual void initialized(GLFWwindow * window) = 0;
    
    /**
     * Called whenever the window is resized.
     */
    virtual void windowFrameBufferResized(GLFWwindow * window, int width, int height) = 0;
    
    /**
     * Callback to application for when it can do opengl stuff.
     * Cursor coords are normalized to the window.
     */
    virtual void updateFrame(GLFWwindow * window, double cursor_posx, double cursor_posy) = 0;
    
    /**
     * Called when the window is in manipulate orientation mode.
     */
    virtual void setOrientationWithYawPitchRoll(float yaw, float pitch, float roll) = 0;
    
    /**
     * Called to after all the open gl operations have been finished.
     */
    virtual void frameUpdateDone(void);
    
    /**
     * OpenGL window closed.
     */
    virtual void windowClosed(void);
    
    /**
     * Set the camera target.
     */
    virtual void setCameraTarget(glm::vec3 target) final;
    
    /**
     * Get the camera target.
     */
    virtual glm::vec3 getCameraTarget(void) final;
    
    glm::vec3 cameraPositionCartesian(void);
    
    glm::vec3 returnCartesianFromSpherical(glm::vec3 spherical);
    
    void updateLookAtTransform(void);
    
    glm::mat4 getViewTransform(void);
    
};

#endif /* OpenGLDelegate_hpp */
