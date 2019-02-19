//
//  SensorSimulator.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/2/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "SensorSimulator.hpp"
#include "AttitudeSensor.hpp"

SensorSimulator::SensorSimulator(float simulated_sample_rate,
                                 glm::vec3 world_gravity,
                                 glm::vec3 world_north)
{
    mSimulatedSampleRate = simulated_sample_rate;
    mWorldGravity = world_gravity;
    mWorldNorth = world_north;
    
    mLastSensorToWorldAttitude = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    mLastWorldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    mLastWorldVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    mGyroNoiseDist = std::normal_distribution<float>(GYRO_NOISE_MEAN, GYRO_NOISE_STD_DEV);
    mAcceNoiseDist = std::normal_distribution<float>(ACCEL_NOISE_MEAN, GYRO_NOISE_STD_DEV);
    mMagnNoiseDist = std::normal_distribution<float>(MAGNO_NOISE_MEAN, MAGNO_NOISE_STD_DEV);
}

void SensorSimulator::updateWithSensorToWorldQuaternionAndPosition(glm::quat sensor_to_world_attitude, glm::vec3 position)
{
    glm::mat3 sensor_to_world = glm::toMat3(sensor_to_world_attitude);
    
    glm::mat3 world_to_sensor = glm::transpose(sensor_to_world);

    float gyro_data[3] = {0};
    float grav_data[3] = {0};
    float magn_data[3] = {0};
    float acce_data[3] = {0};
    
    calculate_gryosco(gyro_data, sensor_to_world_attitude);
    calculate_mangeto(magn_data, world_to_sensor);
    calculate_gravity(grav_data, world_to_sensor);
    calculate_acceler(acce_data, world_to_sensor, position, 1.0/mSimulatedSampleRate);
    
    memcpy(mGyroSensorData, gyro_data, sizeof(mGyroSensorData));
    
    memcpy(mAcceSensorData, grav_data, sizeof(mAcceSensorData));
    
    mAcceSensorData[0] += acce_data[0];
    mAcceSensorData[1] += acce_data[1];
    mAcceSensorData[2] += acce_data[2];
    
    mAcceSensorData[0] += mAcceNoiseDist(mNoiseSource);
    mAcceSensorData[1] += mAcceNoiseDist(mNoiseSource);
    mAcceSensorData[2] += mAcceNoiseDist(mNoiseSource);
    
    memcpy(&mMagnSensorData, magn_data, sizeof(mMagnSensorData));
}

void SensorSimulator::getUpdatedSensorData(float gyro_sensor_data[3], float accel_sensor_data[3], float magnetometer_sensor_data[3])
{
    memcpy(gyro_sensor_data, mGyroSensorData, sizeof(mGyroSensorData));
    memcpy(accel_sensor_data, mAcceSensorData, sizeof(mAcceSensorData));
    memcpy(magnetometer_sensor_data, mMagnSensorData, sizeof(mMagnSensorData));
}

void SensorSimulator::calculate_gryosco(float sensor_data[3], glm::quat new_sensor_to_world_attitude)
{
    // calculate angular velocities to simulate gyroscope
    glm::quat attitude_derivative;
    
    // initialize with current orientation
    attitude_derivative = new_sensor_to_world_attitude;
    
    // subtract the last known orientation
    attitude_derivative -= mLastSensorToWorldAttitude;
    
    // multiply by "sample rate"
    attitude_derivative *= mSimulatedSampleRate;
    
    // calculate angular velocity
    glm::quat angular_velocity = glm::conjugate(mLastSensorToWorldAttitude) * attitude_derivative * 2.0f;
    
    // the real algorithm just has angular rates in x, y, z axis.
    angular_velocity.w = 0;
    
    // update the last orientation
    mLastSensorToWorldAttitude = new_sensor_to_world_attitude;
    
    // noise
    angular_velocity.x += mGyroNoiseDist(mNoiseSource);
    angular_velocity.y += mGyroNoiseDist(mNoiseSource);
    angular_velocity.z += mGyroNoiseDist(mNoiseSource);
    
    assert(sensor_data != NULL);
    
    sensor_data[0] = angular_velocity.x;
    sensor_data[1] = angular_velocity.y;
    sensor_data[2] = angular_velocity.z;
    
    //mAxis->setXMagnitude(angular_velocity.x);
    //mAxis->setYMagnitude(angular_velocity.y);
    //mAxis->setZMagnitude(angular_velocity.z);
}

void SensorSimulator::calculate_mangeto(float sensor_data[3], glm::mat3 world_to_sensor_transform)
{
    glm::vec3 world_magnometer_vector = glm::vec3(0.0f, 0.0f, -1.0f);
    
    glm::mat4 magnetometer_distortion;
    
    world_magnometer_vector = glm::mat3(magnetometer_distortion) * world_magnometer_vector;
    
    glm::vec3 sensor_magnometer_vector = world_to_sensor_transform * world_magnometer_vector;
    
    assert(sensor_data != NULL);
    
    sensor_magnometer_vector.x += mMagnNoiseDist(mNoiseSource);
    sensor_magnometer_vector.y += mMagnNoiseDist(mNoiseSource);
    sensor_magnometer_vector.z += mMagnNoiseDist(mNoiseSource);
    
    sensor_data[0] = sensor_magnometer_vector.x;
    sensor_data[1] = sensor_magnometer_vector.y;
    sensor_data[2] = sensor_magnometer_vector.z;
    
    //mAxis->setXMagnitude(sensor_magnometer_vector.x);
    //mAxis->setYMagnitude(sensor_magnometer_vector.y);
    //mAxis->setZMagnitude(sensor_magnometer_vector.z);
}

void SensorSimulator::calculate_gravity(float sensor_data[3], glm::mat3 world_to_sensor_transform)
{
    glm::vec3 sensor_gravity_vector = world_to_sensor_transform * mWorldGravity;
    
    assert(sensor_data != NULL);
    
    //sensor_gravity_vector.x += mAcceNoiseDist(mNoiseSource);
    //sensor_gravity_vector.y += mAcceNoiseDist(mNoiseSource);
    //sensor_gravity_vector.z += mAcceNoiseDist(mNoiseSource);
    
    sensor_data[0] = sensor_gravity_vector.x;
    sensor_data[1] = sensor_gravity_vector.y;
    sensor_data[2] = sensor_gravity_vector.z;
    
    //mAxis->setXMagnitude(sensor_gravity_vector.x);
    //mAxis->setYMagnitude(sensor_gravity_vector.y);
    //mAxis->setZMagnitude(sensor_gravity_vector.z);
}

void SensorSimulator::calculate_acceler(float sensor_data[3], glm::mat3 world_to_sensor_transform, glm::vec3 new_world_position, float time_delta)
{
    glm::vec3 new_world_velocity = (new_world_position - mLastWorldPosition) / time_delta;
    
    glm::vec3 new_world_accelera = (new_world_velocity - mLastWorldVelocity) / time_delta;
    
    mLastWorldPosition = new_world_position;
    mLastWorldVelocity = new_world_velocity;
    
    glm::vec3 sensor_accel = world_to_sensor_transform * new_world_accelera;
    
    // normalize by gravity
    sensor_accel /= 9.8f;
    
    sensor_data[0] = sensor_accel.x;
    sensor_data[1] = sensor_accel.y;
    sensor_data[2] = sensor_accel.z;
}
