//
//  AttitudeSensor.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "AttitudeSensor.hpp"

AttitudeSensor::AttitudeSensor(glm::quat initial_attitude)
{
    mAttitude = initial_attitude;
}

void AttitudeSensor::sensorUpdate(float time_step,
                                  float gyro_data[3],
                                  float accel_data[3],
                                  float mag_data[3])
{
    glm::quat angular_velocity = glm::quat(0.0f, gyro_data[0], gyro_data[1], gyro_data[2]);
    
    //glm::quat next_attitude = mAttitude + (mAttitude * angular_velocity) * time_step * 0.5f;
    
    //next_attitude = glm::normalize(next_attitude);
    
    //mAttitude = next_attitude;
    
    
    glm::vec3 world_gravity = glm::vec3(0.0f, -1.0f,  0.0f);
    glm::vec3 world_north   = glm::vec3(0.0f,  0.0f, -1.0f);
    
    glm::vec3 accel_measure = glm::vec3(accel_data[0], accel_data[1], accel_data[2]);
    glm::vec3 magno_measure = glm::vec3(  mag_data[0],   mag_data[1],   mag_data[2]);
    
    glm::quat gravity_error_vector =
        calculateErrorGradient(world_gravity, accel_measure);
    
    glm::quat magnometer_error_vector =
        calculateErrorGradient(world_north, magno_measure);
    
    float step_size = (0.5 + length(angular_velocity)) * time_step;
    
    glm::quat error_vector = gravity_error_vector + magnometer_error_vector;
    
    if(length(error_vector) > 1E-3)
    {
        float scale = 1.25f;
        mAttitude -= scale * step_size * glm::normalize(error_vector);
        mAttitude = glm::normalize(mAttitude);
    }
}

void print_vec4(const char name[], glm::vec4 v)
{
    printf("%s %.3f   %.3f   %.3f   %.3f\n", name, v.w, v.x, v.y, v.z);
}

glm::quat AttitudeSensor::calculateErrorGradient(glm::vec3 world, glm::vec3 sensor)
{
    // not related to derivatives dx dy dz
    // just short convenient names
    float dx = world.x;
    float dy = world.y;
    float dz = world.z;
    
    float sx = sensor.x;
    float sy = sensor.y;
    float sz = sensor.z;
    
    float q1 = mAttitude.w;
    float q2 = mAttitude.x;
    float q3 = mAttitude.y;
    float q4 = mAttitude.z;
    
    float fx = 2*dx*(0.5-q3*q3-q4*q4) + 2*dy*(q1*q4+q2*q3) + 2*dz*(q2*q4-q1*q3) - sx;
    float fy = 2*dx*(q2*q3-q1*q4) + 2*dy*(0.5-q2*q2-q4*q4) + 2*dz*(q1*q2+q3*q4) - sy;
    float fz = 2*dx*(q1*q3+q2*q4) + 2*dy*(q3*q4-q1*q2) + 2*dz*(0.5-q2*q2-q3*q3) - sz;
    
    //printf("fx: %.3f\n", fx);
    //printf("fy: %.3f\n", fy);
    //printf("fz: %.3f\n", fz);
    
    float j11 =  2*dy*q4-2*dz*q3;
    float j12 =  2*dy*q3+2*dz*q4;
    float j13 = -4*dx*q3+2*dy*q2-2*dz*q1;
    float j14 = -4*dx*q4+2*dy*q1+2*dz*q2;
    
    float j21 = -2*dx*q4+2*dz*q2;
    float j22 =  2*dx*q3-4*dy*q2+2*dz*q1;
    float j23 =  2*dx*q2+2*dz*q4;
    float j24 = -2*dx*q1-4*dy*q4+2*dz*q3;
    
    float j31 =  2*dx*q3-2*dy*q2;
    float j32 =  2*dx*q4-2*dy*q1-4*dz*q2;
    float j33 =  2*dx*q1+2*dy*q4-4*dz*q3;
    float j34 =  2*dx*q2+2*dy*q3;
    
    // contruct the transpose
    float y1 = j11*fx + j21*fy + j31*fz;
    float y2 = j12*fx + j22*fy + j32*fz;
    float y3 = j13*fx + j23*fy + j33*fz;
    float y4 = j14*fx + j24*fy + j34*fz;
    
    glm::quat grad = glm::quat(y1, y2, y3, y4);

    return grad;
}


glm::quat AttitudeSensor::getAttitude(void)
{
    return mAttitude;
}
