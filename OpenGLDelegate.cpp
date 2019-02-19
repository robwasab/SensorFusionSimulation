//
//  OpenGLDelegate.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLDelegate.hpp"

using namespace glm;

OpenGLDelegate::OpenGLDelegate(int width, int height, float near_plane, float far_plane)
{
    mWidth = width;
    mHeight = height;
    mAspectRatio = (1.0f * width) / (1.0f * height);
    
    mPerspectiveTransform = perspective(radians(45.0f), mAspectRatio, near_plane, far_plane);
    updateLookAtTransform();
    mTransformationMatrixDirty = true;
    
    mCameraState = STATE_CAMERA_IDLE;
    mUserInputState = STATE_USER_INPUT_ADJUSTS_CAMERA;
    
    last_cursor_x = 0;
    last_cursor_y = 0;
    
    mPhi = radians(90.0f);
    mTheta = radians(-90.0f);
    mR = 3;
    mCameraTarget = vec3(0.0f,0.0f,0.0f);
    
    mLastFPSMeasTime = glfwGetTime();
    mMeasuredFPS = 0.0;
    mFrameCount = 0;
}

#define PAIR(x,y) ( (x << 16) + y )

void OpenGLDelegate::modeOrbitOriginMouseEvent(MouseEventData mouse)
{
    if(mouse.event == MOUSE_EVENT_RELEASE)
    {
        mCameraState = STATE_CAMERA_IDLE;
        return;
    }
    switch(mouse.event)
    {
        case MOUSE_EVENT_RELEASE:
            mCameraState = STATE_CAMERA_IDLE;
            return;
        case MOUSE_EVENT_POSITION:
            break;
        default:
            break;
    }
    
    switch(PAIR(mCameraState, mouse.event))
    {
        case PAIR(STATE_CAMERA_IDLE, MOUSE_EVENT_POSITION):
            last_cursor_y = mouse.cursor_y;
            last_cursor_x = mouse.cursor_x;
            break;
            
        case PAIR(STATE_CAMERA_IDLE, MOUSE_EVENT_SCROLLWHEEL):
        {
            printf("scroll wheel [%.3f   %.3f]\n", mouse.scrollwheel_x, mouse.scrollwheel_y);
            float scale = 0.25;
            float newR = mouse.scrollwheel_y * scale * (0.05 * mR) + mR;
            
            if(newR > 0.5)
            {
                mR = newR;
                mTransformationMatrixDirty = true;
            }
            break;
        }
            
        case PAIR(STATE_CAMERA_IDLE, MOUSE_EVENT_PRESS):
        {
            switch(mouse.button)
            {
                case MOUSE_BUTTON_LEFT:
                    mCameraState = STATE_CAMERA_ADJUST_ORIENTATION;
                	break;
                case MOUSE_BUTTON_MIDDLE:
                    printf("middle button pressed!");
                	break;
                case MOUSE_BUTTON_RIGHT:
                    mCameraState = STATE_CAMERA_ADJUST_POSITION;
                	break;
                default:
                    break;
            }
            break;
        }
            
        case PAIR(STATE_CAMERA_ADJUST_ORIENTATION, MOUSE_EVENT_POSITION):
        {
            float delta_x = mouse.cursor_x - last_cursor_x;
            float delta_y = mouse.cursor_y - last_cursor_y;
            
            //printf("[%.3f   %.3f]\n", delta_x, delta_y);
            
            float scale = 0.5;
            
            mPhi -= delta_x * scale;
            mTheta -= delta_y * scale;
            
            mTransformationMatrixDirty = true;
            break;
        }
            
        case PAIR(STATE_CAMERA_ADJUST_POSITION, MOUSE_EVENT_POSITION):
        {
            break;
        }
            
        default:
        {
            break;
        }
    }
}

void OpenGLDelegate::setCameraOrbitRadius(float radius)
{
    assert(radius >= 0);
    mR = radius;
    
    mTransformationMatrixDirty = true;
}

void OpenGLDelegate::modeRotateObjectMouseEvent(MouseEventData mouse)
{
    float scale = 180;

    switch(PAIR(mOrientationState, mouse.event))
    {
        case PAIR(STATE_ORIENTATION_IDLE, MOUSE_EVENT_POSITION):
        {
            last_cursor_x = mouse.cursor_x;
            last_cursor_y = mouse.cursor_y;
            break;
        }
        
        case PAIR(STATE_ORIENTATION_IDLE, MOUSE_EVENT_PRESS):
        {
            if(mouse.button == MOUSE_BUTTON_LEFT)
            {
                mOrientationState = STATE_ORIENTATION_ADJUST_ROLL_AND_PITCH;
            }
            else if(mouse.button == MOUSE_BUTTON_MIDDLE)
            {
                mOrientationState = STATE_ORIENTATION_IDLE;
            }
            else if(mouse.button == MOUSE_BUTTON_RIGHT)
            {
                mOrientationState = STATE_ORIENTATION_ADJUST_YAW_AND_PITCH;
            }
            break;
        }
        
        case PAIR(STATE_ORIENTATION_ADJUST_ROLL_AND_PITCH, MOUSE_EVENT_PRESS):
        {
            if(mouse.button == MOUSE_BUTTON_LEFT)
            {
                mOrientationState = STATE_ORIENTATION_IDLE;
            }
            break;
        }
        
        case PAIR(STATE_ORIENTATION_ADJUST_ROLL_AND_PITCH, MOUSE_EVENT_POSITION):
        {
            float inverse = -1.0f;
            mObjectRoll  = (mouse.cursor_x - last_cursor_x) * scale * inverse;
            mObjectPitch = (mouse.cursor_y - last_cursor_y) * scale * inverse;
            
            setOrientationWithYawPitchRoll(mObjectYaw, mObjectPitch, mObjectRoll);
            break;
        }
            
        case PAIR(STATE_ORIENTATION_ADJUST_YAW_AND_PITCH, MOUSE_EVENT_PRESS):
        {
            if(mouse.button == MOUSE_BUTTON_RIGHT)
            {
                mOrientationState = STATE_ORIENTATION_IDLE;
            }
            break;
        }
        
        case PAIR(STATE_ORIENTATION_ADJUST_YAW_AND_PITCH, MOUSE_EVENT_POSITION):
        {
            float inverse = -1.0f;
            mObjectYaw   = (mouse.cursor_x - last_cursor_x) * scale;
            mObjectPitch = (mouse.cursor_y - last_cursor_y) * scale * inverse;
            
            setOrientationWithYawPitchRoll(mObjectYaw, mObjectPitch, mObjectRoll);
            break;
        }
    }
}

void OpenGLDelegate::handleMouseEvent(MouseEventData mouse)
{
    switch(mUserInputState)
    {
        case STATE_USER_INPUT_ADJUSTS_CAMERA:
            modeOrbitOriginMouseEvent(mouse);
            break;
        
        case STATE_USER_INPUT_ADJUSTS_OBJECT_ORIENTATION:
            modeRotateObjectMouseEvent(mouse);
            break;
    }
}

void OpenGLDelegate::handleKeyboardEvent(KeyboardEventData keyboard_event_data)
{
    if(keyboard_event_data.key   == KEYBOARD_KEY_SPACE &&
       keyboard_event_data.event == KEYBOARD_EVENT_PRESS)
    {
        switch(mUserInputState)
        {
            case STATE_USER_INPUT_ADJUSTS_CAMERA:
                mUserInputState = STATE_USER_INPUT_ADJUSTS_OBJECT_ORIENTATION;
                mOrientationState = STATE_ORIENTATION_IDLE;
                
                printf("Orientation Mode!\n");
                break;
                
            case STATE_USER_INPUT_ADJUSTS_OBJECT_ORIENTATION:
                mUserInputState = STATE_USER_INPUT_ADJUSTS_CAMERA;
                mCameraState = STATE_CAMERA_IDLE;
                
                printf("Camera Mode!\n");
                break;
        }
    }
    
    else if(keyboard_event_data.key   == KEYBOARD_KEY_R &&
            keyboard_event_data.event == KEYBOARD_EVENT_PRESS)
    {
        mObjectYaw   = 0;
        mObjectPitch = 0;
        mObjectRoll  = 0;
        setOrientationWithYawPitchRoll(mObjectYaw, mObjectPitch, mObjectRoll);
    }
}

glm::vec3 OpenGLDelegate::cameraPositionCartesian(void)
{
    float x = mR * sin(mTheta) * cos(mPhi);
    float y = mR * sin(mTheta) * sin(mPhi);
    float z = mR * cos(mTheta);
    
    return glm::vec3(x, y, z);
}

glm::vec3 OpenGLDelegate::returnCartesianFromSpherical(glm::vec3 spherical)
{
    vec3 col0 = vec3(sin(mTheta)*cos(mPhi), sin(mTheta)*sin(mPhi), cos(mTheta));
    vec3 col1 = vec3(cos(mTheta)*cos(mPhi), cos(mTheta)*sin(mPhi),-sin(mTheta));
    vec3 col2 = vec3(-sin(mPhi), cos(mPhi), 0);
    
    mat3 transform = glm::mat3(col0, col1, col2);
    
    return transform * spherical;
}

void OpenGLDelegate::updateLookAtTransform(void)
{
    vec3 position = cameraPositionCartesian();
    vec3 thetaUnit = vec3(0.0f, 1.0f, 0.0f);
    vec3 cameraUp = returnCartesianFromSpherical(thetaUnit);
    
    position += mCameraTarget;
    
    mLookAtTransform = lookAt(position, mCameraTarget, cameraUp);
}

glm::mat4 OpenGLDelegate::getViewTransform(void)
{
    if(true == mTransformationMatrixDirty)
    {
        updateLookAtTransform();
        mTransformationMatrix = mat4(1.0f);
        
        // the camera coordinate system treats z as the axis pointing upwards.
        // however, in open gl, z points out of the screen.
        mTransformationMatrix = rotate(mTransformationMatrix, radians(90.0f), ROTATION_XAXIS);
        
        mTransformationMatrix = mPerspectiveTransform * mLookAtTransform * mTransformationMatrix;
        mTransformationMatrixDirty = false;
    }
    return mTransformationMatrix;
}

void OpenGLDelegate::frameUpdateDone(void)
{
    mFrameCount++;
    
    float current_time = glfwGetTime();
    float delta = current_time - mLastFPSMeasTime;
    
    if(delta >= 1.0f)
    {
        mMeasuredFPS = mFrameCount / delta;
        
        mLastFPSMeasTime = current_time;
        mFrameCount = 0;
        
        //printf("Measured FPS: %.3f\n", mMeasuredFPS);
    }
}

void OpenGLDelegate::windowClosed(void)
{
    
}

glm::vec3 OpenGLDelegate::getCameraTarget(void)
{
    return mCameraTarget;
}

void OpenGLDelegate::setCameraTarget(glm::vec3 target)
{
    mCameraTarget = glm::mat3(rotate(glm::mat4(1.0f), radians(90.0f), ROTATION_XAXIS)) * target;
    mTransformationMatrixDirty = true;
}

