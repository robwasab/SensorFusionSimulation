//
//  StdDev.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/3/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef StdDev_h
#define StdDev_h
#include "HighPass.hpp"
#include "Average.hpp"

class StdDev
{
private:
    HighPass mDCBlocker;
    Average mMean;
    
public:
    
    StdDev():
        mDCBlocker(0, 0),
        mMean(0, 0)
    {
        
    }
    
    StdDev(float fs, float window_time=0.5, float dc_blocker_tau=0.1):
        mDCBlocker(fs, dc_blocker_tau),
        mMean(fs, window_time)
    {
        
    }
    
    float work(float sample)
    {
        float ac_coupled = mDCBlocker.work(sample);
        
        //printf("%.3f -> %.3f\n", sample, ac_coupled);
        
        float delta = (sample - ac_coupled);
        
        float in_std_dev = delta * delta;
        
        float mean = mMean.work(in_std_dev);
        
        //printf("std_dev avg: %.3f\n", mean);
        
        return mean;
    }
};

#endif /* StdDev_h */
