//
//  WalkingSimulator_main.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/18/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "OpenGLShader.hpp"
#include "utility.hpp"
#include "stb_image.hpp"
#include "OpenGLMeshSquare.hpp"
#include "OpenGLMeshTriangle.hpp"
#include "OpenGLMeshCube.hpp"
#include "OpenGLSphere.hpp"
#include "OpenGLCone.hpp"
#include "OpenGLPrismCylinder.hpp"
#include "OpenGLArrow.hpp"
#include "OpenGLAxis.hpp"
#include "OpenGLMath.hpp"
#include "OpenGLCircle.hpp"
#include "OpenGLSquare.hpp"
#include "OpenGLPrismIPhone.hpp"
#include "AttitudeSensor.hpp"
#include "OpenGLWalker.hpp"
#include "OpenGLFloor.hpp"
#include "OpenGLText.hpp"
#include "OpenGLGraph.hpp"
#include "OpenGLGraphGrid.hpp"
#include "SensorSimulator.hpp"

class MyOpenGLGraph: public OpenGLGraph
{
public:
    MyOpenGLGraph():
    OpenGLGraph()
    {
        
    }
    
    virtual void updatePlotData(Point * data, size_t len) override
    {
        setMaximumDimensions(1.0, 2.0);
        
        for(size_t k = 0; k < len; k++)
        {
            float x;
            x = k - len/2.0f;
            x /= len/2.0f;
            
            float y;
            y = 2.0f * glm::sin(2.0 * M_PI * x);
            
            data[k].x = x;
            data[k].y = y;
        }
    }
};

class MyOpenGLDelegate: public OpenGLDelegate
{
private:
    OpenGLAxis * mGlobalAxis;
    OpenGLWalker * mWalker;
    OpenGLFloor * mFloor;
    OpenGLArrow * mCameraTarget;
    
    OpenGLGraphGrid * mGraphs;
    
    OpenGLPrismIPhone * mIPhone;
    OpenGLAxis * mIPhoneAxis;
    
    float mSampleRate = 60.0f;
    SensorSimulator * mSensorSimulator;
    AttitudeSensor * mAttitudeSensor;
    
    bool mGraphEnabled = false;
    bool mPrintAccelData = false;
    bool mGyroOnly = false;
    bool mFFTEnabled = false;
    
public:
    
    MyOpenGLDelegate():
    OpenGLDelegate(800, 800, 0.1f, 10000.0f)
    {
        
    }
    
    const char * windowName(void) override
    {
        return "Walking Simulator";
    }
    
    void windowClearColor(float * red, float * green, float * blue, float * alpha) override
    {
        *red   = 0.2f;
        *green = 0.3f;
        *blue  = 0.3f;
        *alpha = 1.0f;
    }
    
    void setupGraphs()
    {
        OpenGLGraph * graphs[3];
        
        mAttitudeSensor->getSensorGraphs(graphs);
        
        mGraphs->setGraphWithRowAndCol(graphs[0], 0, 0);
        mGraphs->setGraphWithRowAndCol(graphs[1], 1, 0);
        mGraphs->setGraphWithRowAndCol(graphs[2], 2, 0);
    }
    
    void initialized(GLFWwindow * window) override
    {
        mSensorSimulator = new SensorSimulator(mSampleRate);
        mAttitudeSensor = new AttitudeSensor(glm::quat(0.0f, 0.0f, 0.0f, 0.0f));
        
        mWalker = new OpenGLWalker(1.0);
        mFloor = new OpenGLFloor(50.0f, 100.0f, 2.0f);
        mGraphs = new OpenGLGraphGrid(3, 1, 800, 800);
        
        setupGraphs();
        
        mIPhone = new OpenGLPrismIPhone();
        mIPhone->setScale(0.5/10.0, 1.0/10.0, 1.0/10.0);
        
        mIPhoneAxis = new OpenGLAxis();
        mIPhoneAxis->setScale(1.0/10.0);
        
        mGlobalAxis = new OpenGLAxis();
        mGlobalAxis->setScale(1.0/10.0);
        
        mCameraTarget = new OpenGLArrow(OpenGLPrimitive::COLOR_RED);
        mCameraTarget->setThickness(0.25);
        mCameraTarget->setVectorDimensions(0, 0.25, 0);
        mCameraTarget->setOrientation(180.0, ROTATION_XAXIS);
        
        setCameraOrbitRadius(glm::length(mWalker->getMaxDimensions()));
        mGraphEnabled = false;
        
        setCameraTarget(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    
    void windowFrameBufferResized(GLFWwindow * window, int width, int height) override
    {
        glViewport(0, 0, width, height);
        mGraphs->windowResizedTo(width, height);
    }
    
    void setOrientationWithYawPitchRoll(float yaw, float pitch, float roll) override
    {
    }
    
    void handleKeyboardEvent(KeyboardEventData keyboard_event_data) override
    {
        //OpenGLDelegate::handleKeyboardEvent(keyboard_event_data);
        
        if(keyboard_event_data.event == KEYBOARD_EVENT_PRESS)
        {
            switch(keyboard_event_data.key)
            {
                case KEYBOARD_KEY_R:
                    // reset walker
                    break;
                case KEYBOARD_KEY_W:
                    // toggle walking
                    break;
                case KEYBOARD_KEY_G:
                    // toggle graph
                    mGraphEnabled ^= true;
                    break;
                case KEYBOARD_KEY_F:
                    mFFTEnabled ^= true;
                    
                    if(mFFTEnabled) {
                        mAttitudeSensor->setFFTMode();
                    } else {
                        mAttitudeSensor->setTimeMode();
                    }
                    break;
                case KEYBOARD_KEY_UP:
                    mWalker->setWalkingState(OpenGLWalker::WALK_FORWARD);
                    mPrintAccelData = true;
                    break;
                case KEYBOARD_KEY_DOWN:
                    mWalker->setWalkingState(OpenGLWalker::WALK_BACKWARD);
                    break;
                case KEYBOARD_KEY_SPACE:
                    mGyroOnly ^= true;
                    if(mGyroOnly) {
                        mAttitudeSensor->enableGyroOnly();
                    }
                    else {
                        mAttitudeSensor->disableGyroOnly();
                    }
                    break;
                default:
                    break;
            }
        }
        else if(keyboard_event_data.event == KEYBOARD_EVENT_RELEASE)
        {
            switch(keyboard_event_data.key)
            {
                case KEYBOARD_KEY_DOWN:
                case KEYBOARD_KEY_UP:
                    mWalker->setWalkingState(OpenGLWalker::STANDING);
                    mPrintAccelData = false;
                    break;
                default:
                    break;
            }
        }
        
    }
    
    void drawMarkerAt(glm::vec3 pos)
    {
        float target_arrow_height = 0.25;
        
        mCameraTarget->setPosition(pos.x, pos.y + target_arrow_height, pos.z);
        mCameraTarget->drawWithShaderAndTransform(getViewTransform());
        
        //printf("[%.3f   %.3f   %.3f]\n", pos.x, pos.y, pos.z);
    }
    
    void updateFrame(GLFWwindow * window, double cursor_posx, double cursor_posy) override
    {
        glm::mat4 view_transform = getViewTransform();
        
        mWalker->drawWithShaderAndTransform(view_transform);
        mFloor->drawWithShaderAndTransform(view_transform);
        
        glm::mat4 identity = glm::mat4(1.0f);
        
        if(mGraphEnabled) {
            mGraphs->drawWithTransform(identity);
        }
        
        // simulate sensor data
        
        glm::vec3 iphone_position;
        glm::mat4 iphone_orientation;
        
        mWalker->getIPhoneCoordinateSystem(&iphone_position, &iphone_orientation);
        
        glm::quat iphone_orientation_quat = glm::toQuat(glm::mat3(iphone_orientation));
        
        iphone_position.x += 0.25;
        
        mSensorSimulator->updateWithSensorToWorldQuaternionAndPosition(iphone_orientation_quat, iphone_position);
        
        float iphone_acce_data[3];
        float iphone_gyro_data[3];
        float iphone_magn_data[3];
        
        mSensorSimulator->getUpdatedSensorData(iphone_gyro_data, iphone_acce_data, iphone_magn_data);
        
        mAttitudeSensor->sensorUpdate(1.0/mSampleRate,
                                      iphone_gyro_data,
                                      iphone_acce_data,
                                      iphone_magn_data);
        
        glm::quat calculated_attitude = mAttitudeSensor->getAttitude();
        
        float * sensor_data = iphone_acce_data;
        
        if(mPrintAccelData)
        {
            //printf("%.9f, %.9f, %.9f\n", iphone_acce_data[0], iphone_acce_data[1], iphone_acce_data[2]);
        }
        
        mIPhoneAxis->setXMagnitude(sensor_data[0]);
        mIPhoneAxis->setYMagnitude(sensor_data[1]);
        mIPhoneAxis->setZMagnitude(sensor_data[2]);
        
        mIPhoneAxis->setPosition(iphone_position);
        mIPhoneAxis->setOrientation(calculated_attitude);
        
        mIPhone->setPosition(iphone_position);
        mIPhone->setOrientation(calculated_attitude);
    
        mIPhone->drawWithShaderAndTransform(view_transform);
        mIPhoneAxis->drawWithShaderAndTransform(view_transform);
        //drawMarkerAt(iphone_position);
        
        mGlobalAxis->drawWithShaderAndTransform(view_transform);
        
        setCameraTarget(glm::vec3(iphone_position.x, 1.0f, iphone_position.z));
    }
    
    void windowClosed(void) override
    {
        delete mWalker;
        delete mFloor;
        delete mGraphs;
        delete mIPhone;
        delete mCameraTarget;
    }
};


int main(int argc, const char * argv[])
{
    MyOpenGLDelegate delegate;
    
    Utility_startOpenglWithDelegate(&delegate);
    return 0;
}
