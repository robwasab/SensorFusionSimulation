//
//  WalkingDetector.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "WalkingDetector.hpp"

#define POW2 8

WalkingDetector::WalkingDetector(float fs):
    mPLL(fs/(1 << DOWNSAMPLE_STAGES), 1.1),
    mDCBlocker(fs, 0.25),
    mAutogain(),
	mHalfBandFilter(),
    mGraph(POW2, "Walking Detector"),
    mLowPass(0.707106, 0.2 /* fp */, 1.0 /* fs */)
{
}

WalkingDetector::~WalkingDetector()
{
    
}

void WalkingDetector::updateWithSensorData(float gyro_data[3],
                                           float accel_data[3],
                                           float mag_data[3])
{
    float sample = gyro_data[2];
    
    sample = mDCBlocker.work(sample);
    
    size_t k;
    
    for(k = 0; k < DOWNSAMPLE_STAGES; k++)
    {
        sample = mHalfBandFilter[k].work(sample);
        
        if(false == mDownSamplerSkipSample[k])
        {
            mDownSamplerSkipSample[k] ^= true;
        } else
        {
            mDownSamplerSkipSample[k] ^= true;
            break;
        }
    }
    
    if(k < DOWNSAMPLE_STAGES) {
        return;
    }

    sample = mLowPass.work(sample);
    
    sample = mAutogain.work(sample);
    
    sample = mPLL.work(sample);
    
    mGraph.updateWithNewSample(sample/2);
}

OpenGLGraph * WalkingDetector::getSensorDataGraph(void)
{
    return &mGraph;
}
