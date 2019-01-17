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

#define SQU(x) (x*x)

class AttitudeSensor
{
private:
    glm::quat mAttitude;
    
public:
    AttitudeSensor(glm::quat initial_attitude);
    
    void sensorUpdate(float time_step,
                      float gyro_data[3],
                      float accel_data[3],
                      float mag_data[3]);
    
    
    glm::quat calculateErrorGradient(glm::vec3 world, glm::vec3 sensor);
    glm::quat getAttitude(void);
};

#endif /* AttitudeSensor_hpp */
