//
//  OpenGLSquare.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/30/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#ifndef OpenGLSquare_hpp
#define OpenGLSquare_hpp


#include "OpenGLShader.hpp"
#include <stdio.h>

#define ROTATION_AXIS_X glm::vec3(1.0, 0.0, 0.0)
#define ROTATION_AXIS_Y glm::vec3(0.0, 1.0, 0.0)
#define ROTATION_AXIS_Z glm::vec3(0.0, 0.0, 1.0)

class OpenGLSquare
{
private:
    glm::mat4 mRotation;
    glm::mat4 mTranslate;
    glm::mat4 mScale;
    GLuint mVAO;
    GLuint mVBO;
    GLuint mTextureId;
    
public:
    OpenGLSquare(const char texture_file[], double r=0.0, double b=0.0, double g=0.0);
    void setLength(double length);
    void setRotation(float degrees, glm::vec3 rotation_axis);
    void setLocation(double x, double y, double z);
    
    void drawWithShaderAndTransform(OpenGLShader * shader, glm::mat4 transform);
};

#endif /* OpenGLSquare_hpp */
