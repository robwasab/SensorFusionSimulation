//
//  WalkingDetector.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef WalkingDetector_hpp
#define WalkingDetector_hpp

#include <stdio.h>
#include "OpenGLGraph.hpp"
#include "PLL.hpp"
#include "HighPass.hpp"
#include "Autogain.hpp"
#include "HalfBandFilter.hpp"
#include "OpenGLTimeSeriesGraph.hpp"
#include "Biquad_LowPass.hpp"

class WalkingDetector
{
private:
    static const int DOWNSAMPLE_STAGES = 3;
    
    // Walking detector analog modules
    HighPass mDCBlocker;
    Autogain mAutogain;
    PLL mPLL;
    OpenGLTimeSeriesGraph mGraph;
    Biquad_LowPass mLowPass;
    
    HalfBandFilter mHalfBandFilter[DOWNSAMPLE_STAGES];
    bool mDownSamplerSkipSample[DOWNSAMPLE_STAGES];
    
public:
    WalkingDetector(float fs);
    ~WalkingDetector();
    
    void updateWithSensorData(float gyro_data[3], float accel_data[3], float mag_data[3]);
    
    OpenGLGraph * getSensorDataGraph(void);

    void setFFTMode(void) {
        mGraph.setFFTMode(true /* peak logging */);
    }
    
    void setTimeMode(void) {
        mGraph.setTimeMode();
    }
};

#endif /* WalkingDetector_hpp */
