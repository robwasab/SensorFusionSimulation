//
//  6_coordinate_systems_main.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/30/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
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

class MyOpenGLDelegate: public OpenGLDelegate
{
private:
    OpenGLAxis * mAxis;
    OpenGLSphere * mLightSource;
    OpenGLPrismIPhone * mIphoneUser;
    OpenGLPrismIPhone * mIphoneAttitude;
    
    glm::quat mLastIphoneOrientation;
    
    float mSimulatedSampleRate = 60.0; // Hz
    
    AttitudeSensor * mAttitudeSensor;
    
    // random noise generator
    std::default_random_engine mNoiseGenerator;
    std::normal_distribution<float> mNoiseDistribution;
    
public:
    
    MyOpenGLDelegate():
    OpenGLDelegate(800, 800)
    {
        
    }
    
    const char * windowName(void)
    {
        return "iPhoneSensorSimulator";
    }
    
    void windowClearColor(float * red, float * green, float * blue, float * alpha)
    {
        *red   = 0.2f;
        *green = 0.3f;
        *blue  = 0.3f;
        *alpha = 1.0f;
    }
    
    void initialized(GLFWwindow * window)
    {
        mAxis = new OpenGLAxis();
        mAxis->setScale(0.75);
        
        mLightSource = new OpenGLSphere(20, OpenGLPrimitive::COLOR_YELLOW);
        mLightSource->setScale(0.5);
        mLightSource->setPosition(2, 1, -4);
        
        mIphoneUser     = new OpenGLPrismIPhone();
        mIphoneUser->setPosition(-1.5, 0, 0);
        
        mIphoneAttitude = new OpenGLPrismIPhone();
        mIphoneAttitude->setPosition(2, 0, 0);
        
        mLastIphoneOrientation = mIphoneUser->getOrientationAsQuaternion();
        
        mAttitudeSensor = new AttitudeSensor(mLastIphoneOrientation);
        
        setCameraOrbitRadius(7.0f);
        
        float noise_std = 0.1;
        float noise_mean = 0.0;
        mNoiseDistribution = std::normal_distribution<float>(noise_mean, noise_std);
    }
    
    void windowFrameBufferResized(GLFWwindow * window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    
    void setOrientationWithYawPitchRoll(float yaw, float pitch, float roll)
    {
        // printf("YAW: %.3f   PITCH: %.3f   ROLL: %.3f\n", yaw, pitch, roll);
        
        glm::quat quatYaw   = OpenGLDrawable::quaternionFromAngleAxis(yaw,   ROTATION_YAXIS);
        glm::quat quatPitch = OpenGLDrawable::quaternionFromAngleAxis(pitch, ROTATION_XAXIS);
        glm::quat quatRoll  = OpenGLDrawable::quaternionFromAngleAxis(roll,  ROTATION_ZAXIS);
        
        // the multiplication sequence is important
        // in the program, multiplication is evaluated from the left to the right
        mIphoneUser->setOrientation(quatYaw * quatPitch * quatRoll);
        mAxis->setOrientation(mIphoneUser->getOrientationAsQuaternion());
    }
    
    void simulateGyroscope(float gyro_sensor_data[3])
    {
        // calculate angular velocities to simulate gyroscope
        glm::quat orientation_derivative;
        
        // initialize with current orientation
        orientation_derivative = mIphoneUser->getOrientationAsQuaternion();
        
        // subtract the last known orientation
        orientation_derivative -= mLastIphoneOrientation;
        
        // multiply by "sample rate"
        orientation_derivative *= mSimulatedSampleRate;
        
        // calculate angular velocity
        glm::quat angular_velocity = glm::conjugate(mLastIphoneOrientation) * orientation_derivative * 2.0f;
        
        // the real algorithm just has angular rates in x, y, z axis.
        angular_velocity.w = 0;
        
        // update the last orientation
        mLastIphoneOrientation = mIphoneUser->getOrientationAsQuaternion();
        
        // noise
        angular_velocity.x += mNoiseDistribution(mNoiseGenerator);
        angular_velocity.y += mNoiseDistribution(mNoiseGenerator);
        angular_velocity.z += mNoiseDistribution(mNoiseGenerator);
        
        assert(gyro_sensor_data != NULL);
        gyro_sensor_data[0] = angular_velocity.x;
        gyro_sensor_data[1] = angular_velocity.y;
        gyro_sensor_data[2] = angular_velocity.z;
        
        //mAxis->setXMagnitude(angular_velocity.x);
        //mAxis->setYMagnitude(angular_velocity.y);
        //mAxis->setZMagnitude(angular_velocity.z);
    }
    
    glm::mat3 calculateWorldToSensorTransformation(void)
    {
        // when we get the orientation of the iPhone, it represents a means
        // to transform from sensor frame to world frame.
        
        // we wish to go in the opposite direction.
        
        glm::quat orientation = mIphoneUser->getOrientationAsQuaternion();
        glm::mat3 sensor_to_world = glm::toMat3(orientation);
        
        glm::mat3 world_to_sensor = glm::transpose(sensor_to_world);
        
        return world_to_sensor;
    }
    
    void simulateAccelerometer(float accel_sensor_data[3])
    {
        glm::vec3 world_gravity_vector = glm::vec3(0.0f, -1.0f, 0.0f);
        
        glm::vec3 sensor_gravity_vector = calculateWorldToSensorTransformation() * world_gravity_vector;
        
        assert(accel_sensor_data != NULL);
        
        sensor_gravity_vector.x += mNoiseDistribution(mNoiseGenerator);
        sensor_gravity_vector.y += mNoiseDistribution(mNoiseGenerator);
        sensor_gravity_vector.z += mNoiseDistribution(mNoiseGenerator);
        
        accel_sensor_data[0] = sensor_gravity_vector.x;
        accel_sensor_data[1] = sensor_gravity_vector.y;
        accel_sensor_data[2] = sensor_gravity_vector.z;
        
        //mAxis->setXMagnitude(sensor_gravity_vector.x);
        //mAxis->setYMagnitude(sensor_gravity_vector.y);
        //mAxis->setZMagnitude(sensor_gravity_vector.z);
    }
    
    void simulateMagnometer(float mag_sensor_data[3])
    {
        glm::vec3 world_magnometer_vector = glm::vec3(0.0f, 0.0f, -1.0f);
        
        glm::vec3 sensor_magnometer_vector = calculateWorldToSensorTransformation() * world_magnometer_vector;
        
        assert(mag_sensor_data != NULL);
        
        sensor_magnometer_vector.x += mNoiseDistribution(mNoiseGenerator);
        sensor_magnometer_vector.y += mNoiseDistribution(mNoiseGenerator);
        sensor_magnometer_vector.z += mNoiseDistribution(mNoiseGenerator);
        
        mag_sensor_data[0] = sensor_magnometer_vector.x;
        mag_sensor_data[1] = sensor_magnometer_vector.y;
        mag_sensor_data[2] = sensor_magnometer_vector.z;
        
        mAxis->setXMagnitude(sensor_magnometer_vector.x);
        mAxis->setYMagnitude(sensor_magnometer_vector.y);
        mAxis->setZMagnitude(sensor_magnometer_vector.z);
    }
    
    void updateFrame(GLFWwindow * window, double cursor_posx, double cursor_posy)
    {
        float gyro_data[3] = {0};
        float accel_data[3] = {0};
        float mag_data[3] = {0};
        
        simulateGyroscope(gyro_data);
        simulateAccelerometer(accel_data);
        simulateMagnometer(mag_data);
        
        mAttitudeSensor->sensorUpdate(1.0/mSimulatedSampleRate, gyro_data, accel_data, mag_data);
        
        mIphoneAttitude->setOrientation(mAttitudeSensor->getAttitude());
        
        // open gl update section
        glm::mat4 view_transform = getViewTransform();
        
        mLightSource->drawWithShaderAndTransform(view_transform);
        mIphoneUser->drawWithShaderAndTransform(view_transform);
        mIphoneAttitude->drawWithShaderAndTransform(view_transform);
        
        mAxis->drawWithShaderAndTransform(view_transform);
    }
};


int main(int argc, const char * argv[])
{
    MyOpenGLDelegate delegate;
    
    Utility_startOpenglWithDelegate(&delegate);
    return 0;
}
