//
//  AttitudeSensor.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "AttitudeSensor.hpp"

#define MAGNETOM_COMPENSATION_ENABLED         1
#define GYROBIAS_COMPENSATION_ENABLED         1

#define BETA_AUGMENTATION_FACTOR            3.0f

AttitudeSensor::AttitudeSensor(glm::quat initial_attitude, float fs):
    mWalkingDetector(fs)
{
    mAttitude = initial_attitude;
    
    // calculate the expected value of the gyro noise for one axis
    float gyro_noise_expected_value_axis = glm::sqrt(SQU(GYRO_NOISE_STD_DEV) + SQU(GYRO_NOISE_MEAN));
    
    // multiply by sqrt(3.0) to get the expected noise for all three combined axes
    float gyro_noise_expected_value = glm::sqrt(3.0/4.0) * gyro_noise_expected_value_axis;
    
    #if 1 == GYROBIAS_COMPENSATION_ENABLED
    
    mBeta = gyro_noise_expected_value * BETA_AUGMENTATION_FACTOR;
    mBetaReference = mBeta;
    
    printf("Attitude beta: %.3f\n", mBeta);
    
    // guessed this value - bias can change at a rate of x deg/sec
    //mZeta = glm::radians(5.0f);
    #warning "mZeta is zero!"
    mZeta = 0.0;
    
    #else
    
    mBeta = 0;
    mZeta = 0;
    #endif
    
    mAngularVelocityErrorBias = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    
    #define POW2 8
    mGraphs[0] = new OpenGLTimeSeriesGraph(POW2, "X Axis");
    mGraphs[1] = new OpenGLTimeSeriesGraph(POW2, "Y Axis");
    mGraphs[2] = new OpenGLTimeSeriesGraph(POW2, "Z Axis");
}

AttitudeSensor::~AttitudeSensor()
{
    delete mGraphs[0];
    delete mGraphs[1];
    delete mGraphs[2];
}

void AttitudeSensor::enableGyroOnly(void)
{
    mGyroOnly = true;
    mBeta = 0.0;
}

void AttitudeSensor::disableGyroOnly(void)
{
    mGyroOnly = false;
    mBeta = mBetaReference;
}

void AttitudeSensor::sensorUpdate(float time_step,
                                  float gyro_data[3],
                                  float accel_data[3],
                                  float mag_data[3])
{
    mWalkingDetector.updateWithSensorData(gyro_data, accel_data, mag_data);
    
    mGraphs[0]->updateWithNewSample(gyro_data[2]);
    
    // Attitude determination algorithm
    
    glm::vec3 accel_measure = glm::vec3(accel_data[0], accel_data[1], accel_data[2]);
    glm::vec3 magno_measure = glm::vec3(  mag_data[0],   mag_data[1],   mag_data[2]);
    
    // reference field vectors
    glm::vec3 world_gravity = glm::vec3(0.0f, -1.0f,  0.0f);
    
    #if 1 == MAGNETOM_COMPENSATION_ENABLED
    
    // magnometer compensation
    glm::vec3 magno_measure_wrt_earth = glm::toMat3(mAttitude) * magno_measure;
    
    float horizontal_plane_magnitude = glm::sqrt(SQU(magno_measure_wrt_earth.z) +
                                                 SQU(magno_measure_wrt_earth.x));
    
    glm::vec3 world_north   = glm::vec3(0.0f,  magno_measure_wrt_earth.y, -horizontal_plane_magnitude);
    
    #else
    
    glm::vec3 world_north   = glm::vec3(0.0f, 0.0f, -1.0f);
    
    #endif
    
    // gradient descent optimization using accel and magnotometer
    glm::quat gravity_error_gradient =
        calculateErrorGradient(world_gravity, accel_measure);
    
    glm::quat magnometer_error_gradient =
        calculateErrorGradient(world_north, magno_measure);
    
    glm::quat error_gradient = gravity_error_gradient + magnometer_error_gradient;
    
    glm::quat error_gradient_norm = glm::normalize(error_gradient);

    // gyroscope bias compensation
    glm::quat angular_velocity_error = 2.0f * (glm::conjugate(mAttitude) * error_gradient_norm);
    angular_velocity_error.w = 0;
    
    mAngularVelocityErrorBias += angular_velocity_error * time_step * mZeta;
    
    #if 0
    printf("gyro bias: %.3f   %.3f   %.3f\n",
           glm::degrees(mAngularVelocityErrorBias.x),
           glm::degrees(mAngularVelocityErrorBias.y),
           glm::degrees(mAngularVelocityErrorBias.z));
    #endif
    
    // gyroscope calculations
    glm::quat angular_velocity_sensor = glm::quat(0.0f, gyro_data[0], gyro_data[1], gyro_data[2]);
    
    #if 0
    printf("gyro meas: %.3f   %.3f   %.3f\n",
           glm::degrees(angular_velocity_sensor.x),
           glm::degrees(angular_velocity_sensor.y),
           glm::degrees(angular_velocity_sensor.z));
    #endif
    
    angular_velocity_sensor -= mAngularVelocityErrorBias;
    
    glm::quat angular_velocity_wrt_earth = (mAttitude * angular_velocity_sensor) * 0.5f;
    
    glm::quat attitude_derivative;
    attitude_derivative = angular_velocity_wrt_earth;
    attitude_derivative-= mBeta * error_gradient_norm;
    
    mAttitude += attitude_derivative * time_step;
    mAttitude = glm::normalize(mAttitude);
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

void AttitudeSensor::getSensorGraphs(OpenGLGraph * graphs[3])
{
    graphs[0] = mGraphs[0];
    graphs[1] = mGraphs[1];
    graphs[2] = mWalkingDetector.getSensorDataGraph();
}

void AttitudeSensor::setFFTMode(void)
{
    mGraphs[0]->setFFTMode();
    mGraphs[1]->setFFTMode();
    mGraphs[2]->setFFTMode();
    mWalkingDetector.setFFTMode();
}

void AttitudeSensor::setTimeMode(void)
{
    mGraphs[0]->setTimeMode();
    mGraphs[1]->setTimeMode();
    mGraphs[2]->setTimeMode();
    mWalkingDetector.setTimeMode();
}
