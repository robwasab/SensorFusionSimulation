//
//  OpenGLWalker.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/20/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLWalker_hpp
#define OpenGLWalker_hpp

#include <stdio.h>
#include "OpenGLComposite.hpp"
#include "Walker.hpp"
#include "OpenGLSphere.hpp"
#include "OpenGLPrismCylinder.hpp"
#include "OpenGLArrow.hpp"
#include "OpenGLPrismIPhone.hpp"

class OpenGLWalker: public OpenGLComposite
{
private:
    OpenGLSphere * mBodyParts[Walker::BODYPART_MAX];
    OpenGLPrismCylinder * mBodyPartConnections[Walker::BODYPART_MAX-1];
    OpenGLArrow * mAxes[3];
    OpenGLPrismIPhone * mIPhone;
    
    double mPeriod;
    double mLastTime;
    
    Walker * mWalker;
    glm::vec3 mWalkerOffset;
    
    glm::mat4 mTransform;
    
    glm::vec3 mIPhonePosition;
    glm::mat4 mIPhoneOrientation;
    
    void advanceWalkerWithRadians(double radians);
    
public:
    
    OpenGLWalker(float period=1.0f);
    ~OpenGLWalker();
    
    glm::vec3 getMaxDimensions(void);
    void drawWithShaderAndTransform(glm::mat4 transform) override;
    
    void getIPhoneCoordinateSystem(glm::vec3 * origin, glm::mat4 * orientation);
    
    void setWalkingFrequency(double walking_frequency);
    
    enum WalkingState
    {
        STANDING,
        WALK_FORWARD,
        WALK_BACKWARD,
    } mWalkingState = STANDING;
    
    void setWalkingState(WalkingState motion);
};

#endif /* OpenGLWalker_hpp */
