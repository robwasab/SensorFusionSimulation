//
//  OpenGLWalker.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/20/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLWalker.hpp"

#include <GLFW/glfw3.h>

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

enum Walker::BODYPART connections[][2] =
{
    {Walker::BODYPART_HEAD,             Walker::BODYPART_CHEST},
    {Walker::BODYPART_CHEST,            Walker::BODYPART_PELVIS},
    {Walker::BODYPART_RIGHT_HAND,       Walker::BODYPART_RIGHT_ELBOW},
    {Walker::BODYPART_RIGHT_ELBOW,      Walker::BODYPART_RIGHT_SHOULDER},
    {Walker::BODYPART_RIGHT_SHOULDER,   Walker::BODYPART_CHEST},
    {Walker::BODYPART_LEFT_HAND,        Walker::BODYPART_LEFT_ELBOW},
    {Walker::BODYPART_LEFT_ELBOW,       Walker::BODYPART_LEFT_SHOULDER},
    {Walker::BODYPART_LEFT_SHOULDER,    Walker::BODYPART_CHEST},
    {Walker::BODYPART_RIGHT_FOOT,       Walker::BODYPART_RIGHT_KNEE},
    {Walker::BODYPART_RIGHT_KNEE,       Walker::BODYPART_RIGHT_HIP},
    {Walker::BODYPART_RIGHT_HIP,        Walker::BODYPART_PELVIS},
    {Walker::BODYPART_LEFT_FOOT,        Walker::BODYPART_LEFT_KNEE},
    {Walker::BODYPART_LEFT_KNEE,        Walker::BODYPART_LEFT_HIP},
    {Walker::BODYPART_LEFT_HIP,         Walker::BODYPART_PELVIS},
};

OpenGLWalker::OpenGLWalker(float period):
OpenGLComposite()
{
    mPeriod = period;
    mWalker = new Walker();
    
    // initialize body part nodes
    for(int k = 0; k < Walker::BODYPART_MAX; k++)
    {
        enum Walker::BODYPART location = (enum Walker::BODYPART) k;
        float * color = NULL;
        
        switch(location)
        {
            default:
                color = OpenGLPrimitive::COLOR_WHITE;
                break;
        }
        
        mBodyParts[k] = new OpenGLSphere(10, color);
        
        
        switch(location)
        {
            case Walker::BODYPART_RIGHT_SHOULDER:
            case Walker::BODYPART_LEFT_SHOULDER:
            case Walker::BODYPART_RIGHT_HAND:
            case Walker::BODYPART_LEFT_HAND:
            case Walker::BODYPART_HEAD:
            case Walker::BODYPART_LEFT_FOOT:
            case Walker::BODYPART_RIGHT_FOOT:
                mBodyParts[k]->setScale(30.0);
                break;
            default:
                mBodyParts[k]->setScale(20.0);
                break;
        }
        
        addDrawable(mBodyParts[k]);
    }
    
    // initialize body part connections
    for(int k = 0; k < Walker::BODYPART_MAX - 1; k++)
    {
        mBodyPartConnections[k] = new OpenGLPrismCylinder(OpenGLPrimitive::COLOR_BLACK);
        mBodyPartConnections[k]->setThickness(2.0);
        addDrawable(mBodyPartConnections[k]);
    }
    
    mWalkingState = STANDING;
    
    mAxes[0] = new OpenGLArrow(OpenGLPrimitive::COLOR_RED);
    mAxes[1] = new OpenGLArrow(OpenGLPrimitive::COLOR_GREEN);
    mAxes[2] = new OpenGLArrow(OpenGLPrimitive::COLOR_BLUE);
    
    addDrawable(mAxes[0]);
    addDrawable(mAxes[1]);
    addDrawable(mAxes[2]);
    
    mIPhone = new OpenGLPrismIPhone();
    mIPhone->setScale(58.6, 10.0, 123.8);
    
    addDrawable(mIPhone);
    
    mLastTime = glfwGetTime();
    
    // dimensions are given in mm, we want meters
    setScale(1.0f / 1000.0f);
    
    mTransform = glm::mat4(1.0f);
    
    mTransform = glm::rotate(mTransform, glm::radians(-90.0f), ROTATION_XAXIS);
    
    advanceWalkerWithRadians(0);
}

OpenGLWalker::~OpenGLWalker()
{
    delete mWalker;
    
    for(int k = 0; k < Walker::BODYPART_MAX; k++)
    {
        delete mBodyParts[k];
    }
    
    for(int k = 0; k < Walker::BODYPART_MAX - 1; k++)
    {
        delete mBodyPartConnections[k];
    }
    
    delete mAxes[0];
    delete mAxes[1];
    delete mAxes[2];
    
    delete mIPhone;
}

glm::vec3 OpenGLWalker::getMaxDimensions(void)
{
    double max_xyz[3] = {0};
    
    mWalker->getMaximumDimensions(max_xyz);
    
    return glm::vec3(max_xyz[0]/1000.f, max_xyz[1]/1000.0f, max_xyz[2]/1000.0f);
}

static glm::vec3 convertBodyPartLocationToVec3(enum Walker::BODYPART location, Walker::Point * locations)
{
    Walker::Point p = locations[location];
    return glm::vec3(p.x, p.y, p.z);
}

void OpenGLWalker::advanceWalkerWithRadians(double radian_increment)
{
    Walker::Point body_part_locations[Walker::BODYPART_MAX] = {0};
    
    mWalker->advanceWithRadians(radian_increment, body_part_locations);
    
    for(int k = 0; k < Walker::BODYPART_MAX; k++)
    {
        Walker::Point p = body_part_locations[k];
        mBodyParts[k]->setPosition(p.x, p.y, p.z);
    }
    
    for(int k = 0; k < Walker::BODYPART_MAX-1; k++)
    {
        enum Walker::BODYPART start = connections[k][0];
        enum Walker::BODYPART end   = connections[k][1];
        
        Walker::Point start_point = body_part_locations[start];
        Walker::Point end_point   = body_part_locations[end];
        
        glm::vec3 connection_vector = glm::vec3(end_point.x, end_point.y, end_point.z) -
                                        glm::vec3(start_point.x, start_point.y, start_point.z);
        
        
        mBodyPartConnections[k]->setVectorDimensions(connection_vector.x,
                                                     connection_vector.y,
                                                     connection_vector.z);
        
        mBodyPartConnections[k]->setPosition(start_point.x,
                                             start_point.y,
                                             start_point.z);
    }
    
    // update pocket coordinate system
    glm::vec3 knee_from_hip = convertBodyPartLocationToVec3(Walker::BODYPART_RIGHT_KNEE, body_part_locations) -
                                convertBodyPartLocationToVec3(Walker::BODYPART_RIGHT_HIP, body_part_locations);
    
    glm::vec3 hip_from_pelivis = convertBodyPartLocationToVec3(Walker::BODYPART_RIGHT_HIP, body_part_locations) -
                                    convertBodyPartLocationToVec3(Walker::BODYPART_PELVIS, body_part_locations);
    
    glm::vec3 origin = knee_from_hip / 4.0f + convertBodyPartLocationToVec3(Walker::BODYPART_RIGHT_HIP, body_part_locations);
    
    glm::vec3 x_axis = glm::normalize(glm::cross(hip_from_pelivis, knee_from_hip));
    glm::vec3 z_axis = glm::normalize(knee_from_hip);
    glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, x_axis));
    
    glm::mat3 pocket_rotation_matrix = glm::mat3(x_axis, y_axis, z_axis);
    glm::quat pocket_rotation_quat   = glm::toQuat(pocket_rotation_matrix);
    
    // draw the iPhone Here...
    origin += y_axis * 10.0f;
    
    mIPhone->setOrientation(pocket_rotation_quat);
    mIPhone->setPosition(origin.x, origin.y, origin.z);
    
    mIPhonePosition = origin;
    mIPhoneOrientation = pocket_rotation_matrix;
    
    x_axis *= 100.0f;
    y_axis *= 100.0f;
    z_axis *= 100.0f;
    
    mAxes[0]->setVectorDimensions(x_axis.x, x_axis.y, x_axis.z);
    mAxes[0]->setPosition(origin.x, origin.y, origin.z);
    mAxes[0]->setThickness(50.0);
    
    mAxes[1]->setVectorDimensions(y_axis.x, y_axis.y, y_axis.z);
    mAxes[1]->setPosition(origin.x, origin.y, origin.z);
    mAxes[1]->setThickness(50.0);
    
    mAxes[2]->setVectorDimensions(z_axis.x, z_axis.y, z_axis.z);
    mAxes[2]->setPosition(origin.x, origin.y, origin.z);
    mAxes[2]->setThickness(50.0);
}

void OpenGLWalker::drawWithShaderAndTransform(glm::mat4 transform)
{
    double current_time = glfwGetTime();
    
    double time = current_time - mLastTime;
    double radian_increment = mPeriod * time * M_PI * 2.0;
    
    mLastTime = current_time;
    
    switch (mWalkingState) {
        case WALK_FORWARD:
            advanceWalkerWithRadians(radian_increment);
            break;
        case WALK_BACKWARD:
            advanceWalkerWithRadians(-radian_increment);
            break;
        case STANDING:
            advanceWalkerWithRadians(0.0f);
            break;
    }
    
    OpenGLComposite::drawWithShaderAndTransform(transform * mTransform);
}

void OpenGLWalker::getIPhoneCoordinateSystem(glm::vec3 * origin, glm::mat4 * orientation)
{
    assert(origin != NULL);
    assert(orientation != NULL);
    
    glm::mat4 transform = this->getTransformationMatrix() * mTransform;
    
    *origin = transform * glm::vec4(mIPhonePosition, 1.0);
    *orientation = mTransform * mIPhoneOrientation;
}

void OpenGLWalker::setWalkingFrequency(double walking_frequency)
{
    assert(walking_frequency >= 0.0);
    
    mPeriod = walking_frequency;
}

void OpenGLWalker::setWalkingState(WalkingState motion)
{
    mWalkingState = motion;
}

