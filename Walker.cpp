//
//  WalkerMale.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/18/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Walker.hpp"

static double walker_male_data[] =
{
    0.871084096,    -3.343924923,   -21.2927191,    -0.01768901,    0.059374257,
    206.3194551,    -1.641184617,	-12.82043967,	1.463396324,	3.479965642,
    221.7680799,    -21.30808297,	-20.04172446,	2.368378962,	0.831867551,
    170.356044,     -6.877941871,	-19.39397007,	-0.192394888,	0.395008319,
    -0.290641177,   -7.60325798,	-19.3363063,	0.067153524,	0.077623682,
    -169.6836108,   -7.548938466,	-19.96179365,	0.31399096,     -0.224898302,
    -225.3324784,   -23.55721397,	-20.92474861,	-1.426920243,   -2.237860573,
    -199.2632133,	-3.260559327,	-15.88860994,	-1.409299817,	-5.276274204,
    54.2181439,     6.080650629,	12.92069511,	-0.029122723,   0.973396544,
    83.94705773,    1.926766941,    10.28979238,	-0.393952502,	4.9130966,
    89.33776692,    -7.204032272,	-13.33381281,	-0.110385797,	-0.060908072,
    1.784930895,    -6.904597917,	-10.91637076,   -0.089057128,	-0.051133562,
    -85.47855544,	-7.177332206,   -13.29988618,	-0.099870221,	-0.041574626,
    -86.82267934,   0.750913009,    9.102163291,	0.350327572,    -4.92196095,
    -58.6018417,    5.190726961,    12.29610493,	0.266520969,	-1.35057094,
    26.96230567,    -1.744711073,   -0.410553558,	-5.041613988,	2.540547626,
    50.52753107,    -101.5856329,   -25.43887847,   2.427849855,	0.074013421,
    -69.95904243,   -60.69038395,   -17.64089356,	-1.801298242,	1.756341665,
    -3.43494367,    -16.48870667,	-5.955438777,	-6.609292138,	3.121047431,
    -9.820916764,   -1.220946665,	-0.323950533,	-6.856805755,	3.461882051,
    -14.28189605,   13.93626693,	5.214680318,	-6.784435739,	3.378005696,
    -71.18800893,   74.60356375,	19.10643157,	-1.655667267,	2.195478451,
    43.06693036,    126.365215,     28.5266177,     2.571532923,	0.287525294,
    -56.83570715,   223.9202002,    -58.74282129,   31.41522553,	-25.36891664,
    84.91828323,    122.4383978,    14.94194831,	3.672388876,	28.31884997,
    -7.011951672,   -0.850549606,   -2.946246255,	-10.48478072,	8.867056298,
    8.512274633,    -1.143906006,	-0.142379399,   -10.33313206,	7.354783913,
    -8.120762716,   -1.255238766,	2.911468931,	-10.55696158,	8.761256899,
    80.66300085,    -121.0930742,	-17.41022127,   3.782207709,	29.77407629,
    -58.58440116,	-225.0619058,	60.8014221,     32.1944364,     -26.50663664,
    1382.397336,    0.240475723,    0.043858443,	7.730525727,	8.800619208,
    767.649093,     -20.21760026,	0.660393714,	13.10327623,	-0.069732019,
    975.2054453,    6.872008946,	5.917186037,	8.772088267,	6.059586915,
    1222.385705,    -2.023648712,	3.291699258,    8.328462806,	8.626157844,
    1206.483495,    0.195303037,    -0.059142235,	8.026787345,	8.639179265,
    1230.132035,	2.3216193,      -3.54407563,	8.411296425,	8.347436751,
    981.4131371,	-7.236483252,	-5.57659039,    8.836940471,	4.365213799,
    774.8258901,	25.37712842,	0.672699892,	12.27967668,	-3.727648079,
    94.99542934,	-12.95057821,	55.95981958,	-10.91592367,	27.11498312,
    411.7783492,	25.8074657,     5.920080825,    12.46022465,	3.978803696,
    785.7400223,	-0.31608526,	-1.915005001,	7.85149189,     8.643138915,
    873.5597208,    0.161767275,    0.038057394,	7.845341469,	8.833136907,
    785.9573216,    0.660957123,    2.023429517,	7.929084674,	8.617789562,
    417.0224457,	-24.83512489,	-6.358332306,	12.45784523,	4.822347323,
    96.08054703,	13.96851867,	-57.59519443,	-11.79146609,	28.250563,
};

Walker::Walker(Gender gender)
{
    switch(gender)
    {
        case GENDER_MALE:
        case GENDER_FEMALE:
        case GENDER_HERMAPHRODITE:
            mData = walker_male_data;
            break;
    }
    memset(&mOffset, 0, sizeof(mOffset));
    memset(&mLastDownedFootPosition, 0, sizeof(mLastDownedFootPosition));
    mFeetState = BOTH_FEET_DOWN;
}

double Walker::getFrequencyComponent(enum BODYPART body_part,
                                     enum FREQ_COMPONENT freq_comp,
                                     enum AXIS axis)
{
    assert(body_part <= BODYPART_LEFT_FOOT);
    assert(freq_comp <= FREQ_COMPONENT_F2_EVEN);
    assert(axis <= AXIS_Z);
    
    size_t index = (( 5 * (size_t) body_part) + (size_t) freq_comp) + (15 * 5 * (size_t) axis);

    return mData[index];
}

Walker::Point Walker::calculateBodyPartWithRadians(enum BODYPART body_part, double radians)
{
    Point p;
    
    float stride = 1.0f;
    
    p.x = getFrequencyComponent(body_part, FREQ_COMPONENT_DC, AXIS_X);
    p.y = getFrequencyComponent(body_part, FREQ_COMPONENT_DC, AXIS_Y);
    p.z = getFrequencyComponent(body_part, FREQ_COMPONENT_DC, AXIS_Z);
    
    p.x += sin(radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F1_ODD, AXIS_X);
    p.y += sin(radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F1_ODD, AXIS_Y);
    p.z += sin(radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F1_ODD, AXIS_Z);

    p.x += cos(radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F1_EVEN, AXIS_X);
    p.y += cos(radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F1_EVEN, AXIS_Y);
    p.z += cos(radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F1_EVEN, AXIS_Z);

    p.x += stride * sin(2*radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F2_ODD, AXIS_X);
    p.y += stride * sin(2*radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F2_ODD, AXIS_Y);
    p.z += stride * sin(2*radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F2_ODD, AXIS_Z);
    
    p.x += stride * cos(2*radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F2_EVEN, AXIS_X);
    p.y += stride * cos(2*radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F2_EVEN, AXIS_Y);
    p.z += stride * cos(2*radians) * getFrequencyComponent(body_part, FREQ_COMPONENT_F2_EVEN, AXIS_Z);

    return p;
}

double Walker::getMaximumDimensionForBodyPartAndAxis(enum BODYPART body_part, enum AXIS axis)
{
    double dc;
    double mag_f1;
    double mag_f2;
    
    dc     = sqrt(pow(getFrequencyComponent(body_part, FREQ_COMPONENT_DC,      axis), 2.0));
    
    /*
    mag_f1 = sqrt(pow(getFrequencyComponent(body_part, FREQ_COMPONENT_F1_ODD,  axis), 2.0) +
                  pow(getFrequencyComponent(body_part, FREQ_COMPONENT_F1_EVEN, axis), 2.0));
    
    mag_f2 = sqrt(pow(getFrequencyComponent(body_part, FREQ_COMPONENT_F2_ODD,  axis), 2.0) +
                  pow(getFrequencyComponent(body_part, FREQ_COMPONENT_F2_ODD,  axis), 2.0));
    */
    mag_f1 = 0;
    mag_f2 = 0;
    return dc + mag_f1 + mag_f2;
}

void Walker::getMaximumDimensions(double xyz[3])
{
    double max_x = 0;
    double max_y = 0;
    double max_z = 0;
    
    for(int k = 0; k < BODYPART_MAX; k++)
    {
        double x = getMaximumDimensionForBodyPartAndAxis((enum BODYPART) k, AXIS_X);
        double y = getMaximumDimensionForBodyPartAndAxis((enum BODYPART) k, AXIS_Y);
        double z = getMaximumDimensionForBodyPartAndAxis((enum BODYPART) k, AXIS_Z);
        
        if(x > max_x) max_x = x;
        if(y > max_y) max_y = y;
        if(z > max_z) max_z = z;
    }
    xyz[0] = max_x;
    xyz[1] = max_y;
    xyz[2] = max_z;
}

void Walker::advanceWithRadians(double radians, Point positions[15])
{
    mRadians += radians;
    
    if(mRadians >= 2.0 * M_PI)
    {
        mRadians -= 2.0 * M_PI;
    }
    
    for(int k = 0; k < BODYPART_MAX; k++)
    {
        positions[k] = calculateBodyPartWithRadians((enum BODYPART) k, mRadians);
    }
    
    Point right_foot = positions[BODYPART_RIGHT_FOOT];
    Point left_foot  = positions[BODYPART_LEFT_FOOT];
    Point delta_offset = {0};
    
    if(right_foot.z < left_foot.z)
    {
        // right foot down;
        //printf("right foot down...\n");
     
        if(mFeetState != RIGHT_FOOT_DOWN)
        {
            mFeetState = RIGHT_FOOT_DOWN;
            mLastDownedFootPosition = right_foot;
        }
        
        delta_offset.x = mLastDownedFootPosition.x - right_foot.x;
        delta_offset.y = mLastDownedFootPosition.y - right_foot.y;
        delta_offset.z = 0;
        
        mLastDownedFootPosition = right_foot;
    }
    else if(left_foot.z < right_foot.z)
    {
        // left foot down;
        //printf("left_foot down...\n");
        
        if(mFeetState != LEFT_FOOT_DOWN)
        {
            mFeetState = LEFT_FOOT_DOWN;
            mLastDownedFootPosition = left_foot;
        }
        
        delta_offset.x = mLastDownedFootPosition.x - left_foot.x;
        delta_offset.y = mLastDownedFootPosition.y - left_foot.y;
        delta_offset.z = 0;
        
        mLastDownedFootPosition = left_foot;
    }
    else
    {
        if(mFeetState != RIGHT_FOOT_DOWN)
        {
            mFeetState = BOTH_FEET_DOWN;
        }
    }
    
    //printf("mOffset = [%.3f   %.3f   %.3f]\n", mOffset.x, mOffset.y, mOffset.z);
    
    mOffset.x += delta_offset.x;
    mOffset.y += delta_offset.y;
    mOffset.z += delta_offset.z;
    
    for(int k = 0; k < BODYPART_MAX; k++)
    {
        positions[k].x += mOffset.x;
        positions[k].y += mOffset.y;
        positions[k].z += mOffset.z;
    }
}

Walker::Point Walker::getOffset(void)
{
    return mOffset;
}

void Walker::resetOffset(void)
{
    memset(&mOffset, 0, sizeof(mOffset));
}
