//
//  SensorSimulator.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/2/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef SensorSimulator_hpp
#define SensorSimulator_hpp

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

class SensorSimulator
{
private:
    glm::quat mLastSensorToWorldAttitude;
    glm::vec3 mLastWorldPosition;
    glm::vec3 mLastWorldVelocity;
    glm::vec3 mWorldGravity;
    glm::vec3 mWorldNorth;
    
    float mSimulatedSampleRate;
    
    float mAcceSensorData[3];
    float mGyroSensorData[3];
    float mMagnSensorData[3];
    
    // random noise generator
    std::default_random_engine mNoiseSource;
    std::normal_distribution<float> mGyroNoiseDist;
    std::normal_distribution<float> mAcceNoiseDist;
    std::normal_distribution<float> mMagnNoiseDist;

    void calculate_gryosco(float sensor_data[3], glm::quat new_sensor_to_world_attitude);
    void calculate_mangeto(float sensor_data[3], glm::mat3 world_to_sensor_transform);
    void calculate_gravity(float sensor_data[3], glm::mat3 world_to_sensor_transform);
    void calculate_acceler(float sensor_data[3], glm::mat3 world_to_sensor_transform, glm::vec3 new_position, float time_delta);
    
public:
    SensorSimulator(float simulated_sample_rate=60.0f,
                    glm::vec3 world_gravity=glm::vec3(0.0f, -1.0f,  0.0f),
                    glm::vec3 world_north  =glm::vec3(0.0f,  0.0f, -1.0f));
    
    void updateWithSensorToWorldQuaternionAndPosition(glm::quat attitude, glm::vec3 position);
    
    void getUpdatedSensorData(float gyro_sensor_data[3], float accel_sensor_data[3], float magnetometer_sensor_data[3]);
    
};

#endif /* SensorSimulator_hpp */
