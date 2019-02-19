//
//  Walker.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/18/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef Walker_h
#define Walker_h

#include <stdint.h>

class Walker
{
public:

    enum BODYPART
    {
        BODYPART_HEAD           = 0,
        BODYPART_RIGHT_HAND     = 1,
        BODYPART_RIGHT_ELBOW    = 2,
        BODYPART_RIGHT_SHOULDER = 3,
        BODYPART_CHEST          = 4,
        BODYPART_LEFT_SHOULDER  = 5,
        BODYPART_LEFT_ELBOW     = 6,
        BODYPART_LEFT_HAND      = 7,
        BODYPART_RIGHT_FOOT     = 8,
        BODYPART_RIGHT_KNEE     = 9,
        BODYPART_RIGHT_HIP      = 10,
        BODYPART_PELVIS         = 11,
        BODYPART_LEFT_HIP       = 12,
        BODYPART_LEFT_KNEE      = 13,
        BODYPART_LEFT_FOOT      = 14,
        BODYPART_MAX,
    };
    
    enum Gender
    {
        GENDER_MALE,
        GENDER_FEMALE,
        GENDER_HERMAPHRODITE,
    };
    struct Point
    {
        double x;
        double y;
        double z;
    };
    
    Walker(Gender gender=GENDER_MALE);
    
    void advanceWithRadians(double radians, Point positions[15]);
    
    void getMaximumDimensions(double xyz[3]);
    
    Point getOffset(void);
    
    void resetOffset(void);
    
private:
    double mRadians;
    Point mOffset;
    Point mLastDownedFootPosition;
    enum
    {
        LEFT_FOOT_DOWN,
        RIGHT_FOOT_DOWN,
        BOTH_FEET_DOWN,
    } mFeetState;
    
    double * mData;
    
    enum AXIS
    {
        AXIS_X = 0,
        AXIS_Y = 1,
        AXIS_Z = 2,
    };
    
    enum FREQ_COMPONENT
    {
        FREQ_COMPONENT_DC      = 0,
        FREQ_COMPONENT_F1_ODD  = 1,
        FREQ_COMPONENT_F1_EVEN = 2,
        FREQ_COMPONENT_F2_ODD  = 3,
        FREQ_COMPONENT_F2_EVEN = 4,
        FREQ_COMPONENT_MAX,
    };
    
    double getFrequencyComponent(enum BODYPART body_part,
                                 enum FREQ_COMPONENT freq_comp,
                                 enum AXIS axis);
    
    double getMaximumDimensionForBodyPartAndAxis(enum BODYPART body_part, enum AXIS axis);

    Point calculateBodyPartWithRadians(enum BODYPART body_part, double radians);
};

#endif /* Walker_h */
