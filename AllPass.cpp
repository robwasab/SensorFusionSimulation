//
//  AllPass.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "AllPass.hpp"
#include <string.h>

AllPass::AllPass(double gain, double zero, double pole)
{
    mGain = gain;
    mA = zero;
    mB = pole;
    
    memset(mLastX, 0, sizeof(mLastX));
    memset(mLastY, 0, sizeof(mLastY));
}

float AllPass::work(float sample)
{
    mLastX[0] = sample;
    
    mLastY[0] = mGain * mLastX[0] + mGain * mA * mLastX[2] - mB * mLastY[2];
    
    mLastX[2] = mLastX[1];
    mLastX[1] = mLastX[0];
    
    mLastY[2] = mLastY[1];
    mLastY[1] = mLastY[0];
    
    return mLastY[0];
}
