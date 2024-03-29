//
//  OpenGLDrawable.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/3/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLDrawable_h
#define OpenGLDrawable_h

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define ROTATION_XAXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define ROTATION_YAXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define ROTATION_ZAXIS glm::vec3(0.0f, 0.0f, 1.0f)

class OpenGLDrawable
{
private:
    glm::quat mAttitude;
    glm::mat4 mScale;
    glm::mat4 mPosition;
    glm::mat4 mTransformationMatrix;
    bool mTransformationMatrixDirty;

    
public:
    OpenGLDrawable();
    
    virtual ~OpenGLDrawable()
    {
        
    }
    
    static glm::quat quaternionFromAngleAxis(float degrees, glm::vec3 axis);
    
    glm::quat getOrientationAsQuaternion(void);
    
    virtual void setScale(float xscale, float yscale, float zscale);
    virtual void setScale(float scale);
    
    virtual void setOrientation(glm::quat orientation);
    virtual void setOrientation(float degrees, glm::vec3 rotation_axis);
    virtual void setPosition(float x, float y, float z);
    virtual void setPosition(glm::vec3 position);
    
    virtual void addOrientation(float degrees, glm::vec3 rotation_axis);
    virtual void addPosition(float x, float y, float z);
    
    virtual void addPositionX(float x);
    virtual void addPositionY(float y);
    virtual void addPositionZ(float z);
    
    virtual glm::mat4 getTransformationMatrix(void);
    
    virtual void drawWithShaderAndTransform(glm::mat4 transform) = 0;

};

#endif /* OpenGLDrawable_h */
