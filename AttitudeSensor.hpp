//
//  AttitudeSensor.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef AttitudeSensor_hpp
#define AttitudeSensor_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "OpenGLGraph.hpp"
#include "OpenGLTimeSeriesGraph.hpp"
#include "WalkingDetector.hpp"

#define SQU(x) (x*x)
#define GYRO_NOISE_STD_DEV (glm::radians(5.0f))
#define GYRO_NOISE_MEAN    (glm::radians(0.0f))

#define ACCEL_NOISE_STD_DEV    0.1f
#define ACCEL_NOISE_MEAN       0.f

#define MAGNO_NOISE_STD_DEV    0.1f
#define MAGNO_NOISE_MEAN       0.0f

#define MAGNETOMETER_YAW_DISTORTION_DEGREES     0.0f
#define MAGNETOMETER_PITCH_DISTORTION_DEGREES   45.0f


class AttitudeSensor
{
private:
    // Attitude determination variables
    glm::quat mAttitude;
    float mBetaReference;
    float mBeta;
    float mZeta;
    glm::quat mAngularVelocityErrorBias;
    bool mGyroOnly;
    
    enum PLOT_MODE
    {
        PLOT_MODE_ACCELEROMETER,
        PLOT_MODE_GYROSCOPE,
    } mPlotMode = PLOT_MODE_GYROSCOPE;
    
    void detect_bad_accel_data(glm::vec3 accel_data);
    
    OpenGLTimeSeriesGraph * mGraphs[3];
    
    WalkingDetector mWalkingDetector;
    
public:
    AttitudeSensor(glm::quat initial_attitude, float fs=60.0f);
    ~AttitudeSensor();
    
    void sensorUpdate(float time_step,
                      float gyro_data[3],
                      float accel_data[3],
                      float mag_data[3]);
    
    glm::quat calculateErrorGradient(glm::vec3 world, glm::vec3 sensor);
    glm::quat getAttitude(void);
    
    void enableGyroOnly(void);
    void disableGyroOnly(void);
    
    void setFFTMode(void);
    void setTimeMode(void);

    void getSensorGraphs(OpenGLGraph * graphs[3]);
};

#endif /* AttitudeSensor_hpp */
