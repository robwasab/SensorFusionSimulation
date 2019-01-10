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
    
    mState = STATE_CAMERA_IDLE;
    last_cursor_x = 0;
    last_cursor_y = 0;
    
    mPhi = radians(90.0f);
    mTheta = radians(-90.0f);
    mR = 3;
    mCameraTarget = vec3(0.0f,0.0f,0.0f);
}

#define PAIR(x,y) ( (x << 16) + y )

void OpenGLDelegate::handleMouseEvent(MouseEventData mouse)
{
    if(mouse.event == MOUSE_EVENT_RELEASE)
    {
        mState = STATE_CAMERA_IDLE;
        return;
    }
    switch(mouse.event)
    {
        case MOUSE_EVENT_RELEASE:
            mState = STATE_CAMERA_IDLE;
            return;
        case MOUSE_EVENT_POSITION:
            break;
        default:
            break;
    }
    
    switch(PAIR(mState, mouse.event))
    {
        case PAIR(STATE_CAMERA_IDLE, MOUSE_EVENT_POSITION):
            last_cursor_y = mouse.cursor_y;
            last_cursor_x = mouse.cursor_x;
            break;
        
        case PAIR(STATE_CAMERA_IDLE, MOUSE_EVENT_SCROLLWHEEL):
        {
            printf("scroll wheel [%.3f   %.3f]\n", mouse.scrollwheel_x, mouse.scrollwheel_y);
            float scale = 0.25;
            float newR = mouse.scrollwheel_y * scale + mR;
            if(newR > 0.5)
            {
                mR = newR;
                mTransformationMatrixDirty = true;
            }
            break;
        }
            
        case PAIR(STATE_CAMERA_IDLE, MOUSE_EVENT_PRESS):
            
            switch(mouse.button)
            {
                case MOUSE_BUTTON_LEFT:
                    mState = STATE_CAMERA_ADJUST_ORIENTATION;
                    break;
                case MOUSE_BUTTON_MIDDLE:
                    printf("middle button pressed!");
                    break;
                case MOUSE_BUTTON_RIGHT:
                    mState = STATE_CAMERA_ADJUST_POSITION;
                    break;
                default:
                    break;
            }
            break;
        
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
            
            
        }
            
        default:
            break;
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
    
    mLookAtTransform = lookAt(position, mCameraTarget, cameraUp);
}


glm::mat4 OpenGLDelegate::getTransformationMatrix(void)
{
    if(true == mTransformationMatrixDirty)
    {
        updateLookAtTransform();
        mTransformationMatrix = mat4(1.0f);
        
        // the camera coordinate system treats z as the axis pointing upwards.
        // however, in open gl, z points out of the screen.
        mTransformationMatrix = rotate(mTransformationMatrix, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        
        mTransformationMatrix = mPerspectiveTransform * mLookAtTransform * mTransformationMatrix;
        mTransformationMatrixDirty = false;
    }
    return mTransformationMatrix;
}
